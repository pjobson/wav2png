# TODO - Feature Suggestions

This document tracks potential features and improvements for wav2png.

## High Priority

### Channel Selection and Visualization

* Add support for Mid, Side, Left, and Right channel selection for stereo signals (noted in original TODO)
* Implement separate waveform visualization for each channel in stereo files
* Add channel mixing options (average, max, min, RMS)
* Support for multi-channel audio beyond stereo

### Performance Optimizations

* Multi-threaded processing for batch operations
* SIMD optimizations for sample processing (SSE, AVX)
* Memory-mapped file I/O for large audio files
* Chunk-based processing to reduce memory footprint
* Profile-guided optimization builds

### Output Format Improvements

* SVG output for scalable web graphics
* WebP output for better compression
* Multiple output formats in single run
* Animated waveform GIF generation
* Raw data export (JSON, CSV) for custom rendering

## Medium Priority

### Visualization Enhancements

* Gradient color support for waveforms
* Alpha gradient transitions
* Peak detection with visual markers
* Time grid overlays with configurable intervals
* Frequency-based coloring (spectral waveform)
* RMS overlay visualization
* Customizable vertical scaling

### User Interface Improvements

* Verbose mode with detailed processing information
* Quiet mode for scripting
* Dry-run mode to preview settings
* Progress bar with ETA
* Summary statistics after processing
* Validation mode to check output quality

### Configuration and Flexibility

* YAML/JSON configuration file support
* Configuration profiles (web, print, mobile)
* Template system for common use cases
* Environment variable support for settings
* Config file inheritance and merging

### Audio Processing

* Automatic gain normalization option
* DC offset removal
* High-pass/low-pass filtering before visualization
* Sample rate conversion
* Mono mixdown option for stereo files

## Low Priority

### Metadata and Annotations

* Embed audio metadata in PNG comments
* Add timestamp watermarks
* Custom text annotations on waveform
* Copyright and attribution overlays
* Processing parameters in metadata

### Batch Processing Improvements

* Modernize batch_process.py to Python 3
* Add command-line arguments to batch script
* Parallel batch processing
* Resume capability for interrupted batches
* File watching for automatic processing
* Integration with audio management systems

### Quality of Life

* Shell completion scripts (bash, zsh, fish)
* Man page generation
* Desktop integration (MIME types, thumbnails)
* Plugin system for custom renderers
* Pre/post processing hooks

### Advanced Features

* Spectrogram generation mode
* Mel-frequency visualization
* Beat detection and marking
* Silence detection and trimming visualization
* Comparative waveform overlay (diff mode)
* Zoom and pan for specific time ranges

## Build and Distribution

### Build System

* CMake build system as alternative to Make
* Meson build system support
* pkg-config integration
* Debian package creation
* RPM package creation
* AppImage distribution
* Docker container for reproducible builds

### Testing and Quality

* Unit test suite
* Integration tests with sample files
* Fuzzing for robustness
* Continuous integration improvements
* Code coverage analysis
* Static analysis integration (clang-tidy, cppcheck)
* Benchmark suite for performance tracking

### Documentation

* Interactive examples on web
* Video tutorials
* API documentation for library usage
* Architecture documentation
* Contributing guidelines
* Code of conduct

## Library Mode

### Separate Library

* Extract core into libwav2png shared library
* C API for language bindings
* Python bindings
* Node.js bindings
* Example programs using library
* Thread-safe API design

## Breaking Changes (Consider for v2.0)

### Modern Redesign

* Complete rewrite using modern C++20/23
* Modular architecture with plugin support
* GPU acceleration support (CUDA, OpenCL)
* Real-time waveform generation
* Streaming input support
* Network streaming capabilities

## Research and Experimental

### Novel Visualizations

* 3D waveform rendering
* Circular/radial waveform display
* Artistic rendering modes
* AI-enhanced visualization
* Adaptive color schemes based on audio content
* Musical notation overlay

### Integration

* DAW plugin version (VST, AU)
* Web Assembly port for browser use
* Mobile app version
* REST API server mode
* Integration with audio editors

## Notes

This TODO list is community-driven. Features are prioritized based on:

* User demand and feedback
* Implementation complexity
* Maintainability impact
* Backward compatibility considerations

To suggest a feature or contribute implementation:

1. Open an issue on the project repository
2. Discuss approach with maintainers
3. Submit pull request with tests and documentation
4. Update this TODO when features are completed

See [CHANGELOG.md](CHANGELOG.md) for completed features.

## Original TODO from Benjamin Schulz

The following item was in the original project TODO:

* Add channel interpolation. Currently the max of all channels is used. For stereo signals, it would be cool to be able to use the Mid, Side, Left or Right channel.

This remains the highest priority feature request from the original author.
