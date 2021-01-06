#include "LanguageChecker/LanguageChecker.h"
#include <iostream>
#include <cassert>

void Test(const std::vector<std::pair<char, std::string>>& productions,
          const std::vector<std::string>& test_words,
          const std::vector<bool>& result) {
  LanguageChecker helper;
  CFGrammar grammar;
  grammar.GetCFGrammar('S', productions);
  helper.GetGrammar(grammar);
  for (int i = 0; i < test_words.size(); ++i) {
    assert(helper.WordInLanguage(test_words[i]) == result[i]);
  }
}

int main() {
  std::vector<std::pair<char, std::string>> productions;
  std::vector<std::string> test_words;
  std::vector<bool> result;

  // a^n * b^n
  productions = {{'S', "aSb"}, {'S', ""}};
  test_words = {"aaaaaaaa", "aabb", "bbbbbb", "aaabb", "abab", "", "aaaaaabbbbbb"};
  result = {false, true, false, false, false, true, true};
  Test(productions, test_words, result);

  // Balanced seq
  productions = {{'S', "(S)S"}, {'S', ""}};
  test_words = {"(((((())))", "(()(()()))", ")", "", "(((()())())((()())()))", "()()))(()())((())()))((()())())()()())("};
  result = {false, true, false, true, true, false};
  Test(productions, test_words, result);

  // |w|_a = |w|_b
  productions = {{'S', "aSbS"}, {'S', "bSaS"}, {'S', ""}};
  test_words = {"abaabaaaabbb", "abbbaababbaabaabaabaabbbbaaaabaaaabbbbbb", "ab", "ba", "bbbbbbbbbbbbbbaaaaaaaaaaaaaaa", ""};
  result = {false, true, true, true, false, true};
  Test(productions, test_words, result);

  // arithmetic expressions x, y
  productions = {{'S', "P+S"}, {'S', "P"}, {'P', "F*P"},
                 {'P', "FP"}, {'P', "F"}, {'F', "x"},
                 {'F', "y"}, {'F', "(S)"}};
  test_words = {"x+y", "(x+y)*x+y", "x", "y+", "xx", "xxyxyxyxxxx"};
  result = {true, true, true, false, true, true};
  Test(productions, test_words, result);

  // w = w^R
  productions = {{'S', "aSa"}, {'S', "bSb"}, {'S', "a"},
                 {'S', "b"}, {'S', ""}};
  test_words = {"ababababa", "", "bbaab", "bbaabb", "aaaaaaaaaaaaaaaaa", "bbbbababbbabbbba"};
  result = {true, true, false, true, true, false};
  Test(productions, test_words, result);

  // a^n * b^m (n >= m)
  productions = {{'S', "aS"}, {'S', "T"},
                 {'T', "aTb"}, {'T', ""}};
  test_words = {"aaaaabbb", "", "aaaaaabbbbbb", "aabbb", "aaaaabbbbbbb", "bbbbababbbabbbba"};
  result = {true, true, true, false, false, false};
  Test(productions, test_words, result);
  
  return 0;
}