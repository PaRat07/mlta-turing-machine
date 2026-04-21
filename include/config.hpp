#pragma once

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

struct Config {
  template<typename T>
  struct NoopOr : std::optional<T> {
    using std::optional<T>::operator=;
  };

  using State = std::string;
  using TapeSymbol = std::string;

  struct Alphabet {
    std::vector<TapeSymbol> terms;
    std::vector<TapeSymbol> non_terms;
    TapeSymbol blank;
  } alphabet;

  std::vector<State> states;
  State initial_state;
  State halt_state;

  std::filesystem::path tape_init;

  struct TransitionTarget {
    NoopOr<TapeSymbol> content;
    NoopOr<State> state;

    enum class Move : bool {
      kL, kR
    };

    NoopOr<Move> move;
  };

  std::map<State, std::map<TapeSymbol, TransitionTarget>> transitions;


  std::vector<TapeSymbol> ParseTape() const;
};



