#ifndef PRACTICE_1__SUBWORDS_H_
#define PRACTICE_1__SUBWORDS_H_
#include <string>
#include <vector>

class SubWords {
 public:
  explicit SubWords(const std::string& word);

  void AddSymbol(const std::string& word, char symbol);
  void Concat(const SubWords&  tail);
  void Sum(const SubWords& alternative);
  void KleeneStar();

  bool Get(int i, int j) const;
  void Fulfill();
 private:
  size_t size_;
  std::vector<std::vector<bool>> table;
};

#endif
