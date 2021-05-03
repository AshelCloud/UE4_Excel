#include "System.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <json.h>

System::System()
{
}

System::~System()
{
}

std::vector<std::string> System::GetFilesName(std::string filePath) const
{
	std::vector<std::string> output;

	for(const auto& file : std::filesystem::directory_iterator(filePath))
	{
		output.push_back(file.path().string());
	}
	
	return output;
}

bool System::InvalidValue(XLValueType type)
{
	return type == XLValueType::Error || type == XLValueType::Empty;
}

void System::Generate(XLWorksheet& workSheet, std::string outputDirectory)
{
	SetDataNames(workSheet);

	GenerateSourceCode(workSheet.name(), outputDirectory);
	GenerateJson(workSheet, outputDirectory);
}

void System::GenerateJson(XLWorksheet& workSheet, std::string outputDirectory)
{
	std::cout << "Generate Json to " + workSheet.name() << std::endl;

	if (dataNames.empty())
	{
		std::cout << "ERROR: DataName Is Empty" << std::endl;
		exit(-1);
	}

	/** TODO: Json 데이터 생성 */
	/** 테스트 형식 작성후 UE4에서 파싱*/
	/** 파싱 성공한 형식에 맞춰서 데이터 생성하기 */

	Json::Value root;

	int asciiCode = 66;
	int Index = 3;

	Json::Value id;

	Json::Value data;
	for (auto name : dataNames)
	{
		std::string cellIndex;
		cellIndex = ((char)asciiCode);
		cellIndex += std::to_string(Index);
		auto cell = workSheet.cell(XLCellReference(cellIndex));

		std::cout << name << ": " << cell.value().get<int>() << std::endl;

		data[name] = cell.value().get<int>();

		asciiCode++;
	}
	
	id.append(data);

	root["id"] = id;

	Json::StreamWriterBuilder writer;

	std::string jsonString = Json::writeString(writer, root);

	std::ofstream file;
	file.open(outputDirectory + "Json\\" + workSheet.name() + ".json");
	if(file.is_open())
	{
		file.write(jsonString.c_str(), jsonString.size());
	}
	file.close();
}

void System::GenerateSourceCode(std::string workSheetName, std::string outputDirectory)
{
	std::cout << "Generate SourceCode to " + workSheetName << std::endl;

	if (dataNames.empty())
	{
		std::cout << "ERROR: DataName Is Empty" << std::endl;
		exit(-1);
	}

	std::ofstream file;

	file.open(outputDirectory + workSheetName + ".h");
	if (file.is_open())
	{
		std::string Header = CreateHeaderCode(workSheetName);
		file.write(Header.c_str(), Header.size());
	}
	file.close();

	file.open(outputDirectory + workSheetName + ".cpp");
	if (file.is_open())
	{
		std::string Source = CreateSourceCode(workSheetName);
		file.write(Source.c_str(), Source.size());
	}
	file.close();
}

std::string System::CreateHeaderCode(std::string workSheetName)
{
	std::string HeaderCode =
		"#pragma once\n"
		"\n"
		"#include \"CoreMinimal.h\"\n"
		"#include \"" + workSheetName + ".generated.h\"\n"
		"\n"
		"USTRUCT(Atomic, BlueprintType)\n"
		"struct F" + workSheetName + "\n"
		"{\n"
		"\tGENERATED_USTRUCT_BODY()\n"
		"public:\n";

	for (auto name : dataNames)
	{
		HeaderCode +=
			"\tUPROPERTY(EditAnywhere, BlueprintReadWrite)\n"
			"\tint " + name + ";\n";
	}

	HeaderCode +=
		"};\n\n"
		"class TESTPROJECT_API " + workSheetName + "\n"
		"{\n"
		"public:\n"
		"\t" + workSheetName + "();\n"
		"\t~" + workSheetName + "();\n"
		"};\n";

	return HeaderCode;
}

std::string System::CreateSourceCode(std::string workSheetName)
{
	std::string SourceCode =
		"#include \"" + workSheetName + ".h\"\n"
		"" + workSheetName + "::" + workSheetName + "()\n"
		"{\n}\n"
		"" + workSheetName + "::" + "~" + workSheetName + "()\n"
		"{\n}\n";

	return SourceCode;
}

void System::SetDataNames(XLWorksheet& workSheet)
{
	int asciiCode = 65; // A

	while (true)
	{
		std::string cellIndex;
		cellIndex = (char(asciiCode));
		cellIndex += "1";

		auto cell = workSheet.cell(XLCellReference(cellIndex)).value();
		if (InvalidValue(cell.valueType()))
		{
			break;
		}

		try
		{
			if (cellIndex != "A1")
			{
				dataNames.push_back(cell.get<std::string>());
			}
		}
		catch (XLException e)
		{
			std::cout << e.what() << std::endl;
		}

		asciiCode++;
	}
}
