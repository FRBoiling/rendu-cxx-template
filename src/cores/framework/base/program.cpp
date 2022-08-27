/**********************************
**  Created by boil on 2022/8/14.
***********************************/
#include "program.h"

Program *Program::instance = new Program();

Program *Program::GetInstance() {
  return instance;
}

int Program::Initialize(int argc, char** argv){
  printf("Initialize...");

  printf("Initialize success!");
}

int Program::Run(){

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

int Program::AddComponent(component *component) {

}



