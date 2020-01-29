#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <libgen.h>
#include <stdarg.h>
#include <string>

#define PASSWORD_MOD 								666013LL
#define PASSWORD_BASE 								261
#define INFINITY									2000000000
#define MAX_LEN 									256
#define CONSOLE_LINE_MAX_LEN 						544
#define	RESPONSE_MAX_ALLOCATE_LEN					8192
#define READ_OK 									0
#define READ_TOO_LONG 								1
#define READ_NO_INPUT 								2
#define READ_ERROR 									3
#define OK 											0
#define NO 											1
#define ERROR 										1
#define INVALID 									-1
#define NOT_FOUND									-2
#define PIPES_CMDS_MAX_NO_PIPES						100
#define PIPES_CMDS_MAX_LEN_DESC_STR					15

#define COMMAND_REQUIRE_LOG_IN 						1u
#define COMMAND_NOT_DISPLAYED_WHEN_LOGGED_IN	 	2u
#define COMMAND_HAS_ARGS							4u

#define uint 										unsigned int
#define uchar										unsigned char
#define ll 											long long

uint 	flagStop = 4294967295;
uint 	flagGetMessageFromUser = 4294967294;
uint 	flagPrintMessage = 4294967293;

void 	parentLogic(int fdSocketPairParentChild, int fdFifoChildParent);
void 	printResponseFromChannel(int fdFifoChildParent, uint responseLen);

void 	childLogic(int fdSocketPairParentChild, int fdFifoChildParent);
void	writeResponseToParent(int fdSocketPairParentChild, int fdFifoChildParent);
void 	executePipeCommand(char *command, char* parsedParameters[PIPES_CMDS_MAX_NO_PIPES], char functionCallsNumbers[PIPES_CMDS_MAX_LEN_DESC_STR]);

void 	communicationPrintMessage(int fdSocketPairParentChild, int fdFifoChildParent, const char* message);
void	communicationGetInput(int fdSocketPairParentChild, int fdFifoChildParent, const char* prompt, char *buffer, uint bufferSize);

void 	printErrorMessageAndExitApplicationParent();
void 	printErrorMessageAndExitApplicationChild(int fdSocketPairParentChild, int fdFifoChildParent);
int 	readLine(const char *prompt, char *buff, const uint sz);

const char* 	usersFileName = ".users.txt";
const char* 	internalErrorMessage = "Internal error!";
const char* 	whiteSpaces = " \t\n";
const char* 	usersDirectory = "users/";
const char*		fifoFileName=".channel";

char 			pipesCommandsFile[] = "./build/pipesCmd.bin";
char 			allowedCharacters[MAX_LEN];

const uint 		bufferSize = CONSOLE_LINE_MAX_LEN;

uint 	min(uint a, uint b) { return (a < b ? a : b); }
void 	init();
void	initCommandsArgs();
void 	initAllowedCharacters();
void	uninit();

int 	getHashFromText(char *text);
int		textToLowercase(char *text);
int 	textStartsAndMatchesString(const char *str, const char *text);
int 	textContainsAllowedCharacters(char *text);

#define 		NUMBER_OF_COMMANDS 		15
#define 		COMMAND_REGISTER 		0
#define			COMMAND_LOGIN			1
#define			COMMAND_LOGOUT			2
#define 		COMMAND_MYFIND			3
#define 		COMMAND_MYSTAT			4
#define 		COMMAND_MYCREATE		5
#define 		COMMAND_MYLS			6
#define 		COMMAND_MYCAT			7
#define			COMMAND_MYEDIT			8
#define			COMMAND_MYRM			9
#define 		COMMAND_MYMKDIR			10
#define 		COMMAND_LISTUSERS		11
#define 		COMMAND_RUN				12
#define 		COMMAND_CMD				13
#define 		COMMAND_QUIT 			14
int 			loggedIn, currentUsernameLen;
char 			currentUsername[CONSOLE_LINE_MAX_LEN];
char			currentPath[CONSOLE_LINE_MAX_LEN];
const uint 		numberOfCommands = NUMBER_OF_COMMANDS;
const uint		commandsCommandIndex = COMMAND_CMD;
const uint		lsCommandIndex = COMMAND_MYLS;
const uint		findCommandIndex = COMMAND_MYFIND;
const char* 	commands[15] =
{ 	
	"register",
	"login",
	"logout",
	"myfind",
	"mystat",
	"mycreate",
	"myls",
	"mycat",
	"myedit",
	"myrm",
	"mymkdir",
	"list-users",
	"run",
	"cmd",
	"quit"
};
uint 			commandsFlags[NUMBER_OF_COMMANDS] = 
{
	COMMAND_NOT_DISPLAYED_WHEN_LOGGED_IN,
	COMMAND_NOT_DISPLAYED_WHEN_LOGGED_IN,
	COMMAND_REQUIRE_LOG_IN,
	COMMAND_REQUIRE_LOG_IN | COMMAND_HAS_ARGS,
	COMMAND_REQUIRE_LOG_IN | COMMAND_HAS_ARGS,
	COMMAND_REQUIRE_LOG_IN | COMMAND_HAS_ARGS,
	COMMAND_REQUIRE_LOG_IN | COMMAND_HAS_ARGS,
	COMMAND_REQUIRE_LOG_IN | COMMAND_HAS_ARGS,
	COMMAND_REQUIRE_LOG_IN | COMMAND_HAS_ARGS,
	COMMAND_REQUIRE_LOG_IN | COMMAND_HAS_ARGS,
	COMMAND_REQUIRE_LOG_IN | COMMAND_HAS_ARGS,
	COMMAND_NOT_DISPLAYED_WHEN_LOGGED_IN,
	COMMAND_REQUIRE_LOG_IN | COMMAND_HAS_ARGS,
	COMMAND_HAS_ARGS,
	0
};
const char**	commandsArgs[NUMBER_OF_COMMANDS];
uint			commandsArgsCount[] = 
	{0, 0, 0, 3, 1, 1, 2, 2, 2, 1, 1, 0, 2, 1, 0};

int 	confirmPrompt(int fdSocketPairParentChild, int fdFifoChildParent, const char *message);
int 	canRunCommand(int commandIndex);
int 	getCommandIndex(char *command);
int 	getCommandFromBuffer(char *buffer, char *command, uint commandBufferSize, uint *commandSize);
int 	getCommandArgsFromBuffer(char *buffer, char *args, uint argsBufferSize, uint commandSize);
int		commandHasArg(uint commandIndex, const char* args, const char* arg);
int 	getArgNumberValue(const char* args, const char* arg);
int		getArgStringValue(const char* args, const char* arg, char* dest);
int 	leaveOneCommandArg(char *args);

const uint		commandMyEditEditorsCount = 4;
const char*		commandMyEditEditors[4] = 
{
	"vim",
	"nano",
	"emacs",
	"joe"
};

#define 		LANGUAGE_CPP			0
#define 		LANGUAGE_C				1
#define 		LANGUAGE_PYTHON			2
const uint		commandRunLanguagesCount = 3;
const char*		commandRunLanguages[3] = 
{
	"c++",
	"c",
	"python"
};

std::string response;
char*		responseBuffer;
int 		addFormattedStringToResponse(const char *format, ...);

int 	userExists(const char* username, int* passwordHash);

void 	commandRegister(int fdSocketPairParentChild, int fdFifoChildParent);
int 	registerUser(char *username, char *password);

void 	commandLogin(int fdSocketPairParentChild, int fdFifoChildParent);
int 	enteredCorrectUsername(int fdSocketPairParentChild, int fdFifoChildParent, char *username, int *passwordHash);
int 	enteredCorrectPassword(int fdSocketPairParentChild, int fdFifoChildParent, int *correctPasswordHash);
int 	createFolderForUser(char *username);

void 	commandLogout(int fdSocketPairParentChild, int fdFifoChildParent);

int 	fileExists(const char *fileName);

void 	commandMyFind(char* rootDirectory, const char* fileName, int detailed);
int 	findFile(char* currentPath, const char* fileName, int detailed);

void 	commandMyStat(const char* fileName);
int		fileStat(const char *file, int buildPath, int printPerm, int printFullName);

void 	commandMyCreate(int fdSocketPairParentChild, int fdFifoChildParent, const char* fileName);

void 	commandMyLs(const char* directoryPath, int recursive);
int 	fileIsDirectory(const char *file, int buildPath);
void 	printDirectoryFiles(const char* directoryPath, int recursive);

void 	commandMyCat(const char* fileName, int numberOfLines);
void 	commandMyEdit(const char* fileName, const char* editorName);
void 	commandMyRm(int fdSocketPairParentChild, int fdFifoChildParent, const char* fileName);
void	commandMyMkdir(const char* dirName);
int		createDirectory(const char* dirName, int printAlreadyExistsMessage);
void 	commandShowAll(int showDetails);
void 	commandListUsers();

void 	commandRun(const char* program, const char* programmingLanguage);
void 	runCorCpp(const char* program, const char* compiler);
int 	runCompiledCorCppProgram(const char* compiledFileName);
void 	runPython(const char* program);
int		compileFile(const char* file, const char* compiler, const char* compiledFileName);
char* 	getExecutableFileName(const char* file);
char* 	getFileToCompileName(const char* file);

// -----------------------------------------------------
// Functii utilizate pentru prelucrarea sirului de caractere ce contine
// o secventa de comenzi, despartite de pipe uri (|)
int 	nrc(int x);
void 	setNums(char *s, int nr1, int nr2);
void 	compressSpacesString(char *s);
void 	changeSpaces(char *s);
// -----------------------------------------------------

int main(int argc, char* argv[])
{	
	int		fdSocketPair[2], fdFifo[2];
	pid_t 	pid;

	if (-1 == socketpair(AF_UNIX, SOCK_STREAM, 0, fdSocketPair) ||
		(-1 == mkfifo(fifoFileName, 0666) && errno != EEXIST))
	{
		fprintf(stderr,"%s Try running the program again..\n", internalErrorMessage);
		return 1;
	}

	if (-1 == (fdFifo[0] = open(fifoFileName, O_RDONLY | O_NONBLOCK)))
	{
		fprintf(stderr,"Failed to open fifo %s! Try running the program again..\n", fifoFileName);
		return 1;
	}

	fdFifo[1] = open(fifoFileName, O_WRONLY);

	int readFifoFlags;
	if (-1 == (readFifoFlags = fcntl(fdFifo[0], F_GETFL,0)))
	{
		fprintf(stderr, "Failed to get fifo read descriptor options! Try running the program again..\n");
		return 1;
	}
	readFifoFlags ^= O_NONBLOCK;
	if (-1 == fcntl(fdFifo[0], F_SETFL, readFifoFlags))
	{
		fprintf(stderr, "Failed to set new options for fifo read descriptor! Try running the program again..\n");
		return 1;
	}

	if (-1 == (pid = fork()))
	{
		fprintf(stderr,"%s Try running the program again..\n", internalErrorMessage);
		return 1;
	}

	if (pid)
	{
		close(fdSocketPair[1]);
		close(fdFifo[1]);
		parentLogic(fdSocketPair[0],fdFifo[0]);
	}
	else
	{
		close(fdSocketPair[0]);
		close(fdFifo[0]);
		childLogic(fdSocketPair[1],fdFifo[1]);
	}

	return 0;
}

void parentLogic(int fdSocketPairParentChild, int fdFifoChildParent)
{
	int			consoleRunning = 1, res;
	uint		responseLen, bufferLen;
	char 		buffer[bufferSize];
	uchar		flagResponse;

	read(fdFifoChildParent, &responseLen, sizeof(responseLen));
	printf("\033[1;36m");
	printResponseFromChannel(fdFifoChildParent, responseLen);

	while (consoleRunning)
	{
		printf("\033[1;36m");
		res = readLine("cmd: ", buffer, bufferSize);
		printf("\033[0m");
		if (res == READ_OK)
		{
			bufferLen = strlen(buffer);

			if (-1 == write(fdSocketPairParentChild, &bufferLen, sizeof(int)) 
			 || -1 == write(fdSocketPairParentChild, buffer, bufferLen))
				printErrorMessageAndExitApplicationParent();
				
			while (1)
			{
				if (-1 == read(fdFifoChildParent, &responseLen, sizeof(responseLen)))
					printErrorMessageAndExitApplicationParent();

				if (responseLen == flagStop)
				{
					consoleRunning = 0;
					break;
				}
				if (responseLen == flagGetMessageFromUser)
				{
					if (-1 == read(fdFifoChildParent, &responseLen, sizeof(responseLen)))
						printErrorMessageAndExitApplicationParent();
					printf("\033[1;34m");
					printResponseFromChannel(fdFifoChildParent, responseLen);
					printf("\033[1;36m");
					res = readLine(NULL, buffer, bufferSize);
					printf("\033[0m");
					bufferLen = strlen(buffer);
					if (-1 == write(fdSocketPairParentChild, &bufferLen, sizeof(int))
						|| -1 == write(fdSocketPairParentChild, buffer, bufferLen))
						printErrorMessageAndExitApplicationParent();
					continue;
				}
				if (responseLen == flagPrintMessage)
				{
					if (-1 == read(fdFifoChildParent, &responseLen, sizeof(responseLen)))
						printErrorMessageAndExitApplicationParent();
					printf("\033[1;34m");
					printResponseFromChannel(fdFifoChildParent, responseLen);
					bufferLen = 0;
					if (-1 == write(fdSocketPairParentChild, &bufferLen, sizeof(int)))
						printErrorMessageAndExitApplicationParent();
					continue;
				}
				printResponseFromChannel(fdFifoChildParent, responseLen);
				break;
			}
		}
	}
}
void printResponseFromChannel(int fdFifoChildParent, uint responseLen)
{
	char 	ch;
	int 	tr;
	for (tr=0;tr<responseLen;++tr)
	{
		read(fdFifoChildParent, &ch, 1);
		printf("%c", ch);
	}
}

void childLogic(int fdSocketPairParentChild, int fdFifoChildParent)
{
	int 		cmdIndex, res, running = 1, parameterValue, correctArgs, recursive, detailed;
	uint		currentCommandSize, tr;
	char 		buffer[bufferSize];
	char 		currentCommand[bufferSize];
	char 		currentCommandArgs[bufferSize];
	char 		stringCommandArg[bufferSize];
	uchar		flagResponse;
	char*		parsedParameters[PIPES_CMDS_MAX_NO_PIPES];
	char 		functionCallsNumbers[PIPES_CMDS_MAX_LEN_DESC_STR];

	init();

	addFormattedStringToResponse("!!! --- OLO Console --- !!!\n");
	commandShowAll(0);
	addFormattedStringToResponse("--------------------------\n");
	writeResponseToParent(fdSocketPairParentChild, fdFifoChildParent);

	while (running)
	{
		if (-1 == read(fdSocketPairParentChild, &tr, sizeof(tr))
			|| -1 == read(fdSocketPairParentChild, buffer, tr))
			printErrorMessageAndExitApplicationChild(fdSocketPairParentChild, fdFifoChildParent);

		buffer[tr] = '\0';
		
		response.clear();

		res = getCommandFromBuffer(buffer, currentCommand, bufferSize, &currentCommandSize);
		if (res != OK)
			addFormattedStringToResponse("%s\n", internalErrorMessage);
		else
		{
			cmdIndex = getCommandIndex(currentCommand);
			if (cmdIndex == INVALID)
			{
				if (!loggedIn)
					addFormattedStringToResponse("You have to log in in order to run commands!\n");
				else
				{
					printf("\033[0m");
					executePipeCommand(buffer, parsedParameters, functionCallsNumbers);
				}
			}
			else
			{
				correctArgs = 0;
				if (commandsFlags[cmdIndex] & COMMAND_HAS_ARGS)
				{
					res = getCommandArgsFromBuffer(buffer, currentCommandArgs, bufferSize, currentCommandSize);
					if (res != OK)
						addFormattedStringToResponse("%s\n", internalErrorMessage);
					else 
						correctArgs = 1;
				} 
				else correctArgs = 1;
				
				if (correctArgs)
				{
					switch(cmdIndex)
					{
					case COMMAND_REGISTER:
						commandRegister(fdSocketPairParentChild, fdFifoChildParent);
						break;
					case COMMAND_LOGIN:
						commandLogin(fdSocketPairParentChild, fdFifoChildParent);
						break;
					case COMMAND_LOGOUT:
						commandLogout(fdSocketPairParentChild, fdFifoChildParent);
						break;
					case COMMAND_MYFIND:
						detailed = commandHasArg(COMMAND_MYFIND, currentCommandArgs, commandsArgs[findCommandIndex][1]) == OK ? 1 : 0;
						res = getArgStringValue(currentCommandArgs, "-dir:", stringCommandArg);
						if (res == -1) break;
						leaveOneCommandArg(currentCommandArgs);
						if (res == NOT_FOUND)
							commandMyFind(NULL, currentCommandArgs, detailed);
						else
							commandMyFind(stringCommandArg, currentCommandArgs, detailed);
						break;
					case COMMAND_MYSTAT:
						leaveOneCommandArg(currentCommandArgs);
						commandMyStat(currentCommandArgs);
						break;
					case COMMAND_MYCREATE:
						leaveOneCommandArg(currentCommandArgs);
						commandMyCreate(fdSocketPairParentChild, fdFifoChildParent, currentCommandArgs);
						break;
					case COMMAND_MYLS:
						recursive = commandHasArg(COMMAND_MYLS, currentCommandArgs, commandsArgs[lsCommandIndex][1]) == OK ? 1 : 0;
						res = getArgStringValue(currentCommandArgs, "-dir:", stringCommandArg);
						if (res == -1) break;
						if (res == NOT_FOUND)
							commandMyLs(NULL, recursive);
						else
						if (res == OK)
							commandMyLs(stringCommandArg, recursive);
						break;
					case COMMAND_MYCAT:
						parameterValue = getArgNumberValue(currentCommandArgs, "-first:");
						if (parameterValue == -1) break;
						leaveOneCommandArg(currentCommandArgs);
						commandMyCat(currentCommandArgs, parameterValue);
						break;
					case COMMAND_MYEDIT:
						res = getArgStringValue(currentCommandArgs, "-editor:", stringCommandArg);
						leaveOneCommandArg(currentCommandArgs);
						if (res == -1) break;
						if (res == NOT_FOUND)
							commandMyEdit(currentCommandArgs, "vim");
						else
						if (res == OK)
							commandMyEdit(currentCommandArgs, stringCommandArg);
						break;
					case COMMAND_MYRM:
						leaveOneCommandArg(currentCommandArgs);
						commandMyRm(fdSocketPairParentChild, fdFifoChildParent, currentCommandArgs);
						break;
					case COMMAND_MYMKDIR:
						leaveOneCommandArg(currentCommandArgs);
						if (!(*currentCommandArgs))
							addFormattedStringToResponse("The directory name has to have at least one character!\n");
						else
							commandMyMkdir(currentCommandArgs);
						break;
					case COMMAND_LISTUSERS:
						commandListUsers();
						break;
					case COMMAND_RUN:
						res = getArgStringValue(currentCommandArgs, "-lang:", stringCommandArg);
						if (res == -1) break;
						if (res == NOT_FOUND)
							addFormattedStringToResponse("You have to specify the program language!\n");
						else
						{
							leaveOneCommandArg(currentCommandArgs);
							printf("\033[0m");
							commandRun(currentCommandArgs, stringCommandArg);
						}
						break;
					case COMMAND_CMD:
						commandShowAll((commandHasArg(COMMAND_CMD, currentCommandArgs, commandsArgs[commandsCommandIndex][0]) == OK) ? 1 : 0);
						break;
					case COMMAND_QUIT:
						running = 0;
						write(fdFifoChildParent, &flagStop, sizeof(flagStop));
						break;
					}
				}
			}
		}
		if (running)
			writeResponseToParent(fdSocketPairParentChild, fdFifoChildParent);
	}

	uninit();
}
void writeResponseToParent(int fdSocketPairParentChild, int fdFifoChildParent)
{
	unsigned int responseLen = response.size();
	if (-1 == write(fdFifoChildParent, &responseLen, sizeof(responseLen))
		|| -1 == write(fdFifoChildParent, response.c_str(), responseLen))
		printErrorMessageAndExitApplicationChild(fdSocketPairParentChild, fdFifoChildParent);
}
void executePipeCommand(char *command, char* parsedParameters[PIPES_CMDS_MAX_NO_PIPES], char functionCallsNumbers[PIPES_CMDS_MAX_LEN_DESC_STR])
{
	int fdPipes[2];
	if (-1 == pipe(fdPipes))
	{
		addFormattedStringToResponse("Error when calling pipe() in process spawner child!\n");
		return;
	}
	compressSpacesString(command);
	int nr1 = 1, nr2 = 0, act = 2, returnCode;
	char *point = strtok(command, "|");
	pid_t pid;

	while (point)
	{
		changeSpaces(point);
		parsedParameters[act++] = point;
		++nr2;
		point = strtok(NULL, "|");
	}
	if (!nr2) nr2 = 1;
	
	memset(functionCallsNumbers, 0, PIPES_CMDS_MAX_LEN_DESC_STR);
	setNums(functionCallsNumbers, nr1, nr2);
	parsedParameters[0] = pipesCommandsFile;
	parsedParameters[1] = functionCallsNumbers;
	parsedParameters[act] = NULL;
	
	if (-1 == (pid = fork()))
	{
		addFormattedStringToResponse("Error when calling fork() in process spawner child!\n");
		return;
	}
	if (pid == 0)
	{
		close(fdPipes[0]);
		close(1);
	
		if (-1 == dup(fdPipes[1]))
			addFormattedStringToResponse("Error when calling dup2() in process spawner child!\n");
		else
		{
			close(fdPipes[1]);
			execv(pipesCommandsFile, parsedParameters);
			perror("Could not run the recursive file!\n");
			exit(125);
		}
	}
	
	close(fdPipes[1]);
	wait(&returnCode);
	if ((returnCode >> 8) != 0)
		addFormattedStringToResponse("Your command generated some errors!\n");
	
	int rLen = 0, codR;
	char ch;
	std::string response;
	while (1)
	{
		codR = read(fdPipes[0],&ch,1);
		if (codR == -1) break;
		if (codR == 0) break;
		if (ch == '\0') break;
		response.push_back(ch);
		++rLen;
	}
	addFormattedStringToResponse("%s",response.c_str());
	close(fdPipes[0]);
}

void communicationPrintMessage(int fdSocketPairParentChild, int fdFifoChildParent, const char* message)
{
	uint response, messageLen = strlen(message);
	if (-1 == write(fdFifoChildParent, &flagPrintMessage, sizeof(flagPrintMessage))
		|| -1 == write(fdFifoChildParent, &messageLen, sizeof(messageLen))
		|| -1 == write(fdFifoChildParent, message, messageLen))
		printErrorMessageAndExitApplicationChild(fdSocketPairParentChild, fdFifoChildParent);

	read(fdSocketPairParentChild, &response, sizeof(response));
}

void communicationGetInput(int fdSocketPairParentChild, int fdFifoChildParent, const char* prompt, char *buffer, uint bufferSize)
{
	char ch;
	uint responseLen, promptLen = strlen(prompt), tr;
	if (-1 == write(fdFifoChildParent, &flagGetMessageFromUser, sizeof(flagGetMessageFromUser))
		|| -1 == write(fdFifoChildParent, &promptLen, sizeof(promptLen))
		|| -1 == write(fdFifoChildParent, prompt, promptLen))
		printErrorMessageAndExitApplicationChild(fdSocketPairParentChild, fdFifoChildParent);

	read(fdSocketPairParentChild, &responseLen, sizeof(responseLen));
	for (tr=0;tr<responseLen;++tr)
	{
		read(fdSocketPairParentChild, &ch, 1);
		if (tr < bufferSize-1)
			buffer[tr] = ch;
		else
		if (tr == bufferSize-1)
			buffer[tr] = '\0';
	}
	if (responseLen < bufferSize)
		buffer[responseLen] = '\0';
}
void printErrorMessageAndExitApplicationParent()
{
	printf("Error when calling a C Unix function! The program execution will be stopped!\n");
	exit(1);
}
void printErrorMessageAndExitApplicationChild(int fdSocketPairParentChild, int fdFifoChildParent)
{
	communicationPrintMessage(fdSocketPairParentChild, fdFifoChildParent, "Error when calling a C Unix function! The program execution will be stopped!\n");
	exit(2);
}

// https://stackoverflow.com/questions/4023895/how-do-i-read-a-string-entered-by-the-user-in-c
// aceasta functie a fost luata de la aceasta intrebare de pe stack overflow
int readLine(const char *prompt, char *buff, const uint sz)
{
	if (buff == NULL) return READ_ERROR;
	char 		ch;
	uint 		extra;
	if (prompt)
	{
		printf("%s",prompt);
		fflush(stdout);
	}
	if (fgets(buff, sz, stdin) == NULL)
		return READ_NO_INPUT;
	if (buff[strlen(buff)-1] != '\n') 
	{
		extra = 0;
		while (((ch = getchar()) != '\n') && (ch != EOF))
			extra = 1;
		return (extra == 1) ? READ_TOO_LONG : READ_OK;
	}
	buff[strlen(buff)-1] = '\0';
	return READ_OK;
}
// https://stackoverflow.com/questions/4023895/how-do-i-read-a-string-entered-by-the-user-in-c

void init()
{
	responseBuffer = (char*)(malloc(RESPONSE_MAX_ALLOCATE_LEN));
	if (responseBuffer == NULL)
	{
		perror("Failed to allocate memory for storing responses in child!");
		exit(99);
	}
	initAllowedCharacters();
	initCommandsArgs();
}

void initAllowedCharacters()
{
	allowedCharacters[(uint)'_'] = 1;
	for (uint tr = (uint)'a'; tr <= (uint)'z'; ++tr)
		allowedCharacters[tr] = 1;
	for (uint tr = (uint)'A'; tr <= (uint)'Z'; ++tr)
		allowedCharacters[tr] = 1;
	for (uint tr = (uint)'0'; tr <= (uint)'9'; ++tr)
		allowedCharacters[tr] = 1;
}

void initCommandsArgs()
{
	for (uint i=0;i<numberOfCommands;++i)
		if (commandsFlags[i] & COMMAND_HAS_ARGS)
		{
			commandsArgs[i] = (const char**)malloc(commandsArgsCount[i] * sizeof(const char*));
			if (commandsArgs[i] == NULL)
			{
				perror("Failed to allocate memory for storing information about commands args!");
				exit(0);
			}
			for (uint tr = 0; tr < commandsArgsCount[i]; ++tr)
			{
				commandsArgs[i][tr] = (const char*)(malloc(sizeof(const char*)));
				if (commandsArgs[i][tr] == NULL)
				{
					perror("Failed to allocate memory for storing information about commands args!");
					exit(0);
				}
			}
		}
	commandsArgs[3][0] = "file";
	commandsArgs[3][1] = "-detailed";
	commandsArgs[3][2] = "-dir:<directory>";
	commandsArgs[4][0] = "file";
	commandsArgs[5][0] = "file";
	commandsArgs[6][0] = "-dir:<directory>";
	commandsArgs[6][1] = "-r";
	commandsArgs[7][0] = "file";
	commandsArgs[7][1] = "-first:<number>";
	commandsArgs[8][0] = "file";
	commandsArgs[8][1] = "-editor:<text-editor,default:vim>";
	commandsArgs[9][0] = "file";
	commandsArgs[10][0] = "directory";
	commandsArgs[12][0] = "program";
	commandsArgs[12][1] = "-lang:<programming-language>";

	commandsArgs[commandsCommandIndex][0] = "-details";
}

void uninit()
{
	free(responseBuffer);
	for (uint i=0;i<numberOfCommands;++i)
		if (commandsFlags[i] & COMMAND_HAS_ARGS)
			free(commandsArgs[i]);
}

int getHashFromText(char *text)
{
	if (text == NULL) return 0;
	ll ret = 0, currPow = 1;
	while (*text)
	{
		ret = (ret + currPow * ((int)*text))%PASSWORD_MOD;
		currPow = (currPow * PASSWORD_BASE)%PASSWORD_MOD;
		++text;
	}
	return (int)ret;
}

int	textToLowercase(char *text)
{
	if (text == NULL)
		return NO;
	while (*text)
	{
		if (*text >= 'A' && *text <= 'Z')
			*text = *text - 'A' + 'a';
		++text;
	}
	return OK;
}

int textStartsAndMatchesString(const char *str, const char *text)
{
	if (str == NULL || text == NULL) return NO;
	while (*str && *text)
	{
		if (*str != *text) return NO;
		++str; ++text;
	}
	if (!(*str) && !(*text)) return OK;
	return NO;
}

int textContainsAllowedCharacters(char *text)
{
	if (text == NULL) return NO;
	while (*text)
	{
		if (!allowedCharacters[(uint)*text])
			return NO;
		++text;
	}
	return OK;
}

int confirmPrompt(int fdSocketPairParentChild, int fdFifoChildParent, const char *message)
{
	char 	response[bufferSize];

	communicationGetInput(fdSocketPairParentChild, fdFifoChildParent, message, response, bufferSize);

	textToLowercase(response);
	if (!strcmp(response, "yes") || !strcmp(response, "y"))
		return OK;
	return NO;
}

int canRunCommand(int commandIndex)
{
	if (commandIndex < 0) return NO;
	if (commandIndex >= numberOfCommands) return NO;
	if (loggedIn && commandsFlags[commandIndex] & COMMAND_NOT_DISPLAYED_WHEN_LOGGED_IN) return NO;
	if (!loggedIn && commandsFlags[commandIndex] & COMMAND_REQUIRE_LOG_IN) return NO;
	return OK;
}

int getCommandIndex(char *command)
{
	uint tr;
	for (tr = 0; tr < numberOfCommands; ++tr)
		if (canRunCommand(tr) == OK && textStartsAndMatchesString(commands[tr], command) == OK) 
			return tr;
	return INVALID;
}

int getCommandFromBuffer(char *buffer, char *command, uint commandBufferSize, uint *commandSize)
{
	if (buffer == NULL || command == NULL) return ERROR;
	uint tr = 0;
	while (buffer[tr] && tr < commandBufferSize)
	{
		if (strchr(whiteSpaces, buffer[tr]))
			break;
		command[tr] = buffer[tr];
		++tr;
	}
	if (tr == commandBufferSize)
		--tr;
	command[tr] = 0;
	*commandSize = tr;
	return OK;
}

int getCommandArgsFromBuffer(char *buffer, char *args, uint argsBufferSize, uint commandSize)
{
	if (buffer == NULL || args == NULL) return NO;
	uint tr = 0;
	while (buffer[commandSize] && strchr(whiteSpaces, buffer[commandSize]))
		++commandSize;
	while (buffer[commandSize] && tr < argsBufferSize)
	{
		args[tr] = buffer[commandSize];
		++tr; ++commandSize;
	}
	if (tr == argsBufferSize)
		--tr;
	args[tr] = 0;
	return OK;
}

int	commandHasArg(uint commandIndex, const char* args, const char* arg)
{
	uint lenArgs = strlen(args);
	uint lenArg = strlen(arg);

	for (uint i=0;i<lenArgs;++i)
		if (lenArgs - i < lenArg)
			return NO;
		else
		if (!strncmp(args+i,arg,lenArg))
			return OK;
	return NO;
}

int getArgNumberValue(const char* args, const char* arg)
{
	uint lenArgs = strlen(args);
	uint lenArg = strlen(arg);

	for (uint i=0;i<lenArgs;++i)
		if (lenArgs - i < lenArg)
			break;
		else
		if (!strncmp(args+i,arg,lenArg))
		{
			if (!(args[i+lenArg]>='0' && args[i+lenArg]<='9')	)
			{
				addFormattedStringToResponse("Expecting a number after %s\n", arg);
				return ERROR;
			}
			return atoi(args+i+lenArg);	
		}
	
	return NOT_FOUND;
}

int	getArgStringValue(const char* args, const char* arg, char* dest)
{
	uint lenArgs = strlen(args);
	uint lenArg = strlen(arg);
	for (uint i=0;i<lenArgs;++i)
		if (lenArgs - i < lenArg)
			break;
		else
		if (!strncmp(args+i,arg,lenArg))
		{
			if (!((args[i+lenArg]>='a' && args[i+lenArg]<='z') || (args[i+lenArg]>='A' && args[i+lenArg]<='Z') || args[i+lenArg] == '.'))
			{
				addFormattedStringToResponse("Expecting a valid directory path after %s\n", arg);
				return ERROR;
			}
			uint tr = 0;
			while (args[i+lenArg+tr] && !strchr(whiteSpaces, args[i+lenArg+tr]))
			{
				dest[tr] = args[i+lenArg+tr];
				++tr;
			}
			dest[tr] = '\0';
			return OK;
		}
	
	return NOT_FOUND;
}

int leaveOneCommandArg(char *args)
{
	if (args == NULL) return NO;
	while (*args)
	{
		if (strchr(whiteSpaces, *args))
		{
			*args = 0;
			break;
		}
		++args;
	}
	return OK;
}

int	addFormattedStringToResponse(const char *format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	vsprintf(responseBuffer, format, argptr);
	va_end(argptr);
	uint len = strlen(responseBuffer);
	for (int i=0;i<len;++i){
		response.push_back(responseBuffer[i]);
	}
	return OK;
}

int userExists(const char* username, int *passwordHash)
{
	FILE	*fileIn = fopen(usersFileName, "r");
	if (fileIn == NULL) return NO;
	char 	currentUsername[MAX_LEN];
	int 	psswordHash;
	while (fscanf(fileIn, "%s %d", currentUsername, &psswordHash) != EOF)
		if (!strcmp(currentUsername, username))
		{
			if (passwordHash != NULL)
				*passwordHash = psswordHash;
			fclose(fileIn);
			return OK;
		}
	return NO;
}

void commandRegister(int fdSocketPairParentChild, int fdFifoChildParent)
{
	char 		username[bufferSize];
	char 		password[bufferSize];
	int			passwordHash, res, promptOpen;
	int			goodPassword = 0, goodUsername = 0;
	uint		len;

	communicationPrintMessage(fdSocketPairParentChild, fdFifoChildParent, 
		"!!!     <<< ----- REGISTER ----- >>>    !!!\n!!! <<< type 'q' to go back to menu >>> !!!\n");

	communicationGetInput(fdSocketPairParentChild, fdFifoChildParent, 
		"Enter an username (len>=2 && len<=32):\n", username, bufferSize);
	promptOpen = 1;
	while (promptOpen)
	{
		len = strlen(username);
		if (len == 1 && username[0] == 'q')
		{
			promptOpen = 0;
			continue;
		}
		if (len < 2 || len > 32)
		{
			communicationGetInput(fdSocketPairParentChild, fdFifoChildParent, 
				"This username has an invalid no. of characters ( !(>=2&&<=32) )!\nEnter another one:\n", username, bufferSize);
			continue;
		}
		if (textContainsAllowedCharacters(username) == NO)
		{
			communicationGetInput(fdSocketPairParentChild, fdFifoChildParent,
				"This username contains characters that are not allowed!\nEnter another one:\n", username, bufferSize);
			continue;
		}
		if (userExists(username,NULL) == OK)
		{
			communicationGetInput(fdSocketPairParentChild, fdFifoChildParent,
				"This username already exists! Enter another one:\n", username, bufferSize);
			continue;
		}
		promptOpen = 0;
		goodUsername = 1;
	}
	if (!goodUsername) return;

	communicationGetInput(fdSocketPairParentChild, fdFifoChildParent, 
		"Enter a password (len>=2 && len<=40):\n", password, bufferSize);
	promptOpen = 1;
	while (promptOpen)
	{
		len = strlen(password);
		if (len == 1 && password[0] == 'q')
		{
			promptOpen = 0;
			continue;
		}
		if (len < 2 || len > 40)
		{
			communicationGetInput(fdSocketPairParentChild, fdFifoChildParent, 
				"The password has an invalid no. of characters ( !(>=2&&<=40) )!\nEnter another one:\n", password, bufferSize);
			continue;
		}
		promptOpen = 0;
		goodPassword = 1;
	}
	if (!goodPassword) return;

	res = registerUser(username, password);
	if (res == OK)
		addFormattedStringToResponse("User registered successfully! Now you can login\n");
	else
		addFormattedStringToResponse("%s\n", internalErrorMessage);
}
int registerUser(char *username, char *password)
{
	FILE *fileOut = fopen(usersFileName, "a");
	if (fileOut == NULL)
		return ERROR;
	int passwordHash = getHashFromText(password);
	if (fprintf(fileOut, "%s %d\n", username, passwordHash) < 0)
	{
		fclose(fileOut);
		return ERROR;
	}
	fclose(fileOut);
	return OK;
}

void commandLogin(int fdSocketPairParentChild, int fdFifoChildParent)
{
	char 	username[bufferSize];
	int 	correctPasswordHash, res;

	communicationPrintMessage(fdSocketPairParentChild, fdFifoChildParent, 
		"!!!      <<< ----- LOGIN ----- >>>      !!!\n!!! <<< type 'q' to go back to menu >>> !!!\n");

	res = enteredCorrectUsername(fdSocketPairParentChild, fdFifoChildParent, username, &correctPasswordHash);
	if (res == NO) return;

	res = enteredCorrectPassword(fdSocketPairParentChild, fdFifoChildParent, &correctPasswordHash);
	if (res == NO) return;

	strcpy(currentUsername, username);
	strcpy(currentPath, usersDirectory);
	strcat(currentPath, username);
	createFolderForUser(currentPath);
	strcat(currentPath, "/");
	currentUsernameLen = strlen(currentUsername) + strlen(usersDirectory);

	loggedIn = 1;
	addFormattedStringToResponse("Logged in successfully!\n");
}
int enteredCorrectUsername(int fdSocketPairParentChild, int fdFifoChildParent, char *username, int *passwordHash)
{
	int 		promptOpen, len;
	promptOpen = 1;
	while(promptOpen)
	{
		communicationGetInput(fdSocketPairParentChild, fdFifoChildParent, "username: ", username, bufferSize);
		len = strlen(username);
		if (len == 1 && username[0] == 'q')
		{
			promptOpen = 0;
			continue;
		}
		if (len < 2 || len > 32)
		{
			communicationPrintMessage(fdSocketPairParentChild, fdFifoChildParent, "The username has an invalid no. of characters ( !(>=2&&<=32) )!\n");
			continue;
		}
		if (userExists(username, passwordHash) == OK)
			return OK;
		communicationPrintMessage(fdSocketPairParentChild, fdFifoChildParent, "This user doesn't exist!\n");
	}
	return NO;
}
int enteredCorrectPassword(int fdSocketPairParentChild, int fdFifoChildParent, int *correctPasswordHash)
{
	int 		promptOpen, len, passwordHash;
	char		password[bufferSize];
	promptOpen = 1;
	while(promptOpen)
	{
		communicationGetInput(fdSocketPairParentChild, fdFifoChildParent, "password: ", password, bufferSize);
		len = strlen(password);
		if (len == 1 && password[0] == 'q')
		{
			promptOpen = 0;
			continue;
		}
		if (len < 2 || len > 40)
		{
			communicationPrintMessage(fdSocketPairParentChild, fdFifoChildParent, "The password has an invalid no. of characters ( !(>=2&&<=32) )!\n");
			continue;
		}
		passwordHash = getHashFromText(password);
		if (*correctPasswordHash == passwordHash)
			return OK;
		communicationPrintMessage(fdSocketPairParentChild, fdFifoChildParent, "Invalid password!\n");
	}
	return NO;
}
int createFolderForUser(char *username)
{
	if (username == NULL) return NO;
	return createDirectory(username, 0);
}

void commandLogout(int fdSocketPairParentChild, int fdFifoChildParent)
{
	if (confirmPrompt(fdSocketPairParentChild, fdFifoChildParent, "Are you sure? (type yes to confirm): ") == OK)
	{
		addFormattedStringToResponse("Logged out!\n");
		loggedIn = 0;
	}
}

int fileExists(const char *fileName)
{
	if (fileName == NULL || access(currentPath, F_OK) == -1) return NO;
	return OK;
}

void commandMyFind(char* rootDirectory, const char* fileName, int detailed)
{
	char* startDirectory;
	if (!rootDirectory)
	{
		strcpy(currentPath + currentUsernameLen + 1, "");
		startDirectory = currentPath;
	}
	else
	{
		strcpy(currentPath + currentUsernameLen + 1, rootDirectory);
		if (fileIsDirectory(currentPath, 0) == NO)
		{
			addFormattedStringToResponse("The argument of -dir: option is not a directory!\n");
			return;
		}
		startDirectory = currentPath;
	}
	if (findFile(startDirectory, fileName, detailed) == NO)
		addFormattedStringToResponse("There is no file that contains in name your input!\n");
}

// Bucati din aceasta functie au fost luate (si modificate) de la implementarea de la exercitiul 3. [MyFind&Stat] 
// De la laboratorul 6 Sisteme de operare:
// https://profs.info.uaic.ro/~vidrascu/SO/labs/lab6.html
int findFile(char* currentPath, const char* fileName, int detailed)
{
	DIR *dir;
    struct dirent *de;
    char nume[PATH_MAX];
	
	if (fileIsDirectory(currentPath, 0) == NO)
		return NO;

	if(NULL == (dir = opendir(currentPath)) )
		return NO;

	int res = NO;
	while(NULL != (de = readdir(dir)) )
    {
        if( strcmp(de->d_name,".") && strcmp(de->d_name,"..") )  /* ignoram intrarile implicite . si .. din orice director */
        {
		    sprintf(nume,"%s/%s",currentPath,de->d_name);
			if (strstr(de->d_name,fileName) != NULL)
			{
				res = OK;
				fileStat(nume, 0, detailed, 1);
			}
			if (findFile(nume, fileName, detailed))
				res = OK;
		}
    }
	closedir(dir);
	
	return res;
}
// https://profs.info.uaic.ro/~vidrascu/SO/labs/lab6.html

void commandMyStat(const char* fileName)
{
	if (fileStat(fileName, 1, 1, 0) == ERROR)
		addFormattedStringToResponse("File \"%s\" doesn't exist!\n", fileName);
}

// Bucati din aceasta functie au fost luate (si modificate) de la implementarea de la exercitiul 3. [MyFind&Stat] 
// De la laboratorul 6 Sisteme de operare:
// https://profs.info.uaic.ro/~vidrascu/SO/labs/lab6.html
// De asemenea, unele linii de cod au fost luate din manual (man 2 stat)
int	fileStat(const char *file, int buildPath, int printPerm, int printFullName)
{
	struct stat 		st;
	char*				baseName;
	const char* 		path;
	int 				res = OK;

	if (buildPath)
	{
		strcpy(currentPath + currentUsernameLen + 1, file);
		path = (const char*)currentPath;
	}
	else path = file;
	if (stat(path, &st)) return ERROR;

	if (printPerm)
	{
		addFormattedStringToResponse("------------------------------------\n");
		addFormattedStringToResponse("file name: ");
	}

	if (printFullName)
		addFormattedStringToResponse("%s ", path+currentUsernameLen+1);
	else
	{
		baseName = strdup(path);
		if (baseName != NULL)
		{
			addFormattedStringToResponse("%s ", (char*)basename(baseName));
			free(baseName);
		}
		else
			addFormattedStringToResponse("%s ", path);
	}
	
	if (printPerm) addFormattedStringToResponse("\n");

	addFormattedStringToResponse("type: ");
	switch(st.st_mode & S_IFMT)
    {
        case S_IFDIR : addFormattedStringToResponse("directory"); res = OK; break;
        case S_IFREG : addFormattedStringToResponse("regular file"); break;
        case S_IFLNK : addFormattedStringToResponse("link"); break;
        case S_IFIFO : addFormattedStringToResponse("fifo"); break;
        case S_IFSOCK: addFormattedStringToResponse("socket"); break;
        case S_IFBLK : addFormattedStringToResponse("block device"); break;
        case S_IFCHR : addFormattedStringToResponse("character device"); break;
        default: addFormattedStringToResponse("unknown file type");
    }

	addFormattedStringToResponse(", inode: %d",st.st_ino);
	addFormattedStringToResponse(", size: %d bytes", st.st_size);
	addFormattedStringToResponse(", no. blocks: %d", st.st_blocks);
	addFormattedStringToResponse(", links: %d", st.st_nlink);

	if (printPerm)
	{
		char perm[11]="----------";
		if(S_IRUSR & st.st_mode) perm[0]='r';
   	 	if(S_IWUSR & st.st_mode) perm[1]='w';
    	if(S_IXUSR & st.st_mode) perm[2]='x';
    	if(S_IRGRP & st.st_mode) perm[3]='r';
    	if(S_IWGRP & st.st_mode) perm[4]='w';
    	if(S_IXGRP & st.st_mode) perm[5]='x';
    	if(S_IROTH & st.st_mode) perm[6]='r';
    	if(S_IWOTH & st.st_mode) perm[7]='w';
    	if(S_IXOTH & st.st_mode) perm[8]='x';
		addFormattedStringToResponse(", perm: %s", perm);

		addFormattedStringToResponse("\nlast status change: 	%s", ctime(&st.st_ctime));
		addFormattedStringToResponse("last file access: 	%s", ctime(&st.st_atime));
		addFormattedStringToResponse("last file modification: %s", ctime(&st.st_mtime));
	
		addFormattedStringToResponse("------------------------------------\n");
	}
	else addFormattedStringToResponse("\n");

	return res;
}
// https://profs.info.uaic.ro/~vidrascu/SO/labs/lab6.html

void commandMyCreate(int fdSocketPairParentChild, int fdFifoChildParent, const char* fileName)
{
	strcpy(currentPath + currentUsernameLen + 1, fileName);
	if (fileExists(currentPath) == OK && confirmPrompt(fdSocketPairParentChild, fdFifoChildParent, "File already exists, do you want to recreate it?\n (type yes to confirm): ") == NO)
		return;
	FILE *fileOut = fopen(currentPath, "w");
	if (fileOut != NULL)
	{
		fclose(fileOut);
		addFormattedStringToResponse("File created successfully!\n");
	}
	else addFormattedStringToResponse("%s\n", internalErrorMessage);
}

void commandMyLs(const char* directoryPath, int recursive)
{
	if (directoryPath == NULL)
	{
		*(currentPath+currentUsernameLen+1)='\0';
		addFormattedStringToResponse("------------------------------------\n");
		printDirectoryFiles((const char*)currentPath, recursive);
		addFormattedStringToResponse("------------------------------------\n");
	}
	else
	{
		strcpy(currentPath + currentUsernameLen + 1, directoryPath);
		if (fileIsDirectory(currentPath, 0) == OK)
		{
			addFormattedStringToResponse("------------------------------------\n");
			printDirectoryFiles(currentPath, recursive);
			addFormattedStringToResponse("------------------------------------\n");
		}
		else
			addFormattedStringToResponse("%s is not a directory!\n", directoryPath);
	}
}
int fileIsDirectory(const char *file, int buildPath)
{
	const char* 	path;
    struct stat 	st;
	if (buildPath)
	{
		strcpy(currentPath + currentUsernameLen + 1, file);
		path = (const char*)currentPath;
	}
	else path = file;

	if (stat(path, &st)) return ERROR;

	if ((st.st_mode & S_IFMT) == S_IFDIR) 
		return OK;
	
	return NO;
}
// Bucati din aceasta functie au fost luate (si modificate) de la implementarea de la exercitiul 3. [MyFind&Stat] 
// De la laboratorul 6 Sisteme de operare:
// https://profs.info.uaic.ro/~vidrascu/SO/labs/lab6.html
void printDirectoryFiles(const char* directoryPath, int recursive)
{
	DIR *dir;
    struct dirent *de;
    char nume[PATH_MAX];

	if(NULL == (dir = opendir(directoryPath)) )
		return;

	while(NULL != (de = readdir(dir)) )
    {
        if( strcmp(de->d_name,".") && strcmp(de->d_name,"..") )  /* ignoram intrarile implicite . si .. din orice director */
        {
            sprintf(nume,"%s/%s",directoryPath,de->d_name);
            fileStat(nume, 0, 0, 1);
			if (recursive && fileIsDirectory(nume, 0) == OK){
				printDirectoryFiles(nume, recursive);
			}
        }
    }
	
	closedir(dir);
}
// https://profs.info.uaic.ro/~vidrascu/SO/labs/lab6.html

void commandMyCat(const char* fileName, int numberOfLines)
{
	uint lines = 0;
	if (numberOfLines == NOT_FOUND)
		numberOfLines = INFINITY;
	strcpy(currentPath + currentUsernameLen + 1, fileName);
	FILE *fileIn = fopen(currentPath, "r");
	if (fileIn == NULL)
	{
		addFormattedStringToResponse("Failed to open file: %s\n", fileName);
		return;
	}
	char 	ch;
	addFormattedStringToResponse("--------------- %s ---------------\n", fileName);
	if (numberOfLines)
	{
		while (fscanf(fileIn,"%c",&ch) != EOF)
		{
			addFormattedStringToResponse("%c", ch);
			if (ch == '\n') 
				++lines;
			if (lines == numberOfLines) break;
		}
	}
	fclose(fileIn);
	addFormattedStringToResponse("----------------");
	uint len = strlen(fileName);
	for (uint i=0;i<len;++i)
		addFormattedStringToResponse("-");
	addFormattedStringToResponse("----------------\n");
}

void commandMyEdit(const char* fileName, const char* editorName)
{
	pid_t 	pid;
	int	finishCode, foundEditor = 0;
	if (editorName)
	{
		for (int i=0;i<commandMyEditEditorsCount;++i)
			if (!strcmp(editorName, commandMyEditEditors[i]))
			{
				foundEditor = 1;
				break;
			}
	} 
	else 
	foundEditor = 1;

	if (!foundEditor)
	{
		addFormattedStringToResponse("Invalid text editor!\n");
		return;
	}
	strcpy(currentPath + currentUsernameLen + 1, fileName);
	if (-1 == (pid = fork()))
	{
		addFormattedStringToResponse("Failed to fork() in order to open a text editor");
		return;
	}
	if (pid == 0)
	{
		execlp(editorName,editorName,currentPath,NULL);

		addFormattedStringToResponse("Failed to call exec() to open a text editor, maybe the file doesn't exist");
		exit(173);	
	}
	
	wait(&finishCode);
}

void commandMyRm(int fdSocketPairParentChild, int fdFifoChildParent, const char* fileName)
{
	if (fileName == NULL)
	{
		addFormattedStringToResponse("commandMyRm function: fileName == NULL");
		return;
	}
	strcpy(currentPath + currentUsernameLen + 1, fileName);
	if (access(currentPath, F_OK) == -1)
	{
		addFormattedStringToResponse("No such file exist!\n");
		return;
	}
	if (fileIsDirectory(currentPath, 0) == OK)
	{
		addFormattedStringToResponse("You cannot remove directories!\n");
		return;
	}
	if (confirmPrompt(fdSocketPairParentChild, fdFifoChildParent, "Are you sure you want to delete the file? (type yes to confirm): ") == OK)
	{
		remove(currentPath);
		addFormattedStringToResponse("File removed successfully!\n");
	}
}
void commandMyMkdir(const char* dirName)
{
	strcpy(currentPath + currentUsernameLen + 1, dirName);
	if (createDirectory(currentPath, 1) == ERROR)
		addFormattedStringToResponse("Failed to create directory %s!\n", dirName);
	else
		addFormattedStringToResponse("Directory created successfully!\n");
}
int	createDirectory(const char* dirName, int printAlreadyExistsMessage)
{
	if (dirName == NULL) return ERROR;
	DIR *dir = NULL;
	dir = opendir(dirName);
	if (dir == NULL)
		mkdir(dirName, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	else
	{
		if (printAlreadyExistsMessage)
			addFormattedStringToResponse("Directory %s already exists!\n", dirName);
		closedir(dir);
	}
	return OK;
}

void commandShowAll(int showDetails)
{
	uint tr, index = 0;
	addFormattedStringToResponse("Available commands: \n");
	for (tr = 0; tr < numberOfCommands; ++tr)
	{
		if (canRunCommand(tr) == NO) continue;
		++index;
		addFormattedStringToResponse("%u. %s ", index, commands[tr]);
		for (uint i=0;i<commandsArgsCount[tr];++i)
			if (commandsArgs[tr][i][0] != '-')
				addFormattedStringToResponse("%s ", commandsArgs[tr][i]);
		addFormattedStringToResponse("\n");
		if (showDetails && commandsFlags[tr] & COMMAND_HAS_ARGS)
			for (uint i=0;i<commandsArgsCount[tr];++i)
				if (commandsArgs[tr][i][0] == '-')
					addFormattedStringToResponse("	%s\n", commandsArgs[tr][i]);
	}
}

void commandListUsers()
{
	FILE *fileIn = fopen(usersFileName, "r");
	int usersCount = 0;
	if (fileIn != NULL)
	{
		char 	currentUsername[MAX_LEN];
		int 	psswordHash;
		while (fscanf(fileIn, "%s %d", currentUsername, &psswordHash) != EOF)
		{
			++usersCount;
			addFormattedStringToResponse("%d. %s\n", usersCount, currentUsername);	
		}
	}
	if (!usersCount)
		addFormattedStringToResponse("The are no users!\n");
}

void commandRun(const char* program, const char* programmingLanguage)
{
	if (!program || !programmingLanguage)
	{
		addFormattedStringToResponse("Invalid parameters for commandRun function!\n");
		return;
	}
	int correctProgrammingLanguage = 0;
	uint tr, lang;
	for (tr = 0;tr < commandRunLanguagesCount;++tr)
		if (!strcmp(programmingLanguage, commandRunLanguages[tr]))
		{
			correctProgrammingLanguage = 1;
			lang = tr;
			break;
		}
	if (!correctProgrammingLanguage)
	{
		addFormattedStringToResponse("Invalid programming language! Available: ");
		for (tr = 0; tr < commandRunLanguagesCount; ++tr)
		{
			addFormattedStringToResponse("%s", commandRunLanguages[tr]);
			if (tr + 1 == commandRunLanguagesCount)
				addFormattedStringToResponse("\n");
			else
				addFormattedStringToResponse(", ");
		}
		return;
	}
	strcpy(currentPath + currentUsernameLen + 1, program);
	if (fileExists(currentPath) == NO)
	{
		addFormattedStringToResponse("File %s doesn't exist!\n", program);
		return;
	}
	switch(lang)
	{
		case LANGUAGE_CPP:
			runCorCpp(currentPath,"/usr/bin/g++");
			break;
		case LANGUAGE_C:
			runCorCpp(currentPath,"/usr/bin/gcc");
			break;
		case LANGUAGE_PYTHON:
			runPython(currentPath);
			break;
		default: break;
	}
}
void runCorCpp(const char* program, const char* compiler)
{
	if (!program)
	{
		addFormattedStringToResponse("Invalid parameters for runC function!\n");	
		return;
	}
	char* executableName = getExecutableFileName(program);
	if (!executableName) return;
	char* fileToCompileName = getFileToCompileName(program);
	if (!fileToCompileName)
	{
		free(executableName);
		return;
	}
	if (compileFile(fileToCompileName, compiler, executableName) == NO)
	{
		addFormattedStringToResponse("Failed to compile the program!\n");
		free(executableName);
		free(fileToCompileName);
		return;
	}
	if (runCompiledCorCppProgram(executableName) == NO)
		addFormattedStringToResponse("Failed to run your program!\n");
	remove(executableName);
	free(executableName);
	free(fileToCompileName);
}
int runCompiledCorCppProgram(const char* compiledFileName)
{
	if (!compiledFileName)
	{
		addFormattedStringToResponse("Invalid parameters for runCompiledCorCppProgram function!\n");	
		return NO;
	}
	pid_t pid;
	if (-1 == (pid = fork()))
	{
		addFormattedStringToResponse("%s\n", internalErrorMessage);
		return NO;
	}
	if (!pid)
	{
		execl(compiledFileName, "program", NULL);
		exit(125);
	}
	else
	{
		int response;
		wait(&response);
		if (response>>8) return NO;
		return OK;
	}
}
void runPython(const char* program)
{
	if (!program)
	{
		addFormattedStringToResponse("Invalid parameters for runPython function!\n");	
		return;
	}
	pid_t pid;
	if (-1 == (pid = fork()))
	{
		addFormattedStringToResponse("%s\n", internalErrorMessage);
		return;
	}
	if (pid)
	{
		int response;
		wait(&response);
		if (response>>8)
			addFormattedStringToResponse("Error when running the python program!\n");
	}
	else
	{
		execlp("python","pythonProgram",program,NULL);
		exit(125);
	}
}
int	compileFile(const char* file, const char* compiler, const char* compiledFileName)
{
	if (!file || !compiler)
	{
		addFormattedStringToResponse("Invalid parameters for compileFile function!\n");	
		return NO;
	}
	pid_t pid;
	if (-1 == (pid = fork()))
	{
		addFormattedStringToResponse("compileFile function: error when calling fork()");
		return NO;
	}
	if (!pid)
	{
		int 	fullCommandLen = strlen(compiler) + 1 + strlen(file) + 1 + strlen("-o") + 1 + strlen(compiledFileName) + 1;
		char* 	command = (char*)malloc(fullCommandLen);
		if (!command) exit(125);
		strcpy(command, compiler);
		strcat(command, " ");
		strcat(command, file);
		strcat(command, " -o ");
		strcat(command, compiledFileName);
		system(command);
		//execl(compiler, "compile", file, "-o", compiledFileName, NULL); error???
		exit(125);	
	}
	else
	{
		int response;
		wait(&response);
		if (response>>8 != 125) return NO;
		return OK;
	}
}
char* getExecutableFileName(const char* file)
{
	if (!file)
	{
		addFormattedStringToResponse("Invalid parameters for compileFile function!\n");	
		return NULL;
	}
	uint len = strlen(file), newLen;
	int lastDotPosition = -1;
	char *newName = NULL;
	for (int i=len-1;i>=0;--i)
		if (file[i] == '.')
		{
			lastDotPosition = i;
			break;
		}
	if (lastDotPosition == -1 || lastDotPosition == 0)
	{
		newLen = len + 7; // file + .bin + NULL
		newName = (char*)malloc(newLen);
		if (newName == NULL)
		{
			addFormattedStringToResponse("compileFile function: failed to allocate memory for the new name!\n");	
			return NULL;
		}
		strcpy(newName, "./");
		strcat(newName, file);
		strcat(newName, ".bin");
 	}
	else
	{
		newLen = lastDotPosition + 7;
		newName = (char*)malloc(newLen);
		if (newName == NULL)
		{
			addFormattedStringToResponse("compileFile function: failed to allocate memory for the new name!\n");	
			return NULL;
		}
		strcpy(newName, "./");
		strncat(newName, file, lastDotPosition);
		newName[lastDotPosition+2] = '\0';
		strcat(newName, ".bin");
	}
	return newName;
}
char* getFileToCompileName(const char* file)
{
	if (!file)
	{
		addFormattedStringToResponse("Invalid parameters for getFileToCompileName function!\n");	
		return NULL;
	}
	uint len = strlen(file);
	char* newName = (char*)malloc(len+3); // ./ + file + NULL
	strcpy(newName, "./");
	strcat(newName, file);
	return newName;
}

// -----------------------------------------------------------------
int nrc(int x)
{
	if (!x) return 1;
	int ans = 0;
	while (x) ++ans, x/=10;
	return ans;
}
void setNums(char *s, int nr1, int nr2)
{
	memset(s, 0, sizeof(s));
	int ln1 = nrc(nr1), ln2 = nrc(nr2);
	int ln = ln1 + ln2 + 1, i;

	for (i = ln1-1;i>=0;--i)
		s[i] = nr1 % 10 + '0', nr1/=10;
	s[ln1] = '-';
	for (i = ln-1;i>ln1;--i)
		s[i] = nr2 % 10 + '0', nr2/=10;
}
void compressSpacesString(char *s)
{
	int ln = strlen(s);
	int i, j;
	for (i=0;i<ln;++i)
		while (i<ln-1 && s[i] == s[i+1] && s[i] == ' ')
		{
			for (j=i+1;j<ln;++j)
				s[j] = s[j+1];
			--ln;
		}
	while (ln > 0 && s[ln-1] == '|') 
		--ln;
}
void changeSpaces(char *s)
{
	int i, ln = strlen(s);
	if (!ln) return;

	if (s[0] == ' ') 
		s[0] = 1;
	if (s[ln-1] == ' ') 
		s[ln-1] = 1;
	
	for (i = 1; i<ln-1;++i)
		if (s[i] == ' ' && ( (s[i-1] == '\"' && s[i+1] == '\"') || (s[i-1] == '\'' && s[i+1] == '\'')  ))
			s[i] = 1, s[i+1] = 1, s[i-1] = ' ';
		else if (s[i] == ' ')
			s[i] = 1;
}
