#pragma once
#include "constants.h"
#include "lua.hpp"
#include <filesystem>
#include <string>
#include <vector>

struct Configuration {
  std::string editor;
  std::vector<std::string> packages;
};

Configuration convert(lua::Selector &selector, Configuration *);
Configuration readConfig(Constants &constants);
