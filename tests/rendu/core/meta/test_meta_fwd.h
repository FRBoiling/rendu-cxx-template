/*
* Created by boil on 2023/3/1.
*/

#ifndef TEST_RENDU_CORE_META_TEST_META_FWD_H_
#define TEST_RENDU_CORE_META_TEST_META_FWD_H_

#include <core/base/type_traits.h>
#include <core/meta/resolve.h>
#include <core/meta/pointer.h>
#include <core/meta/meta.h>
#include <core/meta/resolve.h>
#include <core/meta/template.h>

template<typename Type>
struct wrapped_shared_ptr {
  wrapped_shared_ptr(Type init)
      : ptr{new Type{init}} {}

  Type &deref() const {
    return *ptr;
  }

 private:
  std::shared_ptr<Type> ptr;
};

struct self_ptr {
  using element_type = self_ptr;

  self_ptr(int v)
      : value{v} {}

  const self_ptr &operator*() const {
    return *this;
  }

  int value;
};

struct proxy_ptr {
  using element_type = proxy_ptr;

  proxy_ptr(int &v)
      : value{&v} {}

  proxy_ptr operator*() const {
    return *this;
  }

  int *value;
};

template<typename Type>
struct adl_wrapped_shared_ptr : wrapped_shared_ptr<Type> {};

template<typename Type>
struct spec_wrapped_shared_ptr : wrapped_shared_ptr<Type> {};

template<typename Type>
struct rendu::is_meta_pointer_like<adl_wrapped_shared_ptr<Type>> : std::true_type {};

template<typename Type>
struct rendu::is_meta_pointer_like<spec_wrapped_shared_ptr<Type>> : std::true_type {};

template<>
struct rendu::is_meta_pointer_like<self_ptr> : std::true_type {};

template<>
struct rendu::is_meta_pointer_like<proxy_ptr> : std::true_type {};

template<typename Type>
struct rendu::adl_meta_pointer_like<spec_wrapped_shared_ptr<Type>> {
  static decltype(auto) dereference(const spec_wrapped_shared_ptr<Type> &ptr) {
    return ptr.deref();
  }
};

template<typename Type>
Type &dereference_meta_pointer_like(const adl_wrapped_shared_ptr<Type> &ptr) {
  return ptr.deref();
}

template<typename>
struct function_type;

template<typename Ret, typename... Args>
struct function_type<Ret(Args...)> {};

template<typename Ret, typename... Args>
struct rendu::meta_template_traits<function_type<Ret(Args...)>> {
  using class_type = meta_class_template_tag<function_type>;
  using args_type = type_list<Ret, Args...>;
};

#endif //TEST_RENDU_CORE_META_TEST_META_FWD_H_
