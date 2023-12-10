#ifndef DATE_H
#define DATE_H
#pragma once

#include <sstream>

class Date {
public:
	Date(int year, int month, int day);

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
	const int year_;
	const int month_;
	const int day_;
};

Date ParseDate(std::stringstream & stream);

#endif //DATE_H
