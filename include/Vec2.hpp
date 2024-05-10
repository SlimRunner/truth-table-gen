#ifndef VEC2_H
#define VEC2_H

template<class T>
struct Vec2 {
  T x;
  T y;

  Vec2() : x{}, y{} {}
  Vec2(T xv, T yv) : x{xv}, y{yv} {}

  Vec2<T> operator+(Vec2<T> rhs) const {
    return {this->x + rhs.x, this->y + rhs.y};
  }

  Vec2<T> & operator+=(const Vec2<T> & rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
  }
};

#endif // VEC2_H
