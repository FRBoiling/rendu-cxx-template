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

namespace rendu {

  enum class ProgramState {
    IDLE = 0,           //闲置
    INITIALIZED,    //已经初始化
    RUNNING,        //正在运行
    STOPPING,       //正在停止
    STOPPED         //已经停止
  };

  enum class ProgramType {
    All = 0,
    Hall = 1,
    World = 2,
  };

  class Program : public Singleton<Program> {
  public:
    void Initialize();

    void Run();

    bool IsStopped();

    void Exit();

    void AddSystem(ISystem &system);

  private:

    void Start();

    void Stop();

    void Update();

    void LateUpdate();

    ProgramState _state;
    std::map<size_t, ISystem *> _systems;
  };

#define sProgram Program::GetInstance()
}//namespace rendu

#endif //RENDU_PROGRAM_H_