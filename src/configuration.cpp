#include "configuration.h"
#include "lua.hpp"
#include "version.h"
#include <format>
#include <stdexcept>
#include <string>

Configuration convert(lua::Selector &selector, Configuration *) {
  Configuration configuration = Configuration();
  configuration.packages = selector["packages"].get<std::vector<std::string>>();
  configuration.editor = selector["editor"].get<std::string>();
  configuration.version = Version(selector["version"].get<std::string>());
  return configuration;
}

Configuration readConfig(Constants &constants) {
  std::filesystem::path luaPath = constants.configFilePath;
  lua::LuaProgramResult result = lua::LuaProgram().execute(luaPath);
  Configuration userConfiguration = result["Config"].get<Configuration>();
  if (!currentVersion.isCompatible(userConfiguration.version)) {
    if (currentVersion < userConfiguration.version) {
      throw std::runtime_error(
          std::format("Your configuration is ahead of the current "
                      "version. Please "
                      "downgrade your configuration or upgrade zyppd. Your "
                      "version is {} "
                      "and the current version is {}",
                      userConfiguration.version.str(), currentVersion.str()));
    }
    if (currentVersion > userConfiguration.version) {
      throw std::runtime_error(std::format(
          "Your configuration is behind the current version. Please "
          "upgrade your configuration or downgrade zyppd. Your version is {} "
          "and the current version is {}",
          userConfiguration.version.str(), currentVersion.str()));
    }
    throw std::runtime_error(
        std::format("Your configuration versin is not compatibale. Your "
                    "version is {} and the current version is {}",
                    userConfiguration.version.str(), currentVersion.str()));
  }
  return userConfiguration;
}
