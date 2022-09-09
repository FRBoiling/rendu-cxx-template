/*
* Created by boil on 2022/8/27.
*/
#include "program.h"
#include "log.h"
int Program::Initialize() {
  printf("Initialize...\n");

  printf("Initialize success!\n");
  _state = ProgramState::INITIALIZED;
  printf("ProgramState is %d\n", GetInstance()._state);
}

bool Program::IsStopped() {
  return _state == ProgramState::STOPPED;
}

bool Program::Exit() {
  _state = ProgramState::STOPPING;
  printf("Exit ....!\n");
}

void Program::Run() {
  Start();
  while (!IsStopped()) {
    Update();
    LateUpdate();
  }
  Stop();
  printf("Exit success!\n");
}

bool Program::Start() {
  if (_state == ProgramState::INITIALIZED) {
    RD_LOG_INFO("Start...\n");
    //TODO:BOIL
    RD_LOG_INFO("Start success!\n");
    _state = ProgramState::RUNNING;
  }
}

bool Program::Stop() {
  if (_state == ProgramState::STOPPED) {
    _state = ProgramState::STOPPING;
    printf("Stop...\n");
    //TODO:BOIL
    printf("Stop success!\n");
  }
}

void Program::Update() {

}

void Program::LateUpdate() {
  //TODO:BOIL
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




