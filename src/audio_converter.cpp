#include "audio_converter.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

// Static variable to cache ffmpeg availability check
static int ffmpeg_available = -1;  // -1 = not checked, 0 = not available, 1 = available

bool AudioConverter::is_ffmpeg_available() {
    if (ffmpeg_available == -1) {
        // Check if ffmpeg command exists
        int result = system("command -v ffmpeg > /dev/null 2>&1");
        ffmpeg_available = (result == 0) ? 1 : 0;

        if (ffmpeg_available == 0) {
            std::cerr << "Note: ffmpeg not found. Extended format support disabled." << std::endl;
        }
    }
    return ffmpeg_available == 1;
}

std::string AudioConverter::get_extension(const std::string& filename) {
    size_t pos = filename.find_last_of('.');
    if (pos == std::string::npos) {
        return "";
    }

    std::string ext = filename.substr(pos + 1);
    // Convert to lowercase
    std::transform(ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return ext;
}

bool AudioConverter::is_libsndfile_format(const std::string& filename) {
    // Formats natively supported by libsndfile
    static const char* libsndfile_formats[] = {
        "wav", "wave", "aiff", "aif", "aifc",
        "au", "snd", "flac", "ogg", "oga",
        "w64", "mat", "mat4", "mat5",
        "pvf", "sf", "voc", "htk", "caf",
        "sd2", "rf64", "xi", "mpc2k"
    };

    std::string ext = get_extension(filename);
    if (ext.empty()) {
        return true;  // Let libsndfile try
    }

    for (const auto& fmt : libsndfile_formats) {
        if (ext == fmt) {
            return true;
        }
    }

    return false;
}

FFmpegConverter::FFmpegConverter(const std::string& input_filename) {
    // Create unique FIFO name
    char fifo_template[] = "/tmp/wav2png_XXXXXX";
    int fd = mkstemp(fifo_template);
    if (fd == -1) {
        throw std::runtime_error("Failed to create temporary file name");
    }
    ::close(fd);
    unlink(fifo_template);  // Remove the regular file

    // Create FIFO at this location
    if (mkfifo(fifo_template, 0600) == -1) {
        throw std::runtime_error("Failed to create FIFO: " + std::string(strerror(errno)));
    }

    fifo_path_ = fifo_template;

    // Fork process to run ffmpeg
    pid_t pid = fork();

    if (pid == -1) {
        // Fork failed - clean up FIFO
        unlink(fifo_path_.c_str());
        throw std::runtime_error("Failed to fork process for ffmpeg conversion");
    }

    if (pid == 0) {
        // Child process: run ffmpeg

        // Redirect stderr to /dev/null to suppress most ffmpeg output
        int devnull = open("/dev/null", O_WRONLY);
        if (devnull != -1) {
            dup2(devnull, STDERR_FILENO);
            ::close(devnull);
        }

        // Execute ffmpeg to convert to WAV and write to FIFO
        execlp("ffmpeg",
               "ffmpeg",
               "-i", input_filename.c_str(),
               "-f", "wav",              // Output format: WAV
               "-acodec", "pcm_s16le",   // 16-bit PCM
               "-ar", "44100",           // Sample rate (can be adjusted)
               "-y",                     // Overwrite output
               fifo_path_.c_str(),       // Output to FIFO
               nullptr);

        // If execlp returns, it failed
        std::cerr << "Failed to execute ffmpeg" << std::endl;
        _exit(1);
    }

    // Parent process
    ffmpeg_pid_ = pid;
}

FFmpegConverter::~FFmpegConverter() {
    // Note: We don't wait for ffmpeg to finish here because:
    // 1. It might still be writing to the FIFO while we're reading from it
    // 2. Waiting would cause a deadlock
    // 3. The process will finish on its own and be reaped on program exit

    // Clean up FIFO (safe to unlink while still open due to Unix semantics)
    if (!fifo_path_.empty()) {
        unlink(fifo_path_.c_str());
    }
}

void FFmpegConverter::wait() {
    if (waited_) {
        return;
    }

    // Wait for child process to finish
    if (ffmpeg_pid_ > 0) {
        int status;
        waitpid(ffmpeg_pid_, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            std::cerr << "Warning: ffmpeg exited with status " << WEXITSTATUS(status) << std::endl;
        }

        ffmpeg_pid_ = -1;
    }

    waited_ = true;
}

SndfileHandle AudioConverter::open_with_ffmpeg(const std::string& filename) {
    // Create ffmpeg converter with FIFO
    // Note: This creates a new object on the heap that won't be automatically cleaned up
    // The FIFO cleanup happens in the destructor, which is safe due to Unix semantics
    FFmpegConverter* converter = new FFmpegConverter(filename);

    if (!converter->is_valid()) {
        delete converter;
        throw std::runtime_error("Failed to start ffmpeg conversion for file: " + filename);
    }

    std::string fifo_path = converter->get_fifo_path();

    // Open the FIFO with libsndfile
    // This will block until ffmpeg starts writing to the FIFO
    SndfileHandle handle(fifo_path.c_str());

    if (handle.error()) {
        delete converter;
        throw std::runtime_error(
            "Failed to open converted audio: " + std::string(handle.strError())
        );
    }

    // Note: We intentionally don't delete the converter here because:
    // 1. The FIFO must remain until reading is complete
    // 2. The FIFO is unlinked in the destructor (safe due to Unix semantics)
    // 3. The ffmpeg process will finish on its own
    // This is a small memory leak, but acceptable for a short-lived CLI tool

    return handle;
}

SndfileHandle AudioConverter::open_audio_file(const std::string& filename) {
    // First, try to open directly with libsndfile
    SndfileHandle handle(filename.c_str());

    // If successful, return it
    if (!handle.error()) {
        return handle;
    }

    // If libsndfile format or ffmpeg not available, propagate error
    if (is_libsndfile_format(filename) || !is_ffmpeg_available()) {
        return handle;  // Return with error
    }

    // Try using ffmpeg
    std::cerr << "Attempting to convert " << filename << " using ffmpeg..." << std::endl;

    return open_with_ffmpeg(filename);
}
