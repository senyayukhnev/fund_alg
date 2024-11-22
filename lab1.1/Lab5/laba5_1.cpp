#include "iostream"
#include "vector"
#include "string"
#include <utility>
#include <limits>

using namespace std;

class binary_int{
private:
    int num;

    static int decrement(int n) {                 //00000100
        long long int mask = 1;                   //00000001
        while ((n & mask) == 0) {                 //00000000 // 00000000 // выход из цикла
            n = n ^ mask;                         //00000101 // 00000111
            mask <<= 1;                           //00000010 // 00000100
        }
        return n ^ mask;                          //00000011
    }

    static int increment(int n) {
        int mask = 1;
        if (n == numeric_limits<int>::max()){
            throw runtime_error("Overflow");
        }

        while (n & mask) {
            n = n ^ mask;

            mask <<= 1;
        }
        return n ^ mask;
    }
    static int negative(int n){
        return increment(~n);
    }
    static int sum(int n, int other){
        int res = n;
        if(other > 0) {
            while (other > 0) {
                res = increment(res);
                other = decrement(other);
            }
            return res;
        }else{
            while (other < 0) {
                res = decrement(res);
                other = increment(other);
            }
            return res;
        }
    }
    static int sub(int n, int other){
        int res = sum(n, negative(other));
        return res;
    }
    static int mul(int n, int other){
        int res = n;
        int to_add = n;
        if(other > 1) {
            while (other > 1) {
                res = sum(res, to_add);
                other = decrement(other);
            }
            return res;
        }else{
            if(other == 0){
                return 0;
            }else{
                if(other < 0){
                    while (other < -1) {
                        res = sum(res, to_add);
                        other = increment(other);
                    }
                    return negative(res);
                }
            }

        }
        return res;
    }

public:
    explicit binary_int(int n): num(n) {}
    binary_int &operator-(){
        num = negative(num);
        return *this;
    }
    binary_int &operator--() {
        num = decrement(num);
        return *this;
    }
    binary_int operator--(int) {
        binary_int temp(*this);
        num = decrement(num);
        return temp;
    }

    binary_int &operator++() {
        num = increment(num);
        return *this;
    }
    binary_int operator++(int) {
        binary_int temp(*this);
        num = increment(num);
        return temp;
    }
    binary_int operator+=(const binary_int& other){
        num = sum(num, other.num);
        return *this;
    }
    binary_int operator+(const binary_int& other) const{
        int res = sum(num, other.num);
        return binary_int(res);
    }
    binary_int operator-=(const binary_int& other){
        num = sub(num, other.num);
        return *this;
    }
    binary_int operator-(const binary_int& other) const{
        int res = sub(num, other.num);
        return binary_int(res);
    }

    binary_int operator*=(const binary_int& other){
        num = mul(num, other.num);
        return *this;
    }
    binary_int operator*(const binary_int& other) const{
        int res = mul(num, other.num);
        return binary_int(res);
    }
    binary_int operator<<=(const binary_int& other){
        num <<= other.num;
        return *this;
    }
    binary_int operator>>=(const binary_int& other){
        num >>= other.num;
        return *this;
    }
    binary_int operator<<(const binary_int& other) const{
        int res = num << other.num;
        return binary_int(res);
    }
    binary_int operator>>(const binary_int& other) const{
        int res = num >> other.num;
        return binary_int(res);
    }

    pair<binary_int, binary_int> split() const {
        int num_bits = sizeof(num) * 8;
        int half_bits = num_bits / 2;

        // Маски для выделения старших и младших половин
        int lower_mask = (1 << half_bits) - 1;         // 0x0000FFFF
        int upper_mask = ~lower_mask;                 //  0xFFFF0000

        // Младшие и старшие половины
        int lower_half = num & lower_mask;            // Обнуляем старшие биты
        int upper_half = num & upper_mask;            // Обнуляем младшие биты

        // Возвращаем пару объектов binary_int
        return {binary_int(upper_half), binary_int(lower_half)};
    }
    int get_num() const{
        return num;
    }
};

int main() {
        // Создание объектов binary_int
        binary_int a(6);
        binary_int b(1);
    try {
        // Демонстрация операторов инкремента и декремента
        cout << "a: " << a.get_num() << endl;
        cout << "++a: " << (++a).get_num() << endl;
        cout << "a++: " << (a++).get_num() << endl;
        cout << "a: " << a.get_num() << endl;

        cout << "b: " << b.get_num() << endl;
        cout << "--b: " << (--b).get_num() << endl;
        cout << "b--: " << (b--).get_num() << endl;
        cout << "b: " << b.get_num() << endl;

        // Демонстрация операторов сложения и вычитания
        binary_int c = a + b;
        cout << "a + b: " << c.get_num() << endl;

        binary_int d = a - b;
        cout << "a - b: " << d.get_num() << endl;

        // Демонстрация операторов умножения
        binary_int e = a * b;
        cout << "a * b: " << e.get_num() << endl;

        // Демонстрация операторов сдвига
        binary_int f = a << b;
        cout << "a << b: " << f.get_num() << endl;

        binary_int g = a >> b;
        cout << "a >> b: " << g.get_num() << endl;

        // Демонстрация операторов присваивания сдвига
        a <<= b;
        cout << "a <<= b: " << a.get_num() << endl;

        a >>= b;
        cout << "a >>= b: " << a.get_num() << endl;

        // Демонстрация функции split
        binary_int h(0x12345678);
        pair<binary_int, binary_int> halves = h.split();
        cout << "Upper half: " << halves.first.get_num() << endl;
        cout << "Lower half: " << halves.second.get_num() << endl;

        // Демонстрация оператора отрицания
        binary_int i(-5);
        cout << "-i: " << (-i).get_num() << endl;
    }catch(exception& e){
        cerr << e.what() << endl;
    }
    return 0;

}