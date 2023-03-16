/*
* Created by boil on 2022/10/5.
*/

#include <google/protobuf/compiler/command_line_interface.h>
#include <google/protobuf/compiler/cpp/cpp_generator.h>
#include <google/protobuf/compiler/csharp/csharp_generator.h>

// Must be included last.
#include <google/protobuf/port_def.inc>

namespace google {
  namespace protobuf {
    namespace compiler {

      int ProtobufMain(int argc, char *argv[]) {

        CommandLineInterface cli;
        cli.AllowPlugins("protoc-");

        // Proto2 C++
        cpp::CppGenerator cpp_generator;
        cli.RegisterGenerator("--cpp_out", "--cpp_opt", &cpp_generator,
                              "Generate C++ header and source.");

#ifdef GOOGLE_PROTOBUF_RUNTIME_INCLUDE_BASE
        cpp_generator.set_opensource_runtime(true);
  cpp_generator.set_runtime_include_base(GOOGLE_PROTOBUF_RUNTIME_INCLUDE_BASE);
#endif

        // CSharp
        csharp::Generator csharp_generator;
        cli.RegisterGenerator("--csharp_out", "--csharp_opt", &csharp_generator,
                              "Generate C# source file.");

        return cli.Run(argc, argv);
      }

    }  // namespace compiler
  }  // namespace protobuf
}  // namespace google

int main(int argc, char *argv[]) {
  return PROTOBUF_NAMESPACE_ID::compiler::ProtobufMain(argc, argv);
}
