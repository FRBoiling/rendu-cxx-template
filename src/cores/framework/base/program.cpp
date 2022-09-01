/*
* Created by boil on 2022/8/27.
*/
#include "program.h"

int Program::Initialize(int argc, char **argv) {
  printf("Initialize...\n");

  printf("Initialize success!\n");
  GetInstance()._state = ProgramState::INITIALIZED;
  printf("ProgramState is %d\n", GetInstance()._state);
}

bool Program::IsStopped() {
  return GetInstance()._state == ProgramState::STOPPED;
}

void Program::Exit() {
  _state = ProgramState::IDLE;
}

int Program::Run() {
  GetInstance().Start();
  while (!IsStopped()) {
  }
  printf("Exit success!\n");
}

int Program::Start() {
  printf("Start...\n");

  printf("Start success!\n");
  _state = ProgramState::RUNNING;
}

int Program::Stop() {
  printf("Stop...\n");
  GetInstance()._state = ProgramState::STOPPING;

  printf("Stop success!\n");
  GetInstance()._state = ProgramState::STOPPED;
}

int Program::Update() {

}

void Program::AddSystem(ISystem &system) {
  auto hash_code = system.GetType().hash_code();
  if (GetInstance()._systems.contains(hash_code)) {
    printf("AddSystem fail ! The %s already exist.\n", system.ToString().c_str());
    return;
  }
  GetInstance()._systems[hash_code] = &system;
  printf("AddSystem %s success !\n", system.ToString().c_str());
}




