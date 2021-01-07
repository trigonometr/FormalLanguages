#include "SubWords.h"

SubWords::SubWords(const std::string& word) : size_(word.size() + 1) {
  table.resize(size_);
  for (auto& str: table) {
    str = std::vector<bool>(size_, false);
  }
}

void SubWords::AddSymbol(const std::string& word, char symbol) {
  if (symbol == '1') {
    for (int i = 0; i < size_; ++i)
      table[i][i] = true;
    return;
  }
  for (int i = 0; i < word.size(); ++i) {
    if (word[i] == symbol) {
      table[i][i + 1] = true;
    }
  }
}

void SubWords::Concat(const SubWords&  tail) {
  auto new_table = table;
  Fulfill();
  for (int i = 0; i < size_; ++i) {
    for (int j = i; j < size_; ++j) {
      for (int k = i; k <= j; ++k) {
        // if the subwords u[i, k-1] and u[k; j - 1] are accepted
        // then the concat of the subregexp must accept
        // u[i, j - 1]
        if(new_table[i][k] && tail.Get(k, j)) {
          table[i][j] = true;
          break;
        }
      }
    }
  }
}

void SubWords::Sum(const SubWords& alternative) {
  auto new_table = table;
  for (int i = 0; i < size_; ++i) {
    for (int j = 0; j < size_; ++j) {
      table[i][j] = new_table[i][j] || alternative.Get(i, j);
    }
  }
}

void SubWords::KleeneStar() {
  auto new_table = table;
  Fulfill();
  for (int i = 0; i < size_; ++i) {
    for (int j = i; j < size_; ++j) {
      if (i == j) table[i][j] = true;
      for (int k = i; k <= j; ++k) {
        if (table[i][k] && new_table[k][j]) {
          table[i][j] = true;
          break;
        }
      }
    }
  }
}

bool SubWords::Get(int i, int j) const  {
  return table.at(i).at(j);
}

void SubWords::Fulfill() {
  for (auto& str: table) {
    str = std::vector<bool>(size_, false);
  }
}