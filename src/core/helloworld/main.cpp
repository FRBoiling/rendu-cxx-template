/*
* Created by boil on 2022/8/27.
*/

#include "program.h"
#include "log_system.h"
#include "config_system.h"
#include "event_system.h"

/// Launch the rendu core program
extern int main(int argc, char** argv) {

  Program::Initialize(argc, argv);
  Program::AddSystem(LogSystem::GetInstance());
  Program::AddSystem(ConfigSystem::GetInstance());
  Program::AddSystem(EventSystem::GetInstance());
  Program::Run();

  return 0;
}