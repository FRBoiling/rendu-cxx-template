/*
* Created by boil on 2022/9/17.
*/

#ifndef RENDU_ARGUMENT_PARSER_H_
#define RENDU_ARGUMENT_PARSER_H_

#include "argument.h"

namespace rendu {

  class ArgumentParser {
  public:
    explicit ArgumentParser(std::string program_name = {},
                            std::string version = "1.0",
                            default_arguments add_args = default_arguments::all)
        : m_program_name(std::move(program_name)), m_version(std::move(version)) {
      if ((add_args & default_arguments::help) == default_arguments::help) {
        add_argument("-h", "--help")
            .action([&](const auto &/*unused*/) {
              std::cout << help().str();
              std::exit(0);
            })
            .default_value(false)
            .help("shows help message and exits")
            .implicit_value(true)
            .nargs(0);
      }
      if ((add_args & default_arguments::version) == default_arguments::version) {
        add_argument("-v", "--version")
            .action([&](const auto &/*unused*/) {
              std::cout << m_version << std::endl;
              std::exit(0);
            })
            .default_value(false)
            .help("prints version information and exits")
            .implicit_value(true)
            .nargs(0);
      }
    }

    ArgumentParser(ArgumentParser &&) noexcept = default;

    ArgumentParser &operator=(ArgumentParser &&) = default;

    ArgumentParser(const ArgumentParser &other)
        : m_program_name(other.m_program_name),
          m_version(other.m_version),
          m_description(other.m_description),
          m_epilog(other.m_epilog),
          m_is_parsed(other.m_is_parsed),
          m_positional_arguments(other.m_positional_arguments),
          m_optional_arguments(other.m_optional_arguments) {
      for (auto it = std::begin(m_positional_arguments);
           it != std::end(m_positional_arguments); ++it) {
        index_argument(it);
      }
      for (auto it = std::begin(m_optional_arguments);
           it != std::end(m_optional_arguments); ++it) {
        index_argument(it);
      }
    }

    ~ArgumentParser() = default;

    ArgumentParser &operator=(const ArgumentParser &other) {
      auto tmp = other;
      std::swap(*this, tmp);
      return *this;
    }

    // Parameter packing
    // Call add_argument with variadic number of string arguments
    template<typename... Targs>
    Argument &add_argument(Targs... f_args) {
      using array_of_sv = std::array<std::string_view, sizeof...(Targs)>;
      auto argument = m_optional_arguments.emplace(
          std::cend(m_optional_arguments), array_of_sv{f_args...});

      if (!argument->m_is_optional) {
        m_positional_arguments.splice(std::cend(m_positional_arguments),
                                      m_optional_arguments, argument);
      }

      index_argument(argument);
      return *argument;
    }

    // Parameter packed add_parents method
    // Accepts a variadic number of ArgumentParser objects
    template<typename... Targs>
    ArgumentParser &add_parents(const Targs &...f_args) {
      for (const ArgumentParser &parent_parser: {std::ref(f_args)...}) {
        for (const auto &argument: parent_parser.m_positional_arguments) {
          auto it = m_positional_arguments.insert(
              std::cend(m_positional_arguments), argument);
          index_argument(it);
        }
        for (const auto &argument: parent_parser.m_optional_arguments) {
          auto it = m_optional_arguments.insert(std::cend(m_optional_arguments),
                                                argument);
          index_argument(it);
        }
      }
      return *this;
    }

    ArgumentParser &add_description(std::string description) {
      m_description = std::move(description);
      return *this;
    }

    ArgumentParser &add_epilog(std::string epilog) {
      m_epilog = std::move(epilog);
      return *this;
    }

    /* Call parse_args_internal - which does all the work
     * Then, validate the parsed arguments
     * This variant is used mainly for testing
     * @throws std::runtime_error in case of any invalid argument
     */
    void parse_args(const std::vector<std::string> &arguments) {
      parse_args_internal(arguments);
      // Check if all arguments are parsed
      for ([[maybe_unused]] const auto &[unused, argument]: m_argument_map) {
        argument->validate();
      }
    }

    /* Main entry point for parsing command-line arguments using this
     * ArgumentParser
     * @throws std::runtime_error in case of any invalid argument
     */
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays)
    void parse_args(int argc, const char *const argv[]) {
      parse_args({argv, argv + argc});
    }

    /* Getter for options with default values.
     * @throws std::logic_error if parse_args() has not been previously called
     * @throws std::logic_error if there is no such option
     * @throws std::logic_error if the option has no value
     * @throws std::bad_any_cast if the option is not of type T
     */
    template<typename T = std::string>
    auto get(std::string_view arg_name) const
    -> std::conditional_t<details::IsContainer<T>, T, const T &> {
      if (!m_is_parsed) {
        throw std::logic_error("Nothing parsed, no arguments are available.");
      }
      return (*this)[arg_name].get<T>();
    }

    template<typename E>
    auto get_enum(std::string_view arg_name) const
    -> E {
      if (!m_is_parsed) {
        throw std::logic_error("Nothing parsed, no arguments are available.");
      }
      return (*this)[arg_name].get_enum<E>();
    }

    /* Getter for options without default values.
     * @pre The option has no default value.
     * @throws std::logic_error if there is no such option
     * @throws std::bad_any_cast if the option is not of type T
     */
    template<typename T = std::string>
    auto present(std::string_view arg_name) const -> std::optional<T> {
      return (*this)[arg_name].present<T>();
    }

    /* Getter that returns true for user-supplied options. Returns false if not
     * user-supplied, even with a default value.
     */
    auto is_used(std::string_view arg_name) const {
      return (*this)[arg_name].m_is_used;
    }

    /* Indexing operator. Return a reference to an Argument object
     * Used in conjuction with Argument.operator== e.g., parser["foo"] == true
     * @throws std::logic_error in case of an invalid argument name
     */
    Argument &operator[](std::string_view arg_name) const {
      auto it = m_argument_map.find(arg_name);
      if (it != m_argument_map.end()) {
        return *(it->second);
      }
      if (arg_name.front() != '-') {
        std::string name(arg_name);
        // "-" + arg_name
        name = "-" + name;
        it = m_argument_map.find(name);
        if (it != m_argument_map.end()) {
          return *(it->second);
        }
        // "--" + arg_name
        name = "-" + name;
        it = m_argument_map.find(name);
        if (it != m_argument_map.end()) {
          return *(it->second);
        }
      }
      throw std::logic_error("No such argument: " + std::string(arg_name));
    }

    // Print help message
    friend auto operator<<(std::ostream &stream, const ArgumentParser &parser)
    -> std::ostream & {
      stream.setf(std::ios_base::left);
      stream << "Usage: " << parser.m_program_name << " [options] ";
      std::size_t longest_arg_length = parser.get_length_of_longest_argument();

      for (const auto &argument: parser.m_positional_arguments) {
        stream << argument.m_names.front() << " ";
      }
      stream << "\n\n";

      if (!parser.m_description.empty()) {
        stream << parser.m_description << "\n\n";
      }

      if (!parser.m_positional_arguments.empty()) {
        stream << "Positional arguments:\n";
      }

      for (const auto &argument: parser.m_positional_arguments) {
        stream.width(longest_arg_length);
        stream << argument;
      }

      if (!parser.m_optional_arguments.empty()) {
        stream << (parser.m_positional_arguments.empty() ? "" : "\n")
               << "Optional arguments:\n";
      }

      for (const auto &argument: parser.m_optional_arguments) {
        stream.width(longest_arg_length);
        stream << argument;
      }

      if (!parser.m_epilog.empty()) {
        stream << '\n';
        stream << parser.m_epilog << "\n\n";
      }

      return stream;
    }

    // Format help message
    auto help() const -> std::stringstream {
      std::stringstream out;
      out << *this;
      return out;
    }

    // Printing the one and only help message
    // I've stuck with a simple message format, nothing fancy.
    [[deprecated("Use cout << program; instead.  See also help().")]] std::string
    print_help() const {
      auto out = help();
      std::cout << out.rdbuf();
      return out.str();
    }

  private:
    /*
     * @throws std::runtime_error in case of any invalid argument
     */
    void parse_args_internal(const std::vector<std::string> &arguments) {
      if (m_program_name.empty() && !arguments.empty()) {
        m_program_name = arguments.front();
      }
      auto end = std::end(arguments);
      auto positional_argument_it = std::begin(m_positional_arguments);
      for (auto it = std::next(std::begin(arguments)); it != end;) {
        const auto &current_argument = *it;
        if (Argument::is_positional(current_argument)) {
          if (positional_argument_it == std::end(m_positional_arguments)) {
            throw std::runtime_error(
                "Maximum number of positional arguments exceeded");
          }
          auto argument = positional_argument_it++;
          it = argument->consume(it, end);
          continue;
        }

        auto arg_map_it = m_argument_map.find(current_argument);
        if (arg_map_it != m_argument_map.end()) {
          auto argument = arg_map_it->second;
          it = argument->consume(std::next(it), end, arg_map_it->first);
        } else if (const auto &compound_arg = current_argument;
            compound_arg.size() > 1 && compound_arg[0] == '-' &&
            compound_arg[1] != '-') {
          ++it;
          for (std::size_t j = 1; j < compound_arg.size(); j++) {
            auto hypothetical_arg = std::string{'-', compound_arg[j]};
            auto arg_map_it2 = m_argument_map.find(hypothetical_arg);
            if (arg_map_it2 != m_argument_map.end()) {
              auto argument = arg_map_it2->second;
              it = argument->consume(it, end, arg_map_it2->first);
            } else {
              throw std::runtime_error("Unknown argument: " + current_argument);
            }
          }
        } else {
          throw std::runtime_error("Unknown argument: " + current_argument);
        }
      }
      m_is_parsed = true;
    }

    // Used by print_help.
    std::size_t get_length_of_longest_argument() const {
      if (m_argument_map.empty()) {
        return 0;
      }
      std::size_t max_size = 0;
      for ([[maybe_unused]] const auto &[unused, argument]: m_argument_map) {
        max_size = std::max(max_size, argument->get_arguments_length());
      }
      return max_size;
    }

    using list_iterator = std::list<Argument>::iterator;

    void index_argument(list_iterator it) {
      for (const auto &name: std::as_const(it->m_names)) {
        m_argument_map.insert_or_assign(name, it);
      }
    }

    std::string m_program_name;
    std::string m_version;
    std::string m_description;
    std::string m_epilog;
    bool m_is_parsed = false;
    std::list<Argument> m_positional_arguments;
    std::list<Argument> m_optional_arguments;
    std::map<std::string_view, list_iterator, std::less<>> m_argument_map;
  };

}//namespace rendu

#endif //RENDU_ARGUMENT_PARSER_H_
