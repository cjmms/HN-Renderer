#include "pch.h"

#include "Pipeline.hpp"


namespace HN {

	Pipeline::Pipeline(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		createGraphicsPipeline(vertFilePath, fragFilePath);
	}

	std::vector<char> Pipeline::readFile(const std::string& filePath)
	{
		// read file in binary to avoid unexpected text translation
		std::ifstream file{filePath, std::ios::ate | std::ios::binary};

		// check if file is open
		if (!file.is_open()) throw std::runtime_error("Failed to open file(check if address is correct): " + filePath);

		// file.tellg() returns last position of file
		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);

		// move to the beginning of the file
		file.seekg(0);

		// read file into buffer
		file.read(buffer.data(), fileSize);

		file.close();
		return buffer;
	}


	void Pipeline::createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		auto vertCode = readFile(vertFilePath);
		auto fragCode = readFile(fragFilePath);

		std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl;
		std::cout << "Frag Shader Code Size: " << fragCode.size() << std::endl;
	}



	std::wstring ExePath() {
		TCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, buffer, MAX_PATH);
		std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
		return std::wstring(buffer).substr(0, pos);
	}


}