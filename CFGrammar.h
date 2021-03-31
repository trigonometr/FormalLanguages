#ifndef ALGORITHM_LANGUAGECHECKER_CFGRAMMAR_H_
#define ALGORITHM_LANGUAGECHECKER_CFGRAMMAR_H_
#include "Characters.h"
#include <vector>
#include <unordered_map>

struct CFGrammar {
  void GetCFGrammar(char start, const std::vector<std::pair<char, std::string>> &rules);
  AuxiliaryCharacter start_char;
  std::unordered_multimap<AuxiliaryCharacter, CharacterLine, AuxiliaryCharacter::Hash> productions;
};

#endif
