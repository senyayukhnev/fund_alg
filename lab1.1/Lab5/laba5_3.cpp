#include "iostream"
#include "vector"
#include "string"
#include <utility>
#include <limits>

using namespace std;

class logical_values_array{
private:
    unsigned int _value;
public:
    logical_values_array(unsigned int value = 0){
        _value = value;
    }
    logical_values_array inversion() const{
        return logical_values_array(~_value);
    }
    logical_values_array conjunction(logical_values_array other) const{
        return logical_values_array(_value & other._value);
    }
    logical_values_array disjunction(const logical_values_array& other) const{
        return logical_values_array(_value | other._value);
    }

    logical_values_array implication(const logical_values_array& other) const{
        return logical_values_array(~_value | other._value);
    }

    logical_values_array coimplication(const logical_values_array& other) const{
        return logical_values_array(_value & ~other._value);
    }

    logical_values_array xor_mod(const logical_values_array& other) const{
        return logical_values_array(_value ^ other._value);
    }

    logical_values_array equivalence(const logical_values_array& other) const{
        return logical_values_array(~(_value ^ other._value));
    }

    logical_values_array pierce_arrow(const logical_values_array& other) const{
        return logical_values_array(~(_value | other._value));
    }

    logical_values_array sheffer_stroke(const logical_values_array& other) const{
        return logical_values_array(~(_value & other._value));
    }
    static bool equals(const logical_values_array to_comp1, const logical_values_array to_comp2){
        return to_comp1._value == to_comp2._value;
    }
    bool get_bit(int pos){
        if(pos < 0 || pos >= sizeof(int) * 8){
            throw out_of_range("Bit pos is out of range");
        }
        return (int(_value) >> pos) & 1;
    }

    void to_string(char* str) const {
        if (str == nullptr){
            throw invalid_argument("Empty string is passed to method");
        }
        size_t len = sizeof(unsigned int) * 8;
        for (int i = len - 1, j = 0; i >= 0; --i, j++) {
            str[j] = ((_value >> i) & 1) + '0';
        }
        str[len] = '\0';
    }
    unsigned int get_val() const {
        return _value;
    }
};

int main(){
    try{
        logical_values_array a1(1);
        logical_values_array a2(2);
        logical_values_array a3 = a1.xor_mod(a2);

        cout << "a1: " << a1.get_val() << "\n";
        cout << "a2: " << a2.get_val() << "\n";
        cout << "conjunction: " << a1.conjunction(a2).get_val() << "\n";
        cout << "disjunction: " << a1.disjunction(a2).get_val() << "\n";
        cout << "implication: " << a1.implication(a2).get_val() << "\n";
        cout << "coimplication: " << a1.coimplication(a2).get_val() << "\n";
        cout << "xor_mod: " << a1.xor_mod(a2).get_val() << "\n";
        cout << "equivalence: " << a1.equivalence(a2).get_val() << "\n";
        cout << "pierce_arrow: " << a1.pierce_arrow(a2).get_val() << "\n";
        cout << "sheffer_stroke: " << a1.sheffer_stroke(a2).get_val() << "\n";

        cout << "Equals: " << (logical_values_array::equals(a1, a2) ? "yes" : "no") << "\n";
        cout << "0 bit of a1: " << a1.get_bit(0) << "\n";
        cout << "1 bit of a1: " << a1.get_bit(1) << "\n";

        char str[50];
        a1.to_string(str);

        cout << "a1 to string: " << str << "\n";
    }
    catch(const exception& e){
        cerr << e.what() << '\n';
    }

    return 0;
}