#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// "factory" method
void Process::ObtainInfo(int tPid)
{
  SetPid(tPid);
  SetUser(LinuxParser::User(LinuxParser::Uid(tPid)));
  SetRam(LinuxParser::Ram(tPid));
  SetUpTime(LinuxParser::UpTime(tPid));
  SetCmdline(LinuxParser::Command(tPid));
  SetCpuUtilization(LinuxParser::ActiveJiffies(tPid));
}

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { return cpu_; }

string Process::Command() { return cmdline_; }

string Process::Ram() const { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return uptime_; }

bool Process::operator>(Process const& a) const
{
  if ((Ram() == "") || (stoi(Ram()) < stoi(a.Ram())))
    return false;

  return true;
}

bool Process::operator<(Process const& a) const
{
  if (stoi(Ram()) < stoi(a.Ram()))
    return true;
  return false;
}
