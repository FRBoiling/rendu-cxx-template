/*
* Created by boil on 2022/8/27.
*/
#include "program.h"
#include "log.h"
#include "smart_enum.h"

int Program::Initialize() {
  RD_LOG_INFO("initialize...");

  RD_LOG_INFO("initialize success!");
  _state = ProgramState::INITIALIZED;
  RD_LOG_INFO("ProgramState is {}", EnumUtils::ToString(_state));
}

bool Program::IsStopped() {
  return _state == ProgramState::STOPPED;
}

bool Program::Exit() {
  _state = ProgramState::STOPPING;
  RD_LOG_INFO("exit ....!");
}

void Program::Run() {
  Start();
  while (!IsStopped()) {
    Update();
    LateUpdate();
  }
  Stop();
  RD_LOG_INFO("exit success!");
}

bool Program::Start() {
  if (_state == ProgramState::INITIALIZED) {
    RD_LOG_INFO("start...");
    //TODO:BOIL
    RD_LOG_INFO("start success!");
    _state = ProgramState::RUNNING;
  }
}

bool Program::Stop() {
  if (_state == ProgramState::STOPPED) {
    _state = ProgramState::STOPPING;
    RD_LOG_INFO("stop...\n");
    //TODO:BOIL
    RD_LOG_INFO("stop success!\n");
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
        RD_LOG_DEBUG("AddSystem fail ! The {} already exist.", system.ToString());
    return;
  }
  _systems[hash_code] = &system;
      RD_LOG_DEBUG("AddSystem {} success !", system.ToString());
}

void Program::ShowOptions() {

}




