#include "BoolFunction.hpp"
#include "Grid.hpp"

#include <array>
#include <iostream>
#include <sstream>

int main() {
  BooleanFunctor out1 = [](BitVector bits){
    Grid<int, int> grid(3, 3);
    for (size_t i = 0; i < bits.size(); ++i) {
      grid.set(bits.at(i).get(), {static_cast<int>(i) % 3, static_cast<int>(i) / 3});
    }

    return (
      grid.sumSequence({0,0}, {1,0}) <= 1 && 
      grid.sumSequence({0,1}, {1,0}) <= 1 && 
      grid.sumSequence({0,2}, {1,0}) <= 1 && 

      grid.sumSequence({0,0}, {0,1}) <= 1 && 
      grid.sumSequence({1,0}, {0,1}) <= 1 && 
      grid.sumSequence({2,0}, {0,1}) <= 1 && 

      grid.sumSequence({0,0}, {1,1}) <= 1 && 
      grid.sumSequence({1,0}, {1,1}) <= 1 && 
      grid.sumSequence({0,1}, {1,1}) <= 1 && 

      grid.sumSequence({2,0}, {-1,1}) <= 1 && 
      grid.sumSequence({1,0}, {-1,1}) <= 1 && 
      grid.sumSequence({2,1}, {-1,1}) <= 1
    );
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