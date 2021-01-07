#ifndef PRACTICE_1__REGEXPHANDLER_H_
#define PRACTICE_1__REGEXPHANDLER_H_
#include <string>
#include <stack>
#include "SubWords.h"

struct RegExpHandler {
  RegExpHandler(std::string& regexp, std::string& word);
  std::string alpha;
  std::string u;

  SubWords HandleRegExp() const;
  unsigned long SuffixLength() const;
};

#endif
