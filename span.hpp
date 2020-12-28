
// ========================================================================== //

#ifndef INCLUDE_SPAN_HPP
#define INCLUDE_SPAN_HPP

// -------------------------------------------------------------------------- //

#include <cstddef>
#include <initializer_list>
#include <iterator>

// -------------------------------------------------------------------------- //

template<typename T>
class CSpanT {

  public:

  using CValue = T;

  using CIterator = T *;
  using CKonstIterator = T const *;
  using CReverseIterator = std::reverse_iterator<CIterator>;
  using CKonstReverseIterator = std::reverse_iterator<CKonstIterator>;

  CSpanT() = default;

  CSpanT(T * data, size_t size) :
    mData { size > 0 ? data : nullptr },
    mSize { data != nullptr ? size : 0 }
  { }

  CSpanT(std::initializer_list<T> list) :
    mData { std::data(list) },
    mSize { std::size(list) },
  { }

  inline bool empty() const { return (mSize == 0); }
  inline size_t size() const { return mSize; }

  inline T * data() { return mData; }
  inline T const * data() const { return mData; }

  inline T & at(size_t n) { return mData[n]; }
  inline T & operator[](size_t n) { return mData[n]; }
  inline T const & at(size_t n) const { return mData[n]; }
  inline T const & operator[](size_t n) const { return mData[n]; }

  inline CIterator begin() { return mData; }
  inline CKonstIterator begin() const { return mData; }
  inline CKonstIterator cbegin() const { return mData; }

  inline CIterator end() { return (mData + mSize); }
  inline CKonstIterator end() const { return (mData + mSize); }
  inline CKonstIterator cend() const { return (mData + mSize); }

  inline CReverseIterator rbegin() {
    return std::make_reverse_iterator(end());
  }

  inline CKonstReverseIterator rbegin() const {
    return std::make_reverse_iterator(end());
  }

  inline CKonstReverseIterator crbegin() const {
    return std::make_reverse_iterator(end());
  }

  inline CReverseIterator rend() {
    return std::make_reverse_iterator(begin());
  }

  inline CKonstReverseIterator rend() const {
    return std::make_reverse_iterator(begin());
  }

  inline CKonstReverseIterator crend() const {
    return std::make_reverse_iterator(begin());
  }

  private:

  T * mData { nullptr };
  size_t mSize { 0 };

};

// -------------------------------------------------------------------------- //

// ========================================================================== //

#endif
