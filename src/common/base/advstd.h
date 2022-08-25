/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#ifndef RENDU_ADVSTD_H
#define RENDU_ADVSTD_H

#include <cstddef>
#include <type_traits>

 namespace advstd
{
    // C++20 std::remove_cvref_t
    template <class T>
    using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

    // C++20 std::type_identity
    template <typename T>
    struct type_identity
    {
        using type = T;
    };

    // C++20 std::type_identity_t
    template <typename T>
    using type_identity_t = typename type_identity<T>::type;
}

#endif
