#include "iostream"
#include "vector"
#include "string"
#include <utility>
#include <limits>
#include "math.h"

using namespace std;

class Complex_number {
private:
    double real, imag;

public:
    explicit Complex_number(double real = 0, double imag = 0) {
        this->real = real;
        this->imag = imag;
    }

    Complex_number summ(Complex_number to_add) {
        return Complex_number(this->real + to_add.real, this->imag + to_add.imag);
    }

    Complex_number sub(Complex_number to_sub) {
        return Complex_number(this->real - to_sub.real, this->imag - to_sub.imag);
    }

    Complex_number mul(Complex_number to_mul) {
        return Complex_number((this->real * to_mul.real - this->imag * to_mul.imag),
                              (this->real * to_mul.imag + this->imag * to_mul.real));
    }

    Complex_number div(Complex_number to_div) {
        if (to_div.real * to_div.real + to_div.imag * to_div.imag != 0) {
            return Complex_number((this->real * to_div.real + this->imag * to_div.imag) /
                                  (to_div.real * to_div.real + to_div.imag * to_div.imag),
                                  (this->imag * to_div.real - this->real * to_div.imag) /
                                  (to_div.real * to_div.real + to_div.imag * to_div.imag));
        }
        throw runtime_error("Error: division by zero");
    }

    void display() const {
        cout << real << " + " << imag << "i" << endl;
    }

    double module() const {
        return sqrt(real * real + imag * imag);
    }

    double arg() const {
        if (real == 0 && imag == 0) {
            return 0;
        }
        if (real == 0 && imag != 0) {
            return imag > 0 ? M_PI / 2.0 : -M_PI / 2.0;
        }
        if (real > 0) {
            return atan(imag / real);
        }
        if (real < 0 && imag >= 0) {
            return M_PI + atan(imag / real);
        }
        if (real < 0 && imag < 0) {
            return -M_PI + atan(imag / real);
        }
        return atan(imag / real);
    }
};

int main() {
    // Создание комплексных чисел
    Complex_number z1(3, 4);  // 3 + 4i
    Complex_number z2(1, -2); // 1 - 2i

    cout << "z1: ";
    z1.display();
    cout << "z2: ";
    z2.display();

    // Сложение
    Complex_number z_sum = z1.summ(z2);
    cout << "z1 + z2 = ";
    z_sum.display();

    // Вычитание
    Complex_number z_sub = z1.sub(z2);
    cout << "z1 - z2 = ";
    z_sub.display();

    // Умножение
    Complex_number z_mul = z1.mul(z2);
    cout << "z1 * z2 = ";
    z_mul.display();

    // Деление
    try {
        Complex_number z_div = z1.div(z2);
        cout << "z1 / z2 = ";
        z_div.display();
    } catch (const runtime_error &e) {
        cout << e.what() << endl;
    }

    // Модуль
    cout << "|z1| = " << z1.module() << endl;
    cout << "|z2| = " << z2.module() << endl;

    // Аргумент
    cout << "arg(z1) = " << z1.arg() << " radians" << endl;
    cout << "arg(z2) = " << z2.arg() << " radians" << endl;

    return 0;
}
