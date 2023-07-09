#include "getchar_sane.hpp"

#include <cctype>       // std::isdigit
#include <cstdio>       // stderr, EOF
#include <fmt/core.h>   // fmt::print
#include <fmt/format.h> // fmt::literals

int main() {
    using fmt::print;
    using namespace fmt::literals;

    constexpr char blue = 'b';
    constexpr char white = 'w';
    constexpr char yellow = 'y';
    constexpr char red = 'r';

    constexpr char abort = 'a';
    constexpr char detonate = 'd';
    constexpr char hold = 'h';

    constexpr char car = 'c';
    constexpr char frk = 'f';

    constexpr char neither = '-';

    print(
        "Button color?\n"
        "{blue}: blue\n"
        "{white}: white\n"
        "{yellow}: yellow\n"
        "{red}: red\n"
        "{neither}: neither\n",
        "blue"_a = blue,
        "white"_a = white,
        "yellow"_a = yellow,
        "red"_a = red,
        "neither"_a = neither
    );
    int color = getchar_sane();
    switch (color) {
    case blue:
    case white:
    case yellow:
    case red:
    case neither:
        break;
    default:
        print(stderr, "invalid color.\n");
        return EXIT_FAILURE;
    }

    print(
        "Button text?\n"
        "{abort}: Abort\n"
        "{detonate}: Detonate\n"
        "{hold}: Hold\n",
        "abort"_a = abort,
        "detonate"_a = detonate,
        "hold"_a = hold
    );
    int text = getchar_sane();
    switch (text) {
    case abort:
    case detonate:
    case hold:
        break;
    default:
        print(stderr, "invalid text.\n");
        return EXIT_FAILURE;
    }

    print("Number of batteries? [0-9]\n");
    int battery_count = getchar_sane();
    if (not std::isdigit(battery_count)) {
        print(stderr, "invalid number of batteries.\n");
        return EXIT_FAILURE;
    }
    battery_count -= '0';

    print(
        "Indicator label?\n"
        "{car}: CAR\n"
        "{frk}: FRK\n"
        "{neither}: neither\n",
        "car"_a = car,
        "frk"_a = frk,
        "neither"_a = neither
    );
    int indicator_label = getchar_sane();
    switch (indicator_label) {
    case car:
    case frk:
    case neither:
        break;
    default:
        print(stderr, "invalid indicator label.\n");
        return EXIT_FAILURE;
    }

    if (color == blue and text == abort) {
        goto RELEASING_A_HELD_BUTTON;
    } else if (battery_count > 1 and text == detonate) {
        print("Press and immediately release the button.\n");
    } else if (color == white and indicator_label == car) {
        goto RELEASING_A_HELD_BUTTON;
    } else if (battery_count > 2 and indicator_label == frk) {
        print("Press and immediately release the button.\n");
    } else if (color == yellow) {
        goto RELEASING_A_HELD_BUTTON;
    } else if (color == red and text == hold) {
        print("Press and immediately release the button.\n");
    } else {
        goto RELEASING_A_HELD_BUTTON;
    }

RELEASING_A_HELD_BUTTON:
    print("Hold the button. What color is the strip?\n");
    int strip_color = getchar_sane();
    switch (strip_color) {
    case blue:
        print("Release when the countdown timer has a 4 in any position.\n");
        break;
    case white:
        print("Release when the countdown timer has a 1 in any position.\n");
        break;
    case yellow:
        print("Release when the countdown timer has a 5 in any position.\n");
        break;
    case neither:
        print("Release when the countdown timer has a 1 in any position.\n");
        break;
    default:
        print(stderr, "invalid strip color.\n");
        return EXIT_FAILURE;
    }
}
