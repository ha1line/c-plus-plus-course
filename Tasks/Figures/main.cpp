#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>
#include <cmath>

class Figure
{
public:
	virtual std::string Name() = 0;

	virtual double Perimeter() = 0;

	virtual double Area() = 0;
};


class Triangle: public Figure
{
public:
	Triangle(double a, double b, double c): a(a), b(b), c(c)
	{}

	std::string Name() override
	{
		return "TRIANGLE";
	}

	double Perimeter() override
	{
		return a + b + c;
	}

	double Area() override
	{
		double p = (a + b + c) / 2;
		return std::sqrt(p * (p - a) * (p - b) * (p - c));
	}

private:
	double a;
	double b;
	double c;
};

class Rect: public Figure
{
public:
	Rect(double a, double b) : a(a), b(b)
	{}

	std::string Name() override
	{
		return "RECT";
	}

	double Perimeter() override
	{
		return 2 * a + 2 * b;
	}

	double Area() override
	{
		return a * b;
	}

private:
	double a;
	double b;
};

class Circle: public Figure
{
public:
	Circle(const double& r): r(r)
	{}

	std::string Name() override
	{
		return "CIRCLE";
	}

	double Perimeter() override
	{
		return 2 * 3.14 * r;
	}

	double Area() override
	{
		return 3.14 * r * r;
	}

private:
	double r;
};

std::shared_ptr<Figure> CreateFigure (std::istringstream& is)
{
	std::string figure;
	is >> figure;
	if (figure == "CIRCLE") {
		double r;
		is >> r;
		return std::make_shared<Circle>(Circle(r));
	} else if (figure == "RECT"){
		double a;
		double b;
		is >> a;
		is >> b;
		return std::make_shared<Rect>(Rect(a, b));
	} else {
		double a;
		double b;
		double c;
		is >> a;
		is >> b;
		is >> c;
		return std::make_shared<Triangle>(Triangle(a, b, c));
	}
}

int main() {
  std::vector<std::shared_ptr<Figure>> figures;
  for (std::string line; std::getline(std::cin, line); ) {
	std::istringstream is(line);

	std::string command;
	is >> command;
	if (command == "ADD") {
	  // Файловый манипулятор для пропуска ведущих пробелов
	  // https://en.cppreference.com/w/cpp/io/manip/ws
	  is >> std::ws;
	  figures.push_back(CreateFigure(is));
	} else if (command == "PRINT") {
	  for (const auto& current_figure : figures) {
		std::cout << std::fixed << std::setprecision(3)
			<< current_figure->Name() << " "
			<< current_figure->Perimeter() << " "
			<< current_figure->Area() << std::endl;
	  }
	}
  }
  return 0;
}
