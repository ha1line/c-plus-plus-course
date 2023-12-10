#include <iostream>

#include "src/database.h"
#include "src/commands.h"

std::tuple<Date, std::string> ParseDateAndEvent(std::stringstream &stream) {
	const auto date = Date(stream);
	std::string event;
	stream >> event;
	return {date, event};
}

int main() {
	Database db;
	Commands commands;

	commands.RegCommand( "Add",[&db](std::stringstream &stream) {
		const auto [date, event] = ParseDateAndEvent( stream );
		db.AddEvent( date, event );
	});

	commands.RegCommand( "Del",[&db](std::stringstream &stream) {
		const auto [date, event] = ParseDateAndEvent( stream );
		if( !event.empty(  )) {
			if( db.DeleteEvent( date, event ) ) {
				std::cout << "Deleted successfully" << '\n';
			} else {
				std::cout << "Event not found" << '\n';
			}
		} else {
			std::cout << "Deleted " << db.DeleteDate( date ) << " events" << '\n';
		}
	});

	commands.RegCommand( "Find",[&db](std::stringstream &stream) {
		for( const auto &e : db.Find( Date{stream} ) ) {
			std::cout << e << '\n';
 		}
	});

	commands.RegCommand( "Print",[&db](std::stringstream &stream) {
		db.Print(  );
	});

	try {
		std::string commandLine;
		while (std::getline(std::cin, commandLine)) {
			if( commandLine.empty(  ) ) {
				continue;
			}
			std::stringstream input(commandLine);
			std::string operation; input >> operation;
			if( const auto callback = commands.FindCallback(operation); callback ) {
				callback(input);
			} else {
				throw std::runtime_error( "Unknown command: " + std::string(operation) );
			}
		}
	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
