#include <algorithm>   // std::ranges::find
#include <cctype>      // std::isdigit
#include <charconv>    // std::from_chars
#include <clocale>     // std::setlocale, LC_ALL
#include <cstdio>      // std::fgets, stderr, stdin
#include <cstdlib>     // EXIT_FAILURE
#include <cstring>     // std::{strchr, strlen, strtok}
#include <fmt/core.h>  // fmt::print
#include <string_view> // std::{size, string_view}

namespace {

using std::size_t;
using std::string_view;

constexpr string_view glyphs[] = {
    "Ϙ", "Ѧ", "ƛ", "Ϟ", "Ѭ", "ϗ", "Ͽ", "Ӭ", "Ҩ", "☆", "¿", "©", "Ѽ", "Җ",
    "Ԇ", "б", "¶", "Ѣ", "ټ", "Ψ", "Ͼ", "Ѯ", "★", "҂", "æ", "Ҋ", "Ω",
};

consteval size_t operator""_g(char const* glyph, size_t len) {
    return static_cast<size_t>(
        std::ranges::find(glyphs, string_view(glyph, len)) - glyphs
    );
}

constexpr size_t symbol_rows[][7] = {
    {"Ϙ"_g, "Ѧ"_g, "ƛ"_g, "Ϟ"_g,  "Ѭ"_g, "ϗ"_g,  "Ͽ"_g},
    {"Ӭ"_g, "Ϙ"_g, "Ͽ"_g, "Ҩ"_g, "☆"_g, "ϗ"_g,  "¿"_g},
    {"©"_g, "Ѽ"_g, "Ҩ"_g, "Җ"_g,  "Ѯ"_g, "ƛ"_g, "☆"_g},
    {"б"_g, "¶"_g, "Ѣ"_g, "Ѭ"_g,  "Җ"_g, "¿"_g,  "ټ"_g},
    {"Ψ"_g, "ټ"_g, "Ѣ"_g, "Ͼ"_g,  "¶"_g, "Ѯ"_g, "★"_g},
    {"б"_g, "Ӭ"_g, "҂"_g, "æ"_g,  "Ψ"_g, "Ҋ"_g,  "Ω"_g},
};

} // namespace

int main() {
    using fmt::print;
    using std::strtok;

    std::setlocale(LC_ALL, "");

    print("Glyph list:\n");
    for (size_t i = 0; i < std::size(glyphs); ++i) {
        print("{:2}: {}\n", i, glyphs[i]);
    }
    print(
        "Keypad symbol indices? (top to bottom, left to right)\n"
        "example: '{} {} {} {}' for {}, {}, {}, {}.\n>",
        0,
        1,
        2,
        3,
        glyphs[0],
        glyphs[1],
        glyphs[2],
        glyphs[3]
    );
    char line[16];
    if (! std::fgets(line, std::size(line), stdin)) {
        print(stderr, "Failed to read line.\n");
        return EXIT_FAILURE;
    }
    size_t keypad_glyph_indices[4];
    char* token = strtok(line, " ");
    for (size_t i = 0; i < std::size(keypad_glyph_indices); ++i) {
        size_t& keypad_glyph_idx = keypad_glyph_indices[i];
        if (! token) {
            print(stderr, "Missing indices.\n");
            return EXIT_FAILURE;
        }
        auto [end, err] = std::from_chars(
            token,
            token + std::strlen(token),
            keypad_glyph_idx
        );
        if (err != std::errc()) {
            print(stderr, "Failed to parse index.\n");
            return EXIT_FAILURE;
        }
        if (keypad_glyph_idx >= std::size(glyphs)) {
            print(stderr, "Index out of range.\n");
            return EXIT_FAILURE;
        }
        token = strtok(nullptr, " ");
    }
    if (token and *token != '\n') {
        print(stderr, "Too many indices.\n");
        return EXIT_FAILURE;
    }

    print(
        "Specified glyphs: {} {} {} {}\n",
        glyphs[keypad_glyph_indices[0]],
        glyphs[keypad_glyph_indices[1]],
        glyphs[keypad_glyph_indices[2]],
        glyphs[keypad_glyph_indices[3]]
    );

    size_t sorted_glyph_indices[4];
    size_t* sorted_glyph_indices_iter = sorted_glyph_indices;
    size_t correct_row_idx;

    // Iterate over the rows in the game manual, looking for the row that
    // contains all the glyphs specified by the user.
    for (size_t row_idx = 0; row_idx < std::size(symbol_rows); ++row_idx) {
        for (size_t symbol_idx : symbol_rows[row_idx]) {
            for (size_t keypad_idx : keypad_glyph_indices) {
                if (keypad_idx == symbol_idx) {
                    *sorted_glyph_indices_iter++ = symbol_idx;

                    if (sorted_glyph_indices_iter
                        == std::end(sorted_glyph_indices)) {
                        correct_row_idx = static_cast<size_t>(row_idx);
                        goto ALL_GLYPHS_FOUND;
                    }
                }
            }
        }
        // If we got here, we didn't find all the glyphs in this row, so we
        // reset the iterator and try the next row.
        sorted_glyph_indices_iter = sorted_glyph_indices;
    }

    // If we got here, we didn't find all the glyphs in any row. This should
    // never happen, but we'll handle it anyway.
    print(
        stderr,
        "No row contains the specified glyphs.\n"
        "Make sure you entered the correct indices.\n"
    );
    return EXIT_FAILURE;

ALL_GLYPHS_FOUND:
    static constexpr std::string_view ordinal_name_of[] = {
        "first",
        "second",
        "third",
        "fourth",
        "fifth",
        "sixth",
    };

    print(
        "Order is '{} {} {} {}' ({} row).\n",
        glyphs[sorted_glyph_indices[0]],
        glyphs[sorted_glyph_indices[1]],
        glyphs[sorted_glyph_indices[2]],
        glyphs[sorted_glyph_indices[3]],
        ordinal_name_of[correct_row_idx]
    );
}
