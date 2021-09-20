#include "format.h"

#include <sstream>
#include <string>

using std::string;

string Format::ElapsedTime(long seconds) {
  int hours = seconds / 3600;
  int minutes = (seconds % 3600) / 60;
  int secs = (seconds % 3600) % 60;
  std::ostringstream stream;
  stream << hours << ":" << minutes << ":" << secs;
  return stream.str();
}