#include <interpreter.hpp>

#include <algorithm>
#include <cassert>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>

Interpreter::Interpreter(Config config)
  : config(std::move(config))
  , current_state(this->config.initial_state)
  , cur_pos(0) {
  auto tape = this->config.ParseTape();
  for (ptrdiff_t i = 0; i < tape.size(); ++i) {
    if (tape[i] != this->config.alphabet.blank) {
      current_tape[i] = tape[i];
    }
  }
}

void Interpreter::Step() {
  if (current_state == config.halt_state) return;

  const Config::TransitionTarget& target = GetTransitionTarget();

  if (target.content.has_value()) {
    if (*target.content == config.alphabet.blank) {
      current_tape.erase(cur_pos);
    } else {
      current_tape[cur_pos] = *target.content;
    }
  }

  if (target.state.has_value()) {
    current_state = *target.state;
  }

  if (target.move.has_value()) {
    if (*target.move == Config::TransitionTarget::Move::kL) {
      --cur_pos;
    } else {
      ++cur_pos;
    }
  }
}

Interpreter::State Interpreter::GetState() const {
  return current_state == config.halt_state ? State::kTerminated : State::kRunning;
}

void Interpreter::PrintState(std::ostream& out) {
  out << "State: " << current_state << "\n";
  out << "Tape: \n";

  if (current_tape.empty()) {
    out << "[]\n";
    return;
  }

  out << "[";
  ptrdiff_t prev_pos = 0;
  ptrdiff_t head_shift = 1;
  ptrdiff_t head_len;
  bool found_head = false;
  auto write = [&] (std::string_view sv) {
    if (!found_head)
      head_shift += sv.size();
    out << sv;
  };
  if (!current_tape.contains(cur_pos)) {
    current_tape[cur_pos] = config.alphabet.blank;
  }
  for (auto [pos, val] : current_tape) {
    // if (prev_pos + 5 < pos) {
    //   write(std::format("<{} empty cells>, ", pos - prev_pos - 1));
    // } else {
      for (int i = prev_pos + 1; i < pos; ++i) {
        write(", ");
        write(config.alphabet.blank);
      }
    // }
    if (prev_pos < pos) {
      write(", ");
    }
    if (pos == cur_pos) {
      found_head = true;
      head_len = val.size();
    }
    write(val);
    prev_pos = pos;
  }
  if (current_tape[cur_pos] == config.alphabet.blank) {
    current_tape.erase(cur_pos);
  }
  out << "]\n";
  out << std::string(head_shift, ' ') << "^" << std::string(head_len - 1, '~') << "\n\n";
}

Config::TapeSymbol Interpreter::GetTapeSymbol() {
  auto it = current_tape.find(cur_pos);
  if (it == current_tape.end()) return config.alphabet.blank;
  return it->second;
}
Config::TransitionTarget Interpreter::GetTransitionTarget() {
  try {
    auto& symbols_trans = config.transitions.at(current_state);
    Config::TapeSymbol sym = GetTapeSymbol();
    if (symbols_trans.contains(sym)) {
      return symbols_trans[sym];
    } else {
      return symbols_trans.at("*");
    }
  } catch (const std::out_of_range&) {
    throw std::runtime_error("No transition found");
  }
}
