/*
* Created by boil on 2022/8/28.
*/

#include "config_system.h"

void ConfigSystem::Register() {

}

void ConfigSystem::Destroy() {

}

const std::type_info &ConfigSystem::GetType() {
  return typeid(this);
}
