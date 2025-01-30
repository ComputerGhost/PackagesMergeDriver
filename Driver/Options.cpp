#include "Options.hpp"

#include <algorithm>
#include <format>
#include <unordered_map>

namespace {
    Options::PackageFormat ParseFormat(const std::string& format_str)
    {
        static const std::unordered_map<std::string, Options::PackageFormat> format_map = {
            {"csproj", Options::PackageFormat::CsProj},
            {"package.json", Options::PackageFormat::PackageJson},
            {"packages.config", Options::PackageFormat::PackagesConfig},
        };

        auto it = format_map.find(format_str);
        return (it != format_map.end()) ? it->second : Options::PackageFormat::Unknown;
    }
}

Options::Options(int argc, char* argv[]) :
    _format(PackageFormat::Unknown),
    _original_file(),
    _current_file(),
    _other_file(),
    _show_help(false)
{
    ParseOptions(argc, argv);
}

void Options::RenderHelp(std::ostream& out) const
{
    out << "Merge driver to handle package updates." << std::endl
        << std::endl
        << "Usage:" << std::endl
        << "  {driver} --format=<format> original current other" << std::endl
        << "  {driver} --help" << std::endl
        << std::endl
        << "Options:" << std::endl
        << "  --format=<format>    Specify the package file format. Supported formats:" << std::endl
        << "                       packages.config, csproj, package.json" << std::endl
        << "  --help               Show this help message" << std::endl;
}

void Options::ParseOptions(int argc, char* argv[])
{
    if (argc < 2) {
        throw ParseException("Invalid parameters. Use --help for usage.");
    }

    for (int i = 1; i != argc; ++i) {
        std::string current_arg = argv[i];

        if (current_arg == "--help") {
            _show_help = true;
        }
        else if (current_arg.starts_with("--format=")) {
            _format = ParseFormat(current_arg.substr(9));
            if (_format == Unknown) {
                throw ParseException("Invalid format specified. Use --help for supported formats.");
            }
        }
        else {
            if (_original_file.empty()) _original_file = current_arg;
            else if (_current_file.empty()) _current_file = current_arg;
            else if (_other_file.empty()) _other_file = current_arg;
            else throw ParseException("Too many positional arguments. Use --help for usage.");
        }
    }

    if (!_show_help && (_format == Unknown || _original_file.empty() || _current_file.empty() || _other_file.empty())) {
        throw ParseException("Missing required arguments. Use --help for usage.");
    }
}
