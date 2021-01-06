#include "Characters.h"

// RegularCharacter implementation

RegularCharacter::RegularCharacter(char symbol) : buf_(symbol) {
  terminal = true;
}

// AuxiliaryCharacter implementation

AuxiliaryCharacter::AuxiliaryCharacter(std::string str) : buf_(std::move(str)) {
  terminal = false;
}

bool AuxiliaryCharacter::operator==(const AuxiliaryCharacter &non_term) const {
  return this->buf_ == non_term.buf_;
}

size_t AuxiliaryCharacter::Hash::operator()(const AuxiliaryCharacter &character) const {
  return string_hash_(character.buf_);
}

// CharacterLine implementation

CharacterLine::CharacterLine(const AuxiliaryCharacter &character) {
  symbols_.push_back(std::make_shared<AuxiliaryCharacter>(character));
}

bool CharacterLine::operator==(const CharacterLine &line) const {
  if (this->symbols_.size() != line.symbols_.size()) {
    return false;
  }

  for (uint32_t ptr = 0; ptr < this->symbols_.size(); ++ptr) {
    if (this->symbols_[ptr] != line.symbols_[ptr])
      return false;
  }
  return true;
}

size_t CharacterLine::Hash::operator()(const CharacterLine &line) const {
  size_t result = 0;
  for (const auto &symbol : line.symbols_) {
    result += shared_ptr_hash_(symbol);
  }
  return result;
}

