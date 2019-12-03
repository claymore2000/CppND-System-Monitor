#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>

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
vector<int> LinuxParser::Pids()
{
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR)
      {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit))
	{
	  int pid = stoi(filename);
	  pids.push_back(pid);
	}
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
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
  float inactive = 0.0;
  float inactivefile = 0.0;
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (filestream.is_open())
    {
      std::string::size_type sz;   // alias of size_t
      while ((std::getline(filestream, line)) && (foundValues < 2))
	{
	  std::istringstream linestream(line);
	  while (linestream >> key >> value >> units)
	    {
	      if (key == "MemTotal:")
		{
		  totalMem = std::stof(value,&sz);
		  foundValues++;
		}
	      else if (key == "MemFree:")
		{
		  freeMem = std::stof(value,&sz);
		  foundValues++;
		}
	      else if (key == "Inactive:")
		{
		  inactive = std::stof(value,&sz);
		}
	      else if (key == "Inactive(file):")
		{
		  inactivefile = std::stof(value,&sz);
		}
	    }
	}
      if (foundValues == 2)
	{
	  return (totalMem - freeMem - inactive - inactivefile) / totalMem;
	}

    }
  return 0.0;
}


//  Read and return the system uptime
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

//  TODO Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Read and return the number of active jiffies for a PID

float LinuxParser::ActiveJiffies(int pid)
{
  string line;
  std::string pidAsStringDir = std::to_string(pid) + "/";
  std::ifstream filestream(kProcDirectory + pidAsStringDir + kStatFilename);

  if (filestream.is_open())
    {
      vector<string> tokens;
      while (std::getline(filestream, line))
	{
	  std::stringstream stat_line(line);
	  std::string token_of_stat_line;
	  
	  while(getline(stat_line, token_of_stat_line, ' ')) 
	    {
	      tokens.push_back(token_of_stat_line);
	    }
	}
      
      if (tokens.size() == 52) // /proc/[pid]/stat better have 52 tokens or else a problem ...
	{ // local variables made for readability, see man proc for details about accessed variables
	  long uptime = LinuxParser::UpTime();
	  float totaltime = (stof(tokens.at(13)) + stof(tokens.at(14)) + stof(tokens.at(15)) + stof(tokens.at(16)));
	  float totaltimeInSeconds = totaltime / sysconf(_SC_CLK_TCK);
	  float secondsUpTime = uptime - (stof(tokens.at(21)) / sysconf(_SC_CLK_TCK));
	  float cpu_usage = (totaltimeInSeconds) / secondsUpTime;

	  return cpu_usage;

	}
    } 
  return 0.0;  // returned if problem
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<std::string> LinuxParser::CpuUtilization()
{
  string line;
  string key;
  string user;
  string nice;
  string system;
  string idle;
  string iowait;
  string irq;
  string softirq;
  string steal;
  string guest;
  string guestnice;

  std::vector<std::string> cpuUtilization(10);
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open())
    {
      while (std::getline(filestream, line))
	{
	  std::istringstream linestream(line);

	  while (linestream >> key >> user >> nice >> system >> idle 
		 >> iowait >> irq >> softirq >> steal >> guest >> guestnice)
	    {
	      if (key == "cpu")
		{
		  cpuUtilization.at(kUser_) = user;
		  cpuUtilization.at(kNice_) = nice;
		  cpuUtilization.at(kSystem_) = system;
		  cpuUtilization.at(kIdle_) = idle; 
		  cpuUtilization.at(kIOwait_) = iowait;
		  cpuUtilization.at(kIRQ_) = irq; 
		  cpuUtilization.at(kSoftIRQ_) = softirq;
		  cpuUtilization.at(kSteal_) = steal;
		  cpuUtilization.at(kGuest_) = guest;
		  cpuUtilization.at(kGuestNice_) = guestnice;
		  return cpuUtilization;
		}
	    }
	}
    }

  return {};
}

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
string LinuxParser::Command(int pid[[maybe_unused]])
{
  string line;
  std::string pidAsStringDir = std::to_string(pid) + "/";
  std::ifstream filestream(kProcDirectory + pidAsStringDir + kCmdlineFilename);

  if (filestream.is_open())
    {
      while (std::getline(filestream, line))
	{
	  filestream.close();
	  return line;
	}
    }
  return "N/A";
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]])
{
  string line;
  string key;
  string mem;
  std::string pidAsStringDir = std::to_string(pid) + "/";
  std::ifstream filestream(kProcDirectory + pidAsStringDir + kStatusFilename);

  // std::string notAvailable{"N/A"};
  
  if (filestream.is_open())
    {
      while (std::getline(filestream, line))
	{
	  std::istringstream linestream(line);
	  while (linestream >> key >> mem)
	    {
	      if (key == "VmSize:")
		{
		  int memMB = std::stoi(mem) / 1000;
		  return std::to_string( memMB );
		}
	    }
	}
    }
	  
  return "0"; // notAvailable;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]])
{
  string line;
  string key;
  string uid;
  std::string pidAsStringDir = std::to_string(pid) + "/";
  std::ifstream filestream(kProcDirectory + pidAsStringDir + kStatusFilename);

  if (filestream.is_open())
    {
      while (std::getline(filestream, line))
	{
	  std::istringstream linestream(line);
	  while (linestream >> key >> uid)
	    {
	      if (key == "Uid:")
		{
		  return uid;
		}
	    }
	}
    }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(std::string uid[[maybe_unused]])
{
  string line;
  string uid_read;
  string dont_care;
  string user_name;
  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open())
    {
      while (std::getline(filestream, line))
	{
	  std::replace(line.begin(), line.end(), ':', ' ');
	  std::istringstream linestream(line);
	  while (linestream >> user_name >> dont_care >> uid_read)
	    {
	      if (uid_read == uid)
		{
		  return user_name;
		}
	    }
	}
    }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{
  string line;
  std::string pidAsStringDir = std::to_string(pid) + "/";
  std::ifstream filestream(kProcDirectory + pidAsStringDir + kStatFilename);

  if (filestream.is_open())
    {
      while (std::getline(filestream, line))
	{
	  std::stringstream stat_line(line);
	  std::string token_of_stat_line;
	  vector<string> tokens;
	  
	  while(getline(stat_line, token_of_stat_line, ' ')) 
	    {
	      tokens.push_back(token_of_stat_line);
	    }
	  return stol(tokens.at(21))/sysconf(_SC_CLK_TCK);
	}
    }
  return 0;
}
