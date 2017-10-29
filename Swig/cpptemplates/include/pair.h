#ifndef PAIR_H_
#define PAIR_H_

namespace my_random {
template<typename T1, typename T2>
struct pair {
  T1 first;
  T2 second;
  pair() : first(T1()), second(T2()) {};
  pair(const T1 &f, const T2 &s) : first(f), second(s) {};
};
}
#endif  // PAIR_H_
