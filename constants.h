#include <cstdlib>
#include <filesystem>

const bool DEV_MODE = true;

bool isSudo() { return getenv("SUDO_USER"); }

const bool IS_SUDO = isSudo();

std::filesystem::path configDirectory() {
  if (DEV_MODE) {
    return std::filesystem::path("./testConfig");
  }

  if (IS_SUDO) {
    return std::filesystem::path("/home") / getenv("SUDO_USER") / ".config" /
           "zyppd";
  } else {
    return std::filesystem::path("/home") / getenv("USER") / ".config" /
           "zyppd";
  }
}

const std::filesystem::path CONFIG_DIRECTORY = configDirectory();
