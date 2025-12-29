#include "colors.h"
#include "commands.h"
#include "configuration.h"
#include "constants.h"

int main(int args, char *argv[]) {
  try {
    std::function<void()> command = parseCommand(args, argv);
    command();
  } catch (const std::exception &e) {
    printError(e.what());
    return 1;
  }
}
