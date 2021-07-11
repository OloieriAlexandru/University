/*
Documentation:
- I/O Multiplexing:
	https://notes.shichao.io/unp/ch6/
	
The command line functionality is taken from my final project for the University Course "Computer Networks":
- https://github.com/OloieriAlexandru/Part2Part-Chord/tree/master/src
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <libgen.h> // basename
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>

#include "flags.h"
#include "common_functions.h"
#include "command_line.h"
#include "crypto.h"

int nodePort;

int keyManagerPort;

const char* keyManagerIp = "127.0.0.1";

const char* filesDirectoryPath = "./files";

byte buffer[BUFFER_SIZE];

void initCmd(cmd::commandParser& parser);

void nodeLogic(cmd::commandParser& parser);

void readFileFromNode(int sockfd);

bool runCommand(const cmd::commandParser& parser, cmd::commandResult& result);

bool readKeyFromManager(byte buffer[], int bufferSize, const byte flag, int& messageLen);

void commandSendFile(cmd::commandResult& result);

void commandCloseManager(cmd::commandResult& result);

void commandPrintK3(cmd::commandResult& result);

void commandPrintIv(cmd::commandResult& result);

int createServer();

bool initTCPClient(int& sd, const char* ipAddress, int port);

bool initUDPClient(int& sockfd, struct sockaddr_in& servAddr, int port);

void sendFileReceiver(const byte* key, int keyLen, byte mode, int sockfd);

void sendFileSender(const char *fileName, const byte* key, int keyLen, byte mode, int sockfd);

int main(int argc, char* argv[]) {
	keyManagerPort = validateArgs(argc, argv);
	
	cmd::commandParser parser;
	initCmd(parser);
	initFilesDirectory(filesDirectoryPath);

	nodeLogic(parser);
    return 0;
}

void initCmd(cmd::commandParser& parser) {
	parser.addCommand(cmd::commandId::SEND_FILE, "send", "sends a file to another node");
	parser.addCommandArgumentString(cmd::commandId::SEND_FILE, "file-name");
	parser.addCommandOptionString(cmd::commandId::SEND_FILE, "-ip:<nodeIP>", "none");
	parser.addCommandOptionNumber(cmd::commandId::SEND_FILE, "-port:<nodePort>", -1);
	parser.addCommandOptionString(cmd::commandId::SEND_FILE, "-mode:<encryptionMode>", "cfb");
	
	parser.addCommand(cmd::commandId::LISTALL, "list-cmd" ,"displays information about all the commands");
		
	parser.addCommand(cmd::commandId::PRINT_K3, "print-k3", "displays the k3 key");
	
	parser.addCommand(cmd::commandId::PRINT_IV, "print-iv", "displays the iv key");
		
	parser.addCommand(cmd::commandId::CLOSE, "close", "closes the application");
}

void nodeLogic(cmd::commandParser& parser) {
	nodePort = readIntFromConsole("Enter the port for this node: "); // se citeste portul nodului curent
	int sockfd = createServer(); // se creaza un server TCP
	if (sockfd == errno) {
		printf("Failed to create the server! Probably the chosen port is already used!\n");
		exit(1);
	}
	if (-1 == listen(sockfd, 20)) { // se seteaza numarul maxim de cereri care ajung in coada (20)
		printf("Error when calling listen()!\n");
		exit(1);
	}
	
	int maxFd = std::max(sockfd, fileno(stdin)) + 1; // functionalitate specifica functiei "select"
	fd_set readSet; // multimea de descriptori ce va fi verificata

	std::cout << "-----------------------------------\n";
	std::cout << "Olo project!\n";
	std::cout << "-----------------------------------\n";
	std::cout << parser; // se scriu comenzile
	bool running = true;
	printf("node@%d: ", nodePort);
	fflush(stdout);
	while (running) {
	
		FD_ZERO(&readSet);
		FD_SET(fileno(stdin), &readSet);
		FD_SET(sockfd, &readSet);
		// se seteaza descriptorii

		if (-1 == select(maxFd, &readSet, NULL, NULL, NULL)) { // se verifica daca sunt date in stdin/socket
			printf("Error when calling select()!\n");
			exit(1);
		}
		
		if (FD_ISSET(fileno(stdin), &readSet)) { // s-a introdus ceva de la tastatura
			// stdin
			std::string line;
			std::getline(std::cin, line); // se citeste comanda
			cmd::commandResult info = parser.parse(line); // se parseaza
			running = runCommand(parser, info); // se ruleaza comanda
			printf("node@%d: ", nodePort);
			fflush(stdout);
		}
		if (FD_ISSET(sockfd, &readSet)) { // mesaj nou pe socket
			// socket
			printf("\n[socket_server]: ");
			readFileFromNode(sockfd); // se citeste informatia de la peer
			printf("node@%d: ", nodePort);
			fflush(stdout);
		}		
	}
	
	close(sockfd); // se inchide descriptorul
}

void readFileFromNode(int sockfd) { // functie ce va citi fisierul pe blocuri si-l va decripta
	int client;
	struct sockaddr_in from; // adresa nodului ce trimite fisierul

	unsigned int fromLen = sizeof(from); // parametru in-out ce trebuie trimis la functia accept
    if ((client = accept (sockfd, (struct sockaddr *) &from, &fromLen)) < 0) { // eroare la crearea socket ului pt comunicare
    	printf("Error when calling accept(), in order to get a file from another node!\n");
		return;
    }
    
    byte startCommunicationFlag = FLAG_START;
    if (-1 == write(client, &startCommunicationFlag, sizeof(startCommunicationFlag))) {
    	printf("Failed to send the start communication flag to the other node!\n");
    	close(client);
    	return;
    }
    
    byte flag; // tipul criptarii
    int* len = (int*)buffer;
    
    if (-1 == read(client, &flag, sizeof(flag)) || // citesc tipul criptarii
    	-1 == read(client, &(*len), sizeof(*len)) || // citesc lungimea cheii
    	-1 == read(client, buffer + sizeof(*len), *len)) { // citesc cheie
    	printf("Failed to read the type of encryption or the encryption key from another node!\n");
    	close(client);
    	return;	
    }
    
	if (flag == FLAG_ECB) {
		printf("Encryption mode: \"ecb\"\n");
	} else {
		printf("Encryption mode: \"cfb\"\n");
	}

    byte key[keySize];
    int keyLen;
    if (!decrypt(buffer + sizeof(*len), *len, k3, iv, key, keyLen)) { // decriptez cheia primita de la nodul A
    	printf("Failed to decrypt the key received from a node that was supposed to send a file!\n");
    	close(client);
    	return;
    }
    
    sendFileReceiver(key, keyLen, flag, client); // intru in functia care se ocupa de citirea fisierului
    
    close(client);
}

// functie ce va rula comanda citita
bool runCommand(const cmd::commandParser& parser, cmd::commandResult& result) {
	switch (result.id) {
		case cmd::commandId::SEND_FILE:
			commandSendFile(result);
			break;
		case cmd::commandId::LISTALL:
      		std::cout<<parser;
      		break;
      	case cmd::commandId::PRINT_K3:
      		commandPrintK3(result);
      		break;
      	case cmd::commandId::PRINT_IV:
      		commandPrintIv(result);
      		break;
		case cmd::commandId::CLOSE:
			return false;
		case cmd::commandId::WOC:
			std::cout<<"Invalid command!\n";
			break;
		case cmd::commandId::WARG:
			std::cout<<"Invalid argument!\n";
			break;
		case cmd::commandId::WCARG:
			std::cout<<"Too few arguments!\n";
			break;
		case cmd::commandId::WOCOPT:
			std::cout<<"Invalid options for command!\n";
			break;
		default:
			break;
	}
	return true;
}

// functie care va citi o cheie criptata de la KM
bool readKeyFromManager(byte buffer[], int bufferSize, const byte flag, int& messageLen) {
	int sockfd;
    struct sockaddr_in servAddr;
    
    if (!initUDPClient(sockfd, servAddr, keyManagerPort)) { // creez un client UDP
    	printf("Failed to initialize a udp client that was supposed to be used to send a flag to the key manager!\n");
    	return false;
    }

	// trimit un flag ce indica ca doresc cheia
    if ((sendto(sockfd, &flag, sizeof(flag), MSG_CONFIRM, (const struct sockaddr*)&servAddr, sizeof(servAddr))) < 0) {
        printf("Failed to write the flag to the server!\n");
		return false;
    }

    unsigned int len = sizeof(servAddr);
	// citesc un mesaj UDP ce contine keyLen+key (concatenate)
    if ((messageLen = recvfrom(sockfd, buffer, bufferSize, MSG_WAITALL, (struct sockaddr*)&servAddr, &len)) < 0) {
        printf("Failed to read the message from the server!\n");
		return false;
    }
    
    return true;
}

// entry-point-ul pt logica in care nodul A incepe sa trimita un fisier unul alt nod
// nodul B este indicat printr-o pereche (ip, port)
void commandSendFile(cmd::commandResult& result) {
	std::string fileName = result.getStringArgumentValue("file-name");
	std::string nodeIp = result.getStringOptionValue("-ip");
	int nodePort = result.getNumberOptionValue("-port");
	std::string mode = result.getStringOptionValue("-mode");
	
	// Validari:
	if (fileName == "") { // nume de visier vid
		printf("You have to specify the name of the file that will be sent!\n");
		return;
	}
	if (nodeIp == "") { // ip vid
		printf("You have to specify the ip of the other node! (-ip option)\n");
		return;
	}
	if (nodePort == NUM_ARG_MISSING) { // port vid
		printf("You have to specify the port of the other node! (-port option)\n");
		return;
	}
	if (!fileExists(fileName.c_str())) { // numele de fisier indica un fisier care nu exista
		printf("Invalid file name! The file doesn't exist!\n");
		return;
	}
	if (!(mode == "ecb" || mode == "cfb")) { // modul indicat este invalid, trebuie specificat ori ECB ori CFB
		printf("Invalid mode! \"ecb\" or \"cfb\" expected!\n");
		return;
	}

	byte flag = KEY_MANAGER_GET_KEY; // flag-ul care trebuie trimis KM-ului
	byte modeFlag = (mode == "ecb" ? FLAG_ECB : FLAG_CFB);

	int messageLen;
	if (!readKeyFromManager(buffer, BUFFER_SIZE, flag, messageLen)) { // se citeste cheia de la KM
		return;
	}
    if (messageLen != *((int*)buffer) + sizeof(messageLen)) { // bufferul citit este invalid (eroare in timpul trimiterii pe retea)
		printf("Failed to read the initialization info from the key manager!\n");
		return;
    }
	
	int clientSd;
	if (!initTCPClient(clientSd, nodeIp.c_str(), nodePort)) { // creez un client TCP cu care voi comunica cu nodul B
		printf("Failed to initialize a tcp client (that is supposed to be used to send the file)!\n");
		return;
	}
	
	byte startFlag;
	if (-1 == read(clientSd, &startFlag, sizeof(startFlag))) {
		printf("Failed to read the start communication flag from the other node!\n");
		close(clientSd);
		return;
	}
	
	if (-1 == write(clientSd, &modeFlag, sizeof(modeFlag)) // ii trimit un flag ce indica tipul criptarii
		|| -1 == write(clientSd, buffer, messageLen)) { // si intreg bufferul pe care l-am citit de la KM
		printf("Failed to write the type of encryption or the encryption key to another node!\n");
    	close(clientSd);
		return;
	}
	
    byte key[keySize]; // cheia decriptata
    int keyLen;
    if (!decrypt(buffer + sizeof(messageLen), *((int*)buffer), k3, iv, key, keyLen)) { // decriptez cheia
    	printf("Failed to decrypt the key received from a node that was supposed to send a file!\n");
    	close(clientSd);
    	return;
    }
   
    sendFileSender(fileName.c_str(), key, keyLen, modeFlag, clientSd); // logica de citire a fisierului
	printf("File sent successfully!\n");
	
	close(clientSd);
}

// afiseaza cheia K3
void commandPrintK3(cmd::commandResult& result) {
	for (int i=0;i<ivSize;++i) {
		std::cout << int(iv[i]) << ' ';
	}
	std::cout << '\n';
}

// afiseaza vectorul de initializare iv
void commandPrintIv(cmd::commandResult& result) {
	for (int i=0;i<keySize;++i) {
		std::cout << int(k3[i]) << ' ';
	}
	std::cout << '\n';
}

// creaza un server TCP
int createServer() {
	struct sockaddr_in server;
	int sd, on = 1;

	if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1){
		perror ("[server]Error when creating the socket for the server!");
		return errno;
	}
	setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

	bzero (&server, sizeof (server));

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr ("127.0.0.1");
	server.sin_port = htons (nodePort);

	if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1) {
		perror ("[server]Error when binding the socket with the server!");
		return errno;
	}
	return sd;
}

// va initializa un client TCP
bool initTCPClient(int& sd, const char* ipAddress, int port) {
	struct sockaddr_in client;
	if (-1 == (sd = socket(AF_INET, SOCK_STREAM, 0))){
		perror("[client]Error when creating a socket!");
		return false;
	}
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr(ipAddress);
	client.sin_port = htons (port);
	if (-1 == connect (sd, (struct sockaddr *) &client,sizeof (struct sockaddr))){
		std::string errorMsg = std::string("[client]Error when connecting to peer!") + std::string(ipAddress) + std::string(", ") + std::to_string(port);
		perror(errorMsg.c_str());
		close(sd);
		return false;
	}
	return true;
}

// va initializa un client UDP
bool initUDPClient(int& sockfd, struct sockaddr_in& servAddr, int port) {
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    	printf("Failed to create the socket!\n");
    	return false;
    }
    
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons((unsigned short)port);
    servAddr.sin_addr.s_addr = INADDR_ANY;
    
    return true;
}

// logica pt nodul B
// se citesc succesiv blocuri de 128 de biti din socket-ul sockfd
// se decripteaza cu cheia "key" (modul "mode")
// si se scriu intr-un fisier
void sendFileReceiver(const byte* key, int keyLen, byte mode, int sockfd) {
	if (mode != FLAG_ECB && mode != FLAG_CFB) { // modul este invalid
		printf("Invalid mode!\n");
		return;
	}
	std::string modeStr = (mode == FLAG_ECB ? "ecb" : "cfb");
	int fileNameLen;
	std::string fileName;
	if (-1 == read(sockfd, &fileNameLen, sizeof(fileNameLen))) {
		printf("Failed to read the length of the file name from the other node!\n");
		return;
	}
	// primul lucru pe care-l va trimite nodul A este numele fisierului, citit aici
	fileName.resize(fileNameLen);
	for (int i=0;i<fileNameLen;++i) {
		char ch;
		if (-1 == read(sockfd, &ch, sizeof(ch))) {
			printf("Failed to read the %dth byte of the file name!\n", i+1);
			return;
		}
		fileName[i] = ch;
	}
	char *fileNameStr = strdup(fileName.c_str());
	char *fileBaseName = basename(fileNameStr);
	fileName = std::string(filesDirectoryPath) + std::string("/") + std::string(fileBaseName);
	std::string encryptedFileName = std::string(filesDirectoryPath) + std::string("/") + modeStr
		+ std::string("_") + std::string(fileBaseName);
	free(fileNameStr);
	
	// se creaza 2 fisiere, unul in care vor fi pusi bitii, iar in celalalt va fi scris continutul decriptat
	FILE *fileOut = fopen(fileName.c_str(), "w");
	FILE *encryptedFileOut = fopen(encryptedFileName.c_str(), "w");
	
	byte rawBlock[BLOCK_SIZE_BYTES]; // blocul citit din socket
	byte decBlock[BLOCK_SIZE_BYTES]; // buffer in care va fi pus un block decriptat
	byte block[BLOCK_SIZE_BYTES]; // block utilizat pe CFB
	memcpy(block, iv, ivSize);
	// initial in block se pune iv (conform schemei indicate in acest document: https://profs.info.uaic.ro/~liliana.cojocaru/Laborator2.pdf)
	// apoi se copia, pe rand, fiecare ciphertext citit din socket
	int blocksNum = 0;
	while (true) {
		++blocksNum; // contorizez numarul de blocuri
		int blockSize;
		if (-1 == read(sockfd, &blockSize, sizeof(blockSize))) { // citesc size-ul blocului
			printf("Failed to read the size of the %d'th block!\n", blocksNum);
			break;
		}
		// blockSize poate fi si un numar negativ (-1 sau -2), ceea ce indica o eroare in timpul trimierii din nodul A
		// sau faptul ca au fost trimise toate blocurile
		if (blockSize == FLAG_FILE_SUCCESS) {
			printf("Received a file! Path: %s\n", fileName.c_str());
			break;
		}
		if (blockSize == FLAG_FILE_FAIL) {
			printf("The other node had an error while sending the file!\n");
			break;
		}
		if (blockSize != BLOCK_SIZE_BYTES) {
			printf("Invalid block size received! Expected only %d bytes blocks, received a %d bytes block!\n", BLOCK_SIZE_BYTES, blockSize);
			exit(1);
		}
		int actualBlockSize = 0; // ultimul block e posibil sa fi fost padded, deci voi scrie in fisier numai "actualBlockSize" bytes
		if (-1 == read(sockfd, &actualBlockSize, sizeof(actualBlockSize)) || // citesc nr de bytes care trebuie scrisi in fisier
			-1 == read(sockfd, rawBlock, blockSize)) { // si block-ul criptat
			printf("Failed to read the %d'th block!\n", blocksNum);
			break;
		}
		for (int i=0;i<blockSize;++i) {
			fprintf(encryptedFileOut, "%d ", int(rawBlock[i])); // se scrie block-ul in fisier
		}
		fprintf(encryptedFileOut, "\n");
		int decBlockLen;
		if (mode == FLAG_ECB) { // daca modul ales a fost ECB
			if (!decrypt(rawBlock, blockSize, key, iv, decBlock, decBlockLen)) { // se decripteaza block-ul
				printf("Failed to decrypt the %d'th block of the file!\n", blocksNum);
				break;
			}
		} else if (mode == FLAG_CFB) { // daca modul ales a fost CFB
			if (!encrypt(block, blockSize, key, iv, decBlock, decBlockLen)) { // se cripteaza block-ul "block"
				printf("Failed to decrypt the %d'th block of the file!\n", blocksNum);
				break;
			}
			for (int i=0;i<decBlockLen;++i) {
				decBlock[i] ^= rawBlock[i]; 
				// block-ul decriptat se obtine prin aplicarea XOR-ului intre block-ul "block" criptat
				// si block-ul citit din socket
			}
			memcpy(block, rawBlock, blockSize); // block-ul citit in socket va deveni block-ul "block" (pentru urmatoarea iteratie)
		}
		int written = fwrite(decBlock, sizeof(byte), actualBlockSize, fileOut); // se scrie block-ul decriptat in fisier
		if (written != actualBlockSize) {
			printf("Failed to write the decrypted %d'th block to disk!\n", blocksNum);
		}
	}
	fclose(encryptedFileOut);
	fclose(fileOut);
}

// logica principala pentru nodul A
// se vor citi succesiv block-uri de 128 de biti din fisierul cu numele indicat de sirul de caractere
// "fileName", block-urile vor fi criptate cu cheia "key"
// modul de criptare va fi "mode"
void sendFileSender(const char *fileName, const byte* key, int keyLen, byte mode, int sockfd) {
	if (mode != FLAG_ECB && mode != FLAG_CFB) { // modul este invalid
		printf("Invalid mode!\n");
		return;
	}
	int fileNameLen = strlen(fileName); 
	// se trimite numele fisierului nodului B (pentru ca acesta sa-l poata scrie pe disk intr-o
	// locatie cu nume sugestiv)
	if (-1 == write(sockfd, &fileNameLen, sizeof(fileNameLen)) || // lungimea numelui fisierului
		-1 == write(sockfd, fileName, fileNameLen)) { // numele fisierului
		printf("Failed to send the filename to the other node!\n");
		return;	
	}
	FILE *fileIn = fopen(fileName, "r"); // se deschide fisierul
	byte rawBlock[BLOCK_SIZE_BYTES]; // block-ul citit din fisier (plaintext)
	byte encBlock[BLOCK_SIZE_BYTES]; // buffer in care va fi puse block-urile criptate
	byte block[BLOCK_SIZE_BYTES]; // buffer utilizat pentru modul CFB conform: https://profs.info.uaic.ro/~liliana.cojocaru/Laborator2.pdf
	memcpy(block, iv, ivSize); 
	int blocksNum = 0;
	bool failed = false;
	while (true) {
		++blocksNum;
		int count = fread(rawBlock, sizeof(byte), BLOCK_SIZE_BYTES, fileIn);
		if (!count) {
			break;
		}
		for (int i=count;i<BLOCK_SIZE_BYTES;++i) {
			rawBlock[i] = 0;
		}
		int encBlockLen;
		if (mode == FLAG_ECB) {
			// daca modul de criptare e ECB, pur si simplu se cripteaza block-ul
			if (!encrypt(rawBlock, BLOCK_SIZE_BYTES, key, iv, encBlock, encBlockLen)) {
				printf("Failed to encrypt the %d'th block of the file!\n", blocksNum);
				failed = true;
				break;
			}
		} else if (mode == FLAG_CFB) {
			// daca modul de criptare e CFB, se cripteaza ceea ce e in block-ul block
			if (!encrypt(block, BLOCK_SIZE_BYTES, key, iv, encBlock, encBlockLen)) {
				printf("Failed to encrypt the %d'th block of the file!\n", blocksNum);
				failed = true;
				break;
			}
			// iar cipherText-ul se obtine facand XOR intre rawBlock si rezultatul criptat obtinut
			for (int i=0;i<encBlockLen;++i) {
				encBlock[i] ^= rawBlock[i];
			}
			memcpy(block, encBlock, sizeof(encBlock)); // si se actualizeaza buffer-ul block
		}
		// nodului B se trimit: lungimea block-ului, numarul de bytes care trebuie scrisi in fisier
		// si block-ul criptat
		if (-1 == write(sockfd, &encBlockLen, sizeof(encBlockLen)) ||
			-1 == write(sockfd, &count, sizeof(count)) ||
			-1 == write(sockfd, encBlock, encBlockLen)) {
			printf("Failed to send the %d'th block of the file!\n", blocksNum);
			failed = true;
			break;
		}
		if (count < BLOCK_SIZE_BYTES) {
			break; // daca nu am un block complet de 128 de biti, inseamna ca am facut padding la final
			// deci e ultimul block, se opreste criptarea
		}
	}
	int endFlag = FLAG_FILE_SUCCESS;
	if (failed) {
		endFlag = FLAG_FILE_FAIL;
	}
	// se trimite un flag care indica daca trimiterea a esuat in vreun punct
	if (-1 == write(sockfd, &endFlag, sizeof(endFlag))) {
		printf("Failed to send the final flag to the other node! (%d)\n", endFlag);
	}
	fclose(fileIn);
}
