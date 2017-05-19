#pragma once
#include <iostream>
#include <string>
#include <bitset>
#include <sstream>

void printStringBinary(std::string str);

void printCharArrayBinary(char* str, int length);

std::string binaryString(std::string str);
std::string binaryString(const char* str, int length);

char* manchesterEncode(std::string str);

char* manchesterDecode(const char* encoded, int length);