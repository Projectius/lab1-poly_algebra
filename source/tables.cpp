#include "tables.h"
#include <functional>
#include <stdexcept>

using namespace std;

const char* const TABLE_NAMES[] = {
    "LinearArrayTable",
    "LinearListTable",
    "OrderedArrayTable",
    "TreeTable",
    "OpenHashTable",
    "ListHashTable"
};

const int TABLE_COUNT = sizeof(TABLE_NAMES) / sizeof(TABLE_NAMES[0]);

Table::Table(Polynom*, size_t) {}

// ------------------- LinearArrayTable -------------------
LinearArrayTable::LinearArrayTable() : capacity(10), count(0) {
    entries = new ArrayEntry[capacity];
}

LinearArrayTable::~LinearArrayTable() {
    delete[] entries;
}

void LinearArrayTable::resize() {
    size_t newCapacity = capacity * 2;
    ArrayEntry* newEntries = new ArrayEntry[newCapacity];
    for (size_t i = 0; i < count; ++i)
        newEntries[i] = entries[i];
    delete[] entries;
    entries = newEntries;
    capacity = newCapacity;
}

Polynom* LinearArrayTable::find(const string& name) {
    for (size_t i = 0; i < count; ++i)
        if (entries[i].key == name)
            return entries[i].value;
    return nullptr;
}

void LinearArrayTable::add(const string& name, const Polynom* pol) {
    for (size_t i = 0; i < count; ++i)
        if (entries[i].key == name) {
            entries[i].value = const_cast<Polynom*>(pol);
            return;
        }
    if (count == capacity)
        resize();
    entries[count++] = { name, const_cast<Polynom*>(pol) };
}

void LinearArrayTable::remove(const string& name) {
    for (size_t i = 0; i < count; ++i)
        if (entries[i].key == name) {
            for (size_t j = i; j < count - 1; ++j)
                entries[j] = entries[j + 1];
            --count;
            return;
        }
}

// ------------------- LinearListTable -------------------
LinearListTable::LinearListTable() : head(nullptr) {}

LinearListTable::~LinearListTable() {
    while (head) {
        ListNode* tmp = head;
        head = head->next;
        delete tmp;
    }
}

Polynom* LinearListTable::find(const string& name) {
    ListNode* curr = head;
    while (curr) {
        if (curr->key == name)
            return curr->value;
        curr = curr->next;
    }
    return nullptr;
}

void LinearListTable::add(const string& name, const Polynom* pol) {
    ListNode* curr = head;
    while (curr) {
        if (curr->key == name) {
            curr->value = const_cast<Polynom*>(pol);
            return;
        }
        curr = curr->next;
    }
    head = new ListNode{ name, const_cast<Polynom*>(pol), head };
}

void LinearListTable::remove(const string& name) {
    ListNode* curr = head;
    ListNode* prev = nullptr;
    while (curr) {
        if (curr->key == name) {
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

// ------------------- OrderedArrayTable -------------------
OrderedArrayTable::OrderedArrayTable() : capacity(10), count(0) {
    entries = new OrderedEntry[capacity];
}

OrderedArrayTable::~OrderedArrayTable() {
    delete[] entries;
}

void OrderedArrayTable::resize() {
    size_t newCapacity = capacity * 2;
    OrderedEntry* newEntries = new OrderedEntry[newCapacity];
    for (size_t i = 0; i < count; ++i)
        newEntries[i] = entries[i];
    delete[] entries;
    entries = newEntries;
    capacity = newCapacity;
}

int OrderedArrayTable::binarySearch(const string& key) const {
    int left = 0, right = static_cast<int>(count) - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (entries[mid].key == key) return mid;
        if (entries[mid].key < key) left = mid + 1;
        else right = mid - 1;
    }
    return -(left + 1);
}

Polynom* OrderedArrayTable::find(const string& name) {
    int pos = binarySearch(name);
    return (pos >= 0) ? entries[pos].value : nullptr;
}

void OrderedArrayTable::add(const string& name, const Polynom* pol) {
    int pos = binarySearch(name);
    if (pos >= 0) {
        entries[pos].value = const_cast<Polynom*>(pol);
        return;
    }
    int insertPos = -(pos + 1);
    if (count == capacity)
        resize();
    for (size_t i = count; i > static_cast<size_t>(insertPos); --i)
        entries[i] = entries[i - 1];
    entries[insertPos] = { name, const_cast<Polynom*>(pol) };
    ++count;
}

void OrderedArrayTable::remove(const string& name) {
    int pos = binarySearch(name);
    if (pos < 0) return;
    for (size_t i = pos; i < count - 1; ++i)
        entries[i] = entries[i + 1];
    --count;
}

// ------------------- TreeTable -------------------
TreeTable::TreeTable() : root(nullptr) {}

TreeTable::~TreeTable() { deleteTree(root); }

TreeTable::TreeNode* TreeTable::addNode(TreeNode* node, const string& key, const Polynom* pol) {
    if (!node) return new TreeNode{ key, const_cast<Polynom*>(pol), nullptr, nullptr };
    if (key < node->key)
        node->left = addNode(node->left, key, pol);
    else if (key > node->key)
        node->right = addNode(node->right, key, pol);
    else
        node->value = const_cast<Polynom*>(pol);
    return node;
}

TreeTable::TreeNode* TreeTable::findNode(TreeNode* node, const string& key) {
    if (!node || node->key == key) return node;
    return key < node->key ? findNode(node->left, key) : findNode(node->right, key);
}

TreeTable::TreeNode* TreeTable::removeNode(TreeNode* node, const string& key) {
    if (!node) return nullptr;
    if (key < node->key) node->left = removeNode(node->left, key);
    else if (key > node->key) node->right = removeNode(node->right, key);
    else {
        if (!node->left) {
            TreeNode* tmp = node->right;
            delete node;
            return tmp;
        }
        else if (!node->right) {
            TreeNode* tmp = node->left;
            delete node;
            return tmp;
        }
        TreeNode* minNode = node->right;
        while (minNode->left) minNode = minNode->left;
        node->key = minNode->key;
        node->value = minNode->value;
        node->right = removeNode(node->right, minNode->key);
    }
    return node;
}

void TreeTable::deleteTree(TreeNode* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

Polynom* TreeTable::find(const string& name) {
    TreeNode* node = findNode(root, name);
    return node ? node->value : nullptr;
}

void TreeTable::add(const string& name, const Polynom* pol) {
    root = addNode(root, name, pol);
}

void TreeTable::remove(const string& name) {
    root = removeNode(root, name);
}

// ------------------- OpenHashTable -------------------
OpenHashTable::OpenHashTable() : capacity(16), count(0) {
    table = new HashSlot[capacity];
    for (size_t i = 0; i < capacity; ++i)
        table[i].state = EMPTY;
}

OpenHashTable::~OpenHashTable() {
    delete[] table;
}

size_t OpenHashTable::hashKey(const string& key) const {
    return hash<string>()(key) % capacity;
}

void OpenHashTable::resize() {
    size_t oldCapacity = capacity;
    HashSlot* oldTable = table;
    capacity *= 2;
    table = new HashSlot[capacity];
    count = 0;
    for (size_t i = 0; i < oldCapacity; ++i) {
        if (oldTable[i].state == OCCUPIED)
            add(oldTable[i].key, oldTable[i].value);
    }
    delete[] oldTable;
}

Polynom* OpenHashTable::find(const string& name) {
    size_t idx = hashKey(name);
    for (size_t i = 0; i < capacity; ++i) {
        size_t pos = (idx + i) % capacity;
        if (table[pos].state == EMPTY)
            return nullptr;
        if (table[pos].state == OCCUPIED && table[pos].key == name)
            return table[pos].value;
    }
    return nullptr;
}

void OpenHashTable::add(const string& name, const Polynom* pol) {
    if (count * 2 >= capacity)
        resize();
    size_t idx = hashKey(name);
    size_t firstDeleted = capacity;
    for (size_t i = 0; i < capacity; ++i) {
        size_t pos = (idx + i) % capacity;
        if (table[pos].state == OCCUPIED) {
            if (table[pos].key == name) {
                table[pos].value = const_cast<Polynom*>(pol);
                return;
            }
        }
        else if (table[pos].state == DELETED && firstDeleted == capacity) {
            firstDeleted = pos;
        }
        else if (table[pos].state == EMPTY) {
            if (firstDeleted != capacity) pos = firstDeleted;
            table[pos] = { name, const_cast<Polynom*>(pol), OCCUPIED };
            ++count;
            return;
        }
    }
}

void OpenHashTable::remove(const string& name) {
    size_t idx = hashKey(name);
    for (size_t i = 0; i < capacity; ++i) {
        size_t pos = (idx + i) % capacity;
        if (table[pos].state == EMPTY)
            return;
        if (table[pos].state == OCCUPIED && table[pos].key == name) {
            table[pos].state = DELETED;
            --count;
            return;
        }
    }
}

// ------------------- ListHashTable -------------------
ListHashTable::ListHashTable() : capacity(16), count(0) {
    buckets = new ChainNode * [capacity];
    for (size_t i = 0; i < capacity; ++i)
        buckets[i] = nullptr;
}

ListHashTable::~ListHashTable() {
    for (size_t i = 0; i < capacity; ++i) {
        ChainNode* node = buckets[i];
        while (node) {
            ChainNode* tmp = node;
            node = node->next;
            delete tmp;
        }
    }
    delete[] buckets;
}

size_t ListHashTable::hashKey(const string& key) const {
    return hash<string>()(key) % capacity;
}

Polynom* ListHashTable::find(const string& name) {
    size_t idx = hashKey(name);
    ChainNode* node = buckets[idx];
    while (node) {
        if (node->key == name)
            return node->value;
        node = node->next;
    }
    return nullptr;
}

void ListHashTable::add(const string& name, const Polynom* pol) {
    size_t idx = hashKey(name);
    ChainNode* node = buckets[idx];
    while (node) {
        if (node->key == name) {
            node->value = const_cast<Polynom*>(pol);
            return;
        }
        node = node->next;
    }
    buckets[idx] = new ChainNode{ name, const_cast<Polynom*>(pol), buckets[idx] };
    ++count;
}

void ListHashTable::remove(const string& name) {
    size_t idx = hashKey(name);
    ChainNode* node = buckets[idx];
    ChainNode* prev = nullptr;
    while (node) {
        if (node->key == name) {
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
