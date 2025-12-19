#include "configuration.h"
#include "constants.h"
#include <exception>
#include <format>
#include <iostream>
#include <pwd.h>
#include <string>
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

    if (!constants.isSudo) {
      std::cerr << "Needs to be run as super user (sudo).\n";
      return 1;
    }

    zypp::ZConfig &conf = zypp::ZConfig::instance();
    zypp::RepoManager r = zypp::RepoManager(zypp::RepoManagerOptions());
    for (zypp::RepoInfo repoInfo : r.knownRepositories()) {
      if (repoInfo.enabled()) {
        r.loadFromCache(repoInfo);
        r.buildCache(repoInfo);
      }
    }

    zypp::ZYpp::Ptr z = zypp::getZYpp();
    z->initializeTarget(conf.systemRoot());
    z->target()->load();
    z->target()->buildCache();

    for (std::string package : configuration.packages) {
      std::cout << package << "\n";
      z->resolver()->addRequire(
          zypp::Capability(package, zypp::ResKind::package));
    }

    bool result = z->resolver()->resolvePool();
    // TODO error handling
    std::cout << "resolver: " << result << "\n";

    auto pol = zypp::ZYppCommitPolicy();
    auto commitResult = z->commit(pol);
    // TODO error handling
    std::cout << commitResult;
  } catch (const std::exception &e) {
    std::cerr << e.what();
    return 1;
  }
}
