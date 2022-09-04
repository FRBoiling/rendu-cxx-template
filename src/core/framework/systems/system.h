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

#endif //RENDU_SYSTEM_H_
