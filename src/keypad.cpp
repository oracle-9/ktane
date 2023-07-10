#include <algorithm>   // std::ranges::find
#include <charconv>    // std::from_chars
#include <clocale>     // std::setlocale, LC_ALL
#include <cstdio>      // std::fgets, stderr, stdin
#include <cstdlib>     // EXIT_FAILURE
#include <cstring>     // std::{strchr, strlen, strtok}
#include <fmt/core.h>  // fmt::print
#include <string_view> // std::{size, string_view}

int main() {
    using fmt::print;
    using std::size_t;
    using std::string_view;
    using std::strtok;

    static constexpr string_view glyphs[] = {
        "Ϙ", "Ѧ", "ƛ", "Ϟ", "Ѭ", "ϗ", "Ͽ", "Ӭ", "Ҩ", "☆", "¿", "©", "Ѽ", "Җ",
        "Ԇ", "б", "¶", "Ѣ", "ټ", "Ψ", "Ͼ", "Ѯ", "★", "҂", "æ", "Ҋ", "Ω",
    };

    // Hacky way to define a 2D array of indices into the glyphs table.
    class Glyph {
      private:
        size_t table_idx;

      public:
        constexpr explicit(false) Glyph(char const* glyph)
          : table_idx(static_cast<size_t>(
              std::ranges::find(glyphs, string_view(glyph)) - glyphs
          )) {}

        operator size_t() const {
            return table_idx;
        }
    } static constexpr glyph_rows[][7] = {
        {"Ϙ", "Ѧ", "ƛ", "Ϟ",  "Ѭ", "ϗ",  "Ͽ"},
        {"Ӭ", "Ϙ", "Ͽ", "Ҩ", "☆", "ϗ",  "¿"},
        {"©", "Ѽ", "Ҩ", "Җ",  "Ѯ", "ƛ", "☆"},
        {"б", "¶", "Ѣ", "Ѭ",  "Җ", "¿",  "ټ"},
        {"Ψ", "ټ", "Ѣ", "Ͼ",  "¶", "Ѯ", "★"},
        {"б", "Ӭ", "҂", "æ",  "Ψ", "Ҋ",  "Ω"},
    };

    std::setlocale(LC_ALL, "");

    print("Glyph list:\n");
    for (size_t i = 0; i < std::size(glyphs); ++i) {
        print("{:2}: {}\n", i, glyphs[i]);
    }
    print(
        "Keypad glyph indices? (top to bottom, left to right)\n"
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
        auto [_, err] = std::from_chars(
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
    for (size_t row_idx = 0; row_idx < std::size(glyph_rows); ++row_idx) {
        for (size_t glyph_idx : glyph_rows[row_idx]) {
            for (size_t keypad_idx : keypad_glyph_indices) {
                if (keypad_idx == glyph_idx) {
                    *sorted_glyph_indices_iter++ = glyph_idx;

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
    static constexpr string_view ordinal_name_of[] = {
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
