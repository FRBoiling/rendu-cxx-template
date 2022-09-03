/*
* Created by boil on 2022/8/20.
*/

#include "built_in_config.h"
#include "config.h"
#include "git_revision.h"

template<typename Fn>
static std::string GetStringWithDefaultValueFromFunction(
    std::string const &key, Fn getter) {
  std::string const value = sConfigMgr->GetStringDefault(key, "");
  return value.empty() ? getter() : value;
}

std::string rendu::GetCMakeCommand() {
  return GetStringWithDefaultValueFromFunction(
      "CMakeCommand", GitRevision::GetCMakeCommand);
}

std::string rendu::GetBuildDirectory() {
  return GetStringWithDefaultValueFromFunction(
      "BuildDirectory", GitRevision::GetBuildDirectory);
}

std::string rendu::GetSourceDirectory() {
  return GetStringWithDefaultValueFromFunction(
      "SourceDirectory", GitRevision::GetSourceDirectory);
}

std::string rendu::GetMySQLExecutable() {
  return GetStringWithDefaultValueFromFunction(
      "MySQLExecutable", GitRevision::GetMySQLExecutable);
}
