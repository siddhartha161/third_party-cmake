/*============================================================================
  CMake - Cross Platform Makefile Generator
  Copyright 2000-2016 Kitware, Inc.

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifndef CM_AUTO_PTR_HXX
#define CM_AUTO_PTR_HXX

#include <cmsys/Configure.hxx>

#if __cplusplus < 201103L
#include <memory>
#define CM_AUTO_PTR std::auto_ptr
#else
#define CM_AUTO_PTR cm::auto_ptr

// In C++11, clang will warn about using dynamic exception specifications
// as they are deprecated.  But as this class is trying to faithfully
// mimic std::auto_ptr, we want to keep the 'throw()' decorations below.
// So we suppress the warning.
#if defined(__clang__) && defined(__has_warning)
#if __has_warning("-Wdeprecated")
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"
#endif
#endif

namespace cm {

/** C++98 Standard Section 20.4.5 - Template class auto_ptr.  */
template <class X>
class auto_ptr
{
  template <class Y>
  struct auto_ptr_ref
  {
    Y* p_;

    explicit auto_ptr_ref(Y* p)
      : p_(p)
    {
    }
  };

  /** The pointer to the object held.  */
  X* x_;

public:
  /** The type of object held by the auto_ptr.  */
  typedef X element_type;

  /** Construct from an auto_ptr holding a compatible object.  This
      transfers ownership to the newly constructed auto_ptr.  */
  template <class Y>
  auto_ptr(auto_ptr<Y>& a) throw()
    : x_(a.release())
  {
  }

  /** Assign from an auto_ptr holding a compatible object.  This
      transfers ownership to the left-hand-side of the assignment.  */
  template <class Y>
  auto_ptr& operator=(auto_ptr<Y>& a) throw()
  {
    this->reset(a.release());
    return *this;
  }

  /**
   * Explicitly construct from a raw pointer.  This is typically
   * called with the result of operator new.  For example:
   *
   *   auto_ptr<X> ptr(new X());
   */
  explicit auto_ptr(X* p = 0) throw()
    : x_(p)
  {
  }

  /** Construct from another auto_ptr holding an object of the same
      type.  This transfers ownership to the newly constructed
      auto_ptr.  */
  auto_ptr(auto_ptr& a) throw()
    : x_(a.release())
  {
  }

  /** Assign from another auto_ptr holding an object of the same type.
      This transfers ownership to the newly constructed auto_ptr.  */
  auto_ptr& operator=(auto_ptr& a) throw()
  {
    this->reset(a.release());
    return *this;
  }

  /** Destruct and delete the object held.  */
  ~auto_ptr() throw()
  {
    // Assume object destructor is nothrow.
    delete this->x_;
  }

  /** Dereference and return a reference to the object held.  */
  X& operator*() const throw() { return *this->x_; }

  /** Return a pointer to the object held.  */
  X* operator->() const throw() { return this->x_; }

  /** Return a pointer to the object held.  */
  X* get() const throw() { return this->x_; }

  /** Return a pointer to the object held and reset to hold no object.
      This transfers ownership to the caller.  */
  X* release() throw()
  {
    X* x = this->x_;
    this->x_ = 0;
    return x;
  }

  /** Assume ownership of the given object.  The object previously
      held is deleted.  */
  void reset(X* p = 0) throw()
  {
    if (this->x_ != p) {
      // Assume object destructor is nothrow.
      delete this->x_;
      this->x_ = p;
    }
  }

  /** Convert to an auto_ptr holding an object of a compatible type.
      This transfers ownership to the returned auto_ptr.  */
  template <class Y>
  operator auto_ptr<Y>() throw()
  {
    return auto_ptr<Y>(this->release());
  }

  /** Construct from an auto_ptr_ref.  This is used when the
      constructor argument is a call to a function returning an
      auto_ptr.  */
  auto_ptr(auto_ptr_ref<X> r) throw()
    : x_(r.p_)
  {
  }

  /** Assign from an auto_ptr_ref.  This is used when a function
      returning an auto_ptr is passed on the right-hand-side of an
      assignment.  */
  auto_ptr& operator=(auto_ptr_ref<X> r) throw()
  {
    this->reset(r.p_);
    return *this;
  }

  /** Convert to an auto_ptr_ref.  This is used when a function
      returning an auto_ptr is the argument to the constructor of
      another auto_ptr.  */
  template <class Y>
  operator auto_ptr_ref<Y>() throw()
  {
    return auto_ptr_ref<Y>(this->release());
  }
};

} // namespace cm

// Undo warning suppression.
#if defined(__clang__) && defined(__has_warning)
#if __has_warning("-Wdeprecated")
#pragma clang diagnostic pop
#endif
#endif

#endif

#endif
