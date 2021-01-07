#include "RegExpHandler.h"

RegExpHandler::RegExpHandler(std::string& regexp, std::string& word) : alpha(regexp), u(word) {}

SubWords RegExpHandler::HandleRegExp() const {
  std::stack<SubWords> tmp_tables;
  for(auto& symbol: alpha) {
    SubWords table1(u);
    if(symbol >= 'a' && symbol <= 'c' || (symbol == '0' || symbol == '1')) {
      table1.AddSymbol(u, symbol);
    }
    else if(symbol == '.') {
      SubWords table2 = tmp_tables.top();
      tmp_tables.pop();
      table1 = tmp_tables.top();
      tmp_tables.pop();
      table1.Concat(table2);
    }
    else if(symbol == '+') {
      SubWords table2 = tmp_tables.top();
      tmp_tables.pop();
      table1 = tmp_tables.top();
      tmp_tables.pop();
      table1.Sum(table2);
    }
    else if(symbol == '*') {
      table1 = tmp_tables.top();
      tmp_tables.pop();
      table1.KleeneStar();
    }
    tmp_tables.push(table1);
  }

  return tmp_tables.top();
}

unsigned long RegExpHandler::SuffixLength() const {
  SubWords table = HandleRegExp();
  for (int i = 0; i < u.size(); ++i) {
    if (table.Get(i, u.size())) {
      return (u.size() - i);
    }
  }
  return 0;
}