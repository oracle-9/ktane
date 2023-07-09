#include "getchar_sane.hpp"

#include <array>        // std::end
#include <cstdio>       // std::size_t, stderr
#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <fmt/core.h>   // fmt::print
#include <fmt/format.h> // fmt::literals
#include <ranges>       // std::ranges::views::take
#include <utility>      // std::unreachable

#define DEFINE_STATE_MACHINE(COL1, COL2, COL3, COL4)                           \
    [](char prev_color) {                                                      \
        switch (prev_color) {                                                  \
        case red:                                                              \
            return COL1;                                                       \
        case blue:                                                             \
            return COL2;                                                       \
        case green:                                                            \
            return COL3;                                                       \
        case yellow:                                                           \
            return COL4;                                                       \
        default:                                                               \
            std::unreachable();                                                \
        }                                                                      \
    }

int main() {
    using fmt::print;
    using NextColor = char (*)(char);

    constexpr char red = 'r';
    constexpr char blue = 'b';
    constexpr char green = 'g';
    constexpr char yellow = 'y';

    // clang-format off
    static constexpr NextColor state_machine[][3] = {
        {
            // Serial number does not contain a vowel.
            DEFINE_STATE_MACHINE(blue, yellow, green, red), // 0 strikes.
            DEFINE_STATE_MACHINE(red, blue, yellow, green), // 1 strike.
            DEFINE_STATE_MACHINE(yellow, green, blue, red), // 2 strikes.
        },
        {
            // Serial number contains a vowel.
            DEFINE_STATE_MACHINE(blue, red, yellow, green), // 0 strikes.
            DEFINE_STATE_MACHINE(yellow, green, blue, red), // 1 strike.
            DEFINE_STATE_MACHINE(green, red, yellow, blue), // 2 strikes.
        },
    };
    // clang-format on

    bool serial_has_vowel;
    print("Does the serial number contain a vowel? (y/n)\n> ");
    switch (getchar_sane()) {
    case 'y':
    case 'Y':
        serial_has_vowel = true;
        break;
    case 'n':
    case 'N':
        serial_has_vowel = false;
        break;
    default:
        print(stderr, "Invalid input.\n");
        return EXIT_FAILURE;
    }

    print("Number of strikes? [0-2]\n> ");
    int strike_count = getchar_sane();
    switch (strike_count) {
    case '0':
    case '1':
    case '2':
        break;
    default:
        print(stderr, "Invalid number of strikes.\n");
        return EXIT_FAILURE;
    }
    strike_count -= '0'; // Convert from ASCII to integer.

    auto next_color = state_machine[serial_has_vowel][strike_count];
    char sequence[5]; // There are either 3 or 5 flashes in the game.
    std::size_t num_flashes = 0;

    for (char& new_color : sequence) {
        using namespace fmt::literals;
        print(
            "\n"
            "Last flash color?\n"
            "{red}: red\n"
            "{blue}: blue\n"
            "{green}: green\n"
            "{yellow}: yellow\n> ",
            "red"_a = red,
            "blue"_a = blue,
            "green"_a = green,
            "yellow"_a = yellow
        );
        switch (int last_color = getchar_sane(); last_color) {
        case red:
        case blue:
        case green:
        case yellow:
            ++num_flashes;
            new_color = next_color(static_cast<char>(last_color));
            break;
        case EOF:
            return EXIT_SUCCESS;
        default:
            print(stderr, "Invalid color.\n");
            return EXIT_FAILURE;
        }
        print(">>> Current sequence:");
        using std::ranges::views::take;
        for (char c : sequence | take(num_flashes)) {
            switch (c) {
            case red:
                print(" red");
                break;
            case blue:
                print(" blue");
                break;
            case green:
                print(" green");
                break;
            case yellow:
                print(" yellow");
                break;
            }
        }
        print(" <<<\n");
    }
}
