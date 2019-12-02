#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { return cpu_; }

string Process::Command() { return cmdline_; }

string Process::Ram() const { return ram_; }

string Process::User() { return user_; }

long int Process::UpTime() { return uptime_; }

bool Process::operator<(Process const& a) const
{
  if (stoi(Ram()) < stoi(a.Ram()))
    return true;
  return false;
}
