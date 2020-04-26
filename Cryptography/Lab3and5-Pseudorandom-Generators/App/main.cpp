#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>

#include <NTL/ZZ.h>

#include "Generator.h"
#include "GeneratorBBS.h"
#include "GeneratorJacobi.h"
#include "GeneratorLFSR.h"

std::vector<std::string>commands;

void initCommandLine() {
	commands.push_back("bbs");
	commands.push_back("jacobi");
	commands.push_back("lfsr");
	commands.push_back("compare");
	commands.push_back("validate-jacobi");
	commands.push_back("bbs-generate");
	commands.push_back("jacobi-generate");
	commands.push_back("lfsr-generate");
	commands.push_back("list");
	commands.push_back("quit");
}

void printCommands() {
	for (unsigned i = 0; i < commands.size(); ++i) {
		std::cout << i+1 << ". " << commands[i] << '\n';
	}
}

bool stringIsUInt(const std::string& str) {
	if (str.size() > 9) {
		return false;
	}
	for (unsigned i = 0; i < str.size(); ++i) {
		if (!(str[i] >= '0' && str[i] <= '9')) {
			return false;
		}
	}
	return true;
}

int getUIntFromString(const std::string& str) {
	int res = 0;
	for (unsigned i = 0; i < str.size(); ++i) {
		res = res * 10 + (str[i]-'0');
	}
	return res;
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

int parseLine(std::string& line) {
	trim(line);
	if (stringIsUInt(line)) {
		int option = getUIntFromString(line);
		--option;
		if (!(option >= 0 && option < commands.size())) {
			return -1;
		}
		return option;
	}
	for (unsigned i = 0; i < commands.size(); ++i) {
		if (commands[i] == line) {
			return i;
		}
	}
	return -1;
}

const int generatedNumbersCount = 50;

void generatorTimer(Generator* generator) {
	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 1; i <= generatedNumbersCount; ++i) {
		NTL::ZZ randomNumber = generator->generate();
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::cout << generator->getName() << ": " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " milliseconds\n";
}

void compareGenerators(Generator* generators[], int n) {
	std::cout << "Time needed to generate " << generatedNumbersCount << " numbers:\n";
	for (int i = 0; i < n; ++i) {
		generatorTimer(generators[i]);
	}
}

void validateJacobiSymbol() {
	GeneratorJacobi generator;

	generator.checkJacobiSymbolCalculation();
}

int main() {
	srand(time(NULL));

	Generator* generators[3];
	generators[0] = NULL;
	generators[1] = NULL;
	generators[2] = NULL;
	generators[0] = new GeneratorBBS();
	generators[1] = new GeneratorJacobi();
	generators[2] = new GeneratorLFSR();

	if (!generators[0]) {
		std::cout << "Error when creating the Blum-Blum-Shub Generator!\n";
		return 1;
	}
	if (!generators[1]) {
		std::cout << "Error when creating the Jacobi Generator!\n";
		return 1;
	}
	if (!generators[2]) {
		std::cout << "Error when creating the LFSR Generator!\n";
		return 1;
	}

	initCommandLine();
	printCommands();

	bool running = true;
	while (running){
		std::string line;
		std::getline(std::cin, line);
		int command = parseLine(line);
		switch (command)
		{
		case 0:
			std::cout << generators[0]->generate(true) << '\n';
			break;
		case 1:
			std::cout << generators[1]->generate(true) << '\n';
			break;
		case 2:
			std::cout << generators[2]->generate(true) << '\n';
			break;
		case 3:
			compareGenerators(generators, 3);
			break;
		case 4:
			validateJacobiSymbol();
			break;
		case 5:
			generators[0]->generateBytes(2048);
			break;
		case 6:
			generators[1]->generateBytes(2048);
			break;
		case 7:
			generators[2]->generateBytes(2048);
			break;
		case 8:
			printCommands();
			break;
		case 9:
			running = false;
			break;
		default:
			std::cout << "Invalid command!\n";
		}
	}
	return 0;
}

