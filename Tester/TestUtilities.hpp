#pragma once

#include <fstream>
#include <functional>

enum class PackageFormat
{
	Unknown,
	CsProj,
	PackageJson,
	PackagesConfig,
};

enum class TestFile
{
	Original,
	Current,
	Other,
	Expected,
};

class TestFailedException : std::exception
{
	using std::exception::exception;
};

std::string ToString(PackageFormat format);

PackageFormat ParseFormat(const std::string& format);

bool AreTestFilesEqual(TestFile a_type, TestFile b_type);

void CleanUpTestFiles();

int ExecuteMergeDriver(PackageFormat format);

void WriteTestFile(TestFile type, std::function<void(std::ofstream)> writer);

std::ifstream GetTestFileReader(TestFile type);

