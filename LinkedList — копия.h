#include <iostream>
#include <memory>
#include <string>
#pragma once

// LinkedList - шаблонный класс, описывающий структуру однонаправленного связанного списка.
template <typename T>
class LinkedList
{
private:
    // ListNode - cтруктура узла однонаправленного связанного списка.
    template <typename T>
    struct ListNode
    {
        // Значение, хранящееся в узле.
        T value;

        // Указатель на следующий узел списка.
        std::shared_ptr<ListNode<T>> next;

        // Конструктор - инициализирует поля value и next для нового узла.
        ListNode(const T& value) : value(value), next(nullptr) {}
    };

    // Размер списка на текущий момент.
    size_t size_of_list;

    // Указатель на первый узел списка.
    std::shared_ptr<ListNode<T>> head;

    // Указатель на последний узел списка.
    std::shared_ptr<ListNode<T>> tail;

public:
    // Iterator - класс, описывающий структуру итераторов.
    class Iterator
    {
    private:
        // Указатель на текущий узел списка.
        ListNode<T>* current_node;

    public:
        // Явный конструктор с параметрами - инициализирует поле current_node.
        explicit Iterator(ListNode<T>* pointer) : current_node(pointer) {}

        // Оператор инкремента (префиксный ++it);
        // возвращает ссылку на "себя" после перемещения итератора.
        Iterator& operator++()
        {
            if (current_node)
            {
                // next - shared_ptr, поэтому берем "сырой" указатель с помощью метода get().
                current_node = current_node->next.get();
            }

            return *this;
        }

        // Оператор инкремента (постфиксный it++);
        // возвращает "копию" итератора до перемещения.
        Iterator operator++(int)
        {
            Iterator temp(*this); // Сохраняем старое состояние.
            ++(*this);            // Используем префиксный оператор.
            return temp;          // Возвращаем прежнее состояние.
        }

        // Разыменование * (доступ к T&).
        T& operator*() const {
            return current_node->value;
        }

        // Оператор -> (доступ к T*).
        T* operator->() const {
            return &(current_node->value);
        }

        // Сравнение итераторов на равенство.
        bool operator==(const Iterator& other) const {
            return current_node == other.current_node;
        }

        bool operator!=(const Iterator& other) const {
            return current_node != other.current_node;
        }
    };

    // begin() - итератор, указывающий на начальный элемент списка.
    Iterator begin() {
        return Iterator(head.get());
    }

    // end() - итератор, указывающий на nullptr (т.е. "прошли весь список").
    Iterator end() {
        return Iterator(nullptr);
    }

    // Конструктор по умолчанию - инициализирует поля size_of_list, head и tail для пустого списка.
    LinkedList() : size_of_list(0), head(nullptr), tail(nullptr) {}

    // Конструктор глубокого копирования.
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

    // Конструктор перемещения ( для rvalue-объектов ).
    LinkedList(LinkedList<T>&& other) noexcept :
        size_of_list(other.size_of_list), head(other.head), tail(other.tail)
    {
        other.head = nullptr;
        other.tail = nullptr;
    }

    // Оператор глубокого присваивания.
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
    Предоставляет доступ к элементам списка по индексу ( аналогично массивам ).

    Доступна как положительная индексация ( первый элемент имеет индекс 0 ),
    так и отрицательная ( последний элемент имеет индекс -1 ).

    Оператор возвращает значение элемента по ссылке.

    Важно! Обращение к элементу по индексу имеет сложность O(n), что может быть неэффективно для больших списков.
    */
    T& operator[](int index) const
    {
        // Если значение индекса выходит за пределы списка - выбрасываем исключение.
        if (index < -int(size_of_list) || index >= int(size_of_list)) {
            throw std::out_of_range("Индекс вне диапазона.");
        }

        // Создаем временный указатель, с помощью которого будем итерироваться по списку.
        std::shared_ptr<ListNode<T>> temp = head;

        for (int i = 0; i < size_of_list * (index < 0) + index; ++i) {
            temp = temp->next;
        }

        return temp->value;
    }

    // Метод показывает, является ли список пустым.
    bool is_empty() const {
        return head == nullptr;
    }

    // Метод возвращает длину списка на текущий момент.
    size_t size() const {
        return size_of_list;
    }

    // Метод возвращает первый элемент списка по ссылке.
    T& front() const
    {
        // Если список пустой - мы не можем обратиться к первому элементу.
        if (head == nullptr) {
            throw std::runtime_error("Список пуст.");
        }

        return head->value;
    }

    // Метод возвращает последний элемент списка по ссылке.
    T& back() const
    {
        // Если список пустой - мы не можем обратиться к последнему элементу.
        if (head == nullptr) {
            throw std::runtime_error("Список пуст.");
        }

        return tail->value;
    }

    // Метод ищет первый узел со значением value и возвращает умный (shared) указатель на этот узел;
    // если элемент не найден - возвращает nullptr.
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

    // Метод проверяет, содержит ли список определённое значение;
    // возвращает true или false.
    bool contains(const T& value) const {
        return find(value) != nullptr;
    }

    // Метод полностью очищает список.
    void clear()
    {
        while (!is_empty()) {
            pop_front();
        }
    }

    /*
    Метод сортирует список, используя пузырьковую сортировку.
    Важно! Данный алгоритм имеет сложность O(n^2), что может быть неэффективно для больших списков.
    */
    void bubble_sort()
    {
        // Если в списке меньше 2-х элементов - сортировать ничего не нужно.
        if (size_of_list < 2) {
            return;
        }

        // Длина списка как минимум 1.
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

    // Метод удаляет первый узел со значением value;
    // возвращает true, если элемент был найден и удалён, иначе - false.
    bool remove(const T& value_to_remove)
    {
        // Если список пустой - мы не можем ничего удалить.
        if (head == nullptr) {
            return false;
        }

        // Если нужно удалить первый элемент:
        if (head->value == value_to_remove)
        {
            pop_front();
            return true;
        }

        // Создаем указатель на текущий узел для итерации по списку.
        std::shared_ptr<ListNode<T>> current_node = head;

        // Данный цикл закончится в двух случаях:
        // 1) Текущий узел стал последним.
        // 2) Узел, на который указывает текущий узел, необходимо удалить.
        while (current_node->next != nullptr && current_node->next->value != value_to_remove) {
            current_node = current_node->next;
        }

        // Если мы вышли из цикла из-за первого условия, то это означает, что мы ничего не нашли - возвращаем false.
        if (current_node->next == nullptr) {
            return false;
        }

        // Если мы зашли сюда, значит узел, на который указывает текущий узел, необходимо удалить.

        // Создаю указатель на узел, который необходимо удалить.
        std::shared_ptr<ListNode<T>> node_to_remove = current_node->next;

        // Теперь текущий узел должен показывать на узел, на который показывал удаляемый узел.
        current_node->next = node_to_remove->next;

        // Если мы удалили последний узел, то хвост должен показывать на текущий узел.
        if (current_node->next == nullptr) {
            tail = current_node;
        }

        // Декрементируем переменную, отвечающую за размер списка.
        --size_of_list;

        return true;
    }

    // Метод добавляет новый элемент в начало списка.
    void push_front(const T& value)
    {
        // Создаем новый узел и получаем на него указатель.
        std::shared_ptr<ListNode<T>> new_node = std::make_shared<ListNode<T>>(value);

        // Новый узел должен показывать на первый элемент списка.
        new_node->next = head;

        // Голова, в свою очередь, теперь должна показывать на наш новый узел.
        head = new_node;

        // Если список был пустым - хвост указывает на наш новый узел.
        if (tail == nullptr) {
            tail = new_node;
        }

        // Инкрементируем переменную, отвечающую за размер списка.
        ++size_of_list;
    }

    // Метод добавляет новый элемент в конец списка.
    void push_back(const T& value)
    {
        // Создаем новый узел и получаем на него указатель.
        std::shared_ptr<ListNode<T>> new_node = std::make_shared<ListNode<T>>(value);

        // Если список пустой - новый узел становится и головой, и хвостом.
        if (head == nullptr)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            // Последний узел списка должен показывать на наш новый элемент.
            tail->next = new_node;

            // Указатель на хвост теперь должен показывать на наш новый элемент ( так как он - последний в списке ).
            tail = new_node;
        }

        // Инкрементируем переменную, отвечающую за размер списка.
        ++size_of_list;
    }

    // Метод удаляет первый элемент из списка;
    // возвращает значение удаленного элемента.
    T pop_front()
    {
        // Если список пустой - мы не сможем ничего удалить.
        if (head == nullptr) {
            throw std::runtime_error("Невозможно удалить элемент из пустого списка.");
        }

        // value - значение элемента, который мы планируем удалить.
        T value = head->value;

        // Указатель на первый элемент списка должен указывать на второй элемент списка,
        // так как после выполнения данного метода второй элемент списка станет первым.
        head = head->next;

        // Если после удаления список стал пустым - указатель на хвост должен показывать на nullptr.
        if (head == nullptr) {
            tail = nullptr;
        }

        // Декрементируем переменную, отвечающую за размер списка.
        --size_of_list;

        return value;
    }

    // Метод удаляет последний элемент из списка;
    // возвращает значение удаленного элемента.
    T pop_back()
    {
        // Если список пустой - мы не сможем ничего удалить.
        if (head == nullptr) {
            throw std::runtime_error("Невозможно удалить элемент из пустого списка.");
        }

        // value - значение элемента, который мы планируем удалить.
        T value = T();

        // Если список состоит из одного элемента - 'обнуляем' голову.
        if (head == tail)
        {
            // Перед тем, как удалить узел, сохраняем значение, которое он хранит.
            value = head->value;

            head = nullptr;
            tail = nullptr;
        }
        else
        {
            // Ищем предпоследний узел.
            std::shared_ptr<ListNode<T>> current_node = head;

            while (current_node->next != tail) {
                current_node = current_node->next;
            }

            // Перед тем, как удалить последний узел, сохраняем значение, которое он хранит.
            value = tail->value;

            tail = current_node;
            tail->next = nullptr;
        }

        // Декрементируем переменную, отвечающую за размер списка.
        --size_of_list;

        return value;
    }

    // Метод преобразует все элементы списка в вид строки; возвращает строку.
    std::string convert_list_to_string(std::string func(const T& value) = [](const T& value) { return std::to_string(value); })
    {
        /*
        Данный метод работает без параметра только в том случае, если функция std::to_string() определена для типа T.

        В противном случае, пользователь должен передать в данный метод функцию,
        преобразовывающую тип T в тип std::string и возвращающую значение типа std::string.

        std::string func_name(const T& some_value) - сигнатура ожидаемой от пользователя функции.
        */

        std::string result = std::string();

        // Если список пустой - в строке result будет находиться соответствующее сообщение.
        if (head == nullptr) {
            result = "List is empty.";
        }
        else
        {
            // Создаем временный указатель для прохода по всем элементам.
            std::shared_ptr<ListNode<T>> current_node = head;

            while (current_node != nullptr)
            {
                // Преобразовываем элемент в строку и добавляем его в result.
                result += func(current_node->value);

                // Если текущий узел не является последним, после элемента выводим разделяющий символ.
                if (current_node->next != nullptr) { result += " -> "; }

                // Переходим к следующему узлу.
                current_node = current_node->next;
            }
        }

        return result;
    }
};