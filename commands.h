#pragma once
#include "configuration.h"
#include "constants.h"
#include <functional>
#include <string>
#include <vector>

// Command ideas
// Add
// Remove

void install(bool isSudo, std::vector<std::string> packages);

void edit(std::string editor, std::filesystem::path configFilePath);

std::function<void()> parseCommand(int args, char *argv[], Constants constants,
                                   Configuration configuration);
