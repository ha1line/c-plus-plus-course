#include "database.h"

#include <iostream>

void Database::AddEvent(const Date& date, const std::string& event) {
	events_[date].insert( event );
}

bool Database::DeleteEvent(const Date& date, const std::string& event) {
	if(const auto it = events_.find( date ); it != events_.end( )) {
		return it->second.erase( event ) != 0;
	}
	return false;
}

int Database::DeleteDate(const Date& date) {
	if(const auto it = events_.find( date ); it != events_.end( )) {
		const int size = static_cast<int>(it->second.size( ));
		events_.erase( it );
		return size;
	}
	return 0;
}

const Events& Database::Find(const Date& date) const {
	if(const auto it = events_.find( date ); it != events_.end( )) {
		return it->second;
	}
	static Events empty;
	return empty;
}

void Database::Print() const {
	for( const auto &[date, events] : events_ ) {
		for( const auto & event : events ) {
			std::cout << date << ' ' << event << '\n';
		}
	}
}