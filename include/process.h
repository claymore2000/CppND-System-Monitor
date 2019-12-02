#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  void SetPid(int pid) { pid_ = pid; }
  std::string User();                      // TODO: See src/process.cpp
  void SetUser(std::string user) { user_ = user; }
  std::string Command();                   // TODO: See src/process.cpp
  void SetCmdline(std::string command) { cmdline_ = command; }
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram() const;                       // TODO: See src/process.cpp
  void SetRam(std::string ram) { ram_ = ram; } 
  long int UpTime();
  void SetUpTime(long uptime) { uptime_ = uptime; } // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  std::string ram_;
  long uptime_;
  std::string cmdline_;
};

#endif
