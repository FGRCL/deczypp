#include "commands.h"
#include "configuration.h"
#include "constants.h"
#include <iostream>

int main(int args, char *argv[]) {
  try {
    Constants constants = getConstants();
    Configuration configuration = readConfig(constants);
    std::function<void()> command =
        parseCommand(args, argv, constants, configuration);
    command();
  } catch (const std::exception &e) {
    std::cerr << e.what();
    return 1;
  }
}
