#include "Version.hpp"

#include <exception>
#include <regex>
#include <format>

namespace {
	// We will only attempt to parse versions of this pattern.
	// It matches Semantic Versioning 2.0.0 without the extensions for labels.
	const char* const SEMVER_EXPRESSION = R"(^(\d+)\.(\d+)\.(\d+)$)";

	const std::regex version_regex(SEMVER_EXPRESSION);
}

Version Version::ChooseHigher(const Version& a, const Version& b)
{
	if (!a._is_comparable || !b._is_comparable) {
		return Version(0, 0, 0, false);
	}

	if (a._major > b._major) {
		return a;
	}

	if (b._major > a._major) {
		return b;
	}

	if (a._minor > b._minor) {
		return a;
	}

	if (b._minor > a._minor) {
		return b;
	}

	if (a._patch > b._patch) {
		return a;
	}

	if (b._patch > a._patch) {
		return b;
	}

	// They're the same... just choose one.
	return a;
}

Version::Version(const std::string& version_string) :
	_is_comparable(false),
	_major(),
	_minor(),
	_patch()
{
	std::smatch match;
	if (std::regex_match(version_string, match, version_regex)) {
		_is_comparable = true;
		_major = atoi(match[1].str().c_str());
		_minor = atoi(match[2].str().c_str());
		_patch = atoi(match[3].str().c_str());
	}
}

std::string Version::ToString() const
{
	if (!_is_comparable) {
		throw std::logic_error("An incomparable `Version` cannot be converted to a string.");
	}
	return std::format("{}.{}.{}", _major, _minor, _patch);
}

bool Version::operator!=(const Version& other) const
{
	if (!_is_comparable || !other._is_comparable) {
		return false;
	}

	return _major != other._major || _minor != other._minor || _patch != other._patch;
}
