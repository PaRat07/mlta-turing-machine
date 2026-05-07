#include "parser.hpp"

#include <rfl/yaml.hpp>
#include <rfl.hpp>

#include <ranges>
#include <array>


namespace rfl {
template <>
struct Reflector<std::filesystem::path> {
  using ReflType = std::string;

  static std::filesystem::path to(const ReflType& s) {
    std::filesystem::path p = s;
    if (!std::filesystem::is_regular_file(p)) {
      throw std::invalid_argument(std::format("invalid file path: \"{}\"", s));
    }
    return p;
  }
};
template <>
struct Reflector<Config::NoopOr<Config::TransitionTarget::Move>> {
  using Move = Config::TransitionTarget::Move;
  using NoopOrMove = Config::NoopOr<Move>;
  using ReflType = std::string;

  static NoopOrMove to(const ReflType& s) {
    if (s == "_") {
      return {std::nullopt};
    } else if (s == "l") {
      return {Move::kL};
    } else if (s == "r") {
      return {Move::kR};
    } else {
      throw std::invalid_argument("invalid move value");
    }
  }
};

template <>
struct Reflector<Config::NoopOr<std::string>> {
  using ReflType = std::string;
  using T = Config::NoopOr<std::string>;
  static T to(const ReflType& opt) {
    return opt == "_" ? T(std::nullopt) : T(opt);
  }
};

// Parse TransitionTarget from a YAML sequence [content, state, move]
template<>
struct Reflector<Config::TransitionTarget> {
  using ReflType = std::array<Config::NoopOr<std::string>, 3>;

  static Config::TransitionTarget to(const ReflType& arr) {
    Config::TransitionTarget t;
    t.content = arr[0];
    t.state = arr[1];

    t.move = arr[2].transform([] (std::string_view sv) {
      if (sv == "l") return Config::TransitionTarget::Move::kL;
      else if (sv == "r") return Config::TransitionTarget::Move::kR;
      else throw std::invalid_argument("invalid move value");
    });

    return t;
  }
};

} // namespace rfl


template<typename T>
T Unwrap(rfl::Result<T> res) {
  return *res.or_else([] (const rfl::Error& err) -> T {
    throw std::invalid_argument(err.what());
  });
}

Config ParseConfig(std::filesystem::path config_path) {
  std::ifstream fin(config_path);
  if (!fin) {
    throw std::runtime_error("failed to open config file");
  }
  auto config = Unwrap(rfl::yaml::read<Config>(fin));
  std::vector<Config::TapeSymbol> symbols;
  std::ranges::copy(config.alphabet.terms, std::back_inserter(symbols));
  std::ranges::copy(config.alphabet.non_terms, std::back_inserter(symbols));
  if (!std::ranges::count(config.states, config.initial_state)) {
    throw std::invalid_argument("initial state is invalid");
  }
  if (!std::ranges::count(config.states, config.halt_state)) {
    throw std::invalid_argument("initial state is invalid");
  }
  return config;
}
