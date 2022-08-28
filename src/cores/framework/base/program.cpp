/*
* Created by boil on 2022/8/27.
*/
#include "program.h"

int Program::Initialize(int argc, char **argv) {
  printf("Initialize...\n");

  printf("Initialize success!\n");
  _state = ProgramState::INITIALIZED;
  printf("ProgramState is %d\n", _state);
}

bool Program::IsRunning() {
  return _state > ProgramState::IDLE;
}

void Program::Exit() {
  _state = ProgramState::IDLE;
}

int Program::Run() {
  Start();
  while (IsRunning()) {

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
  _state = ProgramState::STOPPING;

  printf("Stop success!\n");
}

int Program::Update() {

}

void Program::AddSystem(ISystem &system) {
  auto hash_code = system.GetType().hash_code();
  if (_systems.contains(hash_code)) {
    printf("AddSystem fail ! The %s already exist.\n", system.ToString().c_str());
    return;
  }
  _systems[hash_code] = &system;
  printf("AddSystem %s success !\n", system.ToString().c_str());
}




