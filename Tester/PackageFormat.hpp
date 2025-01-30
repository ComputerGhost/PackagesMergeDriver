#pragma once

#include <string>

// This is giving a LNK4042 warning.
// I don't understand the linker's reasoning so we will move on.
enum class PackageFormat
{
	Unknown,
	CsProj,
	PackageJson,
	PackagesConfig,
};

inline std::string ToString(PackageFormat format) { return ""; }

inline PackageFormat ParseFormat(const std::string& format) { return PackageFormat::Unknown; }
