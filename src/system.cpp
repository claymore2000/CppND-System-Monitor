#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes()
{
  vector<int> currentPids = LinuxParser::Pids();
  size_t currentPidsSize = currentPids.size();
  processes_.clear();

  for (unsigned int i = 0; i < currentPidsSize; i++)
    {
      Process p;

      int tPid = currentPids.at(i);

      p.SetPid(tPid);
      p.SetUser(LinuxParser::User(LinuxParser::Uid(tPid)));
      p.SetRam(LinuxParser::Ram(tPid));
      p.SetUpTime(LinuxParser::UpTime(tPid));
      p.SetCmdline(LinuxParser::Command(tPid));

      processes_.emplace_back(p);

    }
  sort(processes_.begin(),processes_.end());
  std::reverse(processes_.begin(), processes_.end());

  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime();  }
