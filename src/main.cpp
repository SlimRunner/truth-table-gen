#include "BoolFunction.hpp"

#include <array>
#include <iostream>
#include <sstream>

int main() {
  BooleanFunctor out1 = [](BitVector bits){
    int count = 0;
    for (auto const & bit: bits) {
      count += bit.get();
    }
    return count <= 1;
  };
  TokenFunctor makeNames = [](size_t index, TokenType type){
    std::stringstream ss;
    switch (type) {
    case TokenType::INPUT:
      ss << "X" << (index / 3) + 1 << (index % 3) + 1;
      break;
    case TokenType::OUTPUT:
      ss << "O" << index;
      break;
    
    default:
      ss << "W";
      break;
    }
    return ss.str();
  };
  BoolFunction bf(9, makeNames, out1);

  std::cout << bf.getTruthTable();
}