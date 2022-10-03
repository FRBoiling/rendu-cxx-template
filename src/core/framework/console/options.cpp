/*
* Created by boil on 2022/9/4.
*/

#include "options.h"
#include <iostream>
#include <ostream>
#include "enum.h"

using namespace rendu;

void ParserArguments(ArgumentParser parser) {
  sOptions.m_program_type = parser.get_enum<ProgramType>("-p");
  sOptions.m_zone_id = parser.get<int>("-z");
  sOptions.m_server_id = parser.get<int>("-s");
  sOptions.m_process_num = parser.get<int>("-n");
  sOptions.m_config_path = parser.get<std::string>("-c");
  sOptions.m_run_mode = parser.get_enum<RunMode>("-m");

  sOptions.m_program_name = rendu::StringFormat("{}-{}-{}-{}",
                                                enum_name(sOptions.m_program_type),
                                                sOptions.m_zone_id,
                                                sOptions.m_server_id,
                                                sOptions.m_process_num);
}

int Options::Initialize(int argc, char **argv) {

  ArgumentParser parser("allowed options");
  parser.add_argument("--program", "-p").help("程序类型").required()
      .default_value(ProgramType::All).scan<'i', int>();
  parser.add_argument("--zone", "-z").help("区").required()
      .default_value(1).scan<'i', int>();
  parser.add_argument("--server", "-s").help("服").required()
      .default_value(1).scan<'i', int>();
  parser.add_argument("--number", "-n").help("进程编号").required()
      .default_value(1).scan<'i', int>();
  parser.add_argument("--config", "-c").help("配置文件目录").required()
      .default_value(std::string("../config"));
  parser.add_argument("--mode", "-m").help("运行模式, 0正式 1开发 2压测").required()
      .default_value(RunMode::Online).scan<'i', int>();

  try {
    parser.parse_args(argc, argv);
  }
  catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << parser;
    std::exit(1);
  }
//  std::cout<<parser;
  ParserArguments(parser);
  return 1;
}