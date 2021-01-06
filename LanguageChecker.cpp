#include "LanguageChecker.h"

bool LanguageChecker::WordInLanguage(std::string_view word) {
  states.clear();
  states.resize(word.size() + 1);
  states[0].insert({0, 0, starting_rule});

  for (uint32_t current_word_letter = 0; current_word_letter <= word.size();
       ++current_word_letter) {
    Scan(word, current_word_letter);
    bool edited;
    do {
      edited = false;
      Complete(current_word_letter, &edited);
      Predict(current_word_letter, &edited);
    } while (edited);
  }

  return !(states[word.size()].find({1, 0, starting_rule}) ==
      states[word.size()].end());
}

void LanguageChecker::GetGrammar(CFGrammar cf_grammar) {
  grammar = std::move(cf_grammar);
  AuxiliaryCharacter prev_start = grammar.start_char;
  starting_rule = {AuxiliaryCharacter("START"), CharacterLine(prev_start)};
  grammar.start_char = starting_rule.first;
  grammar.productions.insert(starting_rule);
}

void LanguageChecker::Predict(uint32_t current_letter, bool *edited) {
  for (const auto &state : states[current_letter]) {
    if (state.production.second.symbols_.size() > state.cur_position &&
        !state.production.second.symbols_[state.cur_position]->terminal) {
      const auto &[begin_rule, end_rule] = grammar.productions.equal_range(
          *std::dynamic_pointer_cast<AuxiliaryCharacter>(
              state.production.second.symbols_[state.cur_position]));
      for (auto rule = begin_rule; rule != end_rule; ++rule) {
        State new_state = {0, current_letter, {rule->first, rule->second}};
        if (states[current_letter].find(new_state) ==
            states[current_letter].end()) {
          states[current_letter].insert(new_state);
          *edited = true;
        }
      }
    }
  }
}

void LanguageChecker::Complete(uint32_t current_letter, bool *edited) {
  for (const auto &completed_state : states[current_letter]) {
    if (completed_state.production.second.symbols_.size() ==
        completed_state.cur_position) {
      for (const auto &state : states[completed_state.prev_ref]) {
        if (state.production.second.symbols_.size() > state.cur_position &&
            !state.production.second.symbols_[state.cur_position]->terminal &&
            *std::dynamic_pointer_cast<AuxiliaryCharacter>(
                state.production.second.symbols_[state.cur_position]) ==
                completed_state.production.first) {
          State new_state = {state.cur_position + 1, state.prev_ref,
                             state.production};
          if (states[current_letter].find(new_state) ==
              states[current_letter].end()) {
            states[current_letter].insert(new_state);
            *edited = true;
          }
        }
      }
    }
  }
}

void LanguageChecker::Scan(std::string_view word, uint32_t current_letter) {
  if (current_letter == 0) {
    return;
  }
  for (const auto &state : states[current_letter - 1]) {
    if (state.production.second.symbols_.size() > state.cur_position &&
        state.production.second.symbols_[state.cur_position]->terminal &&
        std::dynamic_pointer_cast<RegularCharacter>(
            state.production.second.symbols_[state.cur_position])
            ->buf_ == word[current_letter - 1]) {
      states[current_letter].insert(
          {state.cur_position + 1, state.prev_ref, state.production});
    }
  }
}

bool LanguageChecker::State::operator==(const LanguageChecker::State &dif_state) const {
  return this->cur_position == dif_state.cur_position &&
      this->prev_ref == dif_state.prev_ref && this->production == dif_state.production;
}
size_t LanguageChecker::State::Hash::operator()(const LanguageChecker::State &state) const {
  return size_t_hash_(state.cur_position) + size_t_hash_(state.prev_ref) +
      non_terminal_hash_(state.production.first) +
      symbol_string_hash_(state.production.second);
}