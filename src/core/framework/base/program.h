/*
* Created by boil on 2022/8/27.
*/

#ifndef RENDU_PROGRAM_H_
#define RENDU_PROGRAM_H_

#include <map>
#include "component.h"
#include "system.h"

enum class ProgramState {
  IDLE,           //闲置
  INITIALIZED,    //已经初始化
  RUNNING,        //正在运行
  STOPPING,       //正在停止
  STOPPED         //已经停止
};

class Program : public System<Program> {
public:
  static int Initialize(int argc, char **argv);
  static int Run();
  static bool IsStopped();
  static void AddSystem(ISystem &system);
  static int Stop();
private:

  ProgramState _state;
  std::map<size_t,ISystem*> _systems;

  int Start();


  void Exit();
  int Update();
};

#endif //RENDU_PROGRAM_H_