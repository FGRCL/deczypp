#pragma once
#include "configuration.h"
#include "constants.h"
#include <functional>
#include <string>
#include <vector>

// Command ideas
// Sync
// Add
// Remove
// Edit

void install(bool isSudo, std::vector<std::string> packages);

std::function<void()> parseCommand(int args, char *argv[], Constants constants,
                                   Configuration configuration);
