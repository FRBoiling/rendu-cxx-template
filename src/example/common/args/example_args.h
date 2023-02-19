/*
* Created by boil on 2022/9/24.
*/
#pragma once
#include <argument_parser.h>
using namespace rendu;

//-abc 4.14 5.718
void example_args(int argc, char **argv) {
  ArgumentParser program("test");

  program.add_argument("-a")
      .default_value(false)
      .implicit_value(true);

  program.add_argument("-b")
      .default_value(false)
      .implicit_value(true);

  program.add_argument("-c")
      .nargs(2)
      .scan<'g', float>();

  try {
    program.parse_args(argc, argv);
  }
  catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }
  std::cout<<program<<std::endl;
  auto a = program.get<bool>("-a");                               // true
  auto b = program.get<bool>("-b");                               // true
  auto c = program.get<std::vector<float>>("-c");

  std::cout<<a<<std::endl;
  std::cout<<b<<std::endl;
  std::for_each(c.begin(), c.end(), [](float f) { std::cout << f << std::endl; });
  
  std::cout<<"--------------"<<std::endl;
}

//例如： 1 2 3 -abc 4.14 5.718
void args_example(int argc, char **argv){
  ArgumentParser program("test");

  program.add_argument("numbers")
      .nargs(3)
      .scan<'i', int>();

  program.add_argument("-a")
      .default_value(false)
      .implicit_value(true);

  program.add_argument("-b")
      .default_value(false)
      .implicit_value(true);

  program.add_argument("-c")
      .nargs(2)
      .scan<'g', float>();

  program.add_argument("--files")
      .nargs(3);

  try {
    program.parse_args(argc, argv);
  }
  catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    std::exit(1);
  }
  auto numbers = program.get<std::vector<int>>("numbers");        // {1, 2, 3}
  auto a = program.get<bool>("-a");                               // true
  auto b = program.get<bool>("-b");                               // true
  auto c = program.get<std::vector<float>>("-c");                 // {4.14f, 5.718f}
  auto files = program.get<std::vector<std::string>>("--files");  // {"a.txt", "b.txt", "c.txt"}

  std::cout<<program;
}

