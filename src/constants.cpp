#include "constants.h"
#include <string>

extern char _binary_initialConfig_lua_start;
extern char _binary_initialConfig_lua_end;

bool isSudo() { return getenv("SUDO_USER"); }

std::filesystem::path configDirectory(bool devMode, bool isSudo) {
  if (devMode) {
    return std::filesystem::path("./testConfigDirectory");
  }

  if (isSudo) {
    return std::filesystem::path("/home") / getenv("SUDO_USER") / ".config" /
           "zyppd";
  } else {
    return std::filesystem::path("/home") / getenv("USER") / ".config" /
           "zyppd";
  }
}

std::string loadInitialConfig() {
  std::string config;
  char *p = &_binary_initialConfig_lua_start;
  while (p != &_binary_initialConfig_lua_end) {
    config += *p++;
  }
  return config;
}

Constants getConstants() {
  Constants constants{};
  constants.devMode = false;
  constants.isSudo = isSudo();
  constants.configDirectory =
      configDirectory(constants.devMode, constants.isSudo);
  constants.configFilePath = constants.configDirectory / "configuration.lua";
  constants.initialConfig = loadInitialConfig();
  return constants;
}
