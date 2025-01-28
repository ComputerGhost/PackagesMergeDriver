# PackagesMergeDriver

Reduce merge conflicts caused by package updates by intelligently choosing package versions.

## Installation

Add the following to your ".git/config" file:

```
[merge "packages"]
	name = A merge driver for handling package upgrades.
	driver = path/to/driver.exe %O %A %B
```

Replace "path/to/driver.exe" with the path to the driver executable.

Add the following to your ".gitattributes" file:

```
packages.config merge=packages
```

## Usage

When you merge a branch that has an update due to a package update, 
the merge driver will automatically resolve version conficts 
for packages that use semantic versioning.

TODO: Explain how the package versions are chosen.

