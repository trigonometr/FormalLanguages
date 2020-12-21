#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>
#include <utility>

struct Character {
  bool terminal;
};

struct AuxiliaryCharacter : public Character {
  explicit AuxiliaryCharacter(std::string str = std::string());
  bool operator==(const AuxiliaryCharacter &non_term) const;

  class Hash {
   public:
    size_t operator()(const AuxiliaryCharacter &symbol) const;
   private:
    std::hash<std::string> string_hash_;
  };
  std::string buf_;
};

struct RegularCharacter : public Character {
  explicit RegularCharacter(char symbol = 0);
  char character_;
};

struct CharacterLine {
  explicit CharacterLine(const char *symbol);
  bool operator==(const CharacterLine &char_line) const;

  class Hash {
   public:
    size_t operator()(const CharacterLine &symbol_string) const;
   private:
    std::hash<std::shared_ptr<Character>> shared_pointer_hash_;
  };

  std::vector<std::shared_ptr<Character>> characters_;
};

struct Grammar {
  void SingleScan(char start,
                  const std::vector<std::pair<char, std::string>> &rules);
  CharacterLine start_;
  std::unordered_multimap<AuxiliaryCharacter, CharacterLine, AuxiliaryCharacter::Hash>
      rules_;
};

class LanguageChecker {
 public:
  bool WordInLanguage(std::string word);
  void GetGrammar(Grammar grammar);

 private:
  struct State {
    uint32_t position_ = 0;
    uint32_t state_ref_ = 0;
    std::pair<AuxiliaryCharacter, CharacterLine> rule_;
    bool operator==(const State &another) const;

    class Hash {
     public:
      size_t operator()(const State &state) const;
     private:
      AuxiliaryCharacter::Hash non_terminal_hash_;
      CharacterLine::Hash symbol_string_hash_;
      std::hash<size_t> size_t_hash_;
    };
  };

  Grammar grammar_;
  std::vector<std::unordered_set<State, State::Hash>> states_;
  std::pair<AuxiliaryCharacter, CharacterLine> begin_rule_;

  void Scan(std::string_view word, uint32_t current_letter);
  void Predict(std::string_view word, uint32_t current_letter, bool *was_changed);
  void Complete(std::string_view word, uint32_t current_letter, bool *was_changed);
  void Scan(std::string word, uint32_t current_letter);
};




AuxiliaryCharacter::AuxiliaryCharacter(std::string str) : Character(), buf_(std::move(str)) {
  terminal = false;
}

bool AuxiliaryCharacter::operator==(const AuxiliaryCharacter &non_term) const {
  return this->buf_ == non_term.buf_;
}

size_t AuxiliaryCharacter::Hash::operator() (const AuxiliaryCharacter &symbol) const {
  return string_hash_(symbol.buf_);
}

RegularCharacter::RegularCharacter(char symbol) : character_(symbol) { terminal = true; }

CharacterLine::CharacterLine(const char *symbol) {
  characters_.push_back(std::make_shared<AuxiliaryCharacter>(symbol));
}

bool CharacterLine::operator==(const CharacterLine &char_line) const {
  if (this->characters_.size() != char_line.characters_.size()) {
    return false;
  }

  for (uint32_t i = 0; i < this->characters_.size(); ++i) {
    if (this->characters_[i] != char_line.characters_[i])
      return false;
  }

  return true;
}

size_t CharacterLine::Hash::operator()(const CharacterLine &symbol_string) const {
  size_t result = 0;
  for (const auto &symbol : symbol_string.characters_) {
    result += shared_pointer_hash_(symbol);
  }
  return result;
}

void Grammar::SingleScan(char start,
                         const std::vector<std::pair<char, std::string>>& rules) {
  rules_.clear();
  start_ = std::string(1, start);
  for (const auto &rule : rules) {
    CharacterLine symbol_string("");
    for (char ch : rule.second) {
      if (ch >= 'A' && ch <= 'Z') {
        symbol_string.characters_.push_back(
            std::make_shared<AuxiliaryCharacter>(std::string(1, ch)));
      }
      if ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') ||
          (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' ||
           ch == ')')) {
        symbol_string.characters_.push_back(std::make_shared<RegularCharacter>(ch));
      }
    }

    rules_.insert({std::string(1, rule.first), symbol_string});
  }
}

void LanguageChecker::CopyGrammar(Grammar grammar) {
  grammar_ = std::move(grammar);
  CharacterLine prev_start = grammar_.start_;
  begin_rule_ = {AuxiliaryCharacter("START"), CharacterLine(prev_start)};
  grammar_.start_ = begin_rule_.first;
  grammar_.rules_.insert(begin_rule_);
}
bool LanguageChecker::WordInLanguage(std::string word) {}(std::string word) {
  states_.clear();
  states_.resize(word.size() + 1);
  states_[0].insert({0, 0, begin_rule_});

  for (uint32_t current_word_letter = 0; current_word_letter <= word.size();
       ++current_word_letter) {
    Scan(word, current_word_letter);
    bool was_changed;
    do {
      was_changed = false;
      Complete(word, current_word_letter, &was_changed);
      Predict(word, current_word_letter, &was_changed);
    } while (was_changed);
  }

  return !(states_[word.size()].find({1, 0, begin_rule_}) ==
           states_[word.size()].end());
}

void LanguageChecker::Predict(string_view word, uint32_t current_letter,
                              bool *was_changed) {
  for (const auto &state : states_[current_letter]) {
    if (state.rule_.second.characters_.size() > state.position_ &&
        !state.rule_.second.characters_[state.position_]->IsTerminal()) {
      const auto &[begin_rule, end_rule] = grammar_.rules_.equal_range(
          *std::dynamic_pointer_cast<AuxiliaryCharacter>(
              state.rule_.second.characters_[state.position_]));
      for (auto rule = begin_rule; rule != end_rule; ++rule) {
        State new_state = {0, current_letter, {rule->first, rule->second}};
        if (states_[current_letter].find(new_state) ==
            states_[current_letter].end()) {
          states_[current_letter].insert(new_state);
          *was_changed = true;
        }
      }
    }
  }
}

void LanguageChecker::Scan(std::string word, uint32_t current_letter) {
  if (current_letter == 0) {
    return;
  }

  for (const auto &state : states_[current_letter - 1]) {
    if (state.rule_.second.characters_.size() > state.position_ &&
        state.rule_.second.characters_[state.position_]->terminal &&
        std::dynamic_pointer_cast<RegularCharacter>(
            state.rule_.second.characters_[state.position_])->buf_ == word[current_letter - 1]) {
      states_[current_letter].insert(
          {state.position_ + 1, state.state_ref_, state.rule_});
    }
  }
}

void LanguageChecker::Complete(std::string word, uint32_t current_letter,
                               bool *was_changed) {
  for (const auto &completed_state : states_[current_letter]) {
    if (completed_state.rule_.second.characters_.size() ==
        completed_state.position_) {
      for (const auto &state : states_[completed_state.state_ref_]) {
        if (state.rule_.second.characters_.size() > state.position_ &&
            !state.rule_.second.characters_[state.position_]->terminal &&
            *std::dynamic_pointer_cast<AuxiliaryCharacter>(
                state.rule_.second.characters_[state.position_]) ==
                completed_state.rule_.first) {
          State new_state = {state.position_ + 1, state.state_ref_,
                             state.rule_};
          if (states_[current_letter].find(new_state) ==
              states_[current_letter].end()) {
            states_[current_letter].insert(new_state);
            *was_changed = true;
          }
        }
      }
    }
  }
}

bool LanguageChecker::State::operator==(const LanguageChecker::State &another) const {
  return this->position_ == another.position_ &&
         this->state_ref_ == another.state_ref_ && this->rule_ == another.rule_;
}

size_t LanguageChecker::State::Hash::operator()(const LanguageChecker::State &state) const {
  return size_t_hash_(state.position_) + size_t_hash_(state.state_ref_) +
         non_terminal_hash_(state.rule_.first) +
         symbol_string_hash_(state.rule_.second);
}
