#ifndef AES_C
#define AES_C

#include "Cryptography.cpp"

class AES : public Cryptography{
private:
	const int nk = 4; // number of 32-bit words in the key
	const int nb = 4; // block size in 32-bit words
	const int nr = 10; // number of rounds

	std::vector<uint32_t> key; // key
	std::vector<uint32_t> w; // expanded key

	// AES S-box
	const unsigned char sbox[256] = {
		// 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, // 0
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, // 1
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, // 2
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, // 3
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, // 4
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, // 5
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, // 6
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, // 7
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, // 8
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, // 9
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, // A
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, // B
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, // C
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, // D
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, // E
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16  // F
	};

	// Rcon
	const uint32_t rcon[10] = {
		0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,
		0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000
	};

	// key expansion algorithm
	void keyExpansion() {

		// rotword and subword
		auto rotWord = [](uint32_t word) {
			return (word << 8) | (word >> 24);
		};
		auto subWord = [this](uint32_t word) {
			uint32_t result = 0;
			for (int i = 0; i < 4; i++) {
				result |= sbox[(word >> (24 - 8 * i)) & 0xff] << (24 - 8 * i);
			}
			return result;
		};

		int i = 0;
		while (i < nk) {
			w[i] = key[i];
			i++;
		}
		while (i < 4 * (nr + 1)) {
			uint32_t temp = w[i - 1];
			
			if (i % nk == 0) {
				temp = subWord(rotWord(temp)) ^ rcon[i / nk];
			}

			w[i] = w[i - nk] ^ temp;
			i++;
		}
	}

	// add round key algorithm
	void addRoundKey(std::vector<std::vector<uint8_t>>& state, int round) {
		for (int i = 0; i < 4; i++) {
			uint32_t temp = w[round * 4 + i];
			for (int j = 0; j < 4; j++) {
				state[j][i] ^= (temp >> (24 - 8 * j)) & 0xff;
			}
		}
	}

	// subBytes algorithm
	void subBytes(std::vector<std::vector<uint8_t>>& state) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				state[i][j] = sbox[state[i][j]];
			}
		}
	}

	// invSubBytes algorithm
	void invSubBytes(std::vector<std::vector<uint8_t>>& state) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				state[i][j] = sbox[state[i][j]];
			}
		}
	}

	// shiftRows algorithm
	void shiftRows(std::vector<std::vector<uint8_t>>& state) {
		for (int i = 1; i < 4; i++) {
			std::rotate(state[i].begin(), state[i].begin() + i, state[i].end());
		}
	}

	// invShiftRows algorithm
	void invShiftRows(std::vector<std::vector<uint8_t>>& state) {
		for (int i = 1; i < 4; i++) {
			std::rotate(state[i].begin(), state[i].begin() + 4 - i, state[i].end());
		}
	}

	// mixColumns algorithm
	void mixColumns(std::vector<std::vector<uint8_t>>& state) {
		std::vector<std::vector<uint8_t>> temp(4, std::vector<uint8_t>(4, 0));
		for (int i = 0; i < 4; i++) {
			temp[0][i] = (uint8_t)(mul(0x02, state[0][i]) ^ mul(0x03, state[1][i]) ^ state[2][i] ^ state[3][i]);
			temp[1][i] = (uint8_t)(state[0][i] ^ mul(0x02, state[1][i]) ^ mul(0x03, state[2][i]) ^ state[3][i]);
			temp[2][i] = (uint8_t)(state[0][i] ^ state[1][i] ^ mul(0x02, state[2][i]) ^ mul(0x03, state[3][i]));
			temp[3][i] = (uint8_t)(mul(0x03, state[0][i]) ^ state[1][i] ^ state[2][i] ^ mul(0x02, state[3][i]));
		}
		state = temp;
	}

	// invMixColumns algorithm
	void invMixColumns(std::vector<std::vector<uint8_t>>& state) {
		std::vector<std::vector<uint8_t>> temp(4, std::vector<uint8_t>(4, 0));
		for (int i = 0; i < 4; i++) {
			temp[0][i] = (uint8_t)(mul(0x0e, state[0][i]) ^ mul(0x0b, state[1][i]) ^ mul(0x0d, state[2][i]) ^ mul(0x09, state[3][i]));
			temp[1][i] = (uint8_t)(mul(0x09, state[0][i]) ^ mul(0x0e, state[1][i]) ^ mul(0x0b, state[2][i]) ^ mul(0x0d, state[3][i]));
			temp[2][i] = (uint8_t)(mul(0x0d, state[0][i]) ^ mul(0x09, state[1][i]) ^ mul(0x0e, state[2][i]) ^ mul(0x0b, state[3][i]));
			temp[3][i] = (uint8_t)(mul(0x0b, state[0][i]) ^ mul(0x0d, state[1][i]) ^ mul(0x09, state[2][i]) ^ mul(0x0e, state[3][i]));
		}
		state = temp;
	}

	// multiplication in GF(2^8)
	uint8_t mul(uint8_t a, uint8_t b) {
		uint8_t result = 0;
		while (b) {
			if (b & 1) {
				result ^= a;
			}
			if (a & 0x80) {
				a = (a << 1) ^ 0x1b;
			}
			else {
				a <<= 1;
			}
			b >>= 1;
		}
		return result;
	}
	
public:
	AES(std::string key) : Cryptography() {
		this->key = std::vector<uint32_t>(4);
		this->w = std::vector<uint32_t>(44);
		for (int i = 0; i < 4; i++) {
			this->key[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16) | (key[4 * i + 2] << 8) | key[4 * i + 3];
		}

		// Key expansion
		keyExpansion();
	}

	std::string encrypt(std::string plainText) {
		// if plaintext is not divisible by 16, pad with spaces
		while (plainText.size() % 16 != 0) {
			plainText += ' ';
		}

		std::string cipherText = "";
		for (int i = 0; i < plainText.size(); i += 16) {
			std::vector<uint8_t> block(16);
			for (int j = 0; j < 16; j++) {
				block[j] = plainText[i + j];
			}

			std::vector<std::vector<uint8_t>> state = blockEncrypt(block);
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					cipherText += (char)state[k][j];
				}
			}
		}

		return cipherText;
	}

	std::string decrypt(std::string cipherText) {

		std::string plaintext = "";
		for (int i = 0; i < plaintext.size(); i += 16) {
			std::vector<uint8_t> block(16);
			for (int j = 0; j < 16; j++) {
				block[j] = cipherText[i + j];
			}

			std::vector<std::vector<uint8_t>> state = blockDecrypt(block);
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					plaintext += (char)state[k][j];
				}
			}
		}

		return plaintext;
	}

	std::vector<std::vector<uint8_t>> blockEncrypt(std::vector<uint8_t> block) {
		std::vector<std::vector<uint8_t>> state(4, std::vector<uint8_t>(4, 0));
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				state[i][j] = block[i + j*4];
			}
		}

		// Add round key
		addRoundKey(state, 0);

		for (int i = 0; i < nr; i++) {
			subBytes(state);
			shiftRows(state);
			mixColumns(state);
			addRoundKey(state, i + 1);
		}

		subBytes(state);
		shiftRows(state);
		addRoundKey(state, nr);

		return state;
	}

	std::vector<std::vector<uint8_t>> blockDecrypt(std::vector<uint8_t> block) {
		std::vector<std::vector<uint8_t>> state(4, std::vector<uint8_t>(4, 0));
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				state[i][j] = block[i + j * 4];
			}
		}

		// Add round key
		addRoundKey(state, nr);

		// roundes
		for (int i = nr - 1; i > 0; i--) {
			invShiftRows(state);
			invSubBytes(state);
			addRoundKey(state, i);
			invMixColumns(state);
		}

		invShiftRows(state);
		invSubBytes(state);
		addRoundKey(state, 0);

		return state;
	}
};

#endif // !AES_C
