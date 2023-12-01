#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
#include <functional>
#include <set>
#include <queue>
#include <string>
#include <memory>
#include <map>
#include <iomanip>
#include <sstream>
#include <cassert>

const double PI = 3.14;

class Figure
{
public:
    virtual std::string Name() const = 0;
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
};
class Triangle : public Figure
{
public:
    Triangle(int a_, int b_, int c_) : a(a_), b(b_), c(c_) {}
    std::string Name() const override
    {
        return "TRIANGLE";
    }
    double Perimeter() const override
    {
        return a + b + c;
    }

    double Area() const override
    {
        double p = Perimeter() / 2.0;
        return sqrt(p * (p - a) * (p - b) * (p - c));
    }
private:
    int a;
    int b;
    int c;

};
class Rect : public Figure
{
public:
    Rect(int width_, int height_) : width(width_), height(height_) {}
    std::string Name() const override
    {
        return "RECT";
    }
    double Perimeter() const override
    {
        return 2 * (width + height);
    }

    double Area() const  override
    {
        return width * height;
    }
private:
    int width;
    int height;

};
class Circle : public Figure
{
public:
    Circle(int radius_) : radius(radius_) {}
    std::string Name() const override
    {
        return "CIRCLE";
    }
    double Perimeter() const override
    {
        return 2 * PI * radius;
    }

    double Area() const override
    {
        return PI * radius * radius;
    }
private:
    int radius;
};

std::shared_ptr<Figure> CreateFigure(std::istringstream& is)
{
    std::string figure_;
    is >> figure_;
    if (figure_ == "RECT") {
        int width, height;
        is >> width >> height;
        return std::shared_ptr<Rect>(new Rect(width, height));
    }
    else if (figure_ == "TRIANGLE") {
        int a, b, c;
        is >> a >> b >> c;
        return std::shared_ptr<Triangle>(new Triangle(a, b, c));
    }
    else if (figure_ == "CIRCLE") {
        int radius;
        is >> radius;
        return std::shared_ptr<Circle>(new Circle(radius));
    }
    else {
        throw std::runtime_error("Wrong figure name") ;
    }
}



int main() {
    std::vector<std::shared_ptr<Figure>> figures;
    for (std::string line; std::getline(std::cin, line); )
    {
        std::istringstream is(line);
        std::string command;
        is >> command;
        if (command == "ADD")
        {
            is >> std::ws;
            try {
                figures.push_back(CreateFigure(is));
            }
            catch(std::runtime_error& name) {
                std::cout << "Wrong figure name" << std::endl;

            }
        }
        else if (command == "PRINT")
        {
            for (const auto& current_figure : figures)
            {
                std::cout << std::fixed << std::setprecision(3)
                    << current_figure->Name() << " "
                    << current_figure->Perimeter() << " "
                    << current_figure->Area() << std::endl;
            }
        }
    }
    return 0;
}
