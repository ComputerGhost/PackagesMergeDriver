#include <functional>
#include <iostream>
#include <source_location>

#include "PackagesConfigTests.hpp"
#include "TestUtilities.hpp"

#define RUN_TEST(f) RunTest(#f, f)

namespace {
	void RunTest(const std::string& test_name, std::function<void()> test)
	{
		std::cout << "Running test: " << test_name << "...";
		try {
			test();
			std::cout << "pass." << std::endl;
		}
		catch (const std::exception &) {
			std::cout << "fail." << std::endl;
			throw;
		}
	}
}

int main(int argc, char* argv[])
{
	try {

		RUN_TEST(PackagesConfigTests::WhenUnchanged_TakesCurrent);
		RUN_TEST(PackagesConfigTests::WhenUnchanged_AndInvalid_TakesCurrent);

		RUN_TEST(PackagesConfigTests::WhenCurrentUpgraded_TakesCurrent);
		RUN_TEST(PackagesConfigTests::WhenCurrentUpgraded_AndOtherDowngraded_Fails);
		RUN_TEST(PackagesConfigTests::WhenCurrentUpgraded_AndOtherInvalid_Fails);
		RUN_TEST(PackagesConfigTests::WhenCurrentUpgraded_AndOtherDeleted_TakesOther);

		RUN_TEST(PackagesConfigTests::WhenCurrentDowngraded_TakesCurrent);
		RUN_TEST(PackagesConfigTests::WhenCurrentDowngraded_AndOtherUpgraded_Fails);
		RUN_TEST(PackagesConfigTests::WhenCurrentDowngraded_AndOtherDeleted_TakesOther);
		RUN_TEST(PackagesConfigTests::WhenCurrentDowngraded_AndOtherInvalid_Fails);

		RUN_TEST(PackagesConfigTests::WhenCurrentDeleted_AndOtherUpgraded_TakesCurrent);
		RUN_TEST(PackagesConfigTests::WhenCurrentDeleted_AndOtherDowngraded_TakesCurrent);

		RUN_TEST(PackagesConfigTests::WhenCurrentInvalid_AndOtherUpgraded_Fails);
		RUN_TEST(PackagesConfigTests::WhenCurrentInvalid_AndOtherDowngraded_Fails);

	}
	catch (const std::exception &) {
		CleanUpTestFiles();
		throw;
	}
}
