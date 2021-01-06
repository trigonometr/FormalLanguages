#include "CFGrammar.h"

void CFGrammar::GetCFGrammar(char start, const std::vector<std::pair<char, std::string>> &rules) {
  productions.clear();
  start_char = std::string(1, start);
  for (const auto &rule : rules) {
    CharacterLine symbol_string;
    for (char ch : rule.second) {
      if (ch >= 'A' && ch <= 'Z') {
        symbol_string.symbols_.push_back(
            std::make_shared<AuxiliaryCharacter>(std::string(1, ch)));
      }
      if ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') ||
          (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' ||
              ch == ')')) {
        symbol_string.symbols_.push_back(std::make_shared<RegularCharacter>(ch));
      }
    }
    productions.insert({std::string(1, rule.first), symbol_string});
  }
}