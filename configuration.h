#include "constants.h"
#include "lua.hpp"
#include <filesystem>
#include <vector>

struct Configuration {
  std::vector<std::string> packages;
};

Configuration convert(Selector &selector, Configuration *) {
  Configuration configuration = Configuration();
  configuration.packages = selector["packages"].get<std::vector<std::string>>();
  return configuration;
}

Configuration readConfig() {
  std::filesystem::path luaPath = CONFIG_DIRECTORY / "configuration.lua";
  LuaProgramResult result = LuaProgram().execute(luaPath);
  return result["config"].get<Configuration>();
}

const Configuration CONFIGURATION = readConfig();
