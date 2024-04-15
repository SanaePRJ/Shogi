#include <iostream>
#include "board.hpp"

auto view = [](board<int>& a) {
	auto a_itr = a.begin();
	while (a_itr != a.end()) {

		auto c_itr = (*a_itr).begin();
		while (c_itr != (*a_itr).end())
		{
			std::cout << *c_itr << " ";

			c_itr++;
		}

		std::cout << std::endl;
		a_itr++;
	}
	};

int main() {
	board<int> a = 
	{
		{1,2,3},
		{4,5,6}
	};

	a.RotateCounterClockWise();

	view(a);
}