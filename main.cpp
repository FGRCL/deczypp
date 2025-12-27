#include "colors.h"
#include "commands.h"
#include "configuration.h"
#include "constants.h"

int main(int args, char *argv[]) {
  try {
    Constants constants = getConstants();
    Configuration configuration = readConfig(constants);
    std::function<void()> command =
        parseCommand(args, argv, constants, configuration);
    command();
  } catch (const std::exception &e) {
    printError(e.what());
    return 1;
  }
}
