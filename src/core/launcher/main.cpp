/*
* Created by boil on 2022/8/27.
*/

#include "program.h"

#include "log_system.h"
#include "config_system.h"
#include "event_system.h"

using namespace rendu;
/// Launch the rendu core program
extern int main(int argc, char **argv) {
  if (sOptions.Initialize(argc, argv) == 0) {
    return 0;
  }
  sProgram.Initialize();

  sProgram.AddSystem(LogSystem::GetInstance());
  sProgram.AddSystem(ConfigSystem::GetInstance());
  sProgram.AddSystem(EventSystem::GetInstance());
  sProgram.Run();
  return 0;
}