#ifndef COMMANDS_H
#define COMMANDS_H
#pragma once

#include <functional>
#include <sstream>
#include <map>

class Commands {
public:
	using CommandCallback = std::function<void(std::stringstream&)>;

	CommandCallback FindCallback(std::string_view command) const;
	void RegCommand(std::string_view command, CommandCallback&& callback);
private:
	std::map<std::string, CommandCallback, std::less<>> commands;
};

#endif //COMMANDS_H
