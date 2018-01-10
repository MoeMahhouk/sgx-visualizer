//
// Created by moe on 07.01.18.
//

#ifndef SCENE_MATHUTILITY_H
#define SCENE_MATHUTILITY_H

#include <type_traits>

namespace moe {
    template <typename T> inline constexpr
    int signum(T x, std::false_type is_signed) {
        return T(0) < x;
    }

    template <typename T> inline constexpr
    int signum(T x, std::true_type is_signed) {
        return (T(0) < x) - (x < T(0));
    }

    template <typename T> inline constexpr
    int signum(T x) {
        return signum(x, std::is_signed<T>());
    }
}

#endif //SCENE_MATHUTILITY_H
