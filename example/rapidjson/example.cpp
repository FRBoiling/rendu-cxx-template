/*
* Created by boil on 2022/9/10.
*/

#include <fmt/format.h>
#include <iostream>
using namespace std;

int main(int, char *[]) {
  string str = fmt::format("{}{}{}",1,2,3);
  cout<<str<< endl;
  return 0;
}