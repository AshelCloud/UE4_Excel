#include "System.h"
#include <filesystem>
#include <iostream>
#include <fstream>

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

void System::GenerateStruct(XLWorksheet& workSheet, std::string workSheetName, std::string outputDirectory)
{
	int asciiCode = 65; // A

	std::vector<std::string> cellNames;

	while (true)
	{
		std::string cellIndex;
		cellIndex = (char(asciiCode));
		cellIndex += "1";

		std::cout << "Å½»ö CellIndex: " << cellIndex << std::endl;

		auto cell = workSheet.cell(XLCellReference(cellIndex)).value();
		if (InvalidValue(cell.valueType()))
		{
			std::cout << "Done!" << std::endl;
			break;
		}

		try
		{
			std::cout << cell.get<std::string>() << std::endl;
			
			if(cellIndex != "A1")
			{
				cellNames.push_back(cell.get<std::string>());
			}
		}
		catch (XLException e)
		{
			std::cout << e.what() << std::endl;
		}

		asciiCode++;
	}

	GenerateSourceCode(cellNames, workSheetName, outputDirectory);
}

void System::GenerateSourceCode(std::vector<std::string> cellNames, std::string workSheetName, std::string outputDirectory)
{
	std::cout << "Generate Struct" << std::endl;

	std::ofstream file;

	file.open(outputDirectory + workSheetName + ".h");
	if (file.is_open())
	{
		std::string Header = CreateHeaderCode(cellNames, workSheetName);
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

std::string System::CreateHeaderCode(std::vector<std::string> cellNames, std::string workSheetName)
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

	for (auto name : cellNames)
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
