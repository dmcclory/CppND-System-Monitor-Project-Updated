#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::ExtractKeyFromProcFile(string path, string key) {
  string line;
  string k;
  string v;
  std::ifstream filestream(path);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> k >> v) {
        if (k == key) {
          return v;
        }
      }
    }
  }
  return "N/A";
}

vector<string> LinuxParser::SplitOnSpace(string text) {
  vector<string> result = {};
  std::istringstream linestream(text);
  string entry;
  while (linestream >> entry) {
    result.push_back(entry);
  };
  return result;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// [X] TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  // based on htop's calulation:
  // https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
  float total = stof(LinuxParser::ExtractKeyFromProcFile(
      kProcDirectory + kMeminfoFilename, "MemTotal:"));
  float freeMem = stof(LinuxParser::ExtractKeyFromProcFile(
      kProcDirectory + kMeminfoFilename, "MemFree:"));

  return (total - freeMem) / total;
}

// [X] TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return stof(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// [X] TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    return LinuxParser::SplitOnSpace(line);
  }
  return {};
}

// [X] TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string res = LinuxParser::ExtractKeyFromProcFile(
      kProcDirectory + kStatFilename, "processes");
  return std::stoi(res);
}
// [X] TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string res = LinuxParser::ExtractKeyFromProcFile(
      kProcDirectory + kStatFilename, "procs_running");
  return std::stoi(res);
}

// [X] TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), '\0', ' ');
    return line;
  }
  return "";
}

// [X] TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string res = LinuxParser::ExtractKeyFromProcFile(
      kProcDirectory + to_string(pid) + kStatusFilename, "VmSize:");
  if (res == "N/A") {
    return "0";
  }
  return to_string(stoi(res) / 1000);
}

// [X] TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string res = ExtractKeyFromProcFile(
      kProcDirectory + "/" + to_string(pid) + kStatusFilename, "Uid:");
  return res;
}

// [X] TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string username;
  string x;
  string row_id;

  string uid = LinuxParser::Uid(pid);

  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> x >> row_id) {
        if (row_id == uid) {
          return username;
        }
      }
    }
  }

  return string();
}

// [X] TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  vector<string> data;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    data = LinuxParser::SplitOnSpace(line);
    long int procRawUpTime = std::stol(data[21]);
    // learned about this sysconf denominator via this forum thread:
    // https://knowledge.udacity.com/questions/191147
    int upTimePid = UpTime() - procRawUpTime / sysconf(_SC_CLK_TCK);
    return upTimePid;
  }
  return 0;
}