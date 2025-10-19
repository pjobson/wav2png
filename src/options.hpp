#pragma once

#include <boost/program_options.hpp>
#include <png++/png.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "./version.hpp"

class Options {
public:
    Options(int argc, char* argv[]) {
        namespace po = boost::program_options;

        po::options_description generic("Generic options");
        generic.add_options()
            ("version,v", "print version string")
            ("help", "produce help message");

        po::options_description config("Configuration");
        config.add_options()
            ("width,w", po::value<unsigned>(&width)->default_value(1800),
                "width of generated image")
            ("height,h", po::value<unsigned>(&height)->default_value(280),
                "height of generated image")
            ("background-color,b", po::value<std::string>(&background_color_string)->default_value("efefefff"),
                "color of background in hex rgba")
            ("foreground-color,f", po::value<std::string>(&foreground_color_string)->default_value("00000000"),
                "color of foreground in hex rgba")
            ("output,o", po::value<std::string>(&output_file_name)->default_value(""),
                "name of output file, defaults to <name of inputfile>.png")
            ("config-file,c", po::value<std::string>(&config_file_name)->default_value("wav2png.cfg"),
                "config file to use")
            ("db-scale,d", po::value(&use_db_scale)->zero_tokens()->default_value(false),
                "use logarithmic (e.g. decibel) scale instead of linear scale")
            ("db-min", po::value(&db_min)->default_value(-48.0f),
                "minimum value of the signal in dB, that will be visible in the waveform")
            ("db-max", po::value(&db_max)->default_value(0.0f),
                "maximum value of the signal in dB, that will be visible in the waveform. "
                "Useful if you know that your signal peaks at a certain level.")
            ("line-only,l", po::value(&line_only)->zero_tokens()->default_value(false),
                "do a line only (no fill)");

        po::options_description hidden("Hidden options");
        hidden.add_options()
            ("input-file", po::value<std::string>(&input_file_name), "input file");

        po::positional_options_description p;
        p.add("input-file", -1);

        po::options_description cmdline_options;
        cmdline_options.add(generic).add(config).add(hidden);

        po::options_description config_file_options;
        config_file_options.add(config).add(hidden);

        po::options_description visible("Allowed options");
        visible.add(generic).add(config);

        po::variables_map vm;

        bool parse_error = false;

        try {
            po::store(
                po::command_line_parser(argc, argv)
                    .options(cmdline_options)
                    .positional(p)
                    .run(),
                vm
            );
            po::notify(vm);

            // Try to read config file
            std::ifstream config_file(config_file_name);
            if (!config_file.good()) {
                if (config_file_name != "wav2png.cfg") {
                    std::cerr << "Error: failed to read config file '"
                              << config_file_name << "'" << std::endl;
                    parse_error = true;
                }
            } else {
                po::store(po::parse_config_file<char>(config_file, config_file_options), vm);
                po::notify(vm);
            }

            // Command line options override config file
            po::store(
                po::command_line_parser(argc, argv)
                    .options(cmdline_options)
                    .positional(p)
                    .run(),
                vm
            );
            po::notify(vm);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            parse_error = true;
        }

        // Parse colors
        try {
            foreground_color = parse_color(foreground_color_string);
            background_color = parse_color(background_color_string);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            parse_error = true;
        }

        if (input_file_name.empty()) {
            std::cerr << "Error: no input file supplied." << std::endl;
            parse_error = true;
        }

        if (output_file_name.empty()) {
            output_file_name = input_file_name + ".png";
        }

        if (width == 0) {
            std::cerr << "Error: width cannot be 0." << std::endl;
            parse_error = true;
        }

        if (height == 0) {
            std::cerr << "Error: height cannot be 0." << std::endl;
            parse_error = true;
        }

        if (parse_error || vm.count("help")) {
            print_help(visible);
            std::exit(0);
        }

        if (vm.count("version")) {
            print_version();
            std::exit(0);
        }
    }

    unsigned width = 0;
    unsigned height = 0;
    std::string background_color_string;
    std::string foreground_color_string;

    png::rgba_pixel background_color;
    png::rgba_pixel foreground_color;

    std::string output_file_name;
    std::string input_file_name;
    std::string config_file_name;

    bool use_db_scale = false;
    float db_min = -48.0f;
    float db_max = 0.0f;
    bool line_only = false;

private:
    class color_parse_error : public std::runtime_error {
    public:
        explicit color_parse_error(const std::string& message)
            : std::runtime_error(message) {}
    };

    static png::rgba_pixel parse_color(const std::string& str) {
        if (str.length() != 8) {
            throw color_parse_error(
                "supplied color does not have four components. try e.g. aabbccff"
            );
        }

        std::stringstream ss(str);
        unsigned color = 0;
        ss >> std::hex >> color;

        if (!ss.eof()) {
            throw color_parse_error(
                "failed to parse color '" + str +
                "'. are all characters in range [0-9a-f]?"
            );
        }

        return png::rgba_pixel(
            static_cast<png::byte>((color & 0xff000000) >> 24),
            static_cast<png::byte>((color & 0x00ff0000) >> 16),
            static_cast<png::byte>((color & 0x0000ff00) >> 8),
            static_cast<png::byte>((color & 0x000000ff) >> 0)
        );
    }

    static void print_help(const boost::program_options::options_description& visible) {
        std::cout << "wav2png version " << version::version << "\n"
                  << "written by Benjamin Schulz (beschulz[the a with the circle]betabugs.de)\n"
                  << "\n"
                  << "usage: wav2png [options] input_file_name\n"
                  << "example: wav2png my_file.wav\n"
                  << "\n"
                  << visible << std::endl;
    }

    static void print_version() noexcept {
        std::cout << "version: " << version::version << "\n"
                  << "build date: " << version::date << std::endl;
    }
};
