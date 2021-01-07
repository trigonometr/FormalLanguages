//    14. Даны \alpha и слово u \in {a, b, c} * . Найти длину самого длинного суффикса u,
//    принадлежащего L.
#include <iostream>
#include "RegExpHandler.h"

int main() {
  std::string alpha;
  std::string u;
  std::cin >> alpha >> u;

  RegExpHandler helper(alpha, u);
  std::cout << helper.SuffixLength();

  return 0;
}
