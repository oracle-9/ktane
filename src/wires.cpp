#include "getchar_sane.hpp"

#include <cctype>       // std::isdigit
#include <cstdio>       // std::{fgets, size_t, stderr, stdin}
#include <cstdlib>      // EXIT_FAILURE, EXIT_SUCCESS
#include <fmt/core.h>   // fmt::print
#include <fmt/format.h> // fmt::literals
#include <string_view>  // std::{size, string_view}
#include <utility>      // std::unreachable

int main() {
    using fmt::print;
    using std::size_t;
    using namespace fmt::literals;

    constexpr char red = 'r';
    constexpr char blue = 'b';
    constexpr char yellow = 'y';
    constexpr char white = 'w';
    constexpr char black = 'B';

    constexpr size_t min_wire_count = 3;
    constexpr size_t max_wire_count = 6;

    print("Last digit of serial number?\n");
    bool serial_is_odd;
    if (int serial_last_digit = getchar_sane();
        std::isdigit(serial_last_digit)) {
        serial_is_odd = serial_last_digit & 1;
    } else {
        print(stderr, "Invalid digit.\n");
        return EXIT_FAILURE;
    }

    print(
        "Top-to-bottom wire initials?\n"
        "{red}: red\n"
        "{blue}: blue\n"
        "{yellow}: yellow\n"
        "{white}: white\n"
        "{black}: black\n"
        "example: '{red}{blue}{yellow}{white}' for red, blue, yellow, white.\n",
        "red"_a = red,
        "blue"_a = blue,
        "yellow"_a = yellow,
        "white"_a = white,
        "black"_a = black
    );

    char wires[max_wire_count] = {};
    size_t wire_count = 0;

    if (! std::fgets(wires, std::size(wires), stdin)) {
        print(stderr, "Error reading input.\n");
        return EXIT_FAILURE;
    }

    size_t red_wire_count = 0;
    size_t blue_wire_count = 0;
    size_t yellow_wire_count = 0;
    size_t white_wire_count = 0;
    size_t last_red_wire_index = 0;
    size_t last_blue_wire_index = 0;
    bool black_wire_present = false;

    for (size_t i = 0; i < std::size(wires); ++i) {
        switch (char wire = wires[i]; wire) {
        case '\0':
        case '\n':
            if (wire_count < min_wire_count or wire_count > max_wire_count) {
                print(stderr, "Invalid number of wires.\n");
                return EXIT_FAILURE;
            }
            goto PARSE_DONE;

        case red:
            ++red_wire_count;
            last_red_wire_index = i;
            break;
        case blue:
            ++blue_wire_count;
            last_blue_wire_index = i;
            break;
        case yellow:
            ++yellow_wire_count;
            break;
        case black:
            black_wire_present = true;
            break;
        case white:
            ++white_wire_count;
            break;

        default:
            print(stderr, "Invalid wire color '{}'.\n", wire);
            return EXIT_FAILURE;
        }
        ++wire_count;
    }

PARSE_DONE:
    static constexpr std::string_view ordinal_name_of[] = {
        "first",
        "second",
        "third",
        "fourth",
        "fifth",
        "sixth",
    };
    char last_wire = wires[wire_count - 1];

    switch (wire_count) {
    case 3:
        if (red_wire_count == 0) {
            print("Cut the second wire.\n");
        } else if (last_wire == white) {
            print("Cut the last wire.\n");
        } else if (blue_wire_count > 1) {
            print("Cut the {} wire.\n", ordinal_name_of[last_blue_wire_index]);
        } else {
            print("Cut the last wire.\n");
        }
        break;

    case 4:
        if (red_wire_count > 1 and serial_is_odd) {
            print("Cut the {} wire.\n", ordinal_name_of[last_red_wire_index]);
        } else if (last_wire == yellow and red_wire_count == 0) {
            print("Cut the first wire.\n");
        } else if (blue_wire_count == 1) {
            print("Cut the first wire.\n");
        } else if (yellow_wire_count > 1) {
            print("Cut the last wire.\n");
        } else {
            print("Cut the second wire.\n");
        }
        break;

    case 5:
        if (last_wire == black and serial_is_odd) {
            print("Cut the fourth wire.\n");
        } else if (red_wire_count == 1 and yellow_wire_count > 1) {
            print("Cut the first wire.\n");
        } else if (not black_wire_present) {
            print("Cut the second wire.\n");
        } else {
            print("Cut the first wire.\n");
        }
        break;

    case 6:
        if (yellow_wire_count == 0 and serial_is_odd) {
            print("Cut the third wire.\n");
        } else if (yellow_wire_count == 1 and white_wire_count > 1) {
            print("Cut the fourth wire.\n");
        } else if (red_wire_count == 0) {
            print("Cut the last wire.\n");
        } else {
            print("Cut the fourth wire.\n");
        }
        break;

    default:
        std::unreachable();
    }
}
