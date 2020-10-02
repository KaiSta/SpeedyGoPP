#include <iostream>
#include <vector>

int main() {
	std::vector<int> val1{ 1,2,3,4,5 };
	auto val2 = val1;

	for (auto& e : val1) std::cout << e << ' ';
	std::cout << std::endl;
	for (auto& e : val2) std::cout << e << ' ';
	std::cout << std::endl;

	val1[2] = 42;
	for (auto& e : val1) std::cout << e << ' ';
	std::cout << std::endl;
	for (auto& e : val2) std::cout << e << ' ';
	std::cout << std::endl;

	val2.resize(10);
	for (auto& e : val1) std::cout << e << ' ';
	std::cout << std::endl;
	for (auto& e : val2) std::cout << e << ' ';
	std::cout << std::endl;
	return 0;
}