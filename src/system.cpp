#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  processes_ = {};
  for (int pi : pids) {
    procmap.insert(std::pair(pi, Process(pi)));
  }

  // Since I'm constructing the list that will be returned in this fn
  // and since it looks harder to delete
  // what I do instead is: use a reverse iterator over the processes
  // and if the Processe's pid is in the list of pids, I add it to the outgoing
  // result. so technically the old processes won't get deleted until the
  // process exits.. I think that's ok in this case
  // using a reverse iterator because HTOP shows processes in reverse by default
  std::map<int, Process>::reverse_iterator rit;
  for (rit = procmap.rbegin(); rit != procmap.rend(); ++rit) {
    if (std::find(pids.begin(), pids.end(), rit->first) != pids.end()) {
      processes_.push_back(rit->second);
    }
  }
  return processes_;
}

// [X] TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

// [X] TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// [X] TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// [X] TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// [X] TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }