#pragma once

#include <exception>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "Version.hpp"

#include "Options.hpp"

class Driver
{
public:
	struct FileException : std::exception
	{
		using std::exception::exception;

		static FileException UnableToOpen(const std::string& filename);
		static FileException InvalidFormat(const std::string& filename);
	};

	static std::unique_ptr<Driver> Create(Options options);

	virtual bool Run(Options options) = 0;

protected:
	std::optional<std::map<std::string, Version>> MergeVersions(
		const std::map<std::string, Version>& original,
		const std::map<std::string, Version>& current,
		const std::map<std::string, Version>& other
	) const;
};
