/*
* Created by boil on 2022/9/4.
*/

#include "options.h"
#include "git_revision.h"
#include <iostream>
#include <iomanip>
#include <ostream>
#include <argparse/argparse.hpp>
#include "smart_enum.h"

template<>
struct fmt::formatter<Options> : formatter<std::string> {
  auto format(Options options, format_context &ctx) -> decltype(ctx.out()) {
    return format_to(ctx.out(),
                     "  [程序类型：{}]-[区：{}][服：{}][进程编号：{}]-[运行模式：{}]\n  [配置文件目录：{}]",
                     EnumUtils::ToString(options.m_program_type),
                     options.m_zone_id,
                     options.m_server_id,
                     options.m_process_num,
                     options.m_run_mode,
                     options.m_config_path
    );
  }
};

void ParserArguments(argparse::ArgumentParser parser) {
  sOptions.m_program_type = parser.get_enum<ProgramType>("-p");
  sOptions.m_zone_id = parser.get<int>("-z");
  sOptions.m_server_id = parser.get<int>("-s");
  sOptions.m_process_num = parser.get<int>("-n");
  sOptions.m_config_path = parser.get<std::string>("-c");
  sOptions.m_run_mode = parser.get<int>("-m");
  RD_LOG_INFO("options show:\n{}", sOptions);
}

int Options::Initialize(int argc, char **argv) {

  argparse::ArgumentParser parser("allowed options");
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
      .scan<'i', int>().default_value(0).scan<'i', int>();

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