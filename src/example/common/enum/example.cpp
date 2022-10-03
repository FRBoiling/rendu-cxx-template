/*
* Created by boil on 2022/9/12.
*/
#include "example_enum.h"

int main(int argc, char **argv) {
  try {
    RD_INIT("test",RunMode::Develop,"logs/");
    example_enum();
    RD_INFO("hello world");
  }
  catch (...){
    RD_CRITICAL("catch error");
  }

}


