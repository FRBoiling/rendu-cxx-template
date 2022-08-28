/*
* Created by boil on 2022/8/28.
*/

#ifndef RENDU_SYSTEM_H_
#define RENDU_SYSTEM_H_

#include <typeinfo>
#include <string>

class ISystem {
public:
  virtual void Register() = 0;

  virtual void Destroy() = 0;

  virtual std::string ToString() {
    return GetType().name() ;
  }

  virtual const std::type_info &GetType() {
    return typeid(this);
  };
};

template<typename T>
class System {
public:
  static T &GetInstance() {
    static T instance;
    return instance;
  }

  System(T &&) = delete;

  System(const T &) = delete;

  void operator=(const T &) = delete;

protected:
  System() = default;

  virtual ~System() = default;
};

#endif //RENDU_SYSTEM_H_
