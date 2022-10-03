/*
* Created by boil on 2022/9/25.
*/
#include "example_random.h"

int main(int argc, char **argv) {
  try {
    example_random();
    RD_INFO("hello world");
  }
  catch (...){
    RD_CRITICAL("catch error");
  }

}


