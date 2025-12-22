#include "constants.h"

bool isSudo() { return getenv("SUDO_USER"); }

std::filesystem::path configDirectory(bool devMode, bool isSudo) {
  if (devMode) {
    return std::filesystem::path("./testConfig");
  }

  if (isSudo) {
    return std::filesystem::path("/home") / getenv("SUDO_USER") / ".config" /
           "zyppd";
  } else {
    return std::filesystem::path("/home") / getenv("USER") / ".config" /
           "zyppd";
  }
}

Constants getConstants() {
  Constants constants{};
  constants.devMode = true;
  constants.isSudo = isSudo();
  constants.configDirectory =
      configDirectory(constants.devMode, constants.isSudo);
  constants.configFilePath = constants.configDirectory / "configuration.lua";
  return constants;
}
