#include <config.hpp>

#include <algorithm>
#include <format>
#include <fstream>
#include <ranges>

std::vector<Config::TapeSymbol> Config::ParseTape() const {
  std::vector<TapeSymbol> res;
  std::ifstream fin(tape_init);
  if (!fin) {
    throw std::invalid_argument(std::format("failed to open tape file {}", tape_init.string()));
  }

  res.emplace_back();
  for (char c : std::views::istream<char>(fin)) {
    if (c == ',') {
      res.emplace_back();
    } else {
      res.back().push_back(c);
    }
  }
  for (auto& i : res) {
    if (std::ranges::count(alphabet.terms, i) + std::ranges::count(alphabet.non_terms, i) == 0) {
      throw std::invalid_argument(std::format("invalid tape symbol: \"{}\"", i));
    }
  }
  return res;
}
