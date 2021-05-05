#include <iomanip>
#include <iostream>
#include "System.h"

int main(int argc, char* argv[])
{
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

		std::cout << std::endl;
		std::cout << "선택된 xlsx파일 이름: " << fileName << std::endl;

		int asciiCode = 65; // A
		for(std::string worksheetName : doc.workbook().worksheetNames())
		{
			auto wks = doc.workbook().worksheet(worksheetName);

			std::cout << std::endl;
			std::cout << "선택된 sheet 이름: " << worksheetName << std::endl;
			std::cout << std::endl;

			system.Generate(wks, outputDirectory);
		}

		doc.close();
	}

	std::cout << std::endl;

	return 0;
}