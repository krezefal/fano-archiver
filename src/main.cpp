#include <fstream>
#include <iostream>
#include "../include/fano.hpp"

int main(int argc, char **argv) {
	std::ifstream In;
	In.open(argv[1]);
	std::string ConsoleArg(argv[2]);
	char Action = ConsoleArg[0];
	std::ofstream Out;
	Out.open(argv[3]);
	try {
		FanoCodeStore Archiver(In, Action);
		if (Action == 'c') {
			std::cout << "Begin compress data..." << std::endl;
			Archiver.CompressData(In, Out);
			std::cout << "File was successfully compressed!" << std::endl;
		}
		if (Action == 'd') {
			std::cout << "Try to decompress data..." << std::endl;
			try {
				Archiver.DecompressData(In, Out);
				std::cout << "File was successfully decompressed!" << std::endl;
			}
			catch (const std::exception &e) {
				std::cout << std::endl << e.what() << std::endl;
				In.close();
				Out.close();
				return ENVIRONMENT_ERROR;
			}
		}
	}
	catch (const std::exception &e) {
		std::cout << std::endl << e.what() << std::endl;
		In.close();
		Out.close();
		return ENVIRONMENT_ERROR;
	}
	In.close();
	Out.close();
	return ALL_FINE;
}