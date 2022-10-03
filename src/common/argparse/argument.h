/*
* Created by boil on 2022/9/17.
*/

#ifndef RENDU_ARGUMENT_H_
#define RENDU_ARGUMENT_H_

#include "argument_template.h"
#include "argument_defines.h"

namespace rendu {
  class ArgumentParser;

  class Argument {
    friend class ArgumentParser;

    friend auto operator<<(std::ostream &stream, const ArgumentParser &parser)
    -> std::ostream &;

    template<std::size_t N, std::size_t... I>
    explicit Argument(std::array<std::string_view, N> &&a,
                      std::index_sequence<I...> /*unused*/)
        : m_is_optional((is_optional(a[I]) || ...)), m_is_required(false),
          m_is_repeatable(false), m_is_used(false) {
      ((void) m_names.emplace_back(a[I]), ...);
      std::sort(
          m_names.begin(), m_names.end(), [](const auto &lhs, const auto &rhs) {
            return lhs.size() == rhs.size() ? lhs < rhs : lhs.size() < rhs.size();
          });
    }

  public:
    template<std::size_t N>
    explicit Argument(std::array<std::string_view, N> &&a)
        : Argument(std::move(a), std::make_index_sequence<N>{}) {}

    Argument &help(std::string help_text) {
      m_help = std::move(help_text);
      return *this;
    }

    template<typename T>
    Argument &default_value(T &&value) {
      m_default_value_repr = details::repr(value);
      m_default_value = std::forward<T>(value);
      return *this;
    }

    Argument &required() {
      m_is_required = true;
      return *this;
    }

    Argument &implicit_value(std::any value) {
      m_implicit_value = std::move(value);
      m_num_args_range = NArgsRange{0, 0};
      return *this;
    }

    template<class F, class... Args>
    auto action(F &&callable, Args &&...bound_args)
    -> std::enable_if_t<std::is_invocable_v<F, Args..., std::string const>,
        Argument &> {
      using action_type = std::conditional_t<
          std::is_void_v<std::invoke_result_t<F, Args..., std::string const>>,
          void_action, valued_action>;
      if constexpr (sizeof...(Args) == 0) {
        m_action.emplace<action_type>(std::forward<F>(callable));
      } else {
        m_action.emplace<action_type>(
            [f = std::forward<F>(callable),
                tup = std::make_tuple(std::forward<Args>(bound_args)...)](
                std::string const &opt) mutable {
              return details::apply_plus_one(f, tup, opt);
            });
      }
      return *this;
    }

    auto &append() {
      m_is_repeatable = true;
      return *this;
    }

    template<char Shape, typename T>
    auto scan() -> std::enable_if_t<std::is_arithmetic_v<T>, Argument &> {
      static_assert(!(std::is_const_v<T> || std::is_volatile_v<T>),
                    "T should not be cv-qualified");
      auto is_one_of = [](char c, auto... x) constexpr {
        return ((c == x) || ...);
      };

      if constexpr (is_one_of(Shape, 'd') && details::standard_integer<T>) {
        action(details::parse_number<T, details::radix_10>());
      } else if constexpr (is_one_of(Shape, 'i') && details::standard_integer<T>) {
        action(details::parse_number<T>());
      } else if constexpr (is_one_of(Shape, 'u') &&
                           details::standard_unsigned_integer<T>) {
        action(details::parse_number<T, details::radix_10>());
      } else if constexpr (is_one_of(Shape, 'o') &&
                           details::standard_unsigned_integer<T>) {
        action(details::parse_number<T, details::radix_8>());
      } else if constexpr (is_one_of(Shape, 'x', 'X') &&
                           details::standard_unsigned_integer<T>) {
        action(details::parse_number<T, details::radix_16>());
      } else if constexpr (is_one_of(Shape, 'a', 'A') &&
                           std::is_floating_point_v<T>) {
        action(details::parse_number<T, details::chars_format::hex>());
      } else if constexpr (is_one_of(Shape, 'e', 'E') &&
                           std::is_floating_point_v<T>) {
        action(details::parse_number<T, details::chars_format::scientific>());
      } else if constexpr (is_one_of(Shape, 'f', 'F') &&
                           std::is_floating_point_v<T>) {
        action(details::parse_number<T, details::chars_format::fixed>());
      } else if constexpr (is_one_of(Shape, 'g', 'G') &&
                           std::is_floating_point_v<T>) {
        action(details::parse_number<T, details::chars_format::general>());
      } else {
        static_assert(alignof(T) == 0, "No scan specification for T");
      }

      return *this;
    }

    Argument &nargs(std::size_t num_args) {
      m_num_args_range = NArgsRange{num_args, num_args};
      return *this;
    }

    Argument &nargs(std::size_t num_args_min, std::size_t num_args_max) {
      m_num_args_range = NArgsRange{num_args_min, num_args_max};
      return *this;
    }

    Argument &nargs(nargs_pattern pattern) {
      switch (pattern) {
        case nargs_pattern::optional:
          m_num_args_range = NArgsRange{0, 1};
          break;
        case nargs_pattern::any:
          m_num_args_range = NArgsRange{0, std::numeric_limits<std::size_t>::max()};
          break;
        case nargs_pattern::at_least_one:
          m_num_args_range = NArgsRange{1, std::numeric_limits<std::size_t>::max()};
          break;
      }
      return *this;
    }

    Argument &remaining() {
      m_accepts_optional_like_value = true;
      return nargs(nargs_pattern::any);
    }

    template<typename Iterator>
    Iterator consume(Iterator start, Iterator end,
                     std::string_view used_name = {}) {
      if (!m_is_repeatable && m_is_used) {
        throw std::runtime_error("Duplicate argument");
      }
      m_is_used = true;
      m_used_name = used_name;

      const auto num_args_max = m_num_args_range.get_max();
      const auto num_args_min = m_num_args_range.get_min();
      std::size_t dist = 0;
      if (num_args_max == 0) {
        m_values.emplace_back(m_implicit_value);
        std::visit([](const auto &f) { f({}); }, m_action);
        return start;
      }
      if ((dist = static_cast<std::size_t>(std::distance(start, end))) >= num_args_min) {
        if (num_args_max < dist) {
          end = std::next(start, num_args_max);
        }
        if (!m_accepts_optional_like_value) {
          end = std::find_if(start, end, Argument::is_optional);
          dist = static_cast<std::size_t>(std::distance(start, end));
          if (dist < num_args_min) {
            throw std::runtime_error("Too few arguments");
          }
        }

        struct ActionApply {
          void operator()(valued_action &f) {
            std::transform(first, last, std::back_inserter(self.m_values), f);
          }

          void operator()(void_action &f) {
            std::for_each(first, last, f);
            if (!self.m_default_value.has_value()) {
              if (!self.m_accepts_optional_like_value) {
                self.m_values.resize(std::distance(first, last));
              }
            }
          }

          Iterator first, last;
          Argument &self;
        };
        std::visit(ActionApply{start, end, *this}, m_action);
        return end;
      }
      if (m_default_value.has_value()) {
        return start;
      }
      throw std::runtime_error("Too few arguments for '" +
                               std::string(m_used_name) + "'.");
    }

    /*
     * @throws std::runtime_error if argument values are not valid
     */
    void validate() const {
      if (m_is_optional) {
        // TODO: check if an implicit value was programmed for this argument
        if (!m_is_used && !m_default_value.has_value() && m_is_required) {
          throw_required_arg_not_used_error();
        }
        if (m_is_used && m_is_required && m_values.empty()) {
          throw_required_arg_no_value_provided_error();
        }
      } else {
        if (!m_num_args_range.contains(m_values.size()) && !m_default_value.has_value()) {
          throw_nargs_range_validation_error();
        }
      }
    }

    std::size_t get_arguments_length() const {
      return std::accumulate(std::begin(m_names), std::end(m_names),
                             std::size_t(0), [](const auto &sum, const auto &s) {
            return sum + s.size() +
                   1; // +1 for space between names
          });
    }

    friend std::ostream &operator<<(std::ostream &stream,
                                    const Argument &argument) {
      std::stringstream name_stream;
      std::copy(std::begin(argument.m_names), std::end(argument.m_names),
                std::ostream_iterator<std::string>(name_stream, " "));
      stream << name_stream.str() << "\t" << argument.m_help;
      if (argument.m_default_value.has_value()) {
        if (!argument.m_help.empty()) {
          stream << " ";
        }
        stream << "[default: " << argument.m_default_value_repr << "]";
      } else if (argument.m_is_required) {
        if (!argument.m_help.empty()) {
          stream << " ";
        }
        stream << "[required]";
      }
      stream << "\n";
      return stream;
    }

    template<typename T>
    bool operator!=(const T &rhs) const {
      return !(*this == rhs);
    }

    /*
     * Compare to an argument value of known type
     * @throws std::logic_error in case of incompatible types
     */
    template<typename T>
    bool operator==(const T &rhs) const {
      if constexpr (!details::IsContainer<T>) {
        return get<T>() == rhs;
      } else {
        auto lhs = get<T>();
        return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs),
                          std::end(rhs), [](const auto &lhs, const auto &rhs) {
              return lhs == rhs;
            });
      }
    }

  private:

    class NArgsRange {
      std::size_t m_min;
      std::size_t m_max;

    public:
      NArgsRange(std::size_t minimum, std::size_t maximum)
          : m_min(minimum), m_max(maximum) {
        if (minimum > maximum) {
          throw std::logic_error("Range of number of arguments is invalid");
        }
      }

      bool contains(std::size_t value) const {
        return value >= m_min && value <= m_max;
      }

      bool is_exact() const {
        return m_min == m_max;
      }

      bool is_right_bounded() const {
        return m_max < std::numeric_limits<std::size_t>::max();
      }

      std::size_t get_min() const {
        return m_min;
      }

      std::size_t get_max() const {
        return m_max;
      }
    };

    void throw_nargs_range_validation_error() const {
      std::stringstream stream;
      if (!m_used_name.empty()) {
        stream << m_used_name << ": ";
      }
      if (m_num_args_range.is_exact()) {
        stream << m_num_args_range.get_min();
      } else if (m_num_args_range.is_right_bounded()) {
        stream << m_num_args_range.get_min() << " to " << m_num_args_range.get_max();
      } else {
        stream << m_num_args_range.get_min() << " or more";
      }
      stream << " argument(s) expected. " << m_values.size() << " provided.";
      throw std::runtime_error(stream.str());
    }

    void throw_required_arg_not_used_error() const {
      std::stringstream stream;
      stream << m_names[0] << ": required.";
      throw std::runtime_error(stream.str());
    }

    void throw_required_arg_no_value_provided_error() const {
      std::stringstream stream;
      stream << m_used_name << ": no value provided.";
      throw std::runtime_error(stream.str());
    }

    static constexpr int eof = std::char_traits<char>::eof();

    static auto lookahead(std::string_view s) -> int {
      if (s.empty()) {
        return eof;
      }
      return static_cast<int>(static_cast<unsigned char>(s[0]));
    }

    /*
     * decimal-literal:
     *    '0'
     *    nonzero-digit digit-sequence_opt
     *    integer-part fractional-part
     *    fractional-part
     *    integer-part '.' exponent-part_opt
     *    integer-part exponent-part
     *
     * integer-part:
     *    digit-sequence
     *
     * fractional-part:
     *    '.' post-decimal-point
     *
     * post-decimal-point:
     *    digit-sequence exponent-part_opt
     *
     * exponent-part:
     *    'e' post-e
     *    'E' post-e
     *
     * post-e:
     *    sign_opt digit-sequence
     *
     * sign: one of
     *    '+' '-'
     */
    static bool is_decimal_literal(std::string_view s) {
      auto is_digit = [](auto c) constexpr {
        switch (c) {
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            return true;
          default:
            return false;
        }
      };

      // precondition: we have consumed or will consume at least one digit
      auto consume_digits = [=](std::string_view s) {
        // NOLINTNEXTLINE(readability-qualified-auto)
        auto it = std::find_if_not(std::begin(s), std::end(s), is_digit);
        return s.substr(it - std::begin(s));
      };

      switch (lookahead(s)) {
        case '0': {
          s.remove_prefix(1);
          if (s.empty()) {
            return true;
          }
          goto integer_part;
        }
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
          s = consume_digits(s);
          if (s.empty()) {
            return true;
          }
          goto integer_part_consumed;
        }
        case '.': {
          s.remove_prefix(1);
          goto post_decimal_point;
        }
        default:
          return false;
      }

      integer_part:
      s = consume_digits(s);
      integer_part_consumed:
      switch (lookahead(s)) {
        case '.': {
          s.remove_prefix(1);
          if (is_digit(lookahead(s))) {
            goto post_decimal_point;
          } else {
            goto exponent_part_opt;
          }
        }
        case 'e':
        case 'E': {
          s.remove_prefix(1);
          goto post_e;
        }
        default:
          return false;
      }

      post_decimal_point:
      if (is_digit(lookahead(s))) {
        s = consume_digits(s);
        goto exponent_part_opt;
      }
      return false;

      exponent_part_opt:
      switch (lookahead(s)) {
        case eof:
          return true;
        case 'e':
        case 'E': {
          s.remove_prefix(1);
          goto post_e;
        }
        default:
          return false;
      }

      post_e:
      switch (lookahead(s)) {
        case '-':
        case '+':
          s.remove_prefix(1);
      }
      if (is_digit(lookahead(s))) {
        s = consume_digits(s);
        return s.empty();
      }
      return false;
    }

    static bool is_optional(std::string_view name) {
      return !is_positional(name);
    }

    /*
     * positional:
     *    _empty_
     *    '-'
     *    '-' decimal-literal
     *    !'-' anything
     */
    static bool is_positional(std::string_view name) {
      switch (lookahead(name)) {
        case eof:
          return true;
        case '-': {
          name.remove_prefix(1);
          if (name.empty()) {
            return true;
          }
          return is_decimal_literal(name);
        }
        default:
          return true;
      }
    }

    /*
     * Get argument value given a type
     * @throws std::logic_error in case of incompatible types
     */
    template<typename T>
    auto get() const -> std::conditional_t<details::IsContainer<T>, T, const T &> {
      if (!m_values.empty()) {
        if constexpr (details::IsContainer<T>) {
          return any_cast_container<T>(m_values);
        } else {
          return *std::any_cast<T>(&m_values.front());
        }
      }
      if (m_default_value.has_value()) {
        return *std::any_cast<T>(&m_default_value);
      }
      if constexpr (details::IsContainer<T>) {
        if (!m_accepts_optional_like_value) {
          return any_cast_container<T>(m_values);
        }
      }

      throw std::logic_error("No value provided for '" + m_names.back() + "'.");
    }

    template<typename E>
    auto get_enum() const -> E {
      if (is_scoped_enum_v<E>) {
        if (!m_values.empty()) {
          auto value = *std::any_cast<int>(&m_values.front());
          auto enum_object = enum_cast<E>(value);
          if (enum_object.has_value()) {
            return enum_object.value();
          }
        }
        if (m_default_value.has_value()) {
          return *std::any_cast<E>(&m_default_value);
        }
      }
      throw std::logic_error("No enum value provided for '" + m_names.back() + "'.");
    }


    /*
     * Get argument value given a type.
     * @pre The object has no default value.
     * @returns The stored value if any, std::nullopt otherwise.
     */
    template<typename T>
    auto present() const -> std::optional<T> {
      if (m_default_value.has_value()) {
        throw std::logic_error("Argument with default value always presents");
      }
      if (m_values.empty()) {
        return std::nullopt;
      }
      if constexpr (details::IsContainer<T>) {
        return any_cast_container<T>(m_values);
      }
      return std::any_cast<T>(m_values.front());
    }

    template<typename T>
    static auto any_cast_container(const std::vector<std::any> &operand) -> T {
      using ValueType = typename T::value_type;

      T result;
      std::transform(
          std::begin(operand), std::end(operand), std::back_inserter(result),
          [](const auto &value) { return *std::any_cast<ValueType>(&value); });
      return result;
    }

    std::vector<std::string> m_names;
    std::string_view m_used_name;
    std::string m_help;
    std::any m_default_value;
    std::string m_default_value_repr;
    std::any m_implicit_value;
    using valued_action = std::function<std::any(const std::string &)>;
    using void_action = std::function<void(const std::string &)>;
    std::variant<valued_action, void_action> m_action{
        std::in_place_type<valued_action>,
        [](const std::string &value) { return value; }};
    std::vector<std::any> m_values;
    NArgsRange m_num_args_range{1, 1};
    bool m_accepts_optional_like_value = false;
    bool m_is_optional: true;
    bool m_is_required: true;
    bool m_is_repeatable: true;
    bool m_is_used: true; // True if the optional argument is used by user
  };

} // rendu

#endif //RENDU_ARGUMENT_H_
