#ifndef BOOL_FUNCTION_H
#define BOOL_FUNCTION_H

#include "Bit.hpp"

#include <cstddef>     // portable size_t
#include <functional>  // function
#include <type_traits> // false_type, true_type
#include <map>
#include <vector>

enum class TokenType { INPUT, OUTPUT };

// allows custom initialization for input or output names
using TokenFunctor = std::function<std::string(size_t index, TokenType type)>;
using BooleanParams = const std::vector<Bit> &;
using BooleanFunctor = std::function<bool(BooleanParams bits)>;

class EvalBit {
private:
  std::string mToken;
  BooleanFunctor mFunc;
public:
  EvalBit() = delete;
  EvalBit(std::string name, BooleanFunctor func) : mToken{name}, mFunc{func} {}
  inline bool get(BooleanParams bits) const { return mFunc(bits); }
  inline std::string name() const { return mToken; }
};

using BitVector = std::vector<Bit>;
using FuncVector = std::vector<EvalBit>;


class BoolFunction {
private:
  BitVector mIns;
  FuncVector mOuts;
  std::map<std::string ,size_t> mWidth;

  void clearBits();

public:
  BoolFunction() = delete;
  BoolFunction(size_t inputs, BooleanFunctor output);
  BoolFunction(size_t inputs, TokenFunctor namer, BooleanFunctor output);
  BoolFunction(size_t inputs, std::vector<BooleanFunctor> outputs);
  BoolFunction(size_t inputs, TokenFunctor namer, std::vector<BooleanFunctor> outputs);
  
  std::string getTruthTable();

  std::string toString() {
    return std::string("asd");
  }
};

#endif // BOOL_FUNCTION_H
