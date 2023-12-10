#include "commands.h"

void Commands::DispatchCallback(const std::string & input) {
	if( input.empty(  ) ) {
		return;
	}
	std::stringstream input_stream(input);

	std::string command;
	input_stream >> command;

	const auto it = commands.find( command );
	if( it == commands.end()) {
		throw std::runtime_error( "Unknown command: " + command );
	}

	it->second(input_stream);
}

void Commands::RegCommand(std::string command, CommandCallback && callback) {
	commands[std::move(command)] = std::move(callback);
}