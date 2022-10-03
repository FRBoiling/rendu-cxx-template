/*
* Created by boil on 2022/9/25.
*/

#ifndef RENDU_RUN_MODE_H_
#define RENDU_RUN_MODE_H_

namespace rendu{

  // develop mode, 0正式 1开发 2压测
  enum class RunMode{
    Online,
    Develop,
    Pressure,
  };

}//namespace rendu


#endif //RENDU_RUN_MODE_H_
