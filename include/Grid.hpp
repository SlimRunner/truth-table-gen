#ifndef GRID_H
#define GRID_H

#include "Vec2.hpp"

#include <functional>
#include <vector>

template<class S, class T>
using Predicate = std::function<S(T x, T y)>;

template<class S, class T>
class Grid {
private:
  std::vector<std::vector<S>> mCells;
public:
  Grid();
  Grid(T rows, T cols);
  Grid(T rows, T cols, S value);
  Grid(T rows, T cols, Predicate<S, T> pred);

  bool set(S value, Vec2<T> pos);

  bool get(Vec2<T> pos, S & out) const;
  S sumSequence(Vec2<T> start, Vec2<T> delta) const;
};

template <class S, class T>
Grid<S, T>::Grid() : mCells{} { }

template <class S, class T> inline Grid<S, T>::Grid(T rows, T cols) : Grid{rows, cols, 0} {}

template <class S, class T> inline Grid<S, T>::Grid(T rows, T cols, S value) {
  for (T y = T{}; y < rows; ++y) {
    mCells.push_back({});
    for (T x = T{}; x < cols; ++x) {
      mCells.back().push_back(value);
    }
  }
}

template <class S, class T>
inline Grid<S, T>::Grid(T rows, T cols, Predicate<S, T> pred) {
  for (T y = T{}; y < rows; ++y) {
    mCells.push_back({});
    for (T x = T{}; x < cols; ++x) {
      mCells.back().push_back(pred(x, y));
    }
  }
}

template <class S, class T>
bool Grid<S, T>::set(S value, Vec2<T> pos) {
  if (
    mCells.size() > static_cast<T>(0) &&
    static_cast<T>(0) <= pos.y && pos.y < static_cast<T>(mCells.size()) &&
    static_cast<T>(0) <= pos.x && pos.x < static_cast<T>(mCells.front().size())
  ) {
    mCells.at(static_cast<size_t>(pos.x)).at(static_cast<size_t>(pos.y)) = value;
    return true;
  }
  return false;
}

template <class S, class T>
bool Grid<S, T>::get(Vec2<T> pos, S & out) const {
  // let vector take care of the exceptions
  if (
    mCells.size() > static_cast<T>(0) &&
    static_cast<T>(0) <= pos.y && pos.y < static_cast<T>(mCells.size()) &&
    static_cast<T>(0) <= pos.x && pos.x < static_cast<T>(mCells.front().size())
  ) {
    out = mCells.at(static_cast<size_t>(pos.x)).at(static_cast<size_t>(pos.y));
    return true;
  }
  return false;
}

template <class S, class T>
inline S Grid<S, T>::sumSequence(Vec2<T> start, Vec2<T> delta) const {
  S result = S{};
  S here;

  for (Vec2<T> pos = start; get(pos, here); pos += delta) {
    result += here;
  }

  return result;
}

#endif // GRID_H


