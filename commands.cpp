#include "commands.h"
#include "colors.h"
#include "configuration.h"
#include "constants.h"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <zypp/Capability.h>
#include <zypp/DownloadMode.h>
#include <zypp/IdString.h>
#include <zypp/ProblemTypes.h>
#include <zypp/RepoInfo.h>
#include <zypp/RepoManager.h>
#include <zypp/RepoManagerOptions.h>
#include <zypp/ResKind.h>
#include <zypp/ResPool.h>
#include <zypp/ResTraits.h>
#include <zypp/Resolver.h>
#include <zypp/ZConfig.h>
#include <zypp/ZYpp.h>
#include <zypp/ZYppCommitPolicy.h>
#include <zypp/ZYppFactory.h>

std::function<void()> parseCommand(int args, char *argv[], Constants constants,
                                   Configuration configuration) {
  if (args <= 1) {
    throw std::runtime_error("No command was passed.");
  }

  std::string command = argv[1];
  if (command == "install") {
    return std::bind(install, constants.isSudo, configuration.packages);
  } else if (command == "edit") {
    return std::bind(edit, configuration.editor, constants.configFilePath);
  } else if (command == "unmanaged") {
    return std::bind(unmanaged);
  } else {
    throw std::runtime_error(std::format("Command \"{}\" is unknown", command));
  }
}

std::string getProblemsString(zypp::ResolverProblemList problemList);

zypp::ZYpp initializeZypper();

void install(bool isSudo, std::vector<std::string> packages) {
  if (!isSudo) {
    throw std::runtime_error("Needs to be run as super user (sudo).\n");
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

  for (std::string package : packages) {
    z->resolver()->addRequire(
        zypp::Capability(package, zypp::ResKind::package));
  }

  bool result = z->resolver()->resolvePool();
  if (!result) {
    throw std::runtime_error(
        std::format("Could not resolve packages:\n {}",
                    getProblemsString(z->resolver()->problems())));
  }

  auto pol = zypp::ZYppCommitPolicy();
  auto commitResult = z->commit(pol);

  if (!commitResult.noError()) {
    throw std::runtime_error("Failed to install packages");
  }

  printSuccess("Package install successful.\n");
}

std::string getProblemsString(zypp::ResolverProblemList problemList) {
  std::string result = "";
  for (auto problem : problemList) {
    result += "\t" + problem->description() + ". " + problem->details() + "\n";

    if (problem->solutions().size() > 0) {
      result += "\tSolutions:\n";
      for (auto solution : problem->solutions()) {
        result +=
            "\t\t" + solution->description() + ":" + solution->details() + "\n";
      }
    }
  }
  return result;
}

void edit(std::string editor, std::filesystem::path configFilePath) {
  int result = std::system(
      std::format("{} {}", editor, configFilePath.string()).c_str());

  printSuccess("File edit successful.\n");
  if (result != 0) {
    throw std::runtime_error(std::format("Could not edit configuration file."));
  }
}

void unmanaged() {
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

  std::cout << z->pool().proxy().size() << "\n";
  auto test = z->pool().proxy().byKind<zypp::Package>();
  std::vector<zypp::IdString> installedPackages{};
  for (auto s : test) {
    if (s->hasInstalledObj() && !s->identIsAutoInstalled()) {
      installedPackages.push_back(s->ident());
    }
  }
  std::sort(installedPackages.begin(), installedPackages.end());
  for (zypp::IdString id : installedPackages) {
    std::cout << id.asString() << "\n";
  }
}
