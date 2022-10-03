/*
* Created by boil on 2022/8/27.
*/
#include "program.h"
#include "banner.h"

using namespace rendu;

void Program::Initialize() {
  RD_INIT("custom",sOptions.m_run_mode,"logs/");
  banner::Show();
  RD_INFO("initialize...");

  RD_INFO("initialize success!");
  RD_INFO("ProgramState is {}", enum_name(_state));
  _state = ProgramState::INITIALIZED;
}

bool Program::IsStopped() {
  return _state == ProgramState::STOPPED;
}

void Program::Exit() {
  RD_INFO("exit ....!");
  _state = ProgramState::STOPPING;
}

void Program::Run() {
  Start();
  while (!IsStopped()) {
    Update();
    LateUpdate();
  }
  Stop();
  RD_INFO("exit success!");
}

void Program::Start() {
  if (_state == ProgramState::INITIALIZED) {
    RD_INFO("start...");
    //TODO:BOIL
    RD_INFO("start success!");
    _state = ProgramState::RUNNING;
  }
}

void Program::Stop() {
  if (_state == ProgramState::STOPPED) {
    _state = ProgramState::STOPPING;
    RD_INFO("stop...\n");
    //TODO:BOIL
    RD_INFO("stop success!\n");
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
        RD_DEBUG("AddSystem fail ! The {} already exist.", system.ToString());
    return;
  }
  _systems[hash_code] = &system;
      RD_DEBUG("AddSystem {} success !", system.ToString());
}





