#include <iomanip>
#include <iostream>
#include "System.h"

int main(int argc, char* argv[])
{
	std::cout << "********************************************************************************\n";
	std::cout << "Excel Parsing Program Run!\n";
	std::cout << "********************************************************************************\n";

	if (argc < 2)
	{
		std::cout << "ERROR: argv가 정의되지않았습니다." << std::endl;
		return 0;
	}

	System system;
	std::vector<std::string> fileNames = system.GetFilesName(argv[1]);

	const std::string outputDirectory = argv[2];

	for (std::string fileName : fileNames)
	{
		XLDocument doc;
		doc.open(fileName);

		int asciiCode = 65; // A
		auto wks = doc.workbook().worksheet(doc.workbook().worksheetNames()[0]);

		system.GenerateStruct(wks, doc.workbook().worksheetNames()[0], outputDirectory);

		doc.close();
	}

	return 0;
}