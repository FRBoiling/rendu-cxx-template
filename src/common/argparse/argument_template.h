/*
* Created by boil on 2022/9/17.
*/

#ifndef RENDU_ARGUMENT_TEMPLATE_H_
#define RENDU_ARGUMENT_TEMPLATE_H_

#include <algorithm>
#include <any>
#include <array>
#include <cerrno>
#include <charconv>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#include <define.h>

namespace rendu {
  using valued_action = std::function<std::any(const std::string &)>;
  using void_action = std::function<void(const std::string &)>;

  namespace details { // namespace for helper methods

    template<typename T, typename = void>
    struct HasContainerTraits : std::false_type {
    };

    template<>
    struct HasContainerTraits<std::string> : std::false_type {
    };

    template<typename T>
    struct HasContainerTraits<
        T, std::void_t<typename T::value_type, decltype(std::declval<T>().begin()),
            decltype(std::declval<T>().end()),
            decltype(std::declval<T>().size())>> : std::true_type {
    };

    template<typename T>
    static constexpr bool IsContainer = HasContainerTraits<T>::value;

    template<typename T, typename = void>
    struct HasStreamableTraits : std::false_type {
    };

    template<typename T>
    struct HasStreamableTraits<
        T,
        std::void_t<decltype(std::declval<std::ostream &>() << std::declval<T>())>>
        : std::true_type {
    };

    template<typename T>
    static constexpr bool IsStreamable = HasStreamableTraits<T>::value;

    constexpr std::size_t repr_max_container_size = 5;

    template<typename T>
    std::string repr(T const &val) {
      if constexpr (std::is_same_v<T, bool>) {
        return val ? "true" : "false";
      } else if constexpr (std::is_convertible_v<T, std::string_view>) {
        return '"' + std::string{std::string_view{val}} + '"';
      } else if constexpr (IsContainer<T>) {
        std::stringstream out;
        out << "{";
        const auto size = val.size();
        if (size > 1) {
          out << repr(*val.begin());
          std::for_each(
              std::next(val.begin()),
              std::next(val.begin(),
                        std::min<std::size_t>(size, repr_max_container_size) - 1),
              [&out](const auto &v) { out << " " << repr(v); });
          if (size <= repr_max_container_size) {
            out << " ";
          } else {
            out << "...";
          }
        }
        if (size > 0) {
          out << repr(*std::prev(val.end()));
        }
        out << "}";
        return out.str();
      } else if constexpr (IsStreamable<T>) {
        std::stringstream out;
        out << val;
        return out.str();
      } else if (enum_contains<T>(val)) {
        return enum_flags_name(val);
      } else {
        return "<not representable>";
      }
    }

    namespace {

      template<typename T> constexpr bool standard_signed_integer = false;
      template<> constexpr bool standard_signed_integer<signed char> = true;
      template<> constexpr bool standard_signed_integer<short int> = true;
      template<> constexpr bool standard_signed_integer<int> = true;
      template<> constexpr bool standard_signed_integer<long int> = true;
      template<> constexpr bool standard_signed_integer<long long int> = true;

      template<typename T> constexpr bool standard_unsigned_integer = false;
      template<> constexpr bool standard_unsigned_integer<unsigned char> = true;
      template<> constexpr bool standard_unsigned_integer<unsigned short int> = true;
      template<> constexpr bool standard_unsigned_integer<unsigned int> = true;
      template<> constexpr bool standard_unsigned_integer<unsigned long int> = true;
      template<>
      constexpr bool standard_unsigned_integer<unsigned long long int> = true;

    } // namespace

    constexpr int radix_8 = 8;
    constexpr int radix_10 = 10;
    constexpr int radix_16 = 16;

    template<typename T>
    constexpr bool standard_integer =
        standard_signed_integer<T> || standard_unsigned_integer<T>;

    template<class F, class Tuple, class Extra, std::size_t... I>
    constexpr decltype(auto) apply_plus_one_impl(F &&f, Tuple &&t, Extra &&x,
                                                 std::index_sequence<I...> /*unused*/) {
      return std::invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...,
                         std::forward<Extra>(x));
    }

    template<class F, class Tuple, class Extra>
    constexpr decltype(auto) apply_plus_one(F &&f, Tuple &&t, Extra &&x) {
      return details::apply_plus_one_impl(
          std::forward<F>(f), std::forward<Tuple>(t), std::forward<Extra>(x),
          std::make_index_sequence<
              std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
    }

    constexpr auto pointer_range(std::string_view s) noexcept {
      return std::tuple(s.data(), s.data() + s.size());
    }

    template<class CharT, class Traits>
    constexpr bool starts_with(std::basic_string_view<CharT, Traits> prefix,
                               std::basic_string_view<CharT, Traits> s) noexcept {
      return s.substr(0, prefix.size()) == prefix;
    }

    enum class chars_format {
      scientific = 0x1,
      fixed = 0x2,
      hex = 0x4,
      general = fixed | scientific
    };

    struct ConsumeHexPrefixResult {
      bool is_hexadecimal;
      std::string_view rest;
    };

    using namespace std::literals;

    constexpr auto consume_hex_prefix(std::string_view s)
    -> ConsumeHexPrefixResult {
      if (starts_with("0x"sv, s) || starts_with("0X"sv, s)) {
        s.remove_prefix(2);
        return {true, s};
      }
      return {false, s};
    }

    template<class T, auto Param>
    inline auto do_from_chars(std::string_view s) -> T {
      T x;
      auto [first, last] = pointer_range(s);
      auto [ptr, ec] = std::from_chars(first, last, x, Param);
      if (ec == std::errc()) {
        if (ptr == last) {
          return x;
        }
        throw std::invalid_argument{"pattern does not match to the end"};
      }
      if (ec == std::errc::invalid_argument) {
        throw std::invalid_argument{"pattern not found"};
      }
      if (ec == std::errc::result_out_of_range) {
        throw std::range_error{"not representable"};
      }
      return x; // unreachable
    }

    template<class T, auto Param = 0>
    struct parse_number {
      auto operator()(std::string_view s) -> T {
        return do_from_chars<T, Param>(s);
      }
    };

    template<class T>
    struct parse_number<T, radix_16> {
      auto operator()(std::string_view s) -> T {
        if (auto [ok, rest] = consume_hex_prefix(s); ok) {
          return do_from_chars<T, radix_16>(rest);
        }
        throw std::invalid_argument{"pattern not found"};
      }
    };

    template<class T>
    struct parse_number<T> {
      auto operator()(std::string_view s) -> T {
        auto [ok, rest] = consume_hex_prefix(s);
        if (ok) {
          return do_from_chars<T, radix_16>(rest);
        }
        if (starts_with("0"sv, s)) {
          return do_from_chars<T, radix_8>(rest);
        }
        return do_from_chars<T, radix_10>(rest);
      }
    };

    namespace {

      template<class T> inline const auto generic_strtod = nullptr;
      template<> inline const auto generic_strtod<float> = strtof;
      template<> inline const auto generic_strtod<double> = strtod;
      template<> inline const auto generic_strtod<long double> = strtold;

    } // namespace

    template<class T>
    inline auto do_strtod(std::string const &s) -> T {
      if (isspace(static_cast<unsigned char>(s[0])) || s[0] == '+') {
        throw std::invalid_argument{"pattern not found"};
      }

      auto [first, last] = pointer_range(s);
      char *ptr;

      errno = 0;
      auto x = generic_strtod<T>(first, &ptr);
      if (errno == 0) {
        if (ptr == last) {
          return x;
        }
        throw std::invalid_argument{"pattern does not match to the end"};
      }
      if (errno == ERANGE) {
        throw std::range_error{"not representable"};
      }
      return x; // unreachable
    }

    template<class T>
    struct parse_number<T, chars_format::general> {
      auto operator()(std::string const &s) -> T {
        if (auto r = consume_hex_prefix(s); r.is_hexadecimal) {
          throw std::invalid_argument{
              "chars_format::general does not parse hexfloat"};
        }

        return do_strtod<T>(s);
      }
    };

    template<class T>
    struct parse_number<T, chars_format::hex> {
      auto operator()(std::string const &s) -> T {
        if (auto r = consume_hex_prefix(s); !r.is_hexadecimal) {
          throw std::invalid_argument{"chars_format::hex parses hexfloat"};
        }

        return do_strtod<T>(s);
      }
    };

    template<class T>
    struct parse_number<T, chars_format::scientific> {
      auto operator()(std::string const &s) -> T {
        if (auto r = consume_hex_prefix(s); r.is_hexadecimal) {
          throw std::invalid_argument{
              "chars_format::scientific does not parse hexfloat"};
        }
        if (s.find_first_of("eE") == std::string::npos) {
          throw std::invalid_argument{
              "chars_format::scientific requires exponent part"};
        }

        return do_strtod<T>(s);
      }
    };

    template<class T>
    struct parse_number<T, chars_format::fixed> {
      auto operator()(std::string const &s) -> T {
        if (auto r = consume_hex_prefix(s); r.is_hexadecimal) {
          throw std::invalid_argument{
              "chars_format::fixed does not parse hexfloat"};
        }
        if (s.find_first_of("eE") != std::string::npos) {
          throw std::invalid_argument{
              "chars_format::fixed does not parse exponent part"};
        }

        return do_strtod<T>(s);
      }
    };

  } // namespace details

} // namespace rendu


#endif //RENDU_ARGUMENT_TEMPLATE_H_
