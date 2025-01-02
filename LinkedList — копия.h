#include <iostream>
#include <memory>
#include <string>
#pragma once

// LinkedList - ��������� �����, ����������� ��������� ����������������� ���������� ������.
template <typename T>
class LinkedList
{
private:
    // ListNode - c�������� ���� ����������������� ���������� ������.
    template <typename T>
    struct ListNode
    {
        // ��������, ���������� � ����.
        T value;

        // ��������� �� ��������� ���� ������.
        std::shared_ptr<ListNode<T>> next;

        // ����������� - �������������� ���� value � next ��� ������ ����.
        ListNode(const T& value) : value(value), next(nullptr) {}
    };

    // ������ ������ �� ������� ������.
    size_t size_of_list;

    // ��������� �� ������ ���� ������.
    std::shared_ptr<ListNode<T>> head;

    // ��������� �� ��������� ���� ������.
    std::shared_ptr<ListNode<T>> tail;

public:
    // Iterator - �����, ����������� ��������� ����������.
    class Iterator
    {
    private:
        // ��������� �� ������� ���� ������.
        ListNode<T>* current_node;

    public:
        // ����� ����������� � ����������� - �������������� ���� current_node.
        explicit Iterator(ListNode<T>* pointer) : current_node(pointer) {}

        // �������� ���������� (���������� ++it);
        // ���������� ������ �� "����" ����� ����������� ���������.
        Iterator& operator++()
        {
            if (current_node)
            {
                // next - shared_ptr, ������� ����� "�����" ��������� � ������� ������ get().
                current_node = current_node->next.get();
            }

            return *this;
        }

        // �������� ���������� (����������� it++);
        // ���������� "�����" ��������� �� �����������.
        Iterator operator++(int)
        {
            Iterator temp(*this); // ��������� ������ ���������.
            ++(*this);            // ���������� ���������� ��������.
            return temp;          // ���������� ������� ���������.
        }

        // ������������� * (������ � T&).
        T& operator*() const {
            return current_node->value;
        }

        // �������� -> (������ � T*).
        T* operator->() const {
            return &(current_node->value);
        }

        // ��������� ���������� �� ���������.
        bool operator==(const Iterator& other) const {
            return current_node == other.current_node;
        }

        bool operator!=(const Iterator& other) const {
            return current_node != other.current_node;
        }
    };

    // begin() - ��������, ����������� �� ��������� ������� ������.
    Iterator begin() {
        return Iterator(head.get());
    }

    // end() - ��������, ����������� �� nullptr (�.�. "������ ���� ������").
    Iterator end() {
        return Iterator(nullptr);
    }

    // ����������� �� ��������� - �������������� ���� size_of_list, head � tail ��� ������� ������.
    LinkedList() : size_of_list(0), head(nullptr), tail(nullptr) {}

    // ����������� ��������� �����������.
    LinkedList(const LinkedList<T>& other) :
        size_of_list(other.size_of_list), head(nullptr), tail(nullptr)
    {
        if (other.is_empty()) {
            return;
        }

        head = std::make_shared<ListNode<T>>(other.head->value);
        tail = head;

        std::shared_ptr<ListNode<T>> current_other = other.head->next;

        while (current_other != nullptr)
        {
            this->push_back(current_other->value);
            current_other = current_other->next;
        }
    }

    // ����������� ����������� ( ��� rvalue-�������� ).
    LinkedList(LinkedList<T>&& other) noexcept :
        size_of_list(other.size_of_list), head(other.head), tail(other.tail)
    {
        other.head = nullptr;
        other.tail = nullptr;
    }

    // �������� ��������� ������������.
    LinkedList<T>& operator=(const LinkedList<T>& other)
    {
        if (this == &other) {
            return *this;
        }

        this->clear();

        head = std::make_shared<ListNode<T>>(other.head->value);
        tail = head;

        std::shared_ptr<ListNode<T>> current_other = other.head->next;

        while (current_other != nullptr)
        {
            this->push_back(current_other->value);
            current_other = current_other->next;
        }

        return *this;
    }

    /*
    ������������� ������ � ��������� ������ �� ������� ( ���������� �������� ).

    �������� ��� ������������� ���������� ( ������ ������� ����� ������ 0 ),
    ��� � ������������� ( ��������� ������� ����� ������ -1 ).

    �������� ���������� �������� �������� �� ������.

    �����! ��������� � �������� �� ������� ����� ��������� O(n), ��� ����� ���� ������������ ��� ������� �������.
    */
    T& operator[](int index) const
    {
        // ���� �������� ������� ������� �� ������� ������ - ����������� ����������.
        if (index < -int(size_of_list) || index >= int(size_of_list)) {
            throw std::out_of_range("������ ��� ���������.");
        }

        // ������� ��������� ���������, � ������� �������� ����� ������������� �� ������.
        std::shared_ptr<ListNode<T>> temp = head;

        for (int i = 0; i < size_of_list * (index < 0) + index; ++i) {
            temp = temp->next;
        }

        return temp->value;
    }

    // ����� ����������, �������� �� ������ ������.
    bool is_empty() const {
        return head == nullptr;
    }

    // ����� ���������� ����� ������ �� ������� ������.
    size_t size() const {
        return size_of_list;
    }

    // ����� ���������� ������ ������� ������ �� ������.
    T& front() const
    {
        // ���� ������ ������ - �� �� ����� ���������� � ������� ��������.
        if (head == nullptr) {
            throw std::runtime_error("������ ����.");
        }

        return head->value;
    }

    // ����� ���������� ��������� ������� ������ �� ������.
    T& back() const
    {
        // ���� ������ ������ - �� �� ����� ���������� � ���������� ��������.
        if (head == nullptr) {
            throw std::runtime_error("������ ����.");
        }

        return tail->value;
    }

    // ����� ���� ������ ���� �� ��������� value � ���������� ����� (shared) ��������� �� ���� ����;
    // ���� ������� �� ������ - ���������� nullptr.
    std::shared_ptr<ListNode<T>> find(const T& value) const
    {
        std::shared_ptr<ListNode<T>> current_node = head;

        while (current_node != nullptr)
        {
            if (current_node->value == value) {
                return current_node;
            }

            current_node = current_node->next;
        }

        return nullptr;
    }

    // ����� ���������, �������� �� ������ ����������� ��������;
    // ���������� true ��� false.
    bool contains(const T& value) const {
        return find(value) != nullptr;
    }

    // ����� ��������� ������� ������.
    void clear()
    {
        while (!is_empty()) {
            pop_front();
        }
    }

    /*
    ����� ��������� ������, ��������� ����������� ����������.
    �����! ������ �������� ����� ��������� O(n^2), ��� ����� ���� ������������ ��� ������� �������.
    */
    void bubble_sort()
    {
        // ���� � ������ ������ 2-� ��������� - ����������� ������ �� �����.
        if (size_of_list < 2) {
            return;
        }

        // ����� ������ ��� ������� 1.
        bool is_swapped = bool();

        do
        {
            is_swapped = false;
            std::shared_ptr<ListNode<T>> current = head;

            while (current->next)
            {
                if (current->value > current->next->value)
                {
                    std::swap(current->value, current->next->value);
                    is_swapped = true;
                }

                current = current->next;
            }

        } while (is_swapped);
    }

    // ����� ������� ������ ���� �� ��������� value;
    // ���������� true, ���� ������� ��� ������ � �����, ����� - false.
    bool remove(const T& value_to_remove)
    {
        // ���� ������ ������ - �� �� ����� ������ �������.
        if (head == nullptr) {
            return false;
        }

        // ���� ����� ������� ������ �������:
        if (head->value == value_to_remove)
        {
            pop_front();
            return true;
        }

        // ������� ��������� �� ������� ���� ��� �������� �� ������.
        std::shared_ptr<ListNode<T>> current_node = head;

        // ������ ���� ���������� � ���� �������:
        // 1) ������� ���� ���� ���������.
        // 2) ����, �� ������� ��������� ������� ����, ���������� �������.
        while (current_node->next != nullptr && current_node->next->value != value_to_remove) {
            current_node = current_node->next;
        }

        // ���� �� ����� �� ����� ��-�� ������� �������, �� ��� ��������, ��� �� ������ �� ����� - ���������� false.
        if (current_node->next == nullptr) {
            return false;
        }

        // ���� �� ����� ����, ������ ����, �� ������� ��������� ������� ����, ���������� �������.

        // ������ ��������� �� ����, ������� ���������� �������.
        std::shared_ptr<ListNode<T>> node_to_remove = current_node->next;

        // ������ ������� ���� ������ ���������� �� ����, �� ������� ��������� ��������� ����.
        current_node->next = node_to_remove->next;

        // ���� �� ������� ��������� ����, �� ����� ������ ���������� �� ������� ����.
        if (current_node->next == nullptr) {
            tail = current_node;
        }

        // �������������� ����������, ���������� �� ������ ������.
        --size_of_list;

        return true;
    }

    // ����� ��������� ����� ������� � ������ ������.
    void push_front(const T& value)
    {
        // ������� ����� ���� � �������� �� ���� ���������.
        std::shared_ptr<ListNode<T>> new_node = std::make_shared<ListNode<T>>(value);

        // ����� ���� ������ ���������� �� ������ ������� ������.
        new_node->next = head;

        // ������, � ���� �������, ������ ������ ���������� �� ��� ����� ����.
        head = new_node;

        // ���� ������ ��� ������ - ����� ��������� �� ��� ����� ����.
        if (tail == nullptr) {
            tail = new_node;
        }

        // �������������� ����������, ���������� �� ������ ������.
        ++size_of_list;
    }

    // ����� ��������� ����� ������� � ����� ������.
    void push_back(const T& value)
    {
        // ������� ����� ���� � �������� �� ���� ���������.
        std::shared_ptr<ListNode<T>> new_node = std::make_shared<ListNode<T>>(value);

        // ���� ������ ������ - ����� ���� ���������� � �������, � �������.
        if (head == nullptr)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            // ��������� ���� ������ ������ ���������� �� ��� ����� �������.
            tail->next = new_node;

            // ��������� �� ����� ������ ������ ���������� �� ��� ����� ������� ( ��� ��� �� - ��������� � ������ ).
            tail = new_node;
        }

        // �������������� ����������, ���������� �� ������ ������.
        ++size_of_list;
    }

    // ����� ������� ������ ������� �� ������;
    // ���������� �������� ���������� ��������.
    T pop_front()
    {
        // ���� ������ ������ - �� �� ������ ������ �������.
        if (head == nullptr) {
            throw std::runtime_error("���������� ������� ������� �� ������� ������.");
        }

        // value - �������� ��������, ������� �� ��������� �������.
        T value = head->value;

        // ��������� �� ������ ������� ������ ������ ��������� �� ������ ������� ������,
        // ��� ��� ����� ���������� ������� ������ ������ ������� ������ ������ ������.
        head = head->next;

        // ���� ����� �������� ������ ���� ������ - ��������� �� ����� ������ ���������� �� nullptr.
        if (head == nullptr) {
            tail = nullptr;
        }

        // �������������� ����������, ���������� �� ������ ������.
        --size_of_list;

        return value;
    }

    // ����� ������� ��������� ������� �� ������;
    // ���������� �������� ���������� ��������.
    T pop_back()
    {
        // ���� ������ ������ - �� �� ������ ������ �������.
        if (head == nullptr) {
            throw std::runtime_error("���������� ������� ������� �� ������� ������.");
        }

        // value - �������� ��������, ������� �� ��������� �������.
        T value = T();

        // ���� ������ ������� �� ������ �������� - '��������' ������.
        if (head == tail)
        {
            // ����� ���, ��� ������� ����, ��������� ��������, ������� �� ������.
            value = head->value;

            head = nullptr;
            tail = nullptr;
        }
        else
        {
            // ���� ������������� ����.
            std::shared_ptr<ListNode<T>> current_node = head;

            while (current_node->next != tail) {
                current_node = current_node->next;
            }

            // ����� ���, ��� ������� ��������� ����, ��������� ��������, ������� �� ������.
            value = tail->value;

            tail = current_node;
            tail->next = nullptr;
        }

        // �������������� ����������, ���������� �� ������ ������.
        --size_of_list;

        return value;
    }

    // ����� ����������� ��� �������� ������ � ��� ������; ���������� ������.
    std::string convert_list_to_string(std::string func(const T& value) = [](const T& value) { return std::to_string(value); })
    {
        /*
        ������ ����� �������� ��� ��������� ������ � ��� ������, ���� ������� std::to_string() ���������� ��� ���� T.

        � ��������� ������, ������������ ������ �������� � ������ ����� �������,
        ����������������� ��� T � ��� std::string � ������������ �������� ���� std::string.

        std::string func_name(const T& some_value) - ��������� ��������� �� ������������ �������.
        */

        std::string result = std::string();

        // ���� ������ ������ - � ������ result ����� ���������� ��������������� ���������.
        if (head == nullptr) {
            result = "List is empty.";
        }
        else
        {
            // ������� ��������� ��������� ��� ������� �� ���� ���������.
            std::shared_ptr<ListNode<T>> current_node = head;

            while (current_node != nullptr)
            {
                // ��������������� ������� � ������ � ��������� ��� � result.
                result += func(current_node->value);

                // ���� ������� ���� �� �������� ���������, ����� �������� ������� ����������� ������.
                if (current_node->next != nullptr) { result += " -> "; }

                // ��������� � ���������� ����.
                current_node = current_node->next;
            }
        }

        return result;
    }
};