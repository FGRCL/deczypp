#pragma once
#include <filesystem>

struct Constants {
  bool devMode;
  bool isSudo;
  std::filesystem::path configDirectory;
  std::filesystem::path configFilePath;
  std::string initialConfig;
};

bool isSudo();
std::filesystem::path configDirectory(bool devMode, bool isSudo);

Constants getConstants();
