/*
* Created by boil on 2022/9/4.
*/

#ifndef RENDU_OPTIONS_H_
#define RENDU_OPTIONS_H_

#include <boost/program_options/variables_map.hpp>
#include "singleton.h"
#include "app_enum.h"

class Options : public Singleton<Options> {
public:
  void ShowInfo();

  int Initialize(int argc, char **argv);

public:
  int m_program_type;
  int m_zone_id;
  int m_server_id;
  int m_process_num;
  std::string m_config_path;
  //develop mode, 0正式 1开发 2压测
  int m_run_mode;
};


#define sOptions Options::GetInstance()

#endif //RENDU_OPTIONS_H_
