#include<iostream>;
using namespace std;
class Node {
private:
	int num;
	Node* next;
public:
	Node(int new_num) {
		this->num = new_num;
		this->next = nullptr;
	}
	Node* getNext() {
		return this->next;
	}
	int getNum() {
		return num;
	}
	void setNext(Node* n) {
		next = n;
	}
};

class Stack {
private: 
	Node* head;
public:
	Stack() { this->head = nullptr; };
	bool isEmpty() {
		return head == nullptr;
	}
	void push(int new_num) {
		Node* new_node = new Node(new_num);
		if (!new_node) {
			cout << "Stack overflow\n";
		}
		new_node->setNext(head);
		head = new_node;
	}

	void pop() {
		if (this->isEmpty()) {
			cout << "Stack underflow\n";
		}
		else {
			Node* temp = head;
			head = head->getNext();
			delete temp;
		}
	}

	int get() {
		if (!isEmpty()) {
			return head->getNum();
		}
		else {
			cout << " Stack is empty\n";
			return -1;
		}
	};

	void print() {
		Node* current = head;
		while (current != nullptr) {
			cout << current->getNum() << " ";
			current = current->getNext();
		}
		cout << endl;
	}


};


class Queue {
private:
	Node* begin;
	Node* end;
public:
	Queue() {
		begin = nullptr;
		end = nullptr;
	}
	void push(int value) {
		Node* newNode = new Node(value);
		if (end != nullptr) {
			end->setNext(newNode);
		}
		end = newNode;
		if (begin == nullptr) {
			begin = end;
		}
	}

	void pop() {
		if (begin != nullptr) {
			Node* temp = begin;
			begin = begin->getNext();
			if (begin == nullptr) {
				end = nullptr;
			}
			delete temp;
		}
		else {
			cout << "Queue is empty!\n";
		}
	}

	int get() {
		if (begin != nullptr) {
			return begin->getNum();
		}
		else {
			cout << "Queue is empty!";
			return -1;
		}
	}

	void print() {
		Node* current = begin;
		while (current != nullptr) {
			cout << current->getNum() << " ";
			current = current->getNext();
		}
		cout << endl;
	}

	void reserve() {
		if (begin == nullptr) {
			return;
		}

		Node* prev = nullptr;
		Node* current = begin;
		Node* next = nullptr;
		end = begin;

		while (current != nullptr) {
			next = current->getNext();
			current->setNext(prev);
			prev = current;
			current = next;

		}
		begin = prev;
	}
};

int main() {
	Stack stack;
	stack.push(12);
	stack.push(11);
	stack.print();
	cout << stack.get() <<endl;
	Queue queue;
	queue.push(12);
	queue.push(11);
	queue.print();
	cout << queue.get()<<endl;
	queue.reserve();
	queue.print();
}
