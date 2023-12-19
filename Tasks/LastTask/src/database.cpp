#include "database.h"

#include <iostream>

void Database::AddEvent(const Date& date, const std::string& event) {
	events_[date].insert( event );
}

bool Database::DeleteEvent(const Date& date, const std::string& event) noexcept {
	if(const auto it = events_.find( date ); it != events_.end( )) {
		return it->second.erase( event ) != 0;
	}
	return false;
}

size_t Database::DeleteDate(const Date& date) noexcept {
	if(const auto it = events_.find( date ); it != events_.end( )) {
		const auto size = it->second.size( );
		events_.erase( it );
		return size;
	}
	return 0;
}

Events Database::Find(const Date& date) const noexcept {
	if(const auto it = events_.find( date ); it != events_.end( )) {
		return it->second;
	}
	return {};
}

void Database::Print() const noexcept {
	for( const auto &[date, events] : events_ ) {
		for( const auto & event : events ) {
			std::cout << date << ' ' << event << '\n';
		}
	}
}