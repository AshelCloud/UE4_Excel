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

	/** 구조체 소스코드와 Json 생성 */
	void Generate(XLWorksheet& workSheet, std::string outputDirectory);

private:
	/** .cpp와 .h를 생성 */
	void GenerateSourceCode(std::string workSheetName, std::string outputDirectory);

	/** Json 생성 */
	/** A열에 있는 Cell이 Root이며 그 행을 데이터로 추가 */
	void GenerateJson(XLWorksheet& workSheet);

	/** .h에 들어갈 row code를 반환 */
	std::string CreateHeaderCode(std::string workSheetName);

	/** .cpp에 들어갈 row code를 반환 */
	std::string CreateSourceCode(std::string workSheetName);

private:
	/** 1행에 있는 Cell 이름들 (A1번째 Cell 값 제외) */
	std::vector<std::string> dataNames;
};