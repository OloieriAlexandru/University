#include "common.h"

std::vector<uc> uCharToBinary(uc u) {
	std::vector<uc> arr;

	for (int i = 7; i >= 0; --i) {
		if (u & (1 << i)) {
			arr.push_back(1);
		} else {
			arr.push_back(0);
		}
	}

	return arr;
}

int binaryToInt(const std::vector<uc>& arr) {
	int res = 0;
	for (int i = 0; i < arr.size(); ++i) {
		res = res * 2 + arr[i];
	}
	return res;
}

std::vector<uc> multiply(int x, int times) {
	ull result = 1;
	for (int i = 1; i <= times; ++i) {
		result *= x;
	}
	std::bitset<64> bs(result);
	std::vector<uc> res;
	for (int i = 63; i >= 0; --i) {
		res.push_back(bs.test(i));
	}
	return res;
}

std::vector<uc> multiply(const std::vector<uc>& arr, int times) {
	std::vector<uc> res;

	for (int i = 0; i < times; ++i) {
		for (auto x : arr) {
			res.push_back(x);
		}
	}

	return res;
}

void initHexToBinaryMap() {
	hexToBinary['0'] = "0000";
	hexToBinary['1'] = "0001";
	hexToBinary['2'] = "0010";
	hexToBinary['3'] = "0011";
	hexToBinary['4'] = "0100";
	hexToBinary['5'] = "0101";
	hexToBinary['6'] = "0110";
	hexToBinary['7'] = "0111";
	hexToBinary['8'] = "1000";
	hexToBinary['9'] = "1001";
	hexToBinary['A'] = "1010";
	hexToBinary['B'] = "1011";
	hexToBinary['C'] = "1100";
	hexToBinary['D'] = "1101";
	hexToBinary['E'] = "1110";
	hexToBinary['F'] = "1111";
}

void initBinaryToHexMap() {
	binaryToHex[std::string("0000")] = '0';
	binaryToHex[std::string("0001")] = '1';
	binaryToHex[std::string("0010")] = '2';
	binaryToHex[std::string("0011")] = '3';
	binaryToHex[std::string("0100")] = '4';
	binaryToHex[std::string("0101")] = '5';
	binaryToHex[std::string("0110")] = '6';
	binaryToHex[std::string("0111")] = '7';
	binaryToHex[std::string("1000")] = '8';
	binaryToHex[std::string("1001")] = '9';
	binaryToHex[std::string("1010")] = 'A';
	binaryToHex[std::string("1011")] = 'B';
	binaryToHex[std::string("1100")] = 'C';
	binaryToHex[std::string("1101")] = 'D';
	binaryToHex[std::string("1110")] = 'E';
	binaryToHex[std::string("1111")] = 'F';
}

void trim(std::string& str) {
     reverse(str.begin(), str.end());
     while (str.size() && str.back() == ' ') {
	     str.pop_back();
     }
     reverse(str.begin(), str.end());
     while (str.size() && str.back() == ' ') {
	     str.pop_back();
     }
}

std::vector<uc> readText() {
	std::string textType = readOption("Enter the type of the text: ", std::vector<std::string> {"ascii", "binary", "byte", "hex"});
	std::vector<uc> text;
	
	std::cout << "Enter the text: ";
	if (textType == "ascii") {
		text = readASCIIString();
	} else if (textType == "binary") {
		text = readBinaryString();
	} else if (textType == "byte") {
		text = readByteArray();
	} else {
		text = readHEXString();
	}

	return text;
}

std::vector<uc> readKey(int len) {
	std::string keyType = readOption("Enter the type of the key: ", std::vector<std::string> {"ascii", "byte", "hex"});
	std::vector<uc> key;
	
	std::cout << "Enter the key (" << len * 8 << " bits): ";
	while (true) {
		if (keyType == "ascii") {
			key = readASCIIString();
		}
		else if (keyType == "byte") {
			key = readByteArray();
		}
		else {
			key = readHEXString();
		}
		if (len * 8 == key.size()) {
			break;
		}
		std::cout << "Invalid key len! The key has to have " << len * 8 << " bits!\n";
	}

	return key;
}

std::string readOption(std::string message, std::vector<std::string> options) {
	std::string option;

	std::cout << "Options: { ";
	for (int i = 0; i < options.size(); ++i) {
		std::cout << options[i];
		if (i + 1 < options.size()) {
			std::cout << ", ";
		}
	}
	std::cout << " }\n";
	while (true) {
		std::cout << message;
		std::getline(std::cin, option);
		bool found = false;
		for (auto opt : options) {
			if (opt == option) {
				option = opt;
				found = true;
				break;
			}
		}
		if (found) {
			break;
		}
		std::cout << "Invalid option!\n";
	}

	return option;
}

uc readInt(const std::string& str, int pos) {
	uc res = 0;
	while (pos < str.size() && str[pos] >= '0' && str[pos] <= '9') {
		res = res * 10 + (str[pos] - '0');
		++pos;
	}
	return res;
}

std::vector<uc> stringToByteArr(const std::string& str) {
	std::vector<uc> arr;

	for (auto x : str) {
		std::bitset<8> bs((int)x);
		for (int i = 7; i >= 0 ; --i) {
			arr.push_back(bs.test(i));
		}
	}

	return arr;
}

std::string byteArrToString(const std::vector<uc>& arr) {
	std::string str;

	for (int i = 0; i < arr.size();) {
		char ch = 0;
		for (int j = 0; j < 8; ++i, ++j) {
			ch = ch * 2 + arr[i];
		}
		str.push_back(ch);
	}

	return str;
}

std::vector<uc> binaryByteArrToByArr(const std::vector<uc>& arr) {
	std::vector<uc> res;
	
	for (int i = 0; i < arr.size(); i += 8) {
		uc byte = 0;
		for (int j = i; j < i + 8; ++j) {
			byte = byte * 2 + arr[j];
		}
		res.push_back(byte);
	}

	return res;
}

std::vector<uc> readASCIIString() {
	std::string line;
	std::getline(std::cin, line);
	
	std::vector<uc> arr = stringToByteArr(line);
	return arr;
}

std::vector<uc> readByteArray() {
	std::vector<uc> arr;

	std::string line;
	std::getline(std::cin, line);
	trim(line);
	for (int i = 0; i < line.size(); ++i) {
		if (i == 0 || line[i - 1] == ' ') {
			uc byte = readInt(line, i);
			std::bitset<8> bs(byte);
			for (int j = 8; i>= 0; --j) {
				arr.push_back(bs.test(j));
			}
		}
	}

	return arr;
}

std::vector<uc> readBinaryString() {
	std::vector<uc> arr;

	std::string line;
	std::getline(std::cin, line);
	trim(line);

	for (auto x : line) {
		arr.push_back(x - '0');
	}

	return arr;
}

std::vector<uc> readHEXString() {
	std::vector<uc> arr;

	std::string line;
	std::getline(std::cin, line);
	trim(line);

	for (auto x : line) {
		for (auto y : hexToBinary[x]) {
			arr.push_back(y - '0');
		}
	}

	return arr;
}

std::string binaryToHexTransf(const std::vector<uc>& arr) {
	std::string res;

	for (int i = 0; i < arr.size(); i += 4) {
		std::string str;
		for (int j = 0; j < 4; ++j) {
			str.push_back(arr[i + j] + '0');
		}
		res.push_back(binaryToHex[str]);
	}
	
	return res;
}

void printByteArr(const std::vector<uc>& arr) {
	for (auto x : arr) {
		std::cout << (int)x << ' ';
	}
	std::cout << '\n';
}

void printBinaryArr(const std::vector<uc>& arr) {
	for (auto x : arr) {
		std::cout << (int)x;
	}
	std::cout << '\n';
}

void printInAllForms(const std::vector<uc>& arr) {
	std::cout << "String: " << byteArrToString(arr) << '\n';
	std::cout << "Binary: ";
	printBinaryArr(arr);
	std::cout << "Hex: " << binaryToHexTransf(arr) << '\n';
	std::cout << "Bytes: ";
	printByteArr(binaryByteArrToByArr(arr));
}
