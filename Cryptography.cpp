#ifndef CRYPTOGRAPHY_C
#define CRYPTOGRAPHY_C

#include <iostream>
#include <vector>

class Cryptography {
private:
public:
	Cryptography() {}

	virtual std::string encrypt(std::string plain) = 0;
	virtual std::string decrypt(std::string cipher) = 0;

	std::vector<int> removeSpaces(std::string &str) const {
		int pos = 0;
		std::vector<int> spaces;

		int i = 0;
		while (i < str.size()) {
			if (str[i] == ' ') {
				str.erase(i, 1);
				spaces.push_back(i);
			}
			else {
				i++;
			}
		}

		return spaces;
	}

	std::string insertSpaces(std::string str, std::vector<int> spaces) const {
		int i = 0, j = 0;
		std::string newStr;

		while (i < str.size()) {
			if (j < spaces.size() && i == spaces[j]) {
				newStr.push_back(' ');
				j++;
			}
			else {
				newStr.push_back(str[i]);
				i++;
			}
		}

		return newStr;
	}

	std::string toLowerCase(std::string str) const {
			
		for (int i = 0; i < str.size(); i++) {
			if (str[i] >= 'A' && str[i] <= 'Z') {
				str[i] += 32;
			}
		}
		
		return str;
	}

};

#endif // !CRYPTOGRAPHY_C
