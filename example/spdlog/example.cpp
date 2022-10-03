/*
* Created by boil on 2022/9/12.
*/
#include "example_spdlog.h"
#include "example_custom.h"
#include "log.h"
#include "log_trace.h"

int main(int, char *[]) {


  //  spdlog_example();
//  custom_example();
//  spdlog::set_pattern("%+");
  RD_INIT("custom",spdlog::level::trace);
  TLOG("---------{}---------",spdlog::level::debug);
  RD_TRACE("---------1---------");
  RD_DEBUG("---------2---------");
  RD_WARN("---------3---------");
  RD_INFO("---------4---------");
  RD_ERROR("---------5---------");
  RD_CRITICAL("---------6---------");
  TLOG("---------tlog---2------");
}


