#include "TestUtilities.hpp"

#include <algorithm>
#include <filesystem>
#include <iterator>
#include <stdexcept>
#include <unordered_map>

namespace {
	const char* GetFilename(TestFile type)
	{
		switch (type) {
		case TestFile::Original:
			return "Temp_Original";
		case TestFile::Current:
			return "Temp_Current";
		case TestFile::Other:
			return "Temp_Other";
		case TestFile::Expected:
			return "Temp_Expected";
		default:
			throw std::exception("Unexpected test file type.");
		}
	}
}

std::string ToString(PackageFormat format)
{
	switch (format) {
	case PackageFormat::CsProj: return "csproj";
	case PackageFormat::PackageJson: return "package.json";
	case PackageFormat::PackagesConfig: return "packages.config";
	default: throw std::logic_error("Package format is invalid.");
	}
}

PackageFormat ParseFormat(const std::string& format)
{
	static const std::unordered_map<std::string, PackageFormat> format_map = {
		{"csproj", PackageFormat::CsProj},
		{"package.json", PackageFormat::PackageJson},
		{"packages.config", PackageFormat::PackagesConfig},
	};

	auto it = format_map.find(format);
	return (it != format_map.end()) ? it->second : PackageFormat::Unknown;
}

bool AreTestFilesEqual(TestFile a_type, TestFile b_type)
{
	const char* a_filename = GetFilename(a_type);
	const char* b_filename = GetFilename(b_type);
	std::ifstream a_istream(a_filename, std::ifstream::binary | std::ifstream::ate);
	std::ifstream b_istream(b_filename, std::ifstream::binary | std::ifstream::ate);

	if (a_istream.tellg() != b_istream.tellg()) {
		return false;
	}

	a_istream.seekg(0, std::ifstream::beg);
	b_istream.seekg(0, std::ifstream::beg);
	return std::equal(
		std::istreambuf_iterator<char>(a_istream.rdbuf()),
		std::istreambuf_iterator<char>(),
		std::istreambuf_iterator<char>(b_istream.rdbuf()));
}

void CleanUpTestFiles()
{
	std::filesystem::remove("Temp_Original");
	std::filesystem::remove("Temp_Current");
	std::filesystem::remove("Temp_Other");
	std::filesystem::remove("Temp_Expected");
}

int ExecuteMergeDriver(PackageFormat format)
{
	auto command = std::format(
		"Driver.Exe --format={} {} {} {}",
		ToString(format),
		GetFilename(TestFile::Original),
		GetFilename(TestFile::Current),
		GetFilename(TestFile::Other)
	);

	return system(command.c_str());
}

void WriteTestFile(TestFile type, std::function<void(std::ofstream)> writer)
{
	const char* filename = GetFilename(type);
	writer(std::ofstream(filename));
}

std::ifstream GetTestFileReader(TestFile type)
{
	const char* filename = GetFilename(type);
	return std::ifstream(filename);
}

