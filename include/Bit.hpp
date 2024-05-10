#ifndef BIT_H
#define BIT_H

#include <string>


class Bit {
private:
  std::string mToken;
  bool mBit;

public:
  Bit() = delete;
  Bit(std::string name, bool value) : mToken{name}, mBit{value} {}

  inline void flip() { mBit = !mBit; }
  inline void makeFalse() { mBit = false; }
  inline void set(bool value) { mBit = value; }
  inline void rename(std::string name) { mToken = name; }

  inline bool get() const { return mBit; }
  inline std::string name() const { return mToken; }

};

#endif // BIT_H
