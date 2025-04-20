//
// Created by senya on 03.04.2025.
//

#ifndef FUND_LIST_CONTAINER_H
#define FUND_LIST_CONTAINER_H
#include <iostream>
#include <limits>

template <typename T>
class Container {
public:
    Container() = default;
    Container(const Container& other) = default;
    Container& operator=(const Container& other) = default;

    virtual ~Container() = default;

    virtual T* begin() = 0;
    virtual const T* cbegin() const = 0;
    virtual T* end() = 0;
    virtual const T* cend() const = 0;

    virtual bool operator==(const Container& other) const = 0;
    virtual bool operator!=(const Container& other) const = 0;

    virtual std::size_t size() const = 0;
    virtual std::size_t max_size() const = 0;
    virtual bool empty() const = 0;
};

template <typename T>
class List final : public Container<T> {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& val, Node* p = nullptr, Node* n = nullptr)
                : data(val), prev(p), next(n) {}
        Node(T&& val, Node* p = nullptr, Node* n = nullptr)
                : data(std::move(val)), prev(p), next(n) {}
    };

    Node* head;
    Node* tail;
    std::size_t  list_size;

public:
    List() : head(nullptr), tail(nullptr), list_size(0) {}

    List(const List& other) : List() {
        for (const auto& item : other) {
            push_back(item);
        }
    }
    List(List&& other) noexcept
            : head(other.head), tail(other.tail), list_size(other.list_size) {
        other.head = nullptr;
        other.tail = nullptr;
        other.list_size = 0;
    }
    List(std::initializer_list<T> init) : List() {
        for (const auto& item : init) {
            push_back(item);
        }
    }
    bool empty() const override {return  list_size == 0; }

    void pop_front() {
        if (empty()) return;

        Node* to_delete = head;
        head = head->next;
        if (head) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete to_delete;
        --list_size;
    }

    void clear() {
        while (!empty()) {
            pop_front();
        }
    }
    ~List() override {
        clear();
    }
    std::size_t size() const override { return list_size; }
    std::size_t max_size() const override { return std::numeric_limits<std::size_t>::max(); }

    void push_back(const T& value) {
        Node* new_node = new Node(value, tail, nullptr);
        if (tail) {
            tail->next = new_node;
        } else {
            head = new_node;
        }
        tail = new_node;
        ++list_size;
    }
    void push_back(T&& value) {
        Node* new_node = new Node(std::move(value), tail, nullptr);
        if (tail) {
            tail->next = new_node;
        } else {
            head = new_node;
        }
        tail = new_node;
        ++list_size;
    }

    void pop_back() {
        if (empty()) return;

        Node* to_delete = tail;
        tail = tail->prev;
        if (tail) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        delete to_delete;
        --list_size;
    }

    void push_front(const T& value) {
        Node* new_node = new Node(value, nullptr, head);
        if (head) {
            head->prev = new_node;
        } else {
            tail = new_node;
        }
        head = new_node;
        ++list_size;
    }

    void push_front(T&& value) {
        Node* new_node = new Node(std::move(value), nullptr, head);
        if (head) {
            head->prev = new_node;
        } else {
            tail = new_node;
        }
        head = new_node;
        ++list_size;
    }

    List& operator=(const List& other) {
        if (this != &other) {
            List temp(other);
            swap(temp);
        }
        return *this;
    }

    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            list_size = other.list_size;

            other.head = nullptr;
            other.tail = nullptr;
            other.list_size = 0;
        }
        return *this;
    }

    List& operator=(std::initializer_list<T> init) {
        List temp(init);
        swap(temp);
        return *this;
    }
    T& front() {
        if (empty()) throw std::out_of_range("List is empty");
        return head->data;
    }

    const T& front() const {
        if (empty()) throw std::out_of_range("List is empty");
        return head->data;
    }

    T& back() {
        if (empty()) throw std::out_of_range("List is empty");
        return tail->data;
    }

    const T& back() const {
        if (empty()) throw std::out_of_range("List is empty");
        return tail->data;
    }
    T* begin() override { return head ? &head->data : nullptr; }
    const T* cbegin() const override { return head ? &head->data : nullptr; }

    T* end() override { return tail ? &tail->data + 1 : nullptr; }
    const T* cend() const override { return tail ? &tail->data + 1 : nullptr; }

    T* rbegin() { return tail ? &tail->data : nullptr; }
    const T* crbegin() const { return tail ? &tail->data : nullptr; }

    T* rend() { return head ? &head->data - 1 : nullptr; }
    const T* crend() const { return head ? &head->data - 1 : nullptr; }

    void resize(std::size_t count) {
        resize(count, T());
    }

    void resize(std::size_t count, const T& value) {
        while (list_size > count) {
            pop_back();
        }
        while (list_size < count) {
            push_back(value);
        }
    }

    void swap(List& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(list_size, other.list_size);
    }

    bool operator==(const List& other) const override {
        if (size() != other.size()) return false;
        return std::equal(cbegin(), cend(), other.cbegin());
    }

    bool operator!=(const List& other) const override {
        return !(*this == other);
    }

    auto operator<=>(const List& other) const {
        return std::lexicographical_compare_three_way(
                cbegin(), cend(), other.cbegin(), other.cend());
    }

    T* insert(const T* pos, const T& value) {
        if (pos == nullptr || pos == cbegin()) {
            push_front(value);
            return begin();
        }
        if (pos == cend()) {
            push_back(value);
            return &tail->data;
        }

        Node* current = head;
        while (current && &current->data != pos) {
            current = current->next;
        }

        if (!current) return nullptr;

        Node* new_node = new Node(value, current->prev, current);
        current->prev->next = new_node;
        current->prev = new_node;
        ++list_size;
        return &new_node->data;
    }

    T* erase(const T* pos) {
        if (pos == nullptr || empty()) return nullptr;

        if (pos == cbegin()) {
            pop_front();
            return begin();
        }
        if (pos == &tail->data) {
            pop_back();
            return nullptr;
        }

        Node* current = head;
        while (current && &current->data != pos) {
            current = current->next;
        }

        if (!current) return nullptr;

        Node* next_node = current->next;
        current->prev->next = current->next;
        if (current->next) {
            current->next->prev = current->prev;
        }
        delete current;
        --list_size;
        return next_node ? &next_node->data : nullptr;
    }
};
#endif //FUND_LIST_CONTAINER_H
