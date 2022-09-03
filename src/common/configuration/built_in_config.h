/*
* Created by boil on 2022/8/20.
*/

#ifndef RENDU_BUILT_IN_CONFIG_H_
#define RENDU_BUILT_IN_CONFIG_H_

#include "define.h"
#include <string>

/// Provides helper functions to access built-in values
/// which can be overwritten in config
namespace rendu {
  /// Returns the CMake command when any is specified in the config,
  /// returns the built-in path otherwise
  RD_COMMON_API std::string GetCMakeCommand();
  /// Returns the build directory path when any is specified in the config,
  /// returns the built-in one otherwise
  RD_COMMON_API std::string GetBuildDirectory();
  /// Returns the source directory path when any is specified in the config,
  /// returns the built-in one otherwise
  RD_COMMON_API std::string GetSourceDirectory();
  /// Returns the path to the mysql executable (`mysql`) when any is specified
  /// in the config, returns the built-in one otherwise
  RD_COMMON_API std::string GetMySQLExecutable();

} // namespace rendu

#endif // RENDU_BUILT_IN_CONFIG_H_
