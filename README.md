# wav2png

Create high-quality waveform PNG images from audio files.

## About

wav2png generates waveform visualizations similar to those found on SoundCloud. The images are optimized for web use - they are not anti-aliased but look excellent when scaled down by browsers.

**Original Author:** Benjamin Schulz (beschulz[at]betabugs.de)

**License:** GPL v3 or later

**Build Status:** ![Build Status](https://travis-ci.org/beschulz/wav2png.svg?branch=master)

## Features

* **Extensive audio format support:**
  * Native: WAV, FLAC, OGG, AIFF, AU, CAF, and more via libsndfile
  * Automatic conversion: MP3, M4A, AAC, WMA, and 100+ formats via ffmpeg (optional)
* Customizable foreground and background colors with RGBA support
* Optional logarithmic (decibel) scale
* Configurable image dimensions
* Transparent backgrounds and waveforms
* Fast processing - approximately 70 minutes of audio per second on a 2.4 GHz i5
* Line-only mode for minimal waveforms

## Recent Updates

This fork includes the following modernization updates to Benjamin Schulz's original implementation:

* **libpng 1.6+ support** - Upgraded from deprecated libpng 1.2 to modern libpng 1.6+ for better compatibility with current Linux distributions
* **Linux-focused build** - Removed macOS-specific code and Xcode projects to simplify the codebase for Linux server deployments
* **Simplified binary path** - Changed output from bin/Linux/wav2png to bin/wav2png
* **Modern tooling** - Added comprehensive .gitignore and CHANGELOG.md
* **Extended format support** - Automatic ffmpeg integration for MP3, M4A, and 100+ audio formats

All changes maintain complete backward compatibility with the original command-line interface. Full credit for the core waveform algorithms goes to Benjamin Schulz.

## Installation

### Quick Start (Ubuntu/Debian)

Install dependencies:

    sudo apt-get update
    sudo apt-get install make g++ libsndfile1-dev libpng++-dev libpng-dev libboost-program-options-dev

For extended format support (MP3, M4A, etc.), also install ffmpeg:

    sudo apt-get install ffmpeg

Build:

    make all

### Using Configure Script (Recommended)

The configure script automatically detects dependencies and generates optimized build settings:

    ./configure
    make
    sudo make install

Configure options:

    ./configure --prefix=/opt/wav2png    # Custom installation directory
    ./configure --enable-debug           # Debug build with symbols
    ./configure --enable-profiling       # Build with profiling support
    ./configure --cxx=clang++           # Use different compiler

For all options:

    ./configure --help

### Automated Installation

If your Linux distribution supports apt-get:

    make install_dependencies
    make all

## Usage

Basic usage:

    wav2png [options] input_file_name

Get help:

    wav2png --help

### Options

**Generic:**

* `-v, --version` - Print version string
* `--help` - Show help message

**Configuration:**

* `-w, --width ARG` - Image width in pixels (default: 1800)
* `-h, --height ARG` - Image height in pixels (default: 280)
* `-b, --background-color ARG` - Background color in RGBA hex (default: efefefff)
* `-f, --foreground-color ARG` - Foreground color in RGBA hex (default: 00000000)
* `-o, --output ARG` - Output filename (default: input_filename.png)
* `-c, --config-file ARG` - Config file to use (default: wav2png.cfg)
* `-d, --db-scale` - Use logarithmic (decibel) scale instead of linear
* `--db-min ARG` - Minimum dB value visible (default: -48)
* `--db-max ARG` - Maximum dB value visible (default: 0)
* `-l, --line-only` - Draw line only without fill

## Examples

### Basic Waveform

Foreground and background colors in RGBA:

    wav2png --foreground-color=ffb400aa --background-color=2e4562ff -o output.png input.wav

![example0](https://github.com/beschulz/wav2png/raw/master/examples/example0.png)

### Transparent Waveform

Transparent waveform with solid background:

    wav2png --foreground-color=00000000 --background-color=2e4562ff -o output.png input.wav

![example2](https://github.com/beschulz/wav2png/raw/master/examples/example2.png)

### Solid Waveform on Transparent Background

Useful for overlaying on other images:

    wav2png --foreground-color=2e4562ff --background-color=00000000 -o output.png input.wav

![example3](https://github.com/beschulz/wav2png/raw/master/examples/example3.png)

### Short Samples

wav2png handles short audio samples gracefully:

    wav2png --foreground-color=2e4562ff --background-color=00000000 -o output.png short.wav

![example4](https://github.com/beschulz/wav2png/raw/master/examples/example4.png)

### Decibel Scale

Use logarithmic scale for better visualization:

    wav2png --foreground-color=ffb400aa --background-color=2e4562ff -d -o output.png input.wav

![example7](https://github.com/beschulz/wav2png/raw/master/examples/example7.png)

### Custom dB Range

Specify minimum and maximum dB values:

    wav2png --foreground-color=ffb400aa --background-color=2e4562ff -d --db-min -40 --db-max 3 -o output.png input.wav

![example8](https://github.com/beschulz/wav2png/raw/master/examples/example8.png)

### MP3 and Other Formats

With ffmpeg installed, wav2png automatically handles MP3, M4A, AAC, and other formats:

    wav2png song.mp3 -o output.png
    wav2png podcast.m4a --foreground-color=2e4562ff -o output.png

No additional flags needed - conversion happens automatically!

## Color Format

Colors are specified in RGBA hexadecimal format: `rrggbbaa`

**Examples:**

* `ff0000ff` - 100% red, 100% opaque
* `ff00007f` - 100% red, 50% opaque
* `00ff00ff` - 100% green, 100% opaque
* `0000ffff` - 100% blue, 100% opaque
* `ffffff00` - White, fully transparent

**Tip:** Use Photoshop or an online color picker to get RGB values, then add the alpha channel value (00-ff for 0-100% opacity).

## Performance

Performance was a primary design goal. wav2png is optimized for speed:

* Converts a 2 hour 11 minute mono 16-bit WAV file in approximately 1.8 seconds
* Processing rate: ~70 minutes of audio per second per core (2.4 GHz i5 in VM)
* Single-threaded by design - already faster than disk I/O
* Run multiple instances in parallel for batch processing

The code is intentionally single-threaded because:

* Current performance already exceeds hard disk read speeds
* Multi-threading would increase complexity
* Multiple instances can be run in parallel for batch jobs

## Related Projects

For generating waveformjs.org compatible JSON output, see [wav2json](https://github.com/beschulz/wav2json).

## TODO

* Add channel interpolation options (currently uses max of all channels)
* Support for Mid, Side, Left, and Right channel selection for stereo signals

## Contributing

If you have suggestions for performance improvements or find issues, please open an issue or pull request.

## Credits

**Original Author and Core Algorithms:** Benjamin Schulz

**Contributors:**

* Marvin Killing - GCC compatibility fixes
* John Berthels - Bug fixes
* Additional contributors via merged pull requests

See [CHANGELOG.md](CHANGELOG.md) for detailed version history.

## Support

If you find wav2png useful, consider supporting the original author Benjamin Schulz via [PayPal donation](http://goo.gl/Ey2Bp). Donations are appreciated but not required to use the software.
