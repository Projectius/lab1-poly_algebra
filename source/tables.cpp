#include "tables.h"
#include <string>
#include <functional>
#include <stdexcept>

using namespace std;

// Имена таблиц по порядку (совпадают с индексами в tableman)
const char* const TABLE_NAMES[] = {
    "LinearArrayTable",
    "LinearListTable",
    "OrderedArrayTable",
    "TreeTable",
    "OpenHashTable",
    "ListHashTable"
};

// Количество таблиц
const int TABLE_COUNT = sizeof(TABLE_NAMES) / sizeof(TABLE_NAMES[0]);


// ==========================================================================
// Реализация базового класса Table
// ==========================================================================

Table::Table(Polynom* /*data*/, size_t /*size*/)
{
    // Конструктор не использует никаких данных – дочерние классы инициализируют свои структуры.
}

Polynom* Table::find(const string& /*name*/)
{
    return nullptr;
}

void Table::add(const string& /*name*/, const Polynom* /*pol*/)
{
    // Пустая реализация базового метода.
}

void Table::remove(const string& /*name*/)
{
    // Пустая реализация базового метода.
}

// ==========================================================================
// Реализация LinearArrayTable
// ==========================================================================

namespace
{
    // Запись для линейного массива
    struct ArrayEntry
    {
        string key;
        Polynom* value;
    };
}

class LinearArrayTableImpl : public LinearArrayTable
{
private:
    ArrayEntry* entries;
    size_t capacity;
    size_t count;
    // Метод для расширения массива
    void resize()
    {
        size_t newCapacity = capacity * 2;
        ArrayEntry* newEntries = new ArrayEntry[newCapacity];
        for (size_t i = 0; i < count; ++i)
            newEntries[i] = entries[i];
        delete[] entries;
        entries = newEntries;
        capacity = newCapacity;
    }
public:
    LinearArrayTableImpl() : entries(nullptr), capacity(10), count(0)
    {
        entries = new ArrayEntry[capacity];
    }
    virtual ~LinearArrayTableImpl()
    {
        delete[] entries;
    }
    Polynom* find(const string& name) override
    {
        for (size_t i = 0; i < count; ++i)
            if (entries[i].key == name)
                return entries[i].value;
        return nullptr;
    }
    void add(const string& name, const Polynom* pol) override
    {
        // Если элемент с таким ключом уже есть, обновляем его (или можно выбрасывать ошибку)
        for (size_t i = 0; i < count; ++i)
            if (entries[i].key == name)
            {
                entries[i].value = const_cast<Polynom*>(pol);
                return;
            }
        if (count == capacity)
            resize();
        entries[count].key = name;
        entries[count].value = const_cast<Polynom*>(pol);
        ++count;
    }
    void remove(const string& name) override
    {
        for (size_t i = 0; i < count; ++i)
            if (entries[i].key == name)
            {
                // Сдвигаем оставшиеся элементы
                for (size_t j = i; j < count - 1; ++j)
                    entries[j] = entries[j + 1];
                --count;
                return;
            }
    }
};

// ==========================================================================
// Реализация LinearListTable
// ==========================================================================

namespace
{
    // Узел для связанного списка
    struct ListNode
    {
        string key;
        Polynom* value;
        ListNode* next;
        ListNode(const string& k, Polynom* v, ListNode* n = nullptr) : key(k), value(v), next(n) {}
    };
}

class LinearListTableImpl : public LinearListTable
{
private:
    ListNode* head;
public:
    LinearListTableImpl() : head(nullptr) {}
    virtual ~LinearListTableImpl()
    {
        while (head)
        {
            ListNode* tmp = head;
            head = head->next;
            delete tmp;
        }
    }
    Polynom* find(const string& name) override
    {
        ListNode* curr = head;
        while (curr)
        {
            if (curr->key == name)
                return curr->value;
            curr = curr->next;
        }
        return nullptr;
    }
    void add(const string& name, const Polynom* pol) override
    {
        // Если элемент существует, обновляем его значение.
        ListNode* curr = head;
        while (curr)
        {
            if (curr->key == name)
            {
                curr->value = const_cast<Polynom*>(pol);
                return;
            }
            curr = curr->next;
        }
        // Вставка нового узла в начало списка.
        head = new ListNode(name, const_cast<Polynom*>(pol), head);
    }
    void remove(const string& name) override
    {
        ListNode* curr = head;
        ListNode* prev = nullptr;
        while (curr)
        {
            if (curr->key == name)
            {
                if (prev)
                    prev->next = curr->next;
                else
                    head = curr->next;
                delete curr;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }
};

// ==========================================================================
// Реализация OrderedArrayTable
// ==========================================================================

namespace
{
    // Запись для упорядоченного массива
    struct OrderedEntry
    {
        string key;
        Polynom* value;
    };
}

class OrderedArrayTableImpl : public OrderedArrayTable
{
private:
    OrderedEntry* entries;
    size_t capacity;
    size_t count;
    void resize()
    {
        size_t newCapacity = capacity * 2;
        OrderedEntry* newEntries = new OrderedEntry[newCapacity];
        for (size_t i = 0; i < count; ++i)
            newEntries[i] = entries[i];
        delete[] entries;
        entries = newEntries;
        capacity = newCapacity;
    }
    // Бинарный поиск для нахождения позиции ключа или места вставки
    int binarySearch(const string& key) const
    {
        int left = 0, right = count - 1;
        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            if (entries[mid].key == key)
                return mid;
            if (entries[mid].key < key)
                left = mid + 1;
            else
                right = mid - 1;
        }
        return -(left + 1); // позиция вставки
    }
public:
    OrderedArrayTableImpl() : capacity(10), count(0)
    {
        entries = new OrderedEntry[capacity];
    }
    virtual ~OrderedArrayTableImpl()
    {
        delete[] entries;
    }
    Polynom* find(const string& name) override
    {
        int pos = binarySearch(name);
        if (pos >= 0)
            return entries[pos].value;
        return nullptr;
    }
    void add(const string& name, const Polynom* pol) override
    {
        int pos = binarySearch(name);
        if (pos >= 0)
        {
            // Обновление существующего элемента.
            entries[pos].value = const_cast<Polynom*>(pol);
            return;
        }
        int insertPos = -(pos + 1);
        if (count == capacity)
            resize();
        // Сдвиг элементов для вставки
        for (size_t i = count; i > (size_t)insertPos; --i)
            entries[i] = entries[i - 1];
        entries[insertPos].key = name;
        entries[insertPos].value = const_cast<Polynom*>(pol);
        ++count;
    }
    void remove(const string& name) override
    {
        int pos = binarySearch(name);
        if (pos < 0)
            return;
        // Сдвиг элементов для удаления найденного элемента.
        for (size_t i = pos; i < count - 1; ++i)
            entries[i] = entries[i + 1];
        --count;
    }
};

// ==========================================================================
// Реализация TreeTable (бинарное дерево поиска)
// ==========================================================================

namespace
{
    struct TreeNode
    {
        string key;
        Polynom* value;
        TreeNode* left;
        TreeNode* right;
        TreeNode(const string& k, Polynom* v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };
}

class TreeTableImpl : public TreeTable
{
private:
    TreeNode* root;
    TreeNode* addNode(TreeNode* node, const string& key, const Polynom* pol)
    {
        if (!node)
            return new TreeNode(key, const_cast<Polynom*>(pol));
        if (key < node->key)
            node->left = addNode(node->left, key, pol);
        else if (key > node->key)
            node->right = addNode(node->right, key, pol);
        else
            node->value = const_cast<Polynom*>(pol);
        return node;
    }
    TreeNode* findNode(TreeNode* node, const string& key)
    {
        if (!node)
            return nullptr;
        if (key == node->key)
            return node;
        if (key < node->key)
            return findNode(node->left, key);
        else
            return findNode(node->right, key);
    }
    // Удаление узла из дерева
    TreeNode* removeNode(TreeNode* node, const string& key)
    {
        if (!node)
            return nullptr;
        if (key < node->key)
            node->left = removeNode(node->left, key);
        else if (key > node->key)
            node->right = removeNode(node->right, key);
        else
        {
            // найденный узел
            if (!node->left)
            {
                TreeNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (!node->right)
            {
                TreeNode* temp = node->left;
                delete node;
                return temp;
            }
            else
            {
                // Найти минимальный в правом поддереве
                TreeNode* temp = node->right;
                while (temp && temp->left)
                    temp = temp->left;
                // Копируем данные
                node->key = temp->key;
                node->value = temp->value;
                node->right = removeNode(node->right, temp->key);
            }
        }
        return node;
    }
    void deleteTree(TreeNode* node)
    {
        if (node)
        {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
public:
    TreeTableImpl() : root(nullptr) {}
    virtual ~TreeTableImpl()
    {
        deleteTree(root);
    }
    Polynom* find(const string& name) override
    {
        TreeNode* node = findNode(root, name);
        return node ? node->value : nullptr;
    }
    void add(const string& name, const Polynom* pol) override
    {
        root = addNode(root, name, pol);
    }
    void remove(const string& name) override
    {
        root = removeNode(root, name);
    }
};

// ==========================================================================
// Реализация OpenHashTable (хеш-таблица с открытой адресацией)
// ==========================================================================

namespace
{
    enum SlotState { EMPTY, OCCUPIED, DELETED };
    struct HashSlot
    {
        string key;
        Polynom* value;
        SlotState state;
        HashSlot() : key(""), value(nullptr), state(EMPTY) {}
    };
}

class OpenHashTableImpl : public OpenHashTable
{
private:
    HashSlot* table;
    size_t capacity;
    size_t count;
    // Простая хеш-функция с использованием std::hash
    size_t hashKey(const string& key) const
    {
        return hash<string>()(key) % capacity;
    }
    void resize()
    {
        size_t oldCapacity = capacity;
        capacity *= 2;
        HashSlot* oldTable = table;
        table = new HashSlot[capacity];
        count = 0;
        // Перехеширование
        for (size_t i = 0; i < oldCapacity; ++i)
            if (oldTable[i].state == OCCUPIED)
                add(oldTable[i].key, oldTable[i].value);
        delete[] oldTable;
    }
public:
    OpenHashTableImpl() : capacity(16), count(0)
    {
        table = new HashSlot[capacity];
    }
    virtual ~OpenHashTableImpl()
    {
        delete[] table;
    }
    Polynom* find(const string& name) override
    {
        size_t idx = hashKey(name);
        for (size_t i = 0; i < capacity; ++i)
        {
            size_t pos = (idx + i) % capacity;
            if (table[pos].state == EMPTY)
                return nullptr;
            if (table[pos].state == OCCUPIED && table[pos].key == name)
                return table[pos].value;
        }
        return nullptr;
    }
    void add(const string& name, const Polynom* pol) override
    {
        if (count * 2 >= capacity) // загрузка более 50%
            resize();
        size_t idx = hashKey(name);
        size_t firstDeleted = capacity; // индекс первого удалённого слота
        for (size_t i = 0; i < capacity; ++i)
        {
            size_t pos = (idx + i) % capacity;
            if (table[pos].state == OCCUPIED)
                if (table[pos].key == name)
                {
                    table[pos].value = const_cast<Polynom*>(pol);
                    return;
                }
                else if (table[pos].state == DELETED && firstDeleted == capacity)
                    firstDeleted = pos;
                else if (table[pos].state == EMPTY)
                {
                    if (firstDeleted != capacity)
                        pos = firstDeleted;
                    table[pos].key = name;
                    table[pos].value = const_cast<Polynom*>(pol);
                    table[pos].state = OCCUPIED;
                    ++count;
                    return;
                }
        }
    }
    void remove(const string& name) override
    {
        size_t idx = hashKey(name);
        for (size_t i = 0; i < capacity; ++i)
        {
            size_t pos = (idx + i) % capacity;
            if (table[pos].state == EMPTY)
                return;
            if (table[pos].state == OCCUPIED && table[pos].key == name)
            {
                table[pos].state = DELETED;
                --count;
                return;
            }
        }
    }
};

// ==========================================================================
// Реализация ListHashTable (хеш-таблица со списками)
// ==========================================================================

namespace
{
    struct ChainNode
    {
        string key;
        Polynom* value;
        ChainNode* next;
        ChainNode(const string& k, Polynom* v, ChainNode* n = nullptr) : key(k), value(v), next(n) {}
    };
}

class ListHashTableImpl : public ListHashTable
{
private:
    ChainNode** buckets;
    size_t capacity;
    size_t count;
    size_t hashKey(const string& key) const
    {
        return hash<string>()(key) % capacity;
    }
public:
    ListHashTableImpl() : capacity(16), count(0)
    {
        buckets = new ChainNode * [capacity];
        for (size_t i = 0; i < capacity; ++i)
            buckets[i] = nullptr;
    }
    virtual ~ListHashTableImpl()
    {
        for (size_t i = 0; i < capacity; ++i)
        {
            ChainNode* node = buckets[i];
            while (node)
            {
                ChainNode* tmp = node;
                node = node->next;
                delete tmp;
            }
        }
        delete[] buckets;
    }
    Polynom* find(const string& name) override
    {
        size_t idx = hashKey(name);
        ChainNode* node = buckets[idx];
        while (node)
        {
            if (node->key == name)
                return node->value;
            node = node->next;
        }
        return nullptr;
    }
    void add(const string& name, const Polynom* pol) override
    {
        size_t idx = hashKey(name);
        ChainNode* node = buckets[idx];
        while (node)
        {
            if (node->key == name)
            {
                node->value = const_cast<Polynom*>(pol);
                return;
            }
            node = node->next;
        }
        // Добавляем новый узел в начало списка
        buckets[idx] = new ChainNode(name, const_cast<Polynom*>(pol), buckets[idx]);
        ++count;
    }
    void remove(const string& name) override
    {
        size_t idx = hashKey(name);
        ChainNode* node = buckets[idx];
        ChainNode* prev = nullptr;
        while (node)
        {
            if (node->key == name)
            {
                if (prev)
                    prev->next = node->next;
                else
                    buckets[idx] = node->next;
                delete node;
                --count;
                return;
            }
            prev = node;
            node = node->next;
        }
    }
};
