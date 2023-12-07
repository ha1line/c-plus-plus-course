#ifndef DATABASE_H
#define DATABASE_H
#pragma once

#include "date.h"

#include <map>
#include <set>

using Events = std::set<std::string>;

class Database {
public:
	void AddEvent(const Date& date, const std::string& event);
	bool DeleteEvent(const Date& date, const std::string& event);
	int DeleteDate(const Date& date);
	const Events& Find(const Date& date) const;
	void Print() const;
private:
	std::map<Date, Events> events_;
};

#endif //DATABASE_H
