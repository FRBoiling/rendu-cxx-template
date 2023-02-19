/*
* Created by boil on 2023/02/19.
*/

#include <fmt/format.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  string str = fmt::format("{}{}{}",1,2,3);
  cout<<str<< endl;
  return 0;
}