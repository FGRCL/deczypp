#pragma once
#include <format>
#include <ostream>
#include <string>
#include <vector>

class Version {
private:
  int major;
  int minor;
  int patch;

public:
  Version();
  Version(std::string);

  bool isCompatible(Version) const;
  std::strong_ordering operator<=>(const Version &v) const;
  std::string str() const;

private:
  std::vector<std::string> split(std::string, char);
  int partToInteger(std::string, std::string);
};
