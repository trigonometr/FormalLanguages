#ifndef ALGORITHM_LANGUAGECHECKER_LANGUAGECHECKER_H_
#define ALGORITHM_LANGUAGECHECKER_LANGUAGECHECKER_H_
#include "CFGrammar.h"
#include <unordered_set>


class LanguageChecker {
 public:
  bool WordInLanguage(std::string_view word);
  void GetGrammar(CFGrammar grammar);

 private:
  struct State {
    uint32_t cur_position = 0;
    uint32_t prev_ref = 0;
    std::pair<AuxiliaryCharacter, CharacterLine> production;

    bool operator==(const State &dif_state) const;

    class Hash {
     public:
      size_t operator()(const State &state) const;
     private:
      AuxiliaryCharacter::Hash non_terminal_hash_;
      CharacterLine::Hash symbol_string_hash_;
      std::hash<size_t> size_t_hash_;
    };
  };

  CFGrammar grammar;
  std::vector<std::unordered_set<State, State::Hash>> states;
  std::pair<AuxiliaryCharacter, CharacterLine> starting_rule;

  void Predict(uint32_t current_letter, bool *edited);
  void Complete(uint32_t current_letter, bool *edited);
  void Scan(std::string_view word, uint32_t current_letter);
};

#endif
