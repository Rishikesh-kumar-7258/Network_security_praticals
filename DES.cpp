#ifndef DES_C
#define DES_C



#include "Cryptography.cpp"
#include <bitset>

class DES : public Cryptography {
private:
	const int ip[64] = { 58, 50, 42, 34, 26, 18, 10, 2,
						60, 52, 44, 36, 28, 20, 12, 4,
						62, 54, 46, 38, 30, 22, 14, 6,
						64, 56, 48, 40, 32, 24, 16, 8,
						57, 49, 41, 33, 25, 17, 9, 1,
						59, 51, 43, 35, 27, 19, 11, 3,
						61, 53, 45, 37, 29, 21, 13, 5,
						63, 55, 47, 39, 31, 23, 15, 7 };

	const int ipInv[64] = { 40, 8, 48, 16, 56, 24, 64, 32,
						39, 7, 47, 15, 55, 23, 63, 31,
						38, 6, 46, 14, 54, 22, 62, 30,
						37, 5, 45, 13, 53, 21, 61, 29,
						36, 4, 44, 12, 52, 20, 60, 28,
						35, 3, 43, 11, 51, 19, 59, 27,
						34, 2, 42, 10, 50, 18, 58, 26,
						33, 1, 41, 9, 49, 17, 57, 25 };

	const int fp[32] = { 16, 7, 20, 21, 29, 12, 28, 17, 
						1, 15, 23, 26, 5, 18, 31, 10,
						2, 8, 24, 14, 32, 27, 3, 
						9, 19, 13, 30, 6, 22, 11, 4, 25 };

	const int E[48] = { 32, 1, 2, 3, 4, 5,
							4, 5, 6, 7, 8, 9,
							8, 9, 10, 11, 12, 13,
							12, 13, 14, 15, 16, 17,
							16, 17, 18, 19, 20, 21,
							20, 21, 22, 23, 24, 25,
							24, 25, 26, 27, 28, 29,
							28, 29, 30, 31, 32, 1 };

	const int sbox[8][4][16] = {
		{
			{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
			{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
			{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
			{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
		},
		{
			{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
			{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
			{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
			{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
		},
		{
			{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
			{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
			{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
			{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
		},
		{
			{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
			{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
			{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
			{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
		},
		{
			{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
			{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
			{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
			{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
		},
		{
			{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
			{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
			{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
			{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
		},
		{
			{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
			{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
			{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
			{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
		},
		{
			{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
			{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
			{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
			{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11},
		}
	};

	const int pc1[56] = { 57, 49, 41, 33, 25, 17, 9,
							1, 58, 50, 42, 34, 26, 18,
							10, 2, 59, 51, 43, 35, 27,
							19, 11, 3, 60, 52, 44, 36,
							63, 55, 47, 39, 31, 23, 15,
							7, 62, 54, 46, 38, 30, 22,
							14, 6, 61, 53, 45, 37, 29,
							21, 13, 5, 28, 20, 12, 4 };

	const int pc2[48] = { 14, 17, 11, 24, 1, 5,
							3, 28, 15, 6, 21, 10,
							23, 19, 12, 4, 26, 8,
							16, 7, 27, 20, 13, 2,
							41, 52, 31, 37, 47, 55,
							30, 40, 51, 45, 33, 48,
							44, 49, 39, 56, 34, 53,
							46, 42, 50, 36, 29, 32 };

	const int shiftBits[16] = { 1, 1, 2, 2, 2, 2, 2, 2,
									1, 2, 2, 2, 2, 2, 2, 1 };

	bool subkeys[16][48];

	int key[64];

	std::string stringToBits(const std::string& plain) {
		std::string bits = "";
		for (int i = 0; i < plain.length(); i++) {
			bits += std::bitset<8>(plain[i]).to_string();
		}
		return bits;
	}

	void leftShift(bool* bits, int n) {
		std::reverse(bits, bits + n);
		std::reverse(bits + n, bits + 28);
		std::reverse(bits, bits + 28);
	}

	void generateSubkeys() {
		// permutated choice 1
		bool pc1Key[56];
		for (int i = 0; i < 56; i++) {
			pc1Key[i] = key[pc1[i] - 1];
		}

		// splitting in c and d
		bool c[28], d[28];
		for (int i = 0; i < 28; i++) {
			c[i] = pc1Key[i];
			d[i] = pc1Key[i + 28];
		}

		// 16 rounds
		for (int i = 0; i < 16; i++) {
			leftShift(c, shiftBits[i]);
			leftShift(d, shiftBits[i]);

			// combining c and d
			bool cd[56];
			for (int j = 0; j < 28; j++) {
				cd[j] = c[j];
				cd[j + 28] = d[j];
			}

			// permutated choice 2
			for (int j = 0; j < 48; j++) {
				subkeys[i][j] = cd[pc2[j] - 1];
			}
		}
	}

public:
	DES(int key[56]) {
		for (int i = 0; i < 64; i++) {
			this->key[i] = key[i];
		}

		generateSubkeys();
	}

	std::string encrypt(std::string plain) { 
		// converting string to bits
		std::string bits = stringToBits(plain);

		// splitting bits into 64-bit blocks
		std::vector<std::string> blocks;
		for (int i = 0; i < bits.length(); i += 64) {
			blocks.push_back(bits.substr(i, 64));
		}

		// checking if the last block is less than 64 bits
		if (blocks[blocks.size() - 1].length() < 64) {
			int paddingAmount = 64 - blocks[blocks.size() - 1].length() % 64;
			for (int i = 0; i < paddingAmount; i++) {
				blocks[blocks.size() - 1] += (char)(paddingAmount);
			}
		}

		std::string cipher;
		for (std::string& s : blocks) cipher += processBlock(s, 0);

		return cipher;
	}

	std::string decrypt(std::string cipher) {
		// converting string to bits
		std::string bits = stringToBits(cipher);

		// splitting bits into 64-bit blocks
		std::vector<std::string> blocks;
		for (int i = 0; i < bits.length(); i += 64) {
			blocks.push_back(bits.substr(i, 64));
		}

		std::string plain;
		for (std::string& s : blocks) plain += processBlock(s, 1);

		return plain;
	}

	/*
	* used to encrypt as well as decrypt the block
	* block - 64 bit block
	* type - 0 for encryption, 1 for decryption
	* 	
	*/
	std::string processBlock(const std::string& block, int type) {
		// initial permutation
		bool ipBlock[64];
		for (int i = 0; i < 64; i++) {
			ipBlock[i] = block[ip[i] - 1] - '0';
		}

		// splitting the block into left and right
		bool left[32], right[32];
		for (int i = 0; i < 32; i++) {
			left[i] = ipBlock[i];
			right[i] = ipBlock[i + 32];
		}


		// 16 rounds
		for (int i = 0; i < 16; i++) {
			roundDes(left, right, subkeys[type ? 15-i : i]);
		}

		// combining left and right
		bool combined[64];
		for (int i = 0; i < 32; i++) {
			combined[i] = right[i];
			combined[i + 32] = left[i];
		}

		// final permutation
		bool fpBlock[64];
		for (int i = 0; i < 64; i++) {
			fpBlock[i] = combined[ipInv[i] - 1];
		}

		// converting bits to string
		std::string cipher = "";
		for (int i = 0; i < 64; i += 8) {
			std::string s = "";
			for (int j = 0; j < 8; j++) {
				s += fpBlock[i + j] + '0';
			}
			cipher += (char)std::bitset<8>(s).to_ulong();
		}

		return cipher;
	}

	void roundDes(bool* left, bool* right, bool *pc2Key) {
		bool rightCopy[32];
		for (int i = 0; i < 32; i++) {
			rightCopy[i] = right[i];
		}

		// expansion
		bool expanded[48];
		for (int i = 0; i < 48; i++) {
			expanded[i] = right[E[i] - 1];
		}

		// xor with key
		for (int i = 0; i < 48; i++) {
			expanded[i] = expanded[i] ^ pc2Key[i];
		}

		// s-box
		bool sboxOutput[32];
		for (int i = 0; i < 8; i++) {
			int row = 2 * expanded[i * 6] + expanded[i * 6 + 5];
			int col = 8 * expanded[i * 6 + 1] + 4 * expanded[i * 6 + 2] + 2 * expanded[i * 6 + 3] + expanded[i * 6 + 4];
			int val = sbox[i][row][col];
			for (int j = 0; j < 4; j++) {
				sboxOutput[i * 4 + j] = (val >> (3 - j)) & 1;
			}
		}

		// straight permutation
		bool straight[32];
		for (int i = 0; i < 32; i++) {
			straight[i] = sboxOutput[fp[i] - 1];
		}

		// xor with left
		for (int i = 0; i < 32; i++) {
			right[i] = left[i] ^ straight[i];
		}

		// left becomes right
		for (int i = 0; i < 32; i++) {
			left[i] = rightCopy[i];
		}

	}
};

#endif // !DES_C
