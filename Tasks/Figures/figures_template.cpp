#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <cmath>

class Figure {
public:
    virtual std::string Name() {
        return "FIGURE";
    }

    virtual double Perimeter() {
        return 0;
    }

    virtual double Area() {
        return 0;
    }
};

class Rect : public Figure {
private:
    int width, height;

public:
    Rect(int new_width, int new_height) {
        width = new_width;
        height = new_height;
    }

    std::string Name() override {
        return "RECT";
    }

    double Perimeter() override {
        return 2 * width + 2 * height;
    }

    double Area() override {
        return width * height;
    }
};

class Triangle : public Figure {
private:
    int a, b, c;

public:
    Triangle(int new_a, int new_b, int new_c) {
        a = new_a;
        b = new_b;
        c = new_c;
    }

    std::string Name() override {
        return "TRIANGLE";
    }

    double Perimeter() override {
        return a + b + c;
    }

    double Area() override {
        double p = Perimeter() / 2;
        return std::sqrt(p * (p - a) * (p - b) * (p - c));
    }
};

class Circle : public Figure {
private:
    int r;
    const double pi = 3.14;

public:
    Circle(int new_r) {
        r = new_r;
    }

    std::string Name() override {
        return "CIRCLE";
    }

    double Perimeter() override {
        return 2 * pi * r;
    }

    double Area() override {
        return pi * r * r;
    }
};

std::shared_ptr<Figure> CreateFigure(std::istringstream& stream) {
    std::string figure_type;
    stream >> figure_type;

    if (figure_type == "RECT") {
        int width, height;
        stream >> width >> height;
        return std::make_shared<Rect>(width, height);
    } else if (figure_type == "TRIANGLE") {
        int a, b, c;
        stream >> a >> b >> c;
        return std::make_shared<Triangle>(a, b, c);
    } else if (figure_type == "CIRCLE") {
        int r;
        stream >> r;
        return std::make_shared<Circle>(r);
    }

    throw std::runtime_error("Wrong figure name");
}

int main() {
    std::vector<std::shared_ptr<Figure>> figures;
    for (std::string line; std::getline(std::cin, line);) {
        std::istringstream is(line);

        std::string command;
        is >> command;
        try {
            if (command == "ADD") {
                is >> std::ws;
                figures.push_back(CreateFigure(is));
            } else if (command == "PRINT") {
                for (const auto& current_figure : figures) {
                    std::cout << std::fixed << std::setprecision(3)
                            << current_figure->Name() << " "
                            << current_figure->Perimeter() << " "
                            << current_figure->Area() << std::endl;
                }
                break;
            }
        }  catch (const std::runtime_error& e) {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}
