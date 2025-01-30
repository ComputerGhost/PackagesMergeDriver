#pragma once

#include <string>

class Version
{
public:
	static Version ChooseHigher(const Version& a, const Version& b);

	Version() = default;

	Version(int major, int minor, int patch, bool is_comparable = true) :
		_major(major), _minor(minor), _patch(patch), _is_comparable(is_comparable)
	{}

	Version(const std::string& versionString);

	bool is_comparable() { return _is_comparable; }
	int major() { return _major; }
	int minor() { return _minor; }
	int patch() { return _patch; }

	std::string ToString() const;

	bool operator!= (const Version& other) const;

private:
	bool _is_comparable;
	int _major;
	int _minor;
	int _patch;
};
