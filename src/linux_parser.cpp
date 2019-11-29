#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, versionStr, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> versionStr >> kernel;
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

// TODO: Read and return the system memory utilization
// from linux utility free, total memory = used + free + buff/cache
float LinuxParser::MemoryUtilization()
{
  string line;
  string key;
  string value;
  string units;
  short foundValues = 0;
  float totalMem = 0.0;
  float freeMem = 0.0;
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (filestream.is_open())
    {
      while ((std::getline(filestream, line)) && (foundValues < 2))
	{
	  std::istringstream linestream(line);
	  while (linestream >> key >> value >> units)
	    {
	      if (key == "MemTotal:")
		{
		  std::string::size_type sz;   // alias of size_t
		  totalMem = std::stof(value,&sz);
		  foundValues++;
		}
	      else if (key == "MemFree:")
		{
		  std::string::size_type sz;   // alias of size_t
		  freeMem = std::stof(value,&sz);
		  foundValues++;
		}
	    }
	}
      if (foundValues == 2)
	{
	  return (totalMem - freeMem) / totalMem;
	}

    }
  return 0.0;
}


// TODO: Read and return the system uptime
long LinuxParser::UpTime()
{
  string upTimeInSeconds, idleTimeInSeconds;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open())
    {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> upTimeInSeconds >> idleTimeInSeconds;

      // convert upTimeInSeconds to long
      std::string::size_type sz;   // alias of size_t

      return std::stol(upTimeInSeconds,&sz);
    }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open())
    {
      while (std::getline(filestream, line))
	{
	  std::istringstream linestream(line);
	  while (linestream >> key >> value) {
	    if (key == "processes")
	      {
		std::string::size_type sz;   // alias of size_t
		return std::stoi(value,&sz);
	      }
	  }
	}
    }

  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open())
    {
      while (std::getline(filestream, line))
	{
	  std::istringstream linestream(line);
	  while (linestream >> key >> value) {
	    if (key == "procs_running")
	      {
		std::string::size_type sz;   // alias of size_t
		return std::stoi(value,&sz);
	      }
	  }
	}
    }

  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]])
{
  return 0;
}
