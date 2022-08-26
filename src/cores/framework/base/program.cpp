#include "program.h"

Program *Program::instance = new Program();

Program *Program::GetInstance() {
  return instance;
}

int Program::Initialize(int argc, char** argv){

}

int Program::Run(){
  printf("Initialize...");

  printf("Initialize success!");

  Start();

}

int Program::Start(){
  printf("Start...");

  printf("Start success!");
}
int Program::Stop(){
  printf("Stop...");

  printf("Stop success!");
}

int Program::Update(){

}




