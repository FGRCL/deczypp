#pragma once
#include "configuration.h"
#include "constants.h"
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

void install(bool isSudo, std::vector<std::string> packages);

void edit(std::string editor, std::filesystem::path configFilePath);

std::function<void()> parseCommand(int args, char *argv[]);

void unmanaged();

void init(std::filesystem::path, std::string);
