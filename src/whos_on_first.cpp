#include <algorithm>   // std::ranges::find
#include <array>       // std::size
#include <cctype>      // std::toupper
#include <cstdio>      // std::{fgets, putchar, size_t}, stderr, stdin
#include <cstdlib>     // EXIT_FAILURE
#include <cstring>     // std::strchr
#include <fmt/core.h>  // fmt::print
#include <string_view> // std::string_view

namespace {

using std::string_view;

constexpr string_view keys[] = {
    "READY", "FIRST",   "NO",    "BLANK",  "NOTHING", "YES",  "WHAT",
    "UHHH",  "LEFT",    "RIGHT", "MIDDLE", "OKAY",    "WAIT", "PRESS",
    "YOU",   "YOU ARE", "YOUR",  "YOU'RE", "UR",      "U",    "UH HUH",
    "UH UH", "WHAT?",   "DONE",  "NEXT",   "HOLD",    "SURE", "LIKE",
};

// clang-format off
constexpr string_view values[][14] = {
    {"YES", "OKAY", "WHAT", "MIDDLE", "LEFT", "PRESS", "RIGHT", "BLANK", "READY", "NO", "FIRST", "UHHH", "NOTHING", "WAIT"},
    {"LEFT", "OKAY", "YES", "MIDDLE", "NO", "RIGHT", "NOTHING", "UHHH", "WAIT", "READY", "BLANK", "WHAT", "PRESS", "FIRST"},
    {"BLANK", "UHHH", "WAIT", "FIRST", "WHAT", "READY", "RIGHT", "YES", "NOTHING", "LEFT", "PRESS", "OKAY", "NO", "MIDDLE"},
    {"WAIT", "RIGHT", "OKAY", "MIDDLE", "BLANK", "PRESS", "READY", "NOTHING", "NO", "WHAT", "LEFT", "UHHH", "YES", "FIRST"},
    {"UHHH", "RIGHT", "OKAY", "MIDDLE", "YES", "BLANK", "NO", "PRESS", "LEFT", "WHAT", "WAIT", "FIRST", "NOTHING", "READY"},
    {"OKAY", "RIGHT", "UHHH", "MIDDLE", "FIRST", "WHAT", "PRESS", "READY", "NOTHING", "YES", "LEFT", "BLANK", "NO", "WAIT"},
    {"UHHH", "WHAT", "LEFT", "NOTHING", "READY", "BLANK", "MIDDLE", "NO", "OKAY", "FIRST", "WAIT", "YES", "PRESS", "RIGHT"},
    {"READY", "NOTHING", "LEFT", "WHAT", "OKAY", "YES", "RIGHT", "NO", "PRESS", "BLANK", "UHHH", "MIDDLE", "WAIT", "FIRST"},
    {"RIGHT", "LEFT", "FIRST", "NO", "MIDDLE", "YES", "BLANK", "WHAT", "UHHH", "WAIT", "PRESS", "READY", "OKAY", "NOTHING"},
    {"YES", "NOTHING", "READY", "PRESS", "NO", "WAIT", "WHAT", "RIGHT", "MIDDLE", "LEFT", "UHHH", "BLANK", "OKAY", "FIRST"},
    {"BLANK", "READY", "OKAY", "WHAT", "NOTHING", "PRESS", "NO", "WAIT", "LEFT", "MIDDLE", "RIGHT", "FIRST", "UHHH", "YES"},
    {"MIDDLE", "NO", "FIRST", "YES", "UHHH", "NOTHING", "WAIT", "OKAY", "LEFT", "READY", "BLANK", "PRESS", "WHAT", "RIGHT"},
    {"UHHH", "NO", "BLANK", "OKAY", "YES", "LEFT", "FIRST", "PRESS", "WHAT", "WAIT", "NOTHING", "READY", "RIGHT", "MIDDLE"},
    {"RIGHT", "MIDDLE", "YES", "READY", "PRESS", "OKAY", "NOTHING", "UHHH", "BLANK", "LEFT", "FIRST", "WHAT", "NO", "WAIT"},
    {"SURE", "YOU ARE", "YOUR", "YOU'RE", "NEXT", "UH HUH", "UR", "HOLD", "WHAT?", "YOU", "UH UH", "LIKE", "DONE", "U"},
    {"YOUR", "NEXT", "LIKE", "UH HUH", "WHAT?", "DONE", "UH UH", "HOLD", "YOU", "U", "YOU'RE", "SURE", "UR", "YOU ARE"},
    {"UH UH", "YOU ARE", "UH HUH", "YOUR", "NEXT", "UR", "SURE", "U", "YOU'RE", "YOU", "WHAT?", "HOLD", "LIKE", "DONE"},
    {"YOU", "YOU'RE", "UR", "NEXT", "UH UH", "YOU ARE", "U", "YOUR", "WHAT?", "UH HUH", "SURE", "DONE", "LIKE", "HOLD"},
    {"DONE", "U", "UR", "UH HUH", "WHAT?", "SURE", "YOUR", "HOLD", "YOU'RE", "LIKE", "NEXT", "UH UH", "YOU ARE", "YOU"},
    {"UH HUH", "SURE", "NEXT", "WHAT?", "YOU'RE", "UR", "UH UH", "DONE", "U", "YOU", "LIKE", "HOLD", "YOU ARE", "YOUR"},
    {"UH HUH", "YOUR", "YOU ARE", "YOU", "DONE", "HOLD", "UH UH", "NEXT", "SURE", "LIKE", "YOU'RE", "UR", "U", "WHAT?"},
    {"UR", "U", "YOU ARE", "YOU'RE", "NEXT", "UH UH", "DONE", "YOU", "UH HUH", "LIKE", "YOUR", "SURE", "HOLD", "WHAT?"},
    {"YOU", "HOLD", "YOU'RE", "YOUR", "U", "DONE", "UH UH", "LIKE", "YOU ARE", "UH HUH", "UR", "NEXT", "WHAT?", "SURE"},
    {"SURE", "UH HUH", "NEXT", "WHAT?", "YOUR", "UR", "YOU'RE", "HOLD", "LIKE", "YOU", "U", "YOU ARE", "UH UH", "DONE"},
    {"WHAT?", "UH HUH", "UH UH", "YOUR", "HOLD", "SURE", "NEXT", "LIKE", "DONE", "YOU ARE", "UR", "YOU'RE", "U", "YOU"},
    {"YOU ARE", "U", "DONE", "UH UH", "YOU", "UR", "SURE", "WHAT?", "YOU'RE", "NEXT", "HOLD", "UH HUH", "YOUR", "LIKE"},
    {"YOU ARE", "DONE", "LIKE", "YOU'RE", "YOU", "HOLD", "UH HUH", "UR", "SURE", "U", "WHAT?", "NEXT", "YOUR", "UH UH"},
    {"YOU'RE", "NEXT", "U", "UR", "HOLD", "DONE", "UH UH", "WHAT?", "UH HUH", "YOU", "LIKE", "SURE", "YOU ARE", "YOUR"},
};

// clang-format on

constexpr struct {
    int row;
    int col;
} positions[] = {
    {1, 0}, // YES
    {0, 1}, // FIRST
    {2, 1}, // DISPLAY
    {0, 1}, // OKAY
    {2, 1}, // SAYS
    {1, 0}, // NOTHING
    {2, 0}, // ""
    {1, 1}, // BLANK
    {2, 1}, // NO
    {1, 0}, // LED
    {2, 1}, // LEAD
    {1, 1}, // READ
    {1, 1}, // RED
    {2, 0}, // REED
    {2, 0}, // LEED
    {2, 1}, // HOLD ON
    {1, 1}, // YOU
    {2, 1}, // YOU ARE
    {1, 1}, // YOUR
    {1, 1}, // YOU'RE
    {0, 0}, // UR
    {2, 1}, // THERE
    {2, 0}, // THEY'RE
    {1, 1}, // THEIR
    {1, 0}, // THEY ARE
    {2, 1}, // SEE
    {0, 1}, // C
    {2, 1}, // CEE
};

// constexpr size_t index_of_key[]

} // namespace

int main() {
    using fmt::print;

    print("Display label? (case insensitive)\n> ");
    char display_label_buf[9]; // Longest label is "THEY ARE", which is 8 chars
                               // (+null terminator).
    if (std::fgets(display_label_buf, std::size(display_label_buf), stdin)
        == nullptr) {
        print(stderr, "Error reading input.\n");
        return EXIT_FAILURE;
    }
    std::size_t display_label_len;
    for (char& c : display_label_buf) {
        if (c == '\n' or c == '\0') {
            display_label_len
                = static_cast<std::size_t>(&c - display_label_buf);
        } else {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
    }
    auto display_label = string_view(display_label_buf, display_label_len);

    auto display_label_at = [](int row, int col) {
        // Should display the following if row == 1 and col == 2:
        // -------
        // | | | |
        // -------
        // | | |x|
        // -------
        // | | | |
        // -------
        // (x marks the selected label)
        using std::putchar;
        print("-------\n");
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                putchar('|');
                if (i == row and j == col) {
                    putchar('x');
                } else {
                    putchar(' ');
                }
            }
            print("|\n");
            print("-------\n");
        }
    };

    string_view const* match = std::ranges::find(keys, display_label);
    if (match == std::end(keys)) {
        print(
            stderr,
            "Unknown display label.\n"
            "Make sure you entered the label correctly.\n"
        );
        return EXIT_FAILURE;
    }
}
