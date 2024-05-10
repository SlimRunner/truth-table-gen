#include "BoolFunction.hpp"

#include <array>
#include <cstdarg> // portable size_t
#include <iomanip>
#include <sstream>

std::string defaultNames(size_t index, TokenType type) {
  constexpr std::array<char, 2> OPTS = {'X', 'Y'};
  std::stringstream token;
  char letter = OPTS.at(static_cast<size_t>(type));
  token << letter << index;
  return token.str();
}

void BoolFunction::clearBits() {
  for (auto &&bit : mIns) {
    bit.makeFalse();
  }
}

BoolFunction::BoolFunction(
  size_t inputs, BooleanFunctor output
) : mIns{}, mOuts{} {
  for (size_t i = 0; i < inputs; ++i) {
    Bit bit(false);
    mIns.push_back(bit);
  }
  mOuts.push_back({output});
}

BoolFunction::BoolFunction(
  size_t inputs, std::vector<BooleanFunctor> outputs
) : mIns{}, mOuts{} {
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
  size_t offset;

  auto parseBit = [](bool x) { return x ? "1" : "0"; };

  for (size_t i = 0U; i < B_MAX; ++i) {
    result.push_back({});
    offset = i;
    for (auto &&bit : mIns) {
      bit.set(offset & 1U);
      result.back().push_back(parseBit(bit.get()));
      offset >>= 1U;
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

  result << "\n" << std::string(result.str().length(),'~') << "\n";

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

LLVec BoolFunction::getMinterms(const LLVec &terms, size_t nthOut) {
  LLVec result;
  for (size_t i = 0; i < terms.size(); ++i) {
    if (auto maxterm = terms[i] & (1U << nthOut); maxterm) {
      result.push_back(i * maxterm);
    }
  }
  return result;
}

LLVec BoolFunction::getMaxterms(const LLVec &terms, size_t nthOut) {
  LLVec result;
  for (size_t i = 0; i < terms.size(); ++i) {
    if (auto maxterm = ~terms[i] & (1U << nthOut); maxterm) {
      result.push_back(i * maxterm);
    }
  }
  return result;
}
