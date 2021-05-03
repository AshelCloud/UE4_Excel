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


/** TODO: 备炼眉 积己何盒 盒府 */
void System::Generate(XLWorksheet& workSheet, std::string outputDirectory)
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
			if(cellIndex != "A1")
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

	GenerateSourceCode(workSheet.name(), outputDirectory);
	GenerateJson(workSheet);
}

void System::GenerateJson(XLWorksheet& workSheet)
{
	std::cout << "Generate Json to " + workSheet.name() << std::endl;

	if (dataNames.empty())
	{
		std::cout << "ERROR: DataName Is Empty" << std::endl;
		exit(-1);
	}

	/** TODO: Json 单捞磐 积己 */
	Json::Value root;

	Json::StreamWriterBuilder writer;
	std::cout << Json::writeString(writer, root) << std::endl;
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
