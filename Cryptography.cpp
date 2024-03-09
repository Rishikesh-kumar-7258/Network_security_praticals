#include <iostream>
#include <vector>

class Cryptography {
private:
public:
	Cryptography() {}

	std::string encrypt(std::string plain) const {}
	std::string decrypt(std::string cipher) const {}

	std::vector<int> removeSpaces(std::string str) const {
		int pos = 0;
		std::vector<int> spaces;

		for (char c : str) {
			if (c == ' ') {
				str.erase(pos, 1);
				spaces.push_back(pos);
			}
			pos++;
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