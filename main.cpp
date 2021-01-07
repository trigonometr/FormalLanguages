//    14. Даны \alpha и слово u \in {a, b, c} * . Найти длину самого длинного суффикса u,
//    принадлежащего L.
#include <iostream>
#include <fstream>
#include <cassert>
#include "RegExpHandler.h"

void RunTests(const char* file_name) {
  std::string regexp;
  std::string word;
  unsigned long result;
  std::ifstream test_file(file_name);

  assert(test_file.is_open());
  while (!test_file.eof()) {
    test_file >> regexp;
    test_file >> word;
    test_file >> result;

    RegExpHandler helper(regexp, word);
    assert(helper.SuffixLength() == result);
  }
  test_file.close();
  std::cout << "Tests completed successfully\n";
}

int main() {
  RunTests("/tests.txt");

  std::string alpha;
  std::string u;
  std::cin >> alpha >> u;
  RegExpHandler helper(alpha, u);
  std::cout << helper.SuffixLength();

  return 0;
}
