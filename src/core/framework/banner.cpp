/*
* Created by boil on 2022/8/20.
*/

#include "banner.h"
#include "git_revision.h"
#include "options.h"

void rendu::banner::Show() {
  RD_LOG_INFO("\n{}\n{}\n{}\n{}\n{}\n{}\n 进程名称：{}",
              GitRevision::GetFullVersion(),
              "*************************************************",
              "             R E N D U    C O R E                ",
              "   https://github.com/FRBoiling/rendu-core.git   ",
              "*************************************************",
              " <Ctrl-C> to stop.", sOptions.m_program_name);
}
