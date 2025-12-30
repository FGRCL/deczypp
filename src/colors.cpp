#include <format>
#include <iostream>
#include <string>

void printError(std::string text) {
  std::cout << std::format("\e[31m{}\e[1m", text);
}

void printSuccess(std::string text) {
  std::cout << std::format("\e[32m{}\e[1m", text);
}
