#include "commands.h"
#include "configuration.h"
#include "constants.h"
#include <exception>
#include <iostream>
#include <pwd.h>
#include <unistd.h>
#include <zypp/Capability.h>
#include <zypp/DownloadMode.h>
#include <zypp/ProblemTypes.h>
#include <zypp/RepoInfo.h>
#include <zypp/RepoManager.h>
#include <zypp/RepoManagerOptions.h>
#include <zypp/ResKind.h>
#include <zypp/ResPool.h>
#include <zypp/Resolver.h>
#include <zypp/ZConfig.h>
#include <zypp/ZYpp.h>
#include <zypp/ZYppCommitPolicy.h>
#include <zypp/ZYppFactory.h>

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
