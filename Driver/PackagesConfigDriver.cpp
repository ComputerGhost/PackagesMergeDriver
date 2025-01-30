#include "PackagesConfigDriver.hpp"

#include <format>
#include <fstream>
#include <regex>

namespace {
	struct ReadResults {
		int tab_size = 0;
		std::string framework_version = {};
		std::map<std::string, Version> versions = {};
	};

	const char* const FILE_FIRST_LINE = R"bound(<?xml version="1.0" encoding="utf-8"?>)bound";
	const char* const FILE_SECOND_LINE = R"(<packages>)";
	const char* const FILE_LAST_LINE = R"(</packages>)";
	const char* const FILE_PACKAGE_LINE = R"bound(^(\s*)<package\s+id="(.*?)"\s+version="(.*?)"\s+targetFramework="(.*?)"\s*\/>$)bound";

	const std::regex package_info_regex(FILE_PACKAGE_LINE);

	ReadResults ReadVersions(const std::string& filename)
	{
		ReadResults results;

		std::ifstream input(filename);
		if (input.fail()) {
			throw Driver::FileException::UnableToOpen(filename);
		}

		std::string line;

		if (!std::getline(input, line) || line != FILE_FIRST_LINE) {
			throw Driver::FileException::InvalidFormat(filename);
		}

		if (!std::getline(input, line) || line != FILE_SECOND_LINE) {
			throw Driver::FileException::InvalidFormat(filename);
		}

		std::smatch match;
		while (std::getline(input, line) && std::regex_match(line, match, package_info_regex)) {
			if (results.tab_size == 0) {
				results.tab_size = match[1].str().length();
			}

			auto package_name = match[2];
			auto package_version = match[3];
			results.versions.emplace(package_name, package_version);

			auto framework_version = match[4].str();
			if (results.framework_version.length() > 0 && framework_version != results.framework_version) {
				throw Driver::FileException::InvalidFormat(filename);
			}
			else {
				results.framework_version = framework_version;
			}
		}

		if (line != FILE_LAST_LINE) {
			throw Driver::FileException::InvalidFormat(filename);
		}

		return results;
	}

	void WriteVersions(const std::string& filename, const ReadResults& results)
	{
		std::ofstream output(filename);
		if (output.fail()) {
			throw Driver::FileException::UnableToOpen(filename);
		}

		output << FILE_FIRST_LINE << std::endl;
		output << FILE_SECOND_LINE << std::endl;

		const std::string prefix(results.tab_size, ' ');
		for (auto package : results.versions) {
			auto line = std::format(
				R"bound({}<package id="{}" version="{}" targetFramework="{}" />)bound",
				prefix,
				package.first,
				package.second.ToString(),
				results.framework_version
			);
			output << line << std::endl;
		}

		output << FILE_LAST_LINE;
	}
}

bool PackagesConfigDriver::Run(Options options)
{
	auto original = ReadVersions(options.original_file());
	auto current = ReadVersions(options.current_file());
	auto other = ReadVersions(options.other_file());

	// Don't touch it if the framework version doesn't agree.
	if (current.framework_version != other.framework_version) {
		return false;
	}

	auto new_versions = MergeVersions(original.versions, current.versions, other.versions);

	if (new_versions) {
		new_versions->swap(current.versions);
		WriteVersions(options.current_file(), current);
		return true;
	}

	return true;
}

