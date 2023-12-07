#ifndef DATE_H
#define DATE_H
#pragma once

#include <sstream>

class Date {
public:
	explicit Date(std::stringstream & stream);

	int GetYear() const {
		return year_;
	}

	int GetMonth() const {
		return month_;
	}

	int GetDay() const {
		return day_;
	}

	friend std::ostream& operator<<(std::ostream& os, const Date& date);
	friend bool operator<(const Date& lhs, const Date& rhs);

private:
	int year_;
	int month_;
	int day_;
};

#endif //DATE_H
