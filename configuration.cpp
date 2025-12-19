#include "configuration.h"
#include "lua.hpp"
#include <string>

Configuration convert(lua::Selector &selector, Configuration *) {
  Configuration configuration = Configuration();
  configuration.packages = selector["packages"].get<std::vector<std::string>>();
  return configuration;
}

Configuration readConfig(Constants &constants) {
  std::filesystem::path luaPath =
      constants.configDirectory / "configuration.lua";
  lua::LuaProgramResult result = lua::LuaProgram().execute(luaPath);
  return result["Config"].get<Configuration>();
}
