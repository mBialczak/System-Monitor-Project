// TODO: remove before sending project
#include <chrono>
#include <iostream>
#include <thread>

#include "format.h"
#include "linux_parser.h"
#include "process.h"
using namespace std::literals::chrono_literals;
//--------------------------------------------
#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  // DEBUG:
  // std::cout << LinuxParser::Command(101103) << std::endl;

  Process process{101103};
  std::cout << "Pid:\t" << process.Pid() << std::endl;
  std::cout << "User:\t" << process.User() << std::endl;
  std::cout << "Command:\t" << process.Command() << std::endl;
  std::cout << "CPU:\t" << process.CpuUtilization() << std::endl;
  std::cout << "RAM:\t" << process.Ram() << std::endl;
  std::cout << "UpTime:\t" << process.UpTime() << std::endl;

  // for (int i = 0; i < 30; i++) {
  //   std::cout << "Process CPU ussage:\t" << 100 * process.CpuUtilization()
  //             << std::endl;
  //   std::this_thread::sleep_for(1000ms);
  // }

  // LinuxParser::UpTime(36951);
  // std::cout << "PID: " << 58469
  //           << "\tuptime: " //<<
  //           Format::ElapsedTime(LinuxParser::UpTime(58469))
  //           << std::endl;

  // std::cout << LinuxParser::Uid(1555) << std::endl;
  // std::cout << LinuxParser::User(1555) << std::endl;
  // auto stats = LinuxParser::ProccStats(1555);
  // for (size_t i = 0; i < stats.size(); i++) {
  //   std::cout << i << "\t" << stats[i] << std::endl;
  // }
  // NCursesDisplay::Display(system);
}