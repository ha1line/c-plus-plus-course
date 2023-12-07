#include "commands.h"

Commands::CommandCallback Commands::FindCallback(const std::string_view command) const {
	if(const auto it = commands.find( command ); it != commands.end()) {
		return it->second;
	}
	return {};
}
void Commands::RegCommand(const std::string_view command, CommandCallback && callback) {
	commands[std::string(command)] = std::move(callback);
}
