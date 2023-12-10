#include "date.h"

#include <iomanip>

Date::Date(const int year,const int month,const int day) :
	year_(year), month_(month), day_(day) {

	if( !(month_ >= 1 && month_ <= 12) ) {
		throw std::runtime_error( "Month value is invalid: " + std::to_string(month_) );
	}
	if( !(day_ >= 1 && day_ <= 31) ) {
		throw std::runtime_error( "Day value is invalid: " + std::to_string(day_) );
	}
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
	os	<< std::setw(4) << std::setfill('0') << date.year_ << '-'
		<< std::setw(2) << std::setfill('0') << date.month_ << '-'
		<< std::setw(2) << std::setfill('0') << date.day_;
	return os;
}

bool operator<(const Date& lhs, const Date& rhs) {
	return std::tie(lhs.year_, lhs.month_, lhs.day_) <
		   std::tie(rhs.year_, rhs.month_, rhs.day_);
}

bool readValue(std::stringstream &stream, int &outvar) {
	if( !(stream >> outvar) ) {
		return false;
	}
	if( !stream.eof(  ) && stream.peek() != '-' ) {
		return false;
	}
	stream.ignore(1);
	return true;
}

Date ParseDate(std::stringstream & stream) {
	std::string input;
	stream >> input;

	std::stringstream tempstream(input);

	int year = 0, month = 0, day = 0;
	bool okay =	readValue(tempstream, year) &&
				readValue(tempstream, month) &&
				readValue(tempstream, day);

	if( !okay ) {
		throw std::runtime_error("Wrong date format: " + input);
	}

	return {year, month, day};
}