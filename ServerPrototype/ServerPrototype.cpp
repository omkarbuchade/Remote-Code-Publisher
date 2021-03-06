//////////////////////////////////////////////////////////////////////////
// ServerPrototype.h - Console App that processes incoming messages     //
// ver 1.1                                                              //
// Author - Omkar Buchade, CSE687 - Object Oriented Design, Spring 2019 //   
// Source - Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018   //
//////////////////////////////////////////////////////////////////////////

#include "ServerPrototype.h"
#include "../FileSystem/FileSystem.h"
#include "../Project1/executive.h"
#include <chrono>

namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

//----< Method to get files in a given directory >----------
Files Server::getFiles(const Repository::SearchPath& path)
{
  return Directory::getFiles(path);
}
//----< Method to get directories in a given path >----------
Dirs Server::getDirs(const Repository::SearchPath& path)
{
  return Directory::getDirectories(path);
}

//----< Method to display message information onto console >----------
template<typename T>
void show(const T& t, const std::string& msg)
{
  std::cout << "\n  " << msg.c_str();
  for (auto item : t)
  {
    std::cout << "\n    " << item.c_str();
  }
}
//----< Procedure to echo the message onto command line >----------
std::function<Msg(Msg)> echo = [](Msg msg) {
  Msg reply = msg;
  reply.to(msg.from());
  reply.from(msg.to());
  return reply;
};
//----< Procedure to get files from a specified directory >----------
std::function<Msg(Msg)> getFiles = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getFiles");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files files = Server::getFiles(searchPath);
    size_t count = 0;
    for (auto item : files)
    {
      std::string countStr = Utilities::Converter<size_t>::toString(++count);
      reply.attribute("file" + countStr, item);
    }
  }
  else
  {
    std::cout << "\n  getFiles message did not define a path attribute";
  }
  return reply;
};
//----< Procedure to get directories inside a specified directory >----------
std::function<Msg(Msg)> getDirs = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getDirs");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files dirs = Server::getDirs(searchPath);
    size_t count = 0;
    for (auto item : dirs)
    {
      if (item != ".." && item != ".")
      {
        std::string countStr = Utilities::Converter<size_t>::toString(++count);
        reply.attribute("dir" + countStr, item);
      }
    }
  }
  else
  {
    std::cout << "\n  getDirs message did not define a path attribute";
  }
  return reply;
};
//----< Procedure to publish (send a request) to convert files in a given directory matched to the given regex and pattern >----------
std::function<Msg(Msg)> publish = [](Msg msg) {
	Msg reply;
	std::vector<std::string> cmdline;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("publish");
	cmdline.push_back("ServerPrototype.exe");
	std::string searchPath = storageRoot;
	std::string path = msg.value("path");
	if (path != ".")
		searchPath = searchPath + "\\" + path;
	cmdline.push_back(searchPath);
	if(msg.value("subDirs")!="")
		cmdline.push_back(msg.value("subDirs"));
	int i = 1;
	while (msg.value("pattern" + std::to_string(i)) != "")
	{
		cmdline.push_back(msg.value("pattern" + std::to_string(i)));
		i++;
	}
	int j = 1;
	while (msg.value("regex" + std::to_string(j)) != "")
	{
		cmdline.push_back(msg.value("regex" + std::to_string(j)));
		j++;
	}
	int argc = cmdline.size();
	char **argv = new char*[argc];
	for (int i = 0; i < argc; i++)
	{
		std::string argument = cmdline[i];
		argv[i] = new char[cmdline[i].size() + 1];
		strcpy_s(argv[i], cmdline[i].size() + 1, argument.c_str());
	}
	executive ExecutiveObj;
	std::vector<std::string> RegexMatchedFiles=ExecutiveObj.exec(argc, argv);
	delete[] argv;
	if(RegexMatchedFiles.size()==0)
		reply.attribute("error", "No files matched");
	size_t count = 0;
	for (auto item : RegexMatchedFiles)
		{
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("file" + countStr,  item);
		}
	return reply;
};

//----< Procedure to download file to a specified directory at the client side>----------
std::function<Msg(Msg)> download = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command(msg.command());
	reply.attributes()["file"] = msg.value("name");
	reply.attributes()["path"] = msg.value("Downloadpath");
	return reply;
};

//----< Procedure to get converted files from the server >----------
std::function<Msg(Msg)> getPrevConvFiles = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getPrevConvFiles");
	std::string path = msg.value("path");
	if (path != "")
	{
		std::string searchPath = storageRoot;
		if (path != ".")
			searchPath = searchPath + "\\" + path;
		Files files = Server::getFiles(searchPath);
		size_t count = 0;
		for (auto item : files)
		{
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("file" + countStr, item);
		}
	}
	else
	{
		std::cout << "\n  getFiles message did not define a path attribute";
	}
	return reply;
};


int main(int argc, char *argv[])
{
  const MsgPassingCommunication::EndPoint serverEndPoint(argv[1], std::stoi(argv[2]));  // listening endpoint
  storageRoot = argv[3];  // root for all server file storage
  std::cout<<"\nRequirement 3: This project assemble working parts from Projects #1, #2, and #3 into a Client-Server configuration. Unlike conventional Client-Server architectures, the Client need not wait for a reply from the Publisher server for a request before sending additional requests.";
  //Setting up the server. Starting and adding procedures
  Server server(serverEndPoint, "ServerPrototype");
  server.start();
  server.addMsgProc("echo", echo);
  server.addMsgProc("getFiles", getFiles);
  server.addMsgProc("getDirs", getDirs);
  server.addMsgProc("serverQuit", echo);
  server.addMsgProc("serverQuit", echo);
  server.addMsgProc("publish", publish);
  server.addMsgProc("download",download);
  server.addMsgProc("getPrevConvFiles", getPrevConvFiles);
  server.processMessages();
  
  std::cout << "\n  press enter to exit";
  std::cin.get();
  std::cout << "\n";
  server.stop();
  return 0;
}

