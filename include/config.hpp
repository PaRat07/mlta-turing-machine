#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

struct Config {
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

  struct TapeState : std::vector<TapeSymbol> {} tape_init;


  struct TransitionTarget {
    std::optional<TapeSymbol> content;
    std::optional<State> state;

    enum class Move : bool {
      kL, kR
    };

    std::optional<Move> move;
  };

  std::map<State, std::map<TapeSymbol, TransitionTarget>> transitions;
};
