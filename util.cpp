#include "util.h"

#include <sstream>

std::string SimpleIntToString(int i) {
  std::stringstream resultstream;
  resultstream << i;
  return resultstream.str();
}
