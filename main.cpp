//    14. Даны \alpha и слово u \in {a, b, c} * . Найти длину самого длинного суффикса u,
//    принадлежащего L.
#include <iostream>
#include <vector>
#include <stack>

class SubWords {
 public:
  explicit SubWords(const std::string& word) : size_(word.size() + 1) {
    table.resize(size_);
    for (auto& str: table) {
      str = std::vector<bool>(size_, false);
    }
  }
  void AddSymbol(const std::string& word, char symbol) {
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
  void Concat(const SubWords&  tail) {
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
  void Sum(const SubWords& alternative) {
    auto new_table = table;
    for (int i = 0; i < size_; ++i) {
      for (int j = 0; j < size_; ++j) {
        table[i][j] = new_table[i][j] || alternative.Get(i, j);
      }
    }
  }
  void KleeneStar() {
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
  bool Get(int i, int j) const {
    return table.at(i).at(j);
  }
  void Fulfill() {
    for (auto& str: table) {
      str = std::vector<bool>(size_, false);
    }
  }
 private:
  size_t size_;
  std::vector<std::vector<bool>> table;
};

SubWords HandleRegExp(std::string& alpha, std::string& u) {
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

int main() {
  std::string alpha;
  std::string u;
  std::cin >> alpha >> u;

  SubWords final_table = HandleRegExp(alpha, u);
  for (int i = 0; i < u.size(); ++i) {
    if (final_table.Get(i, u.size())) {
      std::cout << u.size() - i << "\n";
      break;
    }
  }
  return 0;
}
