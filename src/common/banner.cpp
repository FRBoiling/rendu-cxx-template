/*
* Created by boil on 2022/8/20.
*/

#include "banner.h"
#include "git_revision.h"
#include "string_format.h"

void rendu::banner::Show(char const *applicationName, void(*log)(char const *text), void(*logExtraInfo)()) {
  log(rendu::StringFormat("%s (%s)", GitRevision::GetFullVersion(), applicationName).c_str());
  log("<Ctrl-C> to stop.\n");
  log("****************************\n");
  log("   R E N D U    C O R E     \n");
  log("   https://RenduCore.org    \n");
  log("****************************\n");
  if (logExtraInfo)
    logExtraInfo();
}
