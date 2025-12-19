#pragma once
#include <filesystem>

struct Constants {
  bool devMode;
  bool isSudo;
  std::filesystem::path configDirectory;
};

bool isSudo();
std::filesystem::path configDirectory(bool devMode, bool isSudo);

Constants getConstants();
