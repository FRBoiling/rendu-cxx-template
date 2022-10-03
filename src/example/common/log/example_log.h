/*
* Created by boil on 2022/9/24.
*/

#pragma once

#include "example_log.h"

#include "log.h"
#include "enum.h"
using namespace rendu;

void example_log(){
  RD_INIT("custom",RunMode::Develop,"logs/");
  RD_TRACE("------{1}-{0}-{0}------", enum_name(spdlog::level::trace),enum_name(spdlog::level::debug),enum_name
  (spdlog::level::info));

  RD_TRACE("---------1---------");
  RD_DEBUG("---------2---------");
  RD_WARN("---------3---------");
  RD_INFO("---------4---------");
  RD_ERROR("---------5---------");
  RD_CRITICAL("---------6---------");
}

