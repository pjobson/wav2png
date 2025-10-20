#pragma once

#include <sndfile.hh>
#include <memory>
#include <string>

// Class to handle audio file conversion using ffmpeg
// Provides transparent format support beyond libsndfile's native formats
class AudioConverter {
public:
    // Opens an audio file, using ffmpeg conversion if needed
    // Returns a SndfileHandle on success, throws on failure
    static SndfileHandle open_audio_file(const std::string& filename);

private:
    // Check if ffmpeg is available on the system
    static bool is_ffmpeg_available();

    // Check if the file format is likely supported by libsndfile
    static bool is_libsndfile_format(const std::string& filename);

    // Open audio file using ffmpeg FIFO
    // Creates a FIFO, spawns ffmpeg, and returns a SndfileHandle reading from FIFO
    static SndfileHandle open_with_ffmpeg(const std::string& filename);

    // Get file extension from filename
    static std::string get_extension(const std::string& filename);
};

// RAII wrapper to manage ffmpeg process and FIFO cleanup
class FFmpegConverter {
public:
    FFmpegConverter(const std::string& input_filename);
    ~FFmpegConverter();

    // Get the FIFO path for reading
    std::string get_fifo_path() const { return fifo_path_; }

    // Check if conversion was successfully started
    bool is_valid() const { return !fifo_path_.empty() && ffmpeg_pid_ > 0; }

    // Wait for ffmpeg to finish
    void wait();

private:
    std::string fifo_path_;
    pid_t ffmpeg_pid_ = -1;
    bool waited_ = false;
};
