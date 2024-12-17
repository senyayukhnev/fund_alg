#include "iostream"
#include "vector"
#include "string"
#include <utility>
#include <limits>
#include "math.h"

using namespace std;

class Vector{
private:
    double* arr;
    size_t size;
    size_t capacity;
public:
    Vector(const size_t size_v, const double& to_fill = 0.0){
        size = size_v;
        capacity = size_v;
        arr = new double[size];
        for(int i = 0; i < size; i++){
            arr[i] = to_fill;
        }
    }

    Vector(vector<double>::const_iterator start,vector<double>::const_iterator end){
        if(start >= end){
            throw invalid_argument("Start iter >= Last iter: ERROR");
        }
        size = end - start;
        capacity = size;
        arr = new double[size];
        copy(start, end, arr);
    }
    Vector(initializer_list<double> init) {
        size = init.size();
        capacity = size;
        arr = new double[size];
        std::copy(init.begin(), init.end(), arr);
    }
    ~Vector(){
        delete[] arr;
    }
    const double &at(size_t index) const{
        if (index > size){
            throw invalid_argument("Incorrect index");
        }
        return arr[index];
    }
    double &at(size_t index, int) const{
        if (index > size){
            throw invalid_argument("Incorrect index");
        }
        return arr[index];
    }
/*    double &at(size_t index, int) const{
        if (index > size){
            throw invalid_argument("Incorrect index");
        }
        return arr[index];
    }*/

    const double &front() const{
        if(size == 0){
            throw runtime_error("Cant get first element from empty vector");
        }
        return arr[0];
    }
    double &front(int) const{
        if(size == 0){
            throw runtime_error("Cant get first element from empty vector");
        }
        return arr[0];
    }
    const double &back() const{
        if(size == 0){
            throw runtime_error("Cant get last element from empty vector");
        }
        return arr[size];
    }
    double &back(int) const{
        if(size == 0){
            throw runtime_error("Cant get last element from empty vector");
        }
        return arr[size];
    }
    const double* data() const{
        return arr;
    }
    double* data(int) const{
        return arr;
    }
    bool empty() const{
        return (size == 0) ? true : false;
    }
    size_t size_v() const{
        return size;
    }

    size_t capacity_v() const{
        return capacity;
    }
    void reserve(size_t num){
        if (num > capacity){
            double* temp = arr;
            arr = new double[num];
            arr = temp;
            capacity = num;
        }
    }
    void push_back(double elem){
        if (capacity == size){
            reserve(capacity + 100);
        }

        arr[size] = elem;
        size += 1;
    }
};



int main(){
    Vector arr({1, 2, 3, 4, 5});
    for (int i = 0; i < 1000; ++i) {
        arr.push_back(i); // Добавляем элементы без реального изменения размера
    }

    // Программа может упасть здесь из-за записи за пределы выделенной памяти
    cout << arr.at(1000) << endl;

    return 0;

}