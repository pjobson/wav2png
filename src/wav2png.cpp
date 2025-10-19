#include "wav2png.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <sndfile.hh>
#include <png++/png.hpp>

namespace {

// Template metaprogramming to get value range of sample format T
template <typename T>
struct sample_scale {};

template <>
struct sample_scale<short> {
    static constexpr unsigned short value = 1 << (sizeof(short) * 8 - 1);
};

template <>
struct sample_scale<float> {
    static constexpr int value = 1;
};

// Convert linear amplitude to decibels
inline float float2db(float x) noexcept {
    x = std::abs(x);
    return (x > 0.0f) ? 20.0f * std::log10(x) : -9999.9f;
}

// Convert decibels to linear amplitude
inline float db2float(float x) noexcept {
    return std::pow(10.0f, x / 20.0f);
}

// Map value x from range [in_min...in_max] to range [out_min...out_max]
inline float map2range(float x, float in_min, float in_max, float out_min, float out_max) noexcept {
    const float mapped = out_min + (out_max - out_min) * (x - in_min) / (in_max - in_min);
    return std::clamp(mapped, out_min, out_max);
}

// Draw a vertical-ish line for line-only mode
void draw_vertish_line(
    png::image<png::rgba_pixel>& image,
    int x0,
    int y0,
    int y1,
    const png::rgba_pixel& color
) {
    const int ydiff = y1 - y0;
    const int abs_ydiff = std::abs(ydiff);
    const int sign = (ydiff == 0) ? 0 : (ydiff / abs_ydiff);

    for (int dy = 0; dy <= abs_ydiff; ++dy) {
        const int y = y0 + (sign * dy);
        const int x = (dy < abs_ydiff / 2) ? x0 : x0 + 1;
        image.set_pixel(x, y, color);
    }
}

} // anonymous namespace

void compute_waveform(
    const SndfileHandle& wav,
    png::image<png::rgba_pixel>& out_image,
    const png::rgba_pixel& bg_color,
    const png::rgba_pixel& fg_color,
    bool use_db_scale,
    float db_min,
    float db_max,
    bool line_only,
    progress_callback_t progress_callback
) {
    using std::size_t;

    const auto h = out_image.get_height();

    // Using short samples for performance
    using sample_type = short;

    // Handle cases where there aren't enough samples
    png::image<png::rgba_pixel> small_image;
    const bool not_enough_samples = wav.frames() < static_cast<sf_count_t>(out_image.get_width());

    if (not_enough_samples) {
        const auto frames = std::max<sf_count_t>(1, wav.frames());
        small_image = png::image<png::rgba_pixel>(frames, out_image.get_height());
    }

    auto& image = not_enough_samples ? small_image : out_image;
    assert(image.get_width() > 0);

    const int frames_per_pixel = std::max(1, static_cast<int>(wav.frames() / image.get_width()));
    const int samples_per_pixel = wav.channels() * frames_per_pixel;
    const size_t progress_divisor = std::max<size_t>(1, image.get_width() / 100);

    // Buffer for samples from audio file
    std::vector<sample_type> block(samples_per_pixel);

    size_t y_median_last = 0;

    for (size_t x = 0; x < image.get_width(); ++x) {
        // Read frames from audio file
        auto& wav_mut = const_cast<SndfileHandle&>(wav);
        const sf_count_t n = wav_mut.readf(block.data(), frames_per_pixel) * wav.channels();
        assert(n <= static_cast<sf_count_t>(block.size()));

        // Find min and max values
        sample_type min_val = 0;
        sample_type max_val = 0;

        for (int i = 0; i < n; ++i) {
            min_val = std::min(min_val, block[i]);
            max_val = std::max(max_val, block[i]);
        }

        // Calculate median
        std::nth_element(block.begin(), block.begin() + block.size() / 2, block.end());
        const sample_type median = block[n / 2];

        // Compute y-coordinates for waveform
        const float y1_float = use_db_scale
            ? h / 2 - map2range(
                float2db(min_val / static_cast<float>(sample_scale<sample_type>::value)),
                db_min, db_max, 0.0f, h / 2.0f
            )
            : map2range(
                min_val,
                -static_cast<float>(sample_scale<sample_type>::value),
                0.0f, 0.0f, h / 2.0f
            );

        assert(y1_float >= 0 && y1_float <= h / 2);
        const size_t y1 = static_cast<size_t>(y1_float);

        const float y2_float = use_db_scale
            ? h / 2 + map2range(
                float2db(max_val / static_cast<float>(sample_scale<sample_type>::value)),
                db_min, db_max, 0.0f, h / 2.0f
            )
            : map2range(
                max_val,
                0.0f,
                static_cast<float>(sample_scale<sample_type>::value),
                h / 2.0f, static_cast<float>(h)
            );

        assert(y2_float >= h / 2 && y2_float <= h);
        const size_t y2 = static_cast<size_t>(y2_float);

        const float y_median_float = use_db_scale
            ? h / 2 + map2range(
                float2db(median / static_cast<float>(sample_scale<sample_type>::value)),
                db_min, db_max, 0.0f, h / 2.0f
            )
            : map2range(
                median,
                -static_cast<float>(sample_scale<sample_type>::value),
                static_cast<float>(sample_scale<sample_type>::value),
                0.0f, static_cast<float>(h)
            );

        const size_t y_median = static_cast<size_t>(y_median_float);

        if (line_only) {
            // Fill background
            for (size_t y = 0; y < h; ++y) {
                image.set_pixel(x, y, bg_color);
            }

            // Draw line connecting median points
            if (x != 0) {
                draw_vertish_line(image, x - 1, y_median_last, y_median, fg_color);
            }
            y_median_last = y_median;
        } else {
            // Fill top background
            for (size_t y = 0; y < y1; ++y) {
                image.set_pixel(x, y, bg_color);
            }

            // Fill waveform
            for (size_t y = y1; y < y2; ++y) {
                image.set_pixel(x, y, fg_color);
            }

            // Fill bottom background
            for (size_t y = y2; y < h; ++y) {
                image.set_pixel(x, y, bg_color);
            }
        }

        // Report progress
        if (x % progress_divisor == 0) {
            if (progress_callback && !progress_callback(100 * x / image.get_width())) {
                return;
            }
        }
    }

    // Final progress report
    if (progress_callback && !progress_callback(100)) {
        return;
    }

    // Upscale small image if necessary (nearest neighbor)
    if (not_enough_samples) {
        for (size_t y = 0; y < out_image.get_height(); ++y) {
            for (size_t x = 0; x < out_image.get_width(); ++x) {
                const size_t xx = x * small_image.get_width() / out_image.get_width();
                assert(xx < out_image.get_width());
                out_image[y][x] = small_image[y][xx];
            }
        }
    }
}
