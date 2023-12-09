#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <iomanip>
#include <memory>

using std::string, std::endl, std::cout, std::fixed, std::vector, std::shared_ptr, std::istringstream, std::cin, std::make_shared;


class Figure {
public:
    virtual string Name() = 0;

    virtual double Perimeter() = 0;

    virtual double Area() = 0;
};

class Triangle : public Figure {
public:
    Triangle(int &a, int &b, int &c) : a(a), b(b), c(c) {
    }

    string Name() override {
        return "TRIANGLE";
    }

    double Perimeter() override {
        return a + b + c;
    }

    double Area() override {
        return sqrt((Perimeter() / 2) * (Perimeter() / 2 - a) * (Perimeter() / 2 - b) * (Perimeter() / 2 - c));
    }

private:
    const int a;
    const int b;
    const int c;
};

class Rect : public Figure {
public:
    Rect(int &width, int &height) : width(width), height(height) {}

    string Name() override {
        return "RECT";
    }

    double Perimeter() override {
        return (width + height) * 2;
    }

    double Area() override {
        return width * height;
    }

private:
    const int width;
    const int height;
};

class Circle : public Figure {
public:
    explicit Circle(int &r) : r(r) {
    }

    string Name() override {
        return "CIRCLE";
    }

    double Perimeter() override {
        return 2 * 3.14 * r;
    }

    double Area() override {
        return 3.14 * r * r;
    }

private:
    const int r;
};

shared_ptr<Figure> CreateFigure(istringstream &args) {
    string figure_name;
    args >> figure_name;

    if (figure_name == "RECT") {
        int width, height;
        args >> width >> height;

        return make_shared<Rect>(width, height);
    }

    if (figure_name == "TRIANGLE") {
        int a, b, c;
        args >> a >> b >> c;

        return make_shared<Triangle>(a, b, c);
    }

    if (figure_name == "CIRCLE") {
        int r;
        args >> r;

        return make_shared<Circle>(r);
    }

    return nullptr;
}

int main() {
    vector<shared_ptr<Figure>> figures;

    for (string line; getline(cin, line);) {
        istringstream is(line);

        string command;
        is >> command;

        if (command == "ADD") {
            is >> std::ws;
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto &current_figure: figures) {
                cout << fixed << std::setprecision(3)
                     << current_figure->Name() << " "
                     << current_figure->Perimeter() << " "
                     << current_figure->Area() << endl;
            }
        }
    }

    return 0;
}
