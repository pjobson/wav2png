# Changelog

All notable changes to wav2png are documented in this file.

## [Unreleased] - 2025-10-18

### Changed

* Upgraded from libpng 1.2 to libpng 1.6+ for better compatibility with modern Linux distributions
* Changed binary output path from bin/Linux/wav2png to bin/wav2png
* Removed all macOS-specific code, Xcode projects, and wav2pngDroplet application
* Simplified Makefile to Linux-only builds
* Updated README.md with recent changes documentation
* Converted README.md to use only Daring Fireball Markdown specification

### Added

* Comprehensive .gitignore for C/C++ development
* CHANGELOG.md file

### Removed

* macOS/Darwin build support
* Xcode project files
* Objective-C code and macOS droplet application
* Platform-specific build conditionals

## Previous History - Benjamin Schulz (Original Author)

### 2016-04-14 - commit 80e277e

* Made Droplet Sandbox ready

### 2015-03-24 - commit 7400cb3

* Fixed issues with wav2png droplet with Xcode versions that are not ancient

### 2015-03-17 - commit 4dd22a8

* Disabled warning in Makefile because gcc on travis does not support it

### 2015-03-17 - commit 54678f9

* Merged pull request from playtestcloud/master

### 2015-03-17 - commit 4ec2023 (Marvin Killing)

* Ignore warning for unused-local-typedefs that occur with boost-1.54.0 and gcc-4.8

### 2014-10-29 - commit a3fc0a9

* Deleted OS X binary from repository

### 2014-10-29 - commit 427978b

* Rebuilt binary for OS X

### 2014-10-29 - commit 2f6b74f

* Improved compatibility with older OS X versions

### 2014-10-29 - commit a0c5b7f

* Fixed compilation on travis (linux)

### 2014-10-29 - commit 116ef8b

* Made batch_process.py executable

### 2014-10-29 - commit 3b054ef

* Added batch-processing script
* Added git-ignore for wav2png_batch_output

### 2014-10-29 - commit c142a3e

* Recompiled wav2png

### 2013-06-23 - commit 4032734

* Re-added brew update

### 2013-06-23 - commit 58fa8db

* Modified Makefile so that dependencies do not get installed more than once

### 2013-06-23 - commit 9edfeae

* Enabled multi-OS builds on travis

### 2013-06-23 - commit 457fba5

* Fixed source highlighting

### 2013-06-23 - commit bec6a65

* Added travis support

## Credits

Original implementation and core algorithms: Benjamin Schulz

Contributors:

* Benjamin Schulz - Original author and maintainer
* Marvin Killing - GCC compatibility fixes
* John Berthels - Uninitialized variable warning fix
* Additional contributors via merged pull requests
