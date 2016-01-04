#ifndef UTIL_AUTO_PTR_H_
#define UTIL_AUTO_PTR_H_

template<class T>
class auto_ptr
{
 public:
  explicit auto_ptr(T* p = 0) : ptr_(p) {
  }
  ~auto_ptr() {
    if (0 != ptr_) {
      delete ptr_;
    }
  }
  auto_ptr(auto_ptr& ap): ptr_(ap.release()) {
  }
  auto_ptr& operator=(auto_ptr& ap) {
    if (&ap != this) {
      delete ptr_;
      ptr_ = ap.release();
    }
    return *this;
  }
  T& operator*() const {
    return *ptr_;
  }
  T* operator->() const {
    return ptr_;
  }
  T* release() {
    T* tmp = ptr_;
    ptr_ = 0;
    return tmp;
  }
  void reset(T* p = 0) {
    if (p != ptr_) {
      delete ptr_;
      ptr_ = p;
    }
  }
 private:
  T* ptr_;
};
#endif  //  UTIL_AUTO_PTR_H_
