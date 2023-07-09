#include "getchar_sane.hpp"

#include <cstdio> // std::getchar

int getchar_sane() noexcept {
    int c = std::getchar();
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    };
    return c;
}
