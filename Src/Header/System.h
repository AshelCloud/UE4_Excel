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
	void Generate(XLWorksheet& workSheet, std::string sourceCodePath, std::string headerCodePath, std::string jsonPath);

private:
	/** .cpp와 .h를 생성 */
	bool GenerateSourceCode(std::string workSheetName, std::string sourceCodePath, std::string headerCodePath);

	/** Json 생성 */
	/** A열에 있는 Cell이 Root이며 그 행을 데이터로 추가 */
	bool GenerateJson(XLWorksheet& workSheet, std::string outputDirectory);

	/** TODO: API 이름(현재 TESTPROJECT_API) argv로 받아오기 */
	/** .h에 들어갈 row code를 반환 */
	std::string CreateHeaderCode(std::string workSheetName);

	/** .cpp에 들어갈 row code를 반환 */
	std::string CreateSourceCode(std::string workSheetName);

	/** 구조체에 들어갈 변수명을 채워넣음 */
	void SetDataNames(XLWorksheet& workSheet);

	/** Row를 전부 순회하며 자료형을 결정하는 함수 */
	const std::string ConvertCellValueTypeToString(XLWorksheet& workSheet, char asciiCode);

private:
	/** 1행에 있는 Cell 이름들 (A1번째 Cell 값 제외) */
	std::vector<std::pair<std::string, std::string>> cellDatas;
};