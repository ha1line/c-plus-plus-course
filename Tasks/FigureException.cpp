#include<iostream>
#include<string>
#include<iomanip>
#include<vector>
#include<memory>
#include<cmath>
#include <exception>
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
    Rect(double width,double height) : width(width),height(height)
    {       
    }

    std::string Name() const final
    {
        return "RECT";
    }
    double Perimeter() const final
    {
        return 2*(width+height);
    }
    double Area() const final
    {
        return width*height;
    }
private:
    double width;
    double height;
};

class Triangle : public Figure
{
public:
    Triangle(double a,double b,double c) : a(a),b(b),c(c)
    {       
    }
    std::string Name() const final
    {
        return "TRIANGLE";
    }
    double Perimeter() const final
    {
		return a+b+c;
    }
    double Area() const final
    {
        return std::sqrt(((a + b + c) / 2)*(((a + b + c) / 2)-a)*(((a + b + c) / 2)-b)*(((a + b + c) / 2)-c));
    }
private:
    double a;
    double b;
    double c;
};

class Circle : public Figure
{
public:
    Circle(double r) : r(r)
    {       
    }
    std::string Name() const final
    {
        return "CIRCLE";
    }
    double Perimeter() const final
    {
        return 3.14 * r * 2;
    }
    double Area() const final
    {
        return 3.14 * r * r;
    }
private:
    double r;
};

std::shared_ptr<Figure>  CreateFigure(std::istringstream& stream)
{
    std::string nameCommand;
    stream >> nameCommand;
    if (nameCommand=="RECT")
    {
        double width,height;
        stream >>width>>height;
        return std::make_shared<Rect>(Rect(width,height));
    }
    else if (nameCommand=="TRIANGLE")
    {
        double a,b,c;
        stream >>a>>b>>c;
        return std::make_shared<Triangle>(Triangle(a,b,c));
    }
    else if (nameCommand=="CIRCLE")
    {
        double r;
        stream >>r;
        return std::make_shared<Circle>(Circle(r));
    }
    else
    {
        throw std::runtime_error("Wrong figure name");
    }
}

int main() {
  std::vector<std::shared_ptr<Figure>> figures;
  try
  {
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
  }
  catch (const std::runtime_error& e)
  {
    std::cout << e.what() << '\n';
    }
  return 0;
}