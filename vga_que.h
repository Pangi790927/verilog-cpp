#ifndef VGA_QUE_H
#define VGA_QUE_H

#include <atomic>
#include <tuple>

template <int max_size>
struct VgaQue {
	using Elemtent = std::tuple<uint, int>;
	int tail;
	int head;
	std::atomic<int> len;
	Elemtent elements[max_size];

	void push(uint char_color, int index) {
		elements[tail] = Elemtent(char_color, index);
		tail = (tail + 1) % max_size;
		len++;
	}

	Elemtent pop() {
		Elemtent elem = elements[head];
		head = (head + 1) % max_size;
		len--;
		return elem;
	}

	int size() {
		return len;
	}
};


#endif