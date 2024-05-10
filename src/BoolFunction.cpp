#include "BoolFunction.hpp"

#include <array>
#include <cstdarg>
#include <iomanip>
#include <sstream>
#include <utility>

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

BoolFunction::BoolFunction(size_t inputs, BooleanFunctor output)
    : BoolFunction{inputs, defaultNames, output} {}

BoolFunction::BoolFunction(
  size_t inputs, TokenFunctor namer, BooleanFunctor output
) : mIns{}, mOuts{} {
  for (size_t i = 0; i < inputs; ++i) {
    Bit bit(namer(i, TokenType::INPUT), false);
    mIns.push_back(std::move(bit));
  }
  mOuts.push_back({namer(0, TokenType::OUTPUT), output});
}

BoolFunction::BoolFunction(size_t inputs, std::vector<BooleanFunctor> outputs)
    : BoolFunction{inputs, defaultNames, outputs} {}

BoolFunction::BoolFunction(
  size_t inputs, TokenFunctor namer, std::vector<BooleanFunctor> outputs
) : mIns{}, mOuts{} {
  for (size_t i = 0; i < inputs; ++i) {
    Bit bit(namer(i, TokenType::INPUT), false);
    mIns.push_back(bit);
  }
  for (auto && bit: outputs) {
    mOuts.push_back({namer(mOuts.size(), TokenType::OUTPUT), bit});
  }
}

void BoolFunction::renameTokens(TokenFunctor namer) {
  for (size_t i = 0; i < mIns.size(); ++i) {
    mIns.at(i).rename(namer(i, TokenType::INPUT));
  }
}

StringTable BoolFunction::getTruthTable() {
  StringTable result;
  const size_t IN_COUNT = mIns.size();
  const size_t B_MAX = 1U << IN_COUNT;
  size_t offset;

  auto parseBit = [](bool x) { return x ? "1" : "0"; };

  result.push_back({});
  for (const auto &bit : mIns) {
    result.back().push_back(bit.name());
  }
  result.back().push_back("");
  for (const auto &bit : mOuts) {
    result.back().push_back(bit.name());
  }

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

std::string BoolFunction::getLogisimTT() {
  const size_t IN_COUNT = mIns.size();
  const size_t B_MAX = 1U << IN_COUNT;
  std::stringstream result;
  size_t offset;

  constexpr size_t COL_SIZE = 4;
  result << std::left;
  for (const auto &bit : mIns) {
    result << std::setw(COL_SIZE);
    result << bit.name();
  }
  result << std::setw(COL_SIZE);
  result << "|";
  for (const auto &bit : mOuts) {
    result << std::setw(COL_SIZE);
    result << bit.name();
  }

  result << "\n";
  result << std::string(result.str().length(),'~');
  result << "\n";

  for (size_t i = 0U; i < B_MAX; ++i) {
    offset = i;
    for (auto &&bit : mIns) {
      bit.set(offset & 1U);
      result << std::setw(COL_SIZE);
      result << bit.get();
      offset >>= 1U;
    }
    result << std::setw(COL_SIZE);
    result << "|";
    for (auto &&bit : mOuts) {
      result << std::setw(COL_SIZE);
      result << bit.get(mIns);
    }
    result << "\n";
  }

  return result.str();
}

std::string BoolFunction::getCSVTT() {
  StringTable table = getTruthTable();
  std::stringstream result;

  bool needComma = false;
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
