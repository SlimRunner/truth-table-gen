#ifndef BOOL_FUNCTION_H
#define BOOL_FUNCTION_H

#include "Bit.hpp"
#include "EvalBit.hpp"

#include <cstddef>     // portable size_t
#include <functional>  // function
#include <string>
#include <vector>

enum class TokenType { INPUT, OUTPUT };

// allows custom initialization for input or output names
using TokenFunctor = std::function<std::string(size_t index, TokenType type)>;
using StringRow = std::vector<std::string>;
using StringTable = std::vector<StringRow>;
using BitVector = std::vector<Bit>;
using FuncVector = std::vector<EvalBit>;

class BoolFunction {
private:
  BitVector mIns;
  FuncVector mOuts;

  void clearBits();

public:
  BoolFunction() = delete;
  BoolFunction(size_t inputs, BooleanFunctor output);
  BoolFunction(size_t inputs, std::vector<BooleanFunctor> outputs);

  StringTable getTruthTable();
  StringRow getTableHeaders(TokenFunctor namer);

  static std::string getLogisimTT(const StringRow & header, const StringTable & table);
  static std::string getCSVTT(const StringRow & header, const StringTable & table);
};

#endif // BOOL_FUNCTION_H
