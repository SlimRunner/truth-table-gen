#include "BoolFunction.hpp"

#include <array>
#include <cstdarg> // portable size_t
#include <iomanip>
#include <sstream>
#include <string_view>

void BoolFunction::clearBits() {
  for (auto &&bit : mIns) {
    bit.makeFalse();
  }
}

BoolFunction::BoolFunction(
  size_t inputs, BooleanFunctor output
) : mIns{}, mOuts{}, mflipMSB{false} {
  for (size_t i = 0; i < inputs; ++i) {
    Bit bit(false);
    mIns.push_back(bit);
  }
  mOuts.push_back({output});
}

BoolFunction::BoolFunction(
  size_t inputs, std::vector<BooleanFunctor> outputs
) : mIns{}, mOuts{}, mflipMSB{false} {
  for (size_t i = 0; i < inputs; ++i) {
    Bit bit(false);
    mIns.push_back(bit);
  }
  for (auto && bit: outputs) {
    mOuts.push_back({bit});
  }
}

size_t BoolFunction::getInputSize() const {
  return mIns.size();
}

size_t BoolFunction::getOutputSize() const {
  return mOuts.size();
}

bool BoolFunction::invertMSB() {
  mflipMSB = !mflipMSB;
  return mflipMSB;
}

LLVec BoolFunction::getOutputList() {
  LLVec result;
  const size_t IN_COUNT = mIns.size();
  const size_t B_MAX = 1U << IN_COUNT;
  size_t offset;

  for (size_t i = 0U; i < B_MAX; ++i) {
    offset = i;
    for (auto &&bit : mIns) {
      bit.set(offset & 1U);
      offset >>= 1U;
    }

    result.push_back(0ULL);

    offset = 0;
    for (auto &&bit : mOuts) {
      result.back() |= static_cast<ULL>(bit.get(mIns)) << offset++;
    }
  }

  return result;
}

StringTable BoolFunction::getTruthTable() {
  StringTable result;
  const size_t IN_COUNT = mIns.size();
  const size_t B_MAX = 1U << IN_COUNT;
  size_t offset, mask;

  switch (mflipMSB) {
  case true:
    mask = B_MAX >> 1;
    break;
  case false:
    mask = 0x1;
    break;
  }

  auto parseBit = [](bool x) { return x ? "1" : "0"; };

  for (size_t i = 0U; i < B_MAX; ++i) {
    result.push_back({});
    offset = i;

    for (auto &&bit : mIns) {
      bit.set(offset & mask);
      result.back().push_back(parseBit(bit.get()));
      switch (mflipMSB) {
      case true:
        offset <<= 1U;
        break;
      case false:
        offset >>= 1U;
        break;
      }
    }

    result.back().push_back("");
    for (auto &&bit : mOuts) {
      result.back().push_back(parseBit(bit.get(mIns)));
    }
  }

  return result;
}

StringRow BoolFunction::getTableHeaders(TokenFunctor namer) const {
  StringRow result;

  for (size_t i = 0; i < mIns.size(); ++i) {
    result.push_back(namer(i, TokenType::INPUT));
  }

  result.push_back("");

  for (size_t i = 0; i < mOuts.size(); ++i) {
    result.push_back(namer(i, TokenType::OUTPUT));
  }

  return result;
}

std::string BoolFunction::getLogisimTT(
  const StringRow & header, const StringTable & table
) {
  using ssvec = std::vector<std::streamsize>;
  std::stringstream result;
  ssvec colSizes;
  ssvec::const_iterator colSizeIt;

  result << std::left;

  for (auto const & cell: header) {
    if (cell.size() > 0) {
      colSizes.push_back(static_cast<int>(cell.size() + 1U));
      result << std::setw(colSizes.back()) << cell;
    } else {
      colSizes.push_back(2);
      result << std::setw(colSizes.back()) << "| ";
    }
  }

  const auto SEPARATOR_LEN = result.str().length();
  result << "\n" << std::string(SEPARATOR_LEN, '~') << "\n";

  const auto colSizeStart = colSizes.cbegin();

  for (auto const & row: table) {
    colSizeIt = colSizeStart;
    for (auto const & cell: row) {
      if (cell.size() > 0) {
        result << std::setw(*(colSizeIt++)) << cell;
      } else {
        result << std::setw(*(colSizeIt++)) << "|";
      }
    }
    result << "\n";
  }

  return result.str();
}

std::string BoolFunction::getCSVTT(
  const StringRow & header, const StringTable & table
) {
  std::stringstream result;
  bool needComma = false;

  needComma = false;
  for (auto const & cell: header) {
    result << (needComma ? "," : "") << cell;
    needComma = true;
  }
  result << "\n";

  needComma = false;
  for (auto const & row: table) {
    needComma = false;
    for (auto const & cell: row) {
      result << (needComma ? "," : "") << cell;
      needComma = true;
    }
    result << "\n";
  }

  return result.str();
}

MintermVec BoolFunction::getMinterms(const LLVec &terms, size_t inputSize, size_t nthOut) {
  MintermVec result(inputSize);
  for (size_t i = 0; i < terms.size(); ++i) {
    if (auto maxterm = terms[i] & (1U << nthOut); maxterm) {
      result.push_back(i * maxterm);
    }
  }
  return result;
}

MaxtermVec BoolFunction::getMaxterms(const LLVec &terms, size_t inputSize, size_t nthOut) {
  MaxtermVec result(inputSize);
  for (size_t i = 0; i < terms.size(); ++i) {
    if (auto maxterm = ~terms[i] & (1U << nthOut); maxterm) {
      result.push_back(i * maxterm);
    }
  }
  return result;
}

std::string BoolFunction::getDNF(MintermVec minterms, TokenFunctor namer) {
  std::stringstream result;
  std::vector<std::vector<std::string>> cheapAST;
  for (const auto & minterm: minterms) {
    cheapAST.push_back({});
    for (size_t i = 0; i < minterms.rank(); ++i) {
      cheapAST.back().push_back(namer(i, TokenType::INPUT));
      if (((minterm >> i) & 0x1) == 0) {
        cheapAST.back().back() = "~" + cheapAST.back().back();
      }
    }
  }

  constexpr std::string_view AND[2] = {" & ", ""};
  constexpr std::string_view  OR[2] = {" | ", ""};

  bool firstTerm = true;
  bool firstCoeff = true;

  for (const auto & terms: cheapAST) {
    result << OR[firstTerm] << "(";
    firstCoeff = true;
    for ( const auto & coeff: terms) {
      result << AND[firstCoeff] << coeff;
      firstCoeff = false;
    }
    result << ")";
    firstTerm = false;
  }
  return result.str();
}

std::string BoolFunction::getCNF(MaxtermVec maxterms, TokenFunctor namer) {
  std::stringstream result;
  std::vector<std::vector<std::string>> cheapAST;
  for (const auto & maxterm: maxterms) {
    cheapAST.push_back({});
    for (size_t i = 0; i < maxterms.rank(); ++i) {
      cheapAST.back().push_back(namer(i, TokenType::INPUT));
      if (((maxterm >> i) & 0x1) == 1) {
        cheapAST.back().back() = "~" + cheapAST.back().back();
      }
    }
  }

  constexpr std::string_view AND[2] = {" & ", ""};
  constexpr std::string_view  OR[2] = {" | ", ""};

  bool firstTerm = true;
  bool firstCoeff = true;

  for (const auto & terms: cheapAST) {
    result << AND[firstTerm] << "(";
    firstCoeff = true;
    for ( const auto & coeff: terms) {
      result << OR[firstCoeff] << coeff;
      firstCoeff = false;
    }
    result << ")";
    firstTerm = false;
  }
  return result.str();
}


std::string BoolFunction::defaultNamer(size_t index, TokenType type) {
  constexpr std::array<char, 2> OPTS = {'X', 'Y'};
  std::stringstream token;
  char letter = OPTS.at(static_cast<size_t>(type));
  token << letter << index;
  return token.str();
}