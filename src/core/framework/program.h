/*
* Created by boil on 2022/8/27.
*/

#ifndef RENDU_PROGRAM_H_
#define RENDU_PROGRAM_H_

#include <map>
#include "component.h"
#include "system.h"
#include "options.h"
#include "singleton.h"


enum class ProgramState {
  IDLE = 0,           //闲置
  INITIALIZED,    //已经初始化
  RUNNING,        //正在运行
  STOPPING,       //正在停止
  STOPPED         //已经停止
};

enum class ProgramType {
  All = 0,
};

class Program : public Singleton<Program> {
public:
  int Initialize();

  void Run();

  bool IsStopped();

  bool Exit();

  void AddSystem(ISystem &system);

private:
  ProgramState _state;
  std::map<size_t, ISystem *> _systems;

  bool Start();

  bool Stop();

  void Update();

  void LateUpdate();

  void ShowOptions();
};

#define sProgram Program::GetInstance()
#endif //RENDU_PROGRAM_H_