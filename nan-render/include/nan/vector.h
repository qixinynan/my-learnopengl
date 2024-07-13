
#include <array>
#include <cstddef>

template <typename T, std::size_t N> class Vector {
public:
  Vector() {
    data.fill(T{}); // 默认初始化为类型 T 的默认值
  }

  template <typename... Args>
  Vector(Args... args) : data{{static_cast<T>(args)...}} {
    static_assert(sizeof...(args) == N,
                  "Number of arguments must match the size of the vector.");
  }

  T &operator[](std::size_t index) {
    if (index >= N) {
      throw std::out_of_range("Index out of range");
    }
    return data[index];
  }

  const T &operator[](std::size_t index) const {
    if (index >= N) {
      throw std::out_of_range("Index out of range");
    }
    return data[index];
  }

  T x() const { return data[0]; }
  T y() const { return data[1]; }
  T z() const { return data[2]; }
  T w() const { return data[3]; }

private:
  std::array<T, N> data;
};