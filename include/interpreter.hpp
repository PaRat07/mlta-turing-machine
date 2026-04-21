#pragma once

#include <config.hpp>

struct Interpreter {
public:
  Interpreter(Config config);

  void Step();

  enum class State : bool {
    kRunning, kTerminated
  };

  State GetState() const;

  void PrintState(std::ostream& out);

private:
  Config config;
  Config::State current_state;
  ptrdiff_t cur_pos;
  std::map<ptrdiff_t, Config::TapeSymbol> current_tape;

  Config::TapeSymbol GetTapeSymbol();
  Config::TransitionTarget GetTransitionTarget();
};