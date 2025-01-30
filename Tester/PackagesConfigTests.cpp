#include "PackagesConfigTests.hpp"

#include <iostream>

#include "TestUtilities.hpp"

void PackagesConfigTests::WhenUnchanged_TakesCurrent()
{
	// Assert
	auto writer = [](auto output)
	{
		output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
			<< "<packages>" << std::endl
			<< R"(  <package id="package" version="1.0.0" targetFramework="net472" />)" << std::endl
			<< "</packages>";
	};
	WriteTestFile(TestFile::Original, writer);
	WriteTestFile(TestFile::Current, writer);
	WriteTestFile(TestFile::Other, writer);
	WriteTestFile(TestFile::Expected, writer);

	// Act
	ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (!AreTestFilesEqual(TestFile::Current, TestFile::Expected)) {
		throw TestFailedException();
	}
}

void PackagesConfigTests::WhenUnchanged_AndInvalid_TakesCurrent()
{
	// Assert
	auto writer = [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package" version="invalid" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		};
	WriteTestFile(TestFile::Original, writer);
	WriteTestFile(TestFile::Current, writer);
	WriteTestFile(TestFile::Other, writer);
	WriteTestFile(TestFile::Expected, writer);

	// Act
	ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (!AreTestFilesEqual(TestFile::Current, TestFile::Expected)) {
		throw TestFailedException();
	}
}

void PackagesConfigTests::WhenCurrentUpgraded_TakesCurrent()
{
	// Assert
	WriteTestFile(TestFile::Original, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Current, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.0.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="2.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Other, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Expected, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.0.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="2.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});

	// Act
	ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (!AreTestFilesEqual(TestFile::Current, TestFile::Expected)) {
		throw TestFailedException();
	}
}

void PackagesConfigTests::WhenCurrentUpgraded_AndOtherDowngraded_Fails()
{
	// Assert
	WriteTestFile(TestFile::Original, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Current, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.2" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.2.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="2.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Other, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.0.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="0.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Expected, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.2" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.2.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="2.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});

	// Act
	ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (!AreTestFilesEqual(TestFile::Current, TestFile::Expected)) {
		throw TestFailedException();
	}
}

void PackagesConfigTests::WhenCurrentUpgraded_AndOtherDeleted_TakesOther()
{
	// Assert
	WriteTestFile(TestFile::Original, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Current, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.2" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.2.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="2.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Other, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Expected, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.2" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.2.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="2.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});

	// Act
	ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (!AreTestFilesEqual(TestFile::Current, TestFile::Expected)) {
		throw TestFailedException();
	}
}

void PackagesConfigTests::WhenCurrentUpgraded_AndOtherInvalid_Fails()
{
	// Assert
	WriteTestFile(TestFile::Original, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Current, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.2" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.2.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="2.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Other, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1-alpha1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="invalid" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});

	// Act
	auto return_value = ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (return_value != EXIT_SUCCESS) {
		throw TestFailedException();
	}
}

void PackagesConfigTests::WhenCurrentDowngraded_TakesCurrent()
{
	// Assert
	WriteTestFile(TestFile::Original, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Current, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.0.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="0.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Other, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Expected, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.0.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="0.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});

	// Act
	ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (!AreTestFilesEqual(TestFile::Current, TestFile::Expected)) {
		throw TestFailedException();
	}
}

void PackagesConfigTests::WhenCurrentDowngraded_AndOtherUpgraded_Fails()
{
	// Assert
	WriteTestFile(TestFile::Original, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Current, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.0.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="0.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Other, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.2" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.2.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="2.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});

	// Act
	int return_value = ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (return_value != EXIT_SUCCESS) {
		throw TestFailedException();
	}
}

void PackagesConfigTests::WhenCurrentDowngraded_AndOtherDeleted_TakesOther()
{
	// Assert
	WriteTestFile(TestFile::Original, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="framework-info" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Current, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.0.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="0.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="framework-info" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Other, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="framework-info" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Expected, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="framework-info" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});

	// Act
	ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (!AreTestFilesEqual(TestFile::Current, TestFile::Expected)) {
		throw TestFailedException();
	}
}

void PackagesConfigTests::WhenCurrentDowngraded_AndOtherInvalid_Fails()
{
	// Assert
	WriteTestFile(TestFile::Original, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Current, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.0.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="0.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Other, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1-alpha1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="invalid" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Expected, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.0.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="0.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});

	// Act
	ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (!AreTestFilesEqual(TestFile::Current, TestFile::Expected)) {
		throw TestFailedException();
	}
}

void PackagesConfigTests::WhenCurrentDeleted_AndOtherUpgraded_TakesCurrent()
{
	// Assert
	WriteTestFile(TestFile::Original, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="framework-info" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Current, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="framework-info" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Other, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.2" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.2.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="2.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="framework-info" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Expected, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="framework-info" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});

	// Act
	ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (!AreTestFilesEqual(TestFile::Current, TestFile::Expected)) {
		throw TestFailedException();
	}
}

void PackagesConfigTests::WhenCurrentDeleted_AndOtherDowngraded_TakesCurrent()
{
	// Assert
	WriteTestFile(TestFile::Original, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="framework-info" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Current, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="framework-info" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Other, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.0.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="0.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="framework-info" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Expected, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="framework-info" version="1.0.0" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});

	// Act
	ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (!AreTestFilesEqual(TestFile::Current, TestFile::Expected)) {
		throw TestFailedException();
	}
}

void PackagesConfigTests::WhenCurrentInvalid_AndOtherUpgraded_Fails()
{
	// Assert
	WriteTestFile(TestFile::Original, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Current, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1-alpha" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="invalid" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Other, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.2" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.2.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="2.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});

	// Act
	int return_value = ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (return_value != EXIT_SUCCESS) {
		throw TestFailedException();
	}
}

void PackagesConfigTests::WhenCurrentInvalid_AndOtherDowngraded_Fails()
{
	// Assert
	WriteTestFile(TestFile::Original, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Current, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.1-alpha" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="invalid" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="1.1.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});
	WriteTestFile(TestFile::Other, [](auto output)
		{
			output << R"(<?xml version="1.0" encoding="utf-8"?>)" << std::endl
				<< "<packages>" << std::endl
				<< R"(  <package id="package1" version="1.1.0" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package2" version="1.0.1" targetFramework="net472" />)" << std::endl
				<< R"(  <package id="package3" version="0.1.1" targetFramework="net472" />)" << std::endl
				<< "</packages>";
		});

	// Act
	int return_value = ExecuteMergeDriver(PackageFormat::PackagesConfig);

	// Assert
	if (return_value != EXIT_SUCCESS) {
		throw TestFailedException();
	}
}