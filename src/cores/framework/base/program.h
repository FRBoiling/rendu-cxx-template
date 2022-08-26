#ifndef RENDU_PROGRAM_H__
#define RENDU_PROGRAM_H__
#include "util.h"

class Program {
private:
  explicit Program() = default;

  static Program *instance;
public:
  static Program *GetInstance();

  int Run();

  int Initialize(int argc, char **argv);

private:
  int Start();

  int Stop();

  int Update();
};

#define RUN(argc, argv) \
Program::GetInstance()->Initialize(argc, argv); \
Program::GetInstance()->Run();

#endif //RENDU_PROGRAM_H__