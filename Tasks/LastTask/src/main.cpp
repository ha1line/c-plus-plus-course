#include <iostream>

#include "database.h"
#include "commands.h"

std::tuple<Date, std::string> ParseDateAndEvent(std::stringstream &stream) {
	const auto date = ParseDate(stream);
	std::string event;
	stream >> event;
	return {date, event};
}

Database db;
Commands commands;

void AddDateCallback(std::stringstream &stream) {
	const auto [date, event] = ParseDateAndEvent( stream );
	db.AddEvent( date, event );
}

void DelDateCallback(std::stringstream &stream) {
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
}

void FindDateCallback(std::stringstream &stream) {
	for( const auto &e : db.Find( ParseDate(stream) ) ) {
		std::cout << e << '\n';
	}
}

void PrintDatesCallback(std::stringstream &) {
	db.Print(  );
}

int main() {
	commands.RegCommand( "Add", AddDateCallback);
	commands.RegCommand( "Del", DelDateCallback);
	commands.RegCommand( "Find", FindDateCallback);
	commands.RegCommand( "Print", PrintDatesCallback);

	try {
		std::string commandLine;
		while (std::getline(std::cin, commandLine)) {
			commands.DispatchCallback(commandLine);
		}
	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
