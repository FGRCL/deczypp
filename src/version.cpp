#include "version.h"
#include <exception>
#include <format>
#include <stdexcept>
#include <string>
#include <vector>

Version::Version() {
  major = 0;
  minor = 0;
  patch = 0;
}

Version::Version(std::string version) {
  std::vector<std::string> parts = split(version, '.');
  if (parts.size() != 3) {
    throw std::runtime_error(std::format(
        "Version does not have the right amout parts. Got {} expected 3",
        parts.size()));
  }

  major = partToInteger(parts[0], version);
  minor = partToInteger(parts[1], version);
  patch = partToInteger(parts[2], version);
}

bool Version::isCompatible(Version version) const {
  if (major != version.major ||
      (major == version.major && minor != version.minor)) {
    return false;
  }
  return true;
}

std::strong_ordering Version::operator<=>(const Version &v) const {
  if (v.major != major) {
    return major <=> v.major;
  }

  if (v.minor != minor) {
    return minor <=> v.minor;
  }

  return v.patch <=> patch;
}

std::string Version::str() const {
  return std::format("{}.{}.{}", major, minor, patch);
}

std::vector<std::string> Version::split(std::string string, char delimiter) {
  std::vector<std::string> parts;
  int begin = 0;
  int end = string.find(delimiter, begin);
  while (end != std::string::npos) {
    parts.push_back(string.substr(begin, end));
    begin = end + 1;
    end = string.find(delimiter, begin);
  }
  parts.push_back(string.substr(begin, end));
  return parts;
}

int Version::partToInteger(std::string part, std::string version) {
  try {
    return stoi(part);
  } catch (std::exception &e) {
    throw std::runtime_error(
        std::format("Could not convert version part into an interger. Part {} "
                    "of {} could not read properly. {}",
                    part, version, e.what()));
  }
}
