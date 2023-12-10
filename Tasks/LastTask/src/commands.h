#ifndef COMMANDS_H
#define COMMANDS_H
#pragma once

#include <functional>
#include <sstream>
#include <map>

class Commands {
public:
	using CommandCallback = std::function<void(std::stringstream&)>;

	void DispatchCallback(const std::string & input);
	void RegCommand(std::string command, CommandCallback&& callback);
private:
	std::map<std::string, CommandCallback, std::less<>> commands;
};

#endif //COMMANDS_H
