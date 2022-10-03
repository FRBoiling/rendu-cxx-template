/*
* Created by boil on 2022/9/4.
*/

#ifndef RENDU_OPTIONS_H_
#define RENDU_OPTIONS_H_

#include "singleton.h"
#include "program.h"
#include "run_mode.h"

namespace rendu {

  enum class ProgramType;

  class Options : public Singleton<Options> {

  public:
    int Initialize(int argc, char **argv);

  public:
    ProgramType m_program_type;
    int m_zone_id;
    int m_server_id;
    int m_process_num;
    std::string m_config_path;
    RunMode m_run_mode;

    std::string m_program_name;
  };

#define sOptions Options::GetInstance()

}//namespace rendu

#endif //RENDU_OPTIONS_H_
