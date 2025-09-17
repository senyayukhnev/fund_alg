#pragma once

#include "deque.h"


namespace my_container {

    template <typename T>
    class Stack {
    private:
        Deque<T> storage;

    public:
        Stack();
        Stack(const Stack& source);
        Stack(Stack&& source) noexcept;
        explicit Stack(const std::initializer_list<T>& init);
        ~Stack() = default;

        Stack& operator=(const Stack& source);
        Stack& operator=(Stack&& source) noexcept;
        T& top();
        const T& top() const;
        bool empty() const { return storage.empty(); }
        size_t size() const { return storage.size(); }

        void push(const T& elem);
        void push(T&& elem);

        void pop();

        void swap(Stack& other) noexcept;

        bool operator==(const Stack& other) const;
        bool operator!=(const Stack& other) const;
        bool operator<(const Stack& other) const;
        bool operator<=(const Stack& other) const;
        bool operator>(const Stack& other) const;
        bool operator>=(const Stack& other) const;
        std::strong_ordering operator<=>(const Stack& other) const;
    };
    template <typename T>
    Stack<T>::Stack() : storage{} {}

    template <typename T>
    Stack<T>::Stack(const Stack& source) : storage(source.storage) {}

    template <typename T>
    Stack<T>::Stack(Stack&& source) noexcept : storage(std::move(source.storage)) {}

    template <typename T>
    Stack<T>::Stack(const std::initializer_list<T>& init) : storage(init) {}

    template <typename T>
    Stack<T>& Stack<T>::operator=(const Stack& source) {
        if (this != &source) {
            storage = source.storage;
        }
        return *this;
    }

    template <typename T>
    Stack<T>& Stack<T>::operator=(Stack&& source) noexcept {
        if (this != &source) {
            storage = std::move(source.storage);
        }
        return *this;
    }

    template <typename T>
    T& Stack<T>::top() {
        if (empty()) throw std::out_of_range("Empty stack");
        return storage.front();
    }

    template <typename T>
    const T& Stack<T>::top() const {
        if (empty()) throw std::out_of_range("Empty stack");
        return storage.front();
    }

    template <typename T>
    void Stack<T>::push(const T& elem) {
        storage.push_front(elem);
    }

    template <typename T>
    void Stack<T>::push(T&& elem) {
        storage.push_front(std::move(elem));
    }

    template <typename T>
    void Stack<T>::pop() {
        if (empty()) throw std::out_of_range("Empty stack");
        storage.pop_front();
    }

    template <typename T>
    void Stack<T>::swap(Stack& other) noexcept {
        storage.swap(other.storage);
    }

    template <typename T>
    bool Stack<T>::operator==(const Stack& other) const {
        return storage == other.storage;
    }

    template <typename T>
    bool Stack<T>::operator!=(const Stack& other) const {
        return !(*this == other);
    }

    template <typename T>
    bool Stack<T>::operator<(const Stack& other) const {
        return (*this <=> other) < 0;
    }

    template <typename T>
    bool Stack<T>::operator<=(const Stack& other) const {
        return (*this <=> other) <= 0;
    }

    template <typename T>
    bool Stack<T>::operator>(const Stack& other) const {
        return (*this <=> other) > 0;
    }

    template <typename T>
    bool Stack<T>::operator>=(const Stack& other) const {
        return (*this <=> other) >= 0;
    }

    template <typename T>
    std::strong_ordering Stack<T>::operator<=>(const Stack& other) const {
        return storage <=> other.storage;
    }

}