#ifndef BIT_H
#define BIT_H

class Bit {
private:
  bool mBit;

public:
  Bit() = delete;
  Bit(bool value) : mBit{value} {}

  inline void flip() { mBit = !mBit; }
  inline void makeFalse() { mBit = false; }
  inline void set(bool value) { mBit = value; }

  inline bool get() const { return mBit; }
};

#endif // BIT_H
