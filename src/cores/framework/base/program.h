/*
* Created by boil on 2022/8/27.
*/

#ifndef RENDU_PROGRAM_H__
#define RENDU_PROGRAM_H__
#include "util.h"
#include "component.h"

enum class ProgramState{
  INITIALIZED,    //已经初始化
  IDLE,           //闲置
  RUNNING,        //正在运行
  STOPPING        //正在停止
};

class Program {
private:
  explicit Program() = default;

  static Program *instance_;
public:
  static Program *GetInstance();
  ProgramState state_;

  int Initialize(int argc, char **argv);

  int Run();
  void Exit();
  int AddComponent(component* component);

private:
  bool IsRunning();

  int Start();

  int Stop();

  int Update();
};

#endif //RENDU_PROGRAM_H__