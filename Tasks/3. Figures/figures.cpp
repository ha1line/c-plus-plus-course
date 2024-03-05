#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <cmath>

class Figure
{
public:
    virtual std::string Name() const = 0;
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
};

class Rect : public Figure
{
public:
    Rect(int _width, int _height) : 
    width(_width),
    height(_height)
    {
    }

    std::string Name() const final
    {
        return "RECT";
    }

    double Perimeter() const final
    {
        return width * 2 + height * 2;
    }

    double Area() const final
    {
        return width * height;
    }

private:
    int width;
    int height;
};

class Triangle : public Figure
{
public:
    Triangle(int _a, int _b, int _c) : 
    a(_a), 
    b(_b), 
    c(_c)
    {
    }

    std::string Name() const final
    {
        return "TRIANGLE";
    }

    double Perimeter() const final
    {
        return a + b + c;
    }

    double Area() const final
    {
        double pp = Perimeter() / 2.0;
        return std::sqrt(pp * (pp - a) * (pp - b) * (pp - c));
    }

private:
    int a;
    int b;
    int c;
};

class Circle : public Figure
{
public:
    Circle(int _r) : 
    r(_r)
    {
    }

    std::string Name() const final
    {
        return "CIRCLE";
    }

    double Perimeter() const final
    {
        return 2 * PI * r;
    }

    double Area() const final
    {
        return PI * std::pow(r, 2);
    }

private:
    int r;
    const double PI = 3.14;
};

std::shared_ptr<Figure> CreateFigure(std::istringstream& stream)
{
    std::string figureName;
    stream >> figureName;
    std::shared_ptr<Figure> figure;
    if (figureName == "RECT")
    {
        int width, height;
        stream >> width >> height;
        figure = std::make_shared<Rect>(Rect(width, height));
    }
    else if (figureName == "TRIANGLE")
    {
        int a, b, c;
        stream >> a >> b >> c;
        figure = std::make_shared<Triangle>(Triangle(a, b, c));
    }
    else if (figureName == "CIRCLE")
    {
        int r;
        stream >> r;
        figure = std::make_shared<Circle>(Circle(r));
    }
    return figure;
};

int main()
{
    std::vector<std::shared_ptr<Figure>> figures;
    for (std::string line; std::getline(std::cin, line);)
    {
        std::istringstream is(line);
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