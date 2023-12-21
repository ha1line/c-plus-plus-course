#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <cmath>
#include <iomanip>

class Figure {
public:
    virtual ~Figure() = default;
    virtual std::string Name() const = 0;
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
};

class Rect : public Figure {
public:
    Rect(double width, double height) : _width(width), _height(height) {}
    std::string Name() const override {return "RECT";}
    double Perimeter() const override {return 2*(_width + _height);}
    double Area() const override {return _width*_height;}
private:
    double _width;
    double _height;

};

class Triangle : public Figure {
public:
    Triangle(double a, double b, double c): _a(a), _b(b), _c(c) {}
    std::string Name() const override {return "TRIANGLE";}
    double Perimeter() const override {return _a + _b + _c;}
    double Area() const override {
        double p = (_a + _b + _c) / 2;
       return std::sqrt(p * (p - _a) * (p - _b) * (p - _c));
    }
private:
    double _a;
    double _b;
    double _c;
};


class Circle : public Figure {
public:
    Circle(double r): _r(r) {}
    std::string Name() const override {return "CIRCLE";}
    double Perimeter() const override {return 2*_pi*_r;}
    double Area() const override {return _pi*_r*_r;}
private:
    double _r;
    double const _pi = 3.14;
};


std::shared_ptr<Figure> CreateFigure(std::istream& is) {
    std::string type;
    is >> type;
    if (type == "RECT") {
        double width, height;
        is >> width >> height;
        return std::make_shared<Rect>(width, height);
    }
    else if (type == "TRIANGLE") {
       double a, b, c;
       is >> a >> b >> c;
       return std::make_shared<Triangle>(a, b, c);
    } 
    else if (type == "CIRCLE") {
       double r;
       is >> r;
       return std::make_shared<Circle>(r);
    } 
    else throw std::runtime_error("Wrong figure name");
    return nullptr;
}


int main() {
  std::vector<std::shared_ptr<Figure>> figures;
  for (std::string line; std::getline(std::cin, line); ) {
    std::istringstream is(line);
 
    std::string command;
    is >> command;
    if (command == "ADD") {
      is >> std::ws;
      try 
      {
        figures.push_back(CreateFigure(is));
      }
      catch(std::runtime_error e) 
      {
        std::cout << e.what() << std::endl;
      }
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
