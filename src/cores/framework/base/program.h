/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#ifndef RENDU_PROGRAM_H__
#define RENDU_PROGRAM_H__
#include "util.h"
#include "component.h"

class Program {
private:
  explicit Program() = default;

  static Program *instance;
public:
  static Program *GetInstance();

  int Initialize(int argc, char **argv);

  int Run();

  int AddComponent(component* component);

private:

  int Start();

  int Stop();

  int Update();
};

#define RUN(argc, argv) \
Program::GetInstance()->Initialize(argc, argv); \
Program::GetInstance()->Run();

#endif //RENDU_PROGRAM_H__