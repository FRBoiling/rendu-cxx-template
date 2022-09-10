/*
* Created by boil on 2022/8/27.
*/
#include "program.h"
#include "log.h"
#include "smart_enum.h"
#include "banner.h"

void Program::Initialize() {
  rendu::banner::Show();
  RD_LOG_INFO("initialize...");

  RD_LOG_INFO("initialize success!");
  RD_LOG_INFO("ProgramState is {}", EnumUtils::ToString(_state));
  _state = ProgramState::INITIALIZED;
}

bool Program::IsStopped() {
  return _state == ProgramState::STOPPED;
}

void Program::Exit() {
  RD_LOG_INFO("exit ....!");
  _state = ProgramState::STOPPING;
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

void Program::Start() {
  if (_state == ProgramState::INITIALIZED) {
    RD_LOG_INFO("start...");
    //TODO:BOIL
    RD_LOG_INFO("start success!");
    _state = ProgramState::RUNNING;
  }
}

void Program::Stop() {
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





