#pragma once
#include "constants.h"
#include "lua.hpp"
#include "version.h"
#include <filesystem>
#include <string>
#include <vector>

const Version currentVersion = Version("0.0.5");

struct Configuration {
  std::string editor;
  std::vector<std::string> packages;
  Version version;
};

Configuration convert(lua::Selector &selector, Configuration *);
Configuration readConfig(Constants &constants);
