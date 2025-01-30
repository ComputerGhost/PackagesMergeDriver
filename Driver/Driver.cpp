#include "Driver.hpp"

#include <algorithm>
#include <format>
#include <iterator>

#include "PackagesConfigDriver.hpp"

namespace {
	struct changes_comparer
	{
		bool operator()(
			const std::pair<std::string, Version>& a, 
			const std::pair<std::string, Version>& b)
		{
			return a.first < b.first;
		}
	};

	std::map<std::string, Version> FindRemovals(
		const std::map<std::string, Version>& bigger,
		const std::map<std::string, Version>& smaller)
	{
		std::map<std::string, Version> result;
		std::set_difference(
			bigger.begin(), bigger.end(),
			smaller.begin(), smaller.end(),
			std::inserter(result, result.end()),
			changes_comparer());
		return result;
	}

	std::map<std::string, Version> FindAdditions(
		const std::map<std::string, Version>& bigger,
		const std::map<std::string, Version>& smaller)
	{
		// It's the opposite of removals, so...
		return FindRemovals(smaller, bigger);
	}

	std::map<std::string, Version> FindChanges(
		const std::map<std::string, Version>& original,
		const std::map<std::string, Version>& newer)
	{
		std::map<std::string, Version> changes;

		for (auto original_item : original) {
			auto newer_item = newer.find(original_item.first);

			// Ignore additions.
			if (newer_item == newer.end()) {
				continue;
			}

			if (original_item.second != newer_item->second) {
				changes.emplace(newer_item->first, newer_item->second);
			}
		}

		return changes;
	}
}

Driver::FileException Driver::FileException::InvalidFormat(const std::string& filename)
{
	auto message = std::format("File format is not valid: {}", filename);
	return Driver::FileException(message.c_str());
}

Driver::FileException Driver::FileException::UnableToOpen(const std::string& filename)
{
	auto message = std::format("Unable to open file: {}", filename);
	return Driver::FileException(message.c_str());
}

std::unique_ptr<Driver> Driver::Create(Options options)
{
	switch (options.format()) {
	case Options::CsProj:
		throw std::exception("*.csproj merge is not supported yet.");
	case Options::PackageJson:
		throw std::exception("package.json merge is not supported yet.");
	case Options::PackagesConfig:
		return std::make_unique<PackagesConfigDriver>();
	default:
		throw std::logic_error("An invalid format was passed to the driver.");
	}
}

std::optional<std::map<std::string, Version>> Driver::MergeVersions(
	const std::map<std::string, Version>& original,
	const std::map<std::string, Version>& current,
	const std::map<std::string, Version>& other) const
{
	std::map<std::string, Version> results(original);

	for (auto removal : FindRemovals(original, current)) {
		results.erase(removal.first);
	}

	for (auto addition : FindAdditions(original, other)) {
		// Handle case where other and current both added the same thing.
		auto existing = results.find(addition.first);
		if (existing != results.end()) {
			existing->second = Version::ChooseHigher(existing->second, addition.second);
		}
		else {
			results.insert(addition);
		}
	}

	// These sets are usually small, so readability is more important than efficiency.
	auto current_changes = FindChanges(original, current);
	auto other_changes = FindChanges(original, other);
	for (auto change : current_changes) {
		auto other_version = other_changes[change.first];
		results[change.first] = Version::ChooseHigher(change.second, other_version);
	}
	for (auto change : other_changes) {
		auto current_version = current_changes[change.first];
		results[change.first] = Version::ChooseHigher(change.second, current_version);
	}

	// Removals take priority so do these last.
	for (auto removal : FindRemovals(original, other)) {
		results.erase(removal.first);
	}
	for (auto addition : FindAdditions(original, current)) {
		results.insert(addition);
	}

	// Check if we have any bad versions that would invalidate the results.
	for (auto result : results) {
		if (!result.second.is_comparable()) {
			return std::nullopt;
		}
	}

	return results;
}
