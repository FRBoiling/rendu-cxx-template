/*
* Created by boil on 2022/8/20.
*/

#include "banner.h"
#include "git_revision.h"
#include "options.h"
#include "enum.h"
using namespace rendu;

template<>
struct fmt::formatter<Options> : formatter<std::string> {
  auto format(Options options, format_context &ctx) -> decltype(ctx.out()) {
    return format_to(ctx.out(),
                     "[程序类型：{}]-[区：{}][服：{}][进程编号：{}]-[运行模式：{}]\n[配置文件目录：{}]",
                     enum_name(options.m_program_type),
                     options.m_zone_id,
                     options.m_server_id,
                     options.m_process_num,
                     enum_name(options.m_run_mode),
                     options.m_config_path
    );
  }
};

void rendu::banner::Show() {
  RD_INFO("\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n 进程名称：{}",
          GitRevision::GetFullVersion(),
          sOptions,
          "*************************************************",
          "             R E N D U    C O R E                ",
          "   https://github.com/FRBoiling/rendu-core.git   ",
          "*************************************************",
          " <Ctrl-C> to stop.", sOptions.m_program_name);
}

