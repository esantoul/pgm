#pragma once

#include <cstddef>

namespace utils
{
  template <typename El_t, std::size_t max_size>
  class StaticVector
  {
  public:
    constexpr StaticVector() {}
    constexpr size_t size() const { return mSize; }
    constexpr size_t capacity() const { return max_size; }

    constexpr const El_t *begin() const { return mData; }
    constexpr El_t *begin() { return mData; }
    constexpr const El_t *end() const { return mData + mSize; }
    constexpr El_t *end() { return mData + mSize; }

    constexpr El_t &operator[](std::size_t idx) { return mData[idx]; }

    constexpr void push_back(const El_t &el)
    {
      if (mSize < max_size)
        mData[mSize++] = el;
    }

    constexpr void clear() { mSize = 0; }
    constexpr void erase(size_t idx)
    {
      if (idx < mSize)
      {
        for (auto it = mData + idx; it != end() - 1; ++it)
          *it = *(it + 1);
        --mSize;
      }
    }
    constexpr void erase(El_t *iterator)
    {
      erase(iterator - begin());
    }

    constexpr El_t *data()
    {
      return mData;
    }

  private:
    size_t mSize{};
    El_t mData[max_size]{};
  };
} // namespace utils
