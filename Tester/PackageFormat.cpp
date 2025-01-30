#include "PackageFormat.hpp"

#include <stdexcept>
#include <unordered_map>>

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

