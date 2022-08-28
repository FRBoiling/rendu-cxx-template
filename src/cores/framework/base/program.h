/*
* Created by boil on 2022/8/27.
*/

#ifndef RENDU_PROGRAM_H__
#define RENDU_PROGRAM_H__

#include <map>
#include "component.h"
#include "system.h"

enum class ProgramState {
  IDLE,           //闲置
  INITIALIZED,    //已经初始化
  RUNNING,        //正在运行
  STOPPING        //正在停止
};

class Program : public System<Program> {
public:
  int Initialize(int argc, char **argv);

  int Run();

  void Exit();

  void AddSystem(ISystem &system);

private:
  ProgramState _state;
  std::map<size_t,ISystem*> _systems;
  bool IsRunning();

  int Start();

  int Stop();

  int Update();
};

#endif //RENDU_PROGRAM_H__