#include <iomanip>
#include <iostream>
#include "System.h"

/** TODO: Console 출력에 색 넣어서 Error, Warning 구분
*	TODO: 자료형이 추정되지않을때 Error 출력하고 exit
*/


int main(int argc, char* argv[])
{
	/*if (argc < 2)
	{
		std::cout << "ERROR: argv가 정의되지않았습니다." << std::endl;
		return 0;
	}*/

	System system;
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
		std::cout << "선택된 xlsx파일 이름: " << fileName << std::endl;

		int asciiCode = 65; // A
		for(std::string worksheetName : doc.workbook().worksheetNames())
		{
			auto wks = doc.workbook().worksheet(worksheetName);

			std::cout << std::endl;
			std::cout << "선택된 sheet 이름: " << worksheetName << std::endl;
			std::cout << std::endl;

			system.Generate(wks, argv[2], argv[3]);
		}

		doc.close();
	}

	std::cout << std::endl;

	return 0;
}