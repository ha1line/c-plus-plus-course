#include <iomanip>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <math.h>

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
    Triangle(double a, double b, double c) : m_a(a),
                                             m_b(b),
                                             m_c(c)
    {
    }

    std::string Name() const final
    {
        return "TRIANGLE";
    }

    double Perimeter() const final
    {
        return m_a + m_b + m_c;
    }
    double Area() const final
    {
        double p = (m_a + m_b + m_c) / 2;
        return sqrt(p * (p - m_a) * (p - m_b) * (p - m_c));
    }

private:
    const double m_a;
    const double m_b;
    const double m_c;
};

class Rectangle : public Figure
{
public:
    Rectangle(double a, double b) : m_a(a),
                                    m_b(b)
    {
    }

    std::string Name() const final
    {
        return "RECT";
    }

    double Perimeter() const final
    {
        return 2 * m_a + 2 * m_b;
    }

    double Area() const final
    {
        return double(m_a * m_b);
    }

private:
    const double m_a;
    const double m_b;
};

class Circle : public Figure
{
public:
    Circle(double radius) : m_radius(radius)  
    {
    }

    std::string Name() const final
    {
        return "CIRCLE";
    }

    double Perimeter() const final
    {
        return 2 * m_pi * m_radius;
    }

    double Area() const final
    {
        return m_radius * m_radius * m_pi;
    }

private:
    const double m_radius;
    const double m_pi = 3.14;
};

std::shared_ptr<Figure> CreateFigure(std::istringstream& is)
{
    std:: string figure;
    is >> figure;
    std::cout << figure;
    if (figure == "RECT")
    {
        double a;
        double b;
        is >> a >> b;
        return std::make_shared<Rectangle>(a, b);
    }

    else if (figure == "TRIANGLE")
    {
        double a;
        double b;
        double c;
        is >> a >> b >> c;
        return std::make_shared<Triangle>(a,b,c);
    }

    else if (figure == "CIRCLE")
    {
        double radius;
        is >> radius;
        return std::make_shared<Circle>(radius);
    }
}

int main()
{
    
    std::vector<std::shared_ptr<Figure>> figures;
    for (std::string line; std::getline(std::cin, line);)
    {
        std::istringstream is(line);
        std::cout << is.beg;
        std::string command;
        is >> command;
        if (command == "ADD")
        {
            // Файловый манипулятор для пропуска ведущих пробелов
            // https://en.cppreference.com/w/cpp/io/manip/ws
            is >> std::ws;
            figures.push_back(CreateFigure(is));
        }
        else if (command == "PRINT")
        {
            for (const auto &current_figure : figures)
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
