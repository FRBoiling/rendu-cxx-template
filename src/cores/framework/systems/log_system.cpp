/*
* Created by boil on 2022/8/28.
*/

#include "log_system.h"

void LogSystem::Register() {

}

void LogSystem::Destroy() {

}

const std::type_info &LogSystem::GetType() {
  return typeid(this);
}
