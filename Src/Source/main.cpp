#include <iomanip>
#include <iostream>
#include "System.h"

int main(int argc, char* argv[])
{
	System system;

	if (argc < 4)
	{
		system.PrintLog(Color::ERROR_RED, "ERROR: argv가 정의되지않았습니다.");
		return 0;
	}

	std::vector<std::string> fileNames = system.GetFilesName(argv[1]);

	/** Debug용 코드 */
	//std::vector<std::string> fileNames = system.GetFilesName("C:\\Users\\PC5\\Desktop\\ExcelParsingProgram\\x64\\Release\\Excels\\");

	/** Debug용 코드*/
	//const std::string outputDirectory = "C:\\Users\\PC5\\Desktop\\ExcelParsingProgram\\x64\\Release\\Project\\Content\\";

	for (std::string fileName : fileNames)
	{
		XLDocument doc;
		doc.open(fileName);

		std::cout << std::endl;
		system.PrintLog(Color::LIGHTGREEN, "선택된 xlsx파일 이름: " + fileName);

		int asciiCode = 65; // A
		for(std::string worksheetName : doc.workbook().worksheetNames())
		{
			auto wks = doc.workbook().worksheet(worksheetName);

			std::cout << std::endl;
			system.PrintLog(Color::GREEN, "선택된 sheet 이름: " + worksheetName);
			std::cout << std::endl;

			system.Generate(wks, argv[2], argv[3], argv[4]);
		}

		doc.close();
	}

	system.PrintResult();

	return 0;
}