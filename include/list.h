#pragma once

template <typename T>
class List
{
	struct Node
	{
		T value;
		Node* next;
	};
	Node* head;

public:
	class iterator;
        void add(T& elem);
};
