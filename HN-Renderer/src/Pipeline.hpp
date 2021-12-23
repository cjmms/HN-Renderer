#pragma once
#include "pch.h"

namespace HN {
	class Pipeline
	{
	public:
		Pipeline(const std::string& vertFilePath, const std::string& fragFilePath);

	private:
		static std::vector<char> readFile(const std::string& filePath);

		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
	};

	static std::wstring ExePath();
}