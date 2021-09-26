#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>

using std::string;

string Format::ElapsedTime(long seconds) {
  long hours = seconds / 3600;
  long minutes = (seconds % 3600) / 60;
  long secs = (seconds % 3600) % 60;
  std::ostringstream stream;
  stream << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2)
         << std::setfill('0') << minutes << ":" << std::setw(2)
         << std::setfill('0') << secs;
  return stream.str();
}

string Format::FormatPID(int pid) {
  string pidString;
  std::ostringstream stream;
  stream << std::setw(7) << std::left << std::setfill(' ') << pid;
  return stream.str();
}
