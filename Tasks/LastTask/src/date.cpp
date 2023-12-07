#include "date.h"

#include <iomanip>
#include <tuple>

Date::Date(std::stringstream & stream) {
	const auto readValue = [](std::stringstream & stream){
		int value;
		stream >> value;
		if( stream.fail(  ) || (!stream.eof() && stream.peek() != '-') ) {
			throw std::runtime_error("");
		}
		stream.ignore(1);
		return value;
	};

	std::string input;
	stream >> input;

	std::stringstream tempstream(input);
	try {
		year_ = readValue(tempstream);
		month_ = readValue(tempstream);
		day_ = readValue(tempstream);
		if( !tempstream.eof() ) {
			throw std::runtime_error("");
		}
	} catch( ... ) {
		throw std::runtime_error("Wrong date format: " + input);
	}

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