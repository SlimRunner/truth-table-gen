#ifndef EVALBIT_H
#define EVALBIT_H

#include "Bit.hpp"

#include <functional>  // function
#include <vector>

using BooleanParams = const std::vector<Bit> &;
using BooleanFunctor = std::function<bool(BooleanParams bits)>;

class EvalBit {
private:
  BooleanFunctor mFunc;
public:
  EvalBit() = delete;
  EvalBit(BooleanFunctor func) : mFunc{func} {}
  inline bool get(BooleanParams bits) const { return mFunc(bits); }
};

#endif // EVALBIT_H
