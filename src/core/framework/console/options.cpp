/*
* Created by boil on 2022/9/4.
*/

#include "options.h"
#include "git_revision.h"
#include <iostream>
#include <iomanip>
#include <ostream>
#include <boost/program_options.hpp>
#include <smart_enum.h>

namespace rd_bp = boost::program_options;

void ParserLogInfo(boost::program_options::variables_map &vm) {
  for (auto &[name, value]: vm) {
    std::cout
        << std::boolalpha
        << "defaulted : " << value.defaulted()
        << "\tkey : " << name;
//        << "\tempty:" << value.empty();
    if (typeid(std::string) == value.value().type())
      std::cout << " - string " << std::__1::quoted(value.as<std::string>()) << "\n";
    else if (typeid(int) == value.value().type())
      std::cout << " - int " << value.as<int>() << "\n";
    else if (!value.empty())
      std::cout << " - unknown type\n";
  }
}

template<>
struct fmt::formatter<Options> : formatter<std::string> {
  auto format(Options options, format_context &ctx) -> decltype(ctx.out()) {
    return format_to(ctx.out(),
                     "options info: \n - 程序类型：{}\n - 区： {}\n - 服： {}\n - 进程编号：{}\n - 配置文件目录：{}\n - 运行模式： {}",
                     EnumUtils::ToString( static_cast<ProgramType>(options.m_program_type)) ,
                     options.m_zone_id,
                     options.m_server_id,
                     options.m_process_num,
                     options.m_config_path,
                     options.m_run_mode
    );
  }
};

void ParserArguments(boost::program_options::variables_map &vm) {
  sOptions.m_program_type = vm["program"].as<int>();
  sOptions.m_zone_id = vm["zone"].as<int>();
  sOptions.m_server_id = vm["server"].as<int>();
  sOptions.m_process_num = vm["number"].as<int>();
  sOptions.m_config_path = vm["config"].as<std::string>();
  sOptions.m_run_mode = vm["mode"].as<int>();

  RD_LOG_INFO(sOptions);
}

int Options::Initialize(int argc, char **argv) {
  boost::program_options::options_description all("allowed options");
  all.add_options()
      ("help,h", "usage message")
      ("version,v", "version build info")

      ("program,p", rd_bp::value<int>(&m_program_type)->default_value(0), "程序类型")
      ("zone,z", rd_bp::value<int>(&m_zone_id)->default_value(1), "区")
      ("server,s", rd_bp::value<int>(&m_server_id)->default_value(1), "服")
      ("number,n", rd_bp::value<int>(&m_process_num)->default_value(1), "进程编号")

      ("config,c", rd_bp::value<std::string>(&m_config_path)->default_value("../config"), "配置文件目录")
      ("mode,m", rd_bp::value<int>(&m_run_mode)->default_value(0), "运行模式, 0正式 1开发 2压测");

#ifdef _WIN32
  boost::program_options::options_description win("Windows platform specific options");
      win.add_options()
          ("service,s", value<std::string>(&configService)->default_value(""), "Windows service options: [install | uninstall]")
          ;

      all.add(win);
#endif
  //读取输入的命令行参数
  try {
    boost::program_options::variables_map vm;
    auto options = boost::program_options::parse_command_line(argc, argv, all);
//    auto options = boost::program_options::parse_command_line(argc, argv, m_desc);
    boost::program_options::store(options, vm);
    boost::program_options::notify(vm);
    if (vm.count("help") > 0) {
      std::cout << all << std::endl;
      return 0;
    } else if (vm.count("version")) {
      std::cout << GitRevision::GetFullVersion() << std::endl;
      return 0;
    } else {
//      ParserLogInfo(vm);
      ParserArguments(vm);
    }
  } catch (std::exception const &e) {
    std::cerr << e.what() << std::endl;
    std::cerr << all << std::endl;
    return 0;
  }
  return 1;
}









