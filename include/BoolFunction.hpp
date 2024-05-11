#ifndef BOOL_FUNCTION_H
#define BOOL_FUNCTION_H

#include "Bit.hpp"
#include "EvalBit.hpp"

#include <cstddef>    // portable size_t
#include <functional> // function
#include <string>
#include <vector>

enum class TokenType { INPUT, OUTPUT };

// allows custom initialization for input or output names
using TokenFunctor = std::function<std::string(size_t index, TokenType type)>;
using StringRow = std::vector<std::string>;
using StringTable = std::vector<StringRow>;
using BitVector = std::vector<Bit>;
using FuncVector = std::vector<EvalBit>;
using ULL = unsigned long long;
using LLVec = std::vector<ULL>;

struct MintermTag {};
struct MaxtermTag {};

template <class Tag>
class TaggedTerms: public std::vector<ULL> {
private:
  size_t mRank;
public:
  TaggedTerms(size_t rank) : mRank {rank} {}
  inline size_t rank() { return mRank; }
};

using MintermVec = TaggedTerms<MintermTag>;
using MaxtermVec = TaggedTerms<MaxtermTag>;

class BoolFunction {
private:
  BitVector mIns;
  FuncVector mOuts;

  void clearBits();

public:
  BoolFunction() = delete;
  BoolFunction(size_t inputs, BooleanFunctor output);
  BoolFunction(size_t inputs, std::vector<BooleanFunctor> outputs);

  size_t getInputSize() const;
  size_t getOutputSize() const;

  LLVec getOutputList();
  StringTable getTruthTable();
  StringRow getTableHeaders(TokenFunctor namer) const;

  static std::string getLogisimTT(const StringRow & header, const StringTable & table);
  static std::string getCSVTT(const StringRow & header, const StringTable & table);
  static MintermVec getMinterms(const LLVec &terms, size_t inputSize, size_t nthOut);
  static MaxtermVec getMaxterms(const LLVec &terms, size_t inputSize, size_t nthOut);
  static std::string getDNF(MintermVec minterms, TokenFunctor namer);
  static std::string getCNF(MaxtermVec maxterms, TokenFunctor namer);

  static std::string defaultNamer(size_t index, TokenType type);
};

#endif // BOOL_FUNCTION_H
