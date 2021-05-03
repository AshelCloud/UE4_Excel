#pragma once

#include<vector>
#include<string>
#include <OpenXLSX.hpp>

using namespace OpenXLSX;

class System
{
public:
	System();
	~System();

public:
	/** filePath에 있는 모든 파일경로 반환 */
	std::vector<std::string> GetFilesName(std::string filePath) const;

	/** ValueType이 Error 혹은 Empty인지 검사 */
	/** Error || Empty 일시 ture */
	bool InvalidValue(XLValueType type);

	/** Sheet에 있는 첫번째 Column을 기준으로 구조체를 생성 */
	void GenerateStruct(XLWorksheet& workSheet, std::string workSheetName, std::string outputDirectory);

	/** Json 생성 */
	/** A열에 있는 Cell이 Root이며 그 행을 데이터로 추가 */
	void GenerateJson();
	
private:
	/** .cpp와 .h를 생성 */
	void GenerateSourceCode(std::vector<std::string> cellNames, std::string workSheetName, std::string outputDirectory);

	/** .h에 들어갈 row code를 반환 */
	std::string CreateHeaderCode(std::vector<std::string> cellNames, std::string workSheetName);

	/** .cpp에 들어갈 row code를 반환 */
	std::string CreateSourceCode(std::string workSheetName);
};