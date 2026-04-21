#include <config.hpp>
#include <parser.hpp>
#include <interpreter.hpp>

#include <iostream>

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    throw std::invalid_argument("Wrong number of cli arguments");
  }
  std::string_view config_path = argv[1];
  auto config = ParseConfig(config_path);

  Interpreter interpreter(std::move(config));
  while (interpreter.GetState() == Interpreter::State::kRunning) {
    interpreter.PrintState(std::cout);
    interpreter.Step();
  }
  interpreter.PrintState(std::cout);
}

