#include "utils.hpp"
#include <iostream>

int main() {
  std::cout << "Welcome To EqbalATM." << std::endl;
  int a = readInt();
  std::cout << "You entered: " << a << std::endl;
  a = readInt();
  std::cout << "You entered: " << a << std::endl;
  return 0;
}
