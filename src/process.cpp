#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// [X] TODO: Return this process's ID
int Process::Pid() { return pid; }

// [X] TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  string line;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                           LinuxParser::kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    vector<string> data = LinuxParser::SplitOnSpace(line);
    float utime = stof(data[13]);
    float stime = stof(data[14]);
    float cutime = stof(data[15]);
    float cstime = stof(data[16]);
    float total_time = utime + stime + cutime + cstime;

    return ((total_time / sysconf(_SC_CLK_TCK)) / LinuxParser::UpTime(pid));
  }

  return 0.0;
}

// [X] TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// [X] TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// [X] TODO: Return the user (name) that generated this process
string Process::User() {
  if (username.empty()) {
    username = LinuxParser::User(pid);
  }
  return username;
  // return string();
}

// [X] TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

// [X] TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return a.CpuUtilization() > CpuUtilization();
}