#include <iostream>
#include "mcil.hpp"

using namespace mcil;

int main() {
	NumberSearch numberSearch;

	numberSearch.SetTargetNumber(302);
	numberSearch.SetInputNumbers("2 3 7 10 25 50");
	std::cout << numberSearch.GetSolution();

	numberSearch.SetTargetNumber(633);
	numberSearch.SetInputNumbers("7 9 2 5 4 1 18");
	std::cout << numberSearch.GetSolution();

	numberSearch.SetTargetNumber(83);
	numberSearch.SetInputNumbers("2 4 10 6 9");
	std::cout << numberSearch.GetSolution();

	numberSearch.SetTargetNumber(41);
	numberSearch.SetInputNumbers("5 9 3 10 4 1");
	std::cout << numberSearch.GetSolution();

	numberSearch.SetTargetNumber(25);
	numberSearch.SetInputNumbers("5 10 2");
	std::cout << numberSearch.GetSolution();

	numberSearch.SetTargetNumber(24);
	numberSearch.SetInputNumbers("1 3 4 6");
	std::cout << numberSearch.GetSolution();

	return 0;
}
