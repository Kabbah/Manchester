#include "Manchester.h"

void printStringBinary(std::string str) {
	for (unsigned int i = 0; i < str.size() + 1; i++) {
		std::bitset<sizeof(char) * 8> bin(str.c_str()[i]);
		std::cout << bin;
	}
	std::cout << std::endl;
}

void printCharArrayBinary(char* str, int length) {
	for (int i = 0; i < length; i++) {
		std::bitset<sizeof(char) * 8> bin(str[i]);
		std::cout << bin;
	}
	std::cout << std::endl;
}

std::string binaryString(std::string str) {
	std::ostringstream oss;

	for (unsigned int i = 0; i < str.size() + 1; i++) {
		std::bitset<sizeof(char) * 8> bin(str.c_str()[i]);
		oss << bin;
	}

	return oss.str();
}

std::string binaryString(const char* str, int length) {
	std::ostringstream oss;

	for (int i = 0; i < length; i++) {
		std::bitset<sizeof(char) * 8> bin(str[i]);
		oss << bin;
	}
	
	return oss.str();
}

char* manchesterEncode(std::string str) {
	// Usa-se str.size() + 1 porque precisa-se do caracter nulo no final da string.
	char* encoded = new char[2 * (str.size() + 1)];

	// i = contador da string
	// j = contador do vetor codificado
	for (unsigned int i = 0, j = 0; i < (str.size() + 1); i++) {
		char character = str.c_str()[i];
		char aux = 0;

		for (int pos = (sizeof(char) * 8) - 1; pos >= 0; pos--) {
			char bit = character >> pos; // Desloca o bit desejado para a posicao LSB
			bit = bit & 1; // Aplica mascara de bit para pegar so o LSB

						   // Como cada caracter vira dois na codificação, quando chega na metade passa para o proximo
			if (pos == sizeof(char) * 4 - 1) {
				encoded[j] = aux;
				aux = 0;
				j++;
			}

			if (bit == 0) {
				// Codificado = 10
				aux = aux << 1;
				aux = aux | 1;
				aux = aux << 1;
			}
			else {
				// Codificado = 01
				aux = aux << 2;
				aux = aux | 1;
			}
		}

		encoded[j] = aux;
		j++;
	}

	return encoded;
}

// Versão anterior da função
//char* manchesterDecode(char* encoded, int length) {
//	char* str = new char[length / 2];
//
//	// i = contador da string
//	// j = contador do vetor codificado
//	for (int i = 0, j = 0; i < length / 2; i++) {
//		char aux = 0;
//		char count = 0;
//
//		for (int pos = sizeof(char) * 8 - 2; pos >= 0; pos -= 2) {
//			// Pega de dois em dois bits
//			char twoBits = encoded[j] >> pos;
//			twoBits = twoBits & 3;
//
//			aux = aux << 1;
//			if (twoBits == 1) {
//				// Se for 01, coloca um 1 no LSB (se for 10, so vai shiftar e ja aparece um 0).
//				aux = aux | 1;
//			}
//			count++;
//
//			// Quando chega em meio char, significa que deve-se passar para o proximo char do vetor codificado
//			// porque cada dois char do vetor codificado equivalem a um char da string
//			if (count == sizeof(char) * 4) {
//				j++;
//				pos = sizeof(char) * 8;
//			}
//		}
//
//		str[i] = aux;
//		j++;
//	}
//
//	return str;
//}

char* manchesterDecode(const char* encoded, int length) {
	char* str = new char[length / 2];

	// i = contador da string
	// j = contador do vetor codificado
	for (int i = 0, j = 0; i < length / 2; i++) {
		char aux = 0;
		char count = 0;

		for (int pos = sizeof(char) * 8 - 2; pos >= 0; pos -= 2) {
			// Pega de dois em dois bits
			char twoBits = encoded[j] >> pos;
			twoBits = twoBits & 3;

			aux = aux << 1;
			if (twoBits == 1) {
				// Se for 01, coloca um 1 no LSB (se for 10, so vai shiftar e ja aparece um 0).
				aux = aux | 1;
			}
			count++;

			// Quando chega em meio char, significa que deve-se passar para o proximo char do vetor codificado
			// porque cada dois char do vetor codificado equivalem a um char da string
			if (count == sizeof(char) * 4) {
				j++;
				pos = sizeof(char) * 8;
			}
		}

		str[i] = aux;
		j++;
	}

	return str;
}