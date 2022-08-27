/*
* Created by boil on 2022/8/27.
*/
#include "program.h"

Program *Program::instance_ = new Program();

Program *Program::GetInstance() {
  return instance_;
}

int Program::Initialize(int argc, char** argv){
  printf("Initialize...\n");

  printf("Initialize success!\n");
  state_ = ProgramState::INITIALIZED;
}

bool Program::IsRunning() {
  return state_ > ProgramState::IDLE;
}

void Program::Exit() {
  state_  = ProgramState::IDLE;
}

int Program::Run(){
  Start();
  while (IsRunning()){

  }

  printf("Exit success!\n");
}

int Program::Start(){
  printf("Start...\n");

  printf("Start success!\n");
  state_ = ProgramState::RUNNING;
}

int Program::Stop(){
  printf("Stop...\n");
  state_ = ProgramState::STOPPING;

  printf("Stop success!\n");
}

int Program::Update(){

}

int Program::AddComponent(component *component) {

}



