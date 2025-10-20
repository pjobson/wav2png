#include <sndfile.hh>
#include <png++/png.hpp>

#include <iostream>
#include <vector>

#include "options.hpp"
#include "wav2png.hpp"
#include "audio_converter.hpp"

namespace {

bool progress_callback(int percent) {
    std::cerr << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bconverting: "
              << percent << "%";
    return true;
}

} // anonymous namespace

int main(int argc, char* argv[]) {
    try {
        const Options options(argc, argv);

        // Open sound file (with automatic ffmpeg conversion if needed)
        SndfileHandle wav = AudioConverter::open_audio_file(options.input_file_name);

        // Handle error
        if (wav.error()) {
            std::cerr << "Error opening audio file '" << options.input_file_name << "'\n"
                      << "Error was: '" << wav.strError() << "'" << std::endl;
            std::cerr << "\nSupported formats: WAV, AIFF, FLAC, OGG, AU, CAF, and more via libsndfile" << std::endl;
            std::cerr << "MP3 and other formats require ffmpeg to be installed" << std::endl;
            return 2;
        }

        // Create image
        png::image<png::rgba_pixel> image(options.width, options.height);

        // Compute waveform
        compute_waveform(
            wav,
            image,
            options.background_color,
            options.foreground_color,
            options.use_db_scale,
            options.db_min,
            options.db_max,
            options.line_only,
            progress_callback
        );

        std::cerr << std::endl;

        // Write image to disk
        image.write(options.output_file_name);

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
}
