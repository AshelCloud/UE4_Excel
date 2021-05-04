#include "System.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <json.h>
#include <cmath>

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
	return (type == XLValueType::Error) || (type == XLValueType::Empty);
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

	auto ConvertCellToJsonValue = [](std::string key, XLCellValue Cell) 
	{
		Json::Value data;
		switch (Cell.valueType())
		{
		case XLValueType::Float:
			data[key] = Cell.get<double>();
			break;

		case XLValueType::Integer:
			data[key] = Cell.get<int>();
			break;

		case XLValueType::String:
			data[key] = Cell.get<std::string>();
			break;

		case XLValueType::Boolean:
			data[key] = Cell.get<bool>();
			break;
		}

		std::cout << key << ": " << data[key] << std::endl;

		return data;
	};

	if (cellDatas.empty())
	{
		std::cout << "ERROR: DataName Is Empty" << std::endl;
		exit(-1);
	}

	/** TODO: Json 데이터 생성 */
	Json::Value root;


	Json::Value id;

	int Index = 3;
	while(true)
	{
		int asciiCode = 66;

		auto checkInvalid = workSheet.cell(XLCellReference("A" + std::to_string(Index)));
		if(InvalidValue(checkInvalid.valueType()))
		{
			break;
		}

		Json::Value data;
		for (auto name : cellDatas)
		{
			std::string cellIndex;
			cellIndex = ((char)asciiCode);
			cellIndex += std::to_string(Index);

			auto currentCell = workSheet.cell(XLCellReference(cellIndex));

			data[name.second] = ConvertCellToJsonValue(name.second, currentCell.value());

			asciiCode++;
		}

		id.append(data);
		Index++;
	}

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

	if (cellDatas.empty())
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

	for (auto name : cellDatas)
	{
		HeaderCode +=
			"\tUPROPERTY(EditAnywhere, BlueprintReadWrite)\n"
			"\t" + name.first + " " + name.second + ";\n";
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
	if(!cellDatas.empty())
	{
		cellDatas.clear();
	}

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
				std::string toString;
				toString = ((char)asciiCode);
				cellDatas.push_back(std::make_pair(ConvertCellValueTypeToString(workSheet, asciiCode), cell.get<std::string>()));
			}
		}
		catch (XLException e)
		{
			std::cout << e.what() << std::endl;
		}

		asciiCode++;
	}
}

const std::string& System::ConvertCellValueTypeToString(XLWorksheet& workSheet, char asciiCode)
{
	std::string result;
	int index = 3;

	while(true)
	{
		std::string toString;
		toString = asciiCode;

		auto checkDataType = workSheet.cell(XLCellReference(toString + std::to_string(index))).value();
		if(InvalidValue(checkDataType.valueType()))
		{
			break;
		}

		switch (checkDataType.valueType())
		{
		case XLValueType::Boolean:
			result = "bool";
			break;
		case XLValueType::Float:
			result = "float";
			break;
		case XLValueType::Integer:
			if (result == "float") { break; }
			result = "int";
			break;
		case XLValueType::String:
			result = "FString";
			break;
		default:
			std::cout << "ERROR: Failed Convert(Error Type)" << std::endl;
			break;
		}

		index++;
	}

	return result;
}
