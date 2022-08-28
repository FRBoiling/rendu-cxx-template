/*
* Created by boil on 2022/8/27.
*/

#include "program.h"
#include "log_system.h"
#include "config_system.h"
#include "event_system.h"

/// Launch the rendu core program
extern int main(int argc, char** argv) {

  Program::GetInstance().Initialize(argc, argv);
  Program::GetInstance().AddSystem(LogSystem::GetInstance());
  Program::GetInstance().AddSystem(ConfigSystem::GetInstance());
  Program::GetInstance().AddSystem(EventSystem::GetInstance());

  Program::GetInstance().Run();

  return 0;
}