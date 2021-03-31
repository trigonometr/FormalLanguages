#ifndef ALGORITHM_LANGUAGECHECKER_CHARACTERS_H_
#define ALGORITHM_LANGUAGECHECKER_CHARACTERS_H_
#include <string>
#include <vector>
#include <memory>

struct Character {
  bool terminal;
  virtual ~Character() = default;
};

struct RegularCharacter : public Character {
  explicit RegularCharacter(char symbol = '\0');
  char buf_;
};

struct AuxiliaryCharacter : public Character {
  AuxiliaryCharacter(std::string str = std::string());
  bool operator==(const AuxiliaryCharacter &non_term) const;

  class Hash {
   public:
    size_t operator()(const AuxiliaryCharacter &character) const;
   private:
    std::hash<std::string> string_hash_;
  };
  std::string buf_;
};

struct CharacterLine {
 public:
  CharacterLine() = default;
  explicit CharacterLine(const AuxiliaryCharacter &character);
  bool operator==(const CharacterLine &line) const;

  class Hash {
   public:
    size_t operator()(const CharacterLine &character_line) const;
   private:
    std::hash<std::shared_ptr<Character>> shared_ptr_hash_;
  };
  std::vector<std::shared_ptr<Character>> symbols_;
};

#endif
