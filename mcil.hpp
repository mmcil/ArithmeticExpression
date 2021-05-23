//
// Created by Mevlut Mert CIL
//

#ifndef MCIL_HPP
#define MCIL_HPP

#endif // MCIL_HPP

// mcil.hpp

#include <string>
#include <sstream>
#include <cmath>
#include <list>
#include <array>
#include <set>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <deque>

namespace mcil {

	class INumberSearch {
	public:
		virtual ~INumberSearch() {}

		virtual void SetTargetNumber(const int value) = 0;

		virtual void SetInputNumbers(const std::string &values) = 0;

		virtual const std::string &GetSolution() = 0;
	};


	class NumberSearch : public INumberSearch {
		int targetNumber;
		std::vector<int> inputNumbers;
		std::vector<std::vector<int>> allSubsets;

		void FindAllSubsets() {
			allSubsets.clear();
			int n = inputNumbers.size();
			for (int i = 0; i < std::pow(2, n); i++) {
				int t = i;
				std::vector<int> subSet;
				for (int j = 0; j < n; j++) {
					if (t & 1)
						subSet.push_back(inputNumbers[j]);
					t >>= 1;
				}
				if (!subSet.empty()) {
					this->allSubsets.push_back(subSet);
				}
			}
		}

		std::string SolveForSubset(std::vector<int> &numbers) {
			int targetKey = (this->targetNumber << numbers.size()) + (1 << numbers.size()) - 1;
			std::unordered_set<int> solvedKeys;
			std::unordered_map<int, int> keyToLeftParent;
			std::unordered_map<int, int> keyToRightParent;
			std::unordered_map<int, char> keyToOperator;
			std::deque<int> queue;

			for (int i = 0; i < numbers.size(); i++) {
				int key = (numbers[i] << numbers.size()) + (1 << i);
				solvedKeys.insert(key);
				queue.push_back(key);
			}

			while (queue.size() > 0 && !(solvedKeys.find(targetKey) != solvedKeys.end())) {
				int curKey = queue.front();
				queue.pop_front();
				int curMask = curKey & ((1 << numbers.size()) - 1);
				int curValue = curKey >> numbers.size();

				std::vector<int> keys(solvedKeys.size());
				std::copy(solvedKeys.begin(), solvedKeys.end(), keys.begin());

				for (int i = 0; i < keys.size(); i++) {
					int mask = keys[i] & ((1 << numbers.size()) - 1);
					int value = keys[i] >> numbers.size();

					if ((mask & curMask) == 0) {
						for (int op = 0; op < 6; op++) {
							char opSign = '\0';
							int newValue = 0;
							switch (op) {
								case 0:
									newValue = curValue + value;
									opSign = '+';
									break;
								case 1:
									newValue = curValue - value;
									opSign = '-';
									break;
								case 2:
									newValue = value - curValue;
									opSign = '-';
									break;
								case 3:
									newValue = curValue * value;
									opSign = '*';
									break;
								case 4:
									newValue = -1;
									if (value != 0 && curValue % value == 0) {
										newValue = curValue / value;
									}
									opSign = '/';
									break;
								case 5:
									newValue = -1;
									if (curValue != 0 && value % curValue == 0) {
										newValue = value / curValue;
									}
									opSign = '/';
									break;
							}

							if (newValue >= 0) {
								int newMask = (curMask | mask);
								int newKey = (newValue << numbers.size()) + newMask;

								if (!(solvedKeys.find(newKey) != solvedKeys.end())) {
									solvedKeys.insert(newKey);
									if (op == 2 || op == 5) {
										keyToLeftParent.emplace(newKey, keys[i]);
										keyToRightParent.emplace(newKey, curKey);
									} else {
										keyToLeftParent.emplace(newKey, curKey);
										keyToRightParent.emplace(newKey, keys[i]);
									}
									keyToOperator.emplace(newKey, opSign);
									solvedKeys.insert(newKey);
									queue.push_back(newKey);
								}
							}
						}
					}
				}
			}

			std::stringstream resultStream;
			if (!(solvedKeys.find(targetKey) != solvedKeys.end())) {
				resultStream << "false";
			} else {
				resultStream << PrintExpression(keyToLeftParent, keyToRightParent, keyToOperator, targetKey, numbers.size());
				resultStream << " = " << this->targetNumber << std::endl;
			}
			return resultStream.str();
		}

		static std::string PrintExpression(std::unordered_map<int, int> &keyToLeftParent,
		                                   std::unordered_map<int, int> &keyToRightParent,
		                                   std::unordered_map<int, char> &keyToOperator, int key, int numbersCount) {
			std::stringstream stream;
			if (keyToOperator.find(key) == keyToOperator.end()) {
				stream << (key >> numbersCount);
			} else {
				stream << "(";
				stream << PrintExpression(keyToLeftParent, keyToRightParent, keyToOperator, keyToLeftParent[key], numbersCount);
				stream << keyToOperator[key];
				stream << PrintExpression(keyToLeftParent, keyToRightParent, keyToOperator, keyToRightParent[key], numbersCount);
				stream << ")";
			}
			return stream.str();
		}

	public:
		NumberSearch() {
			this->targetNumber = 0;
		}

		// Number to reach (302)
		void SetTargetNumber(const int value) override {
			this->targetNumber = value;
		}

		// Accept inputNumbers as a space separated values : “2 3 7 10 25 50”
		void SetInputNumbers(const std::string &values) override {
			this->inputNumbers.clear();
			std::istringstream ss(values);
			std::string v;
			while (ss >> v) {
				this->inputNumbers.push_back(std::stoi(v));
			}
			FindAllSubsets();
		}

		// Return result as a string ready to evaluate : “(((2+10)*3)*7)+50)”
		const std::string &GetSolution() {
			static std::string result;
			for (auto &subset : allSubsets) {
				result = SolveForSubset(subset);
				if (result != "false") {
					return result;
				}
			}
			result = "Solution has not been found.\n";
			return result;
		}

		~NumberSearch() override;
	};

	NumberSearch::~NumberSearch() = default;
} // namespace mcil