#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include <limits>
#include <iomanip>
#include <math.h>
#include <cstdint>
#include <exception>

class Figure {
public:
    virtual std::string Name() const = 0;
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
};

class Triangle : public Figure {
public:
    Triangle(uint16_t a, uint16_t b, uint16_t c) : m_a(a), m_b(b), m_c(c) {}
    
    std::string Name() const override {
        return "TRIANGLE";
    }

    double Perimeter() const override {
        return m_a + m_b + m_c;
    }

    double Area() const override {
        double polyper = Perimeter() / 2.0;
        return sqrt(polyper * (polyper - m_a) * (polyper - m_b) * (polyper - m_c));
    }
    
private:
    const uint16_t m_a;
    const uint16_t m_b;
    const uint16_t m_c;
};

class Circle : public Figure {
public:
    Circle(uint16_t r) : m_r(r) {}
    
    std::string Name() const override {
        return "CIRCLE";
    }

    double Perimeter() const override {
        return 2 * pi * m_r;
    }

    double Area() const override {   
        return pi * m_r * m_r;
    }
    
private:
    const uint16_t m_r;
    const double pi = 3.14;
};

class Rectangle : public Figure {
public:
    Rectangle(uint16_t height, uint16_t width) : m_height(height), m_width(width) {}
    
    std::string Name() const override {
        return "RECT";
    }

    double Perimeter() const override {
        return (m_height + m_width) * 2;
    }

    double Area() const final {
        return m_height * m_width;
    }
    
private:
    const uint16_t m_height;
    const uint16_t m_width;
};

std::shared_ptr<Figure> CreateFigure(std::istream& is){
    std::string figure;
    is >> figure;
    is.ignore(1);

    if (figure == "TRIANGLE"){
        uint16_t a;
        uint16_t b;
        uint16_t c;
        
        is >> a;
        is.ignore(1);
        is >> b;
        is.ignore(1);
        is >> c;

        return std::make_shared<Triangle> (a, b, c);
    }
    else if (figure == "CIRCLE"){
        uint16_t r;
        
        is >> r;

        return std::make_shared<Circle> (r);
    }
    else if (figure == "RECT"){
        uint16_t height, width;
        
        is >> height;
        is.ignore(1);
        is >> width;

        return std::make_shared<Rectangle> (height, width);
    }
    else 
        throw std::runtime_error("Wrong figure name");
    
    return nullptr;
}

int main()
{
    std::vector<std::shared_ptr<Figure>> figures;
    for (std::string line; std::getline(std::cin, line);)
    {
        std::istringstream is(line);

        std::string command;
        is >> command;

        try{
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
        catch (const std::runtime_error& e){
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}