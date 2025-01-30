#pragma once

#include <exception>
#include <iostream>
#include <string>

class Options
{
public:
	struct ParseException : std::exception
	{
		using std::exception::exception;
	};

	enum PackageFormat
	{
		Unknown,
		CsProj,
		PackageJson,
		PackagesConfig,
	};

	Options(int argc, char* argv[]);

	void RenderHelp(std::ostream& out) const;

	PackageFormat format() const { return _format; }
	const std::string& original_file() const { return _original_file; }
	const std::string& current_file() const { return _current_file; }
	const std::string& other_file() const { return _other_file; }
	bool show_help() const { return _show_help; }

private:
	void ParseOptions(int argc, char* argv[]);

	PackageFormat _format;
	std::string _original_file;
	std::string _current_file;
	std::string _other_file;
	bool _show_help;
};
