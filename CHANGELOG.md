# Changelog

All notable changes to wav2png are documented in this file.

## [0.9] - 2025-10-20

### Added

*   **Extended audio format support** - Automatic conversion via ffmpeg for 100+ formats including MP3, M4A, AAC, WMA, and more
*   **Flexible color specification** - Support for both 6-character RGB (`ff0000`) and 8-character RGBA (`ff0000ff`) hex color formats
*   **Automatic format detection** - Seamlessly falls back to ffmpeg when libsndfile cannot read a format
*   New `audio_converter` module for transparent format conversion
*   FIFO-based audio streaming for efficient memory usage during conversion

### Changed

*   Default color format now accepts simplified 6-character RGB values (automatically appends `ff` for full opacity)
*   Updated all documentation examples to use simpler RGB format where transparency is not needed
*   Improved error messages for color parsing to indicate both supported formats
*   Modified default foreground and background colors to use 6-character format (`000000` and `efefef`)

### Technical Details

*   Implemented FFmpegConverter class with FIFO pipe handling
*   Added automatic ffmpeg availability detection
*   Enhanced color parser to handle both RGB and RGBA formats
*   No temporary files created during audio conversion (uses named pipes)
*   Graceful degradation when ffmpeg is not installed

## [Unreleased] - 2025-10-18

### Changed

* Upgraded from libpng 1.2 to libpng 1.6+ for better compatibility with modern Linux distributions
* Changed binary output path from bin/Linux/wav2png to bin/wav2png
* Removed all macOS-specific code, Xcode projects, and wav2pngDroplet application
* Simplified Makefile to Linux-only builds
* Updated README.md with recent changes documentation
* Converted README.md to use only Daring Fireball Markdown specification
* Modernized entire codebase to C++17 standard
* Refactored all source files with modern C++ idioms and best practices
* Improved code safety with noexcept specifications and const correctness
* Enhanced error handling with comprehensive exception handling in main
* Replaced header guards with pragma once directives
* Updated Makefile to use C++17 compiler flags
* Moved build system to project root (build/ directory removed)
* Simplified build process - now run make from project root

### Added

* Comprehensive .gitignore for C/C++ development
* CHANGELOG.md file
* TODO.md with community-driven feature suggestions
* Configure script for automatic dependency detection and build configuration
* Install and uninstall targets in Makefile
* Help target in Makefile for listing available build targets
* C++17 standard library features (std::clamp, constexpr, auto, etc.)
* std::function for flexible callback mechanism
* Anonymous namespaces for better code organization
* Static analysis friendly constructs (explicit, noexcept)
* Comprehensive code documentation and comments

### Removed

* macOS/Darwin build support
* Xcode project files
* Objective-C code and macOS droplet application
* Platform-specific build conditionals
* C-style casts (replaced with static_cast)
* C-style headers (replaced with C++ equivalents)
* Old-style typedef declarations
* Legacy exception specifications (throw())
* Custom clamp implementation (using std::clamp)

### Technical Improvements

* Replaced fabs with std::abs
* Changed typedef to using alias declarations
* Added constexpr to compile-time constants
* Improved type safety with static_cast
* Used .data() method instead of &vector[0]
* Better structured conditional expressions
* Extracted helper methods for better modularity
* Fixed typos in user-facing messages

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
