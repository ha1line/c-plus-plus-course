#include <iostream>
#include <memory>
#include <array>
#include <complex>
#include <format>
#include <iomanip>
#include <vector>

double PI = 3.14;

class Figure{
public:
    explicit Figure(const std::array<double,3>& array, const std::string& name) :
             m_array(array),m_name(name){}
    virtual std::string Name() const = 0;
    virtual std::string Perimeter() const = 0;
    virtual std::string Area() const = 0;
protected:
    std::string ToStr(const double& x) const {
        std::stringstream stream;
        stream << std::fixed << std::setprecision(3) << x;
        return stream.str();
    }
    std::array<double,3> m_array;
    std::string m_name;
};
class Circle: public Figure{
public:
    Circle(const std::array<double,3>& array): Figure(array,"CIRCLE"){}
    std::string Name() const final {
        return m_name;
    }
    std::string Perimeter() const final {
        double result = 2 * m_array[0] * PI;
        return ToStr(result);
    }
    std::string Area() const final {
        double result = PI * (m_array[0]*m_array[0]);
        return ToStr(result);
    }
};
class Rect: public Figure{
public:
    Rect(const std::array<double,3>& array): Figure(array,"RECT"){}
    std::string Name() const final {
        return m_name;
    }
    std::string Perimeter() const final {
        double result = m_array[0]*2 + m_array[1]*2;
        return ToStr(result);
    }
    std::string Area() const final {
        double result = m_array[0]*m_array[1];
        return ToStr(result);
    }
};
class Triangle: public Figure{
public:
    Triangle(const std::array<double,3>& array): Figure(array,"TRIANGLE"){}
    std::string Name() const final {
        return m_name;
    }
    std::string Perimeter() const final {
        double result = _Perimetr();
        return ToStr(result);
    }
    std::string Area() const final {
        double p = _Perimetr()/2;
        double result = std::sqrt(p*(p-m_array[0])*(p-m_array[1])*(p-m_array[2]));
        return ToStr(result);
    }
private:
    double _Perimetr() const {
        return m_array[0] + m_array[1] + m_array[2];
    }
};

std::shared_ptr<Figure> CreateFigure(std::istringstream& is){
    std::string form;
    is >> form;
    if(form == "RECT"){
        double width, height;
        is >> width >> height;
        std::array<double,3> arr = {width,height};
        return std::make_shared<Rect>(arr);

    }else if(form == "TRIANGLE"){
        double a, b, c;
        is >> a >> b >> c;
        std::array<double,3> arr = {a,b,c};
        return std::make_shared<Triangle>(arr);

    }else if(form == "CIRCLE"){
        double r;
        is >> r;
        std::array<double,3> arr = {r};
        return std::make_shared<Circle>(arr);
    }else{
        throw std::runtime_error("Wrong figure name");
    }
}

int main2() {
    std::vector<std::shared_ptr<Figure>> figures;
    for (std::string line; std::getline(std::cin, line); ) {
        std::istringstream is(line);

        std::string command;
        is >> command;
        if (command == "ADD") {
            is >> std::ws;
            try{
                figures.push_back(CreateFigure(is));
            }catch (std::exception& exception){
                std::cout << exception.what() << std::endl;
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
