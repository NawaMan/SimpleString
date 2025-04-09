#include <string.hpp>
#include <iostream>

int main() {
  // Using the correct namespace 'simple' instead of 'sstring'
  simple::String str("Hello, SimpleString!");
  std::cout << "Length: " << str.length() << std::endl;
  std::cout << "String: " << str.to_string() << std::endl;
  return 0;
}
