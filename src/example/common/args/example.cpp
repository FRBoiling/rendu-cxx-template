/*
* Created by boil on 2022/9/12.
*/
#include "example_args.h"
#include "log.h"

int main(int argc, char **argv) {
  try {
    example_args(argc,argv);
    RD_INFO("hello world");
  }
  catch (...){
    RD_CRITICAL("catch error");
  }

}


