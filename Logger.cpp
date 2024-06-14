#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>
#include <sys/types.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
bool IsProcessRunning(const wchar_t *processName)
{
	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_wcsicmp((const wchar_t*)entry.szExeFile, processName))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}
namespace Big
{

	Logger::Logger()
	{
		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);
		SetConsoleTitle("Spirit");
	}

	Logger::~Logger() noexcept
	{
		std::cout << "\n[Spirit] | Logger | You may now close this window without crashing.";
		fclose(stdin);
		fclose(stdout);
		fclose(stderr);
		FreeConsole();
	}
#ifdef INJ_DEBUG
#define PREFIX "DEBUG"
#else
#define PREFIX "RELEASE"
#endif


	std::string base64_encode(const std::string& in) {

		std::string out;

		int val = 0, valb = -6;
		for (int jj = 0; jj < in.size(); jj++) {
			char c = in[jj];
			val = (val << 8) + c;
			valb += 8;
			while (valb >= 0) {
				out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
				valb -= 6;
			}
		}
		if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
		while (out.size() % 4) out.push_back('=');
		return out;
	}

	std::string base64_decode(const std::string& in) {

		std::string out;

		std::vector<int> T(256, -1);
		for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

		int val = 0, valb = -8;
		for (int jj = 0; jj < in.size(); jj++) {
			char c = in[jj];
			if (T[c] == -1) break;
			val = (val << 6) + T[c];
			valb += 6;
			if (valb >= 0) {
				out.push_back(char((val >> valb) & 0xFF));
				valb -= 8;
			}
		}
		return out;
	}

	std::string AVAILABLE_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";

	int index(char c) {
		for (int ii = 0; ii < AVAILABLE_CHARS.size(); ii++) {
			if (AVAILABLE_CHARS[ii] == c) {
				// std::cout << ii << " " << c << std::endl;
				return ii;
			}
		}
		return -1;
	}

	std::string extend_key(std::string msg, std::string key) {
		//generating new key
		int msgLen = msg.size();
		std::string newKey(msgLen, 'x');
		int keyLen = key.size(), i, j;
		for (i = 0, j = 0; i < msgLen; ++i, ++j) {
			if (j == keyLen)
				j = 0;

			newKey[i] = key[j];
		}
		newKey[i] = '\0';
		return newKey;
	}


	std::string encrypt_vigenere(std::string msg, std::string key) {
		int msgLen = msg.size(), keyLen = key.size(), i, j;
		std::string encryptedMsg(msgLen, 'x');
		// char newKey[msgLen], encryptedMsg[msgLen], decryptedMsg[msgLen];

		std::string newKey = extend_key(msg, key);

		//encryption
		for (i = 0; i < msgLen; ++i) {
			// std::cout << msg[i] << " " << isalnum(msg[i]) << std::endl;
			if (isalnum(msg[i]) or msg[i] == ' ') {
				encryptedMsg[i] = AVAILABLE_CHARS[((index(msg[i]) + index(newKey[i])) % AVAILABLE_CHARS.size())];
			}
			else {
				encryptedMsg[i] = msg[i];
			}
		}

		encryptedMsg[i] = '\0';
		return encryptedMsg;
	}

	std::string decrypt_vigenere(std::string encryptedMsg, std::string newKey) {
		// decryption
		int msgLen = encryptedMsg.size();
		std::string decryptedMsg(msgLen, 'x');
		int i;
		for (i = 0; i < msgLen; ++i) {
			if (isalnum(encryptedMsg[i]) or encryptedMsg[i] == ' ') {
				decryptedMsg[i] = AVAILABLE_CHARS[(((index(encryptedMsg[i]) - index(newKey[i])) + AVAILABLE_CHARS.size()) % AVAILABLE_CHARS.size())];
			}
			else {
				decryptedMsg[i] = encryptedMsg[i];
			}
		}
		decryptedMsg[i] = '\0';
		return decryptedMsg;
	}

	std::string Logger::encrypt(std::string msg, std::string key) {
		std::string b64_str = base64_encode(msg);
		std::string vigenere_msg = encrypt_vigenere(b64_str, key);
		// std::cout << vigenere_msg << std::endl;
		return vigenere_msg;
	}
	std::string Logger::decrypt(std::string encrypted_msg, std::string key) {
		std::string newKey = extend_key(encrypted_msg, key);
		std::string b64_encoded_str = decrypt_vigenere(encrypted_msg, newKey);
		std::string b64_decode_str = base64_decode(b64_encoded_str);
		return b64_decode_str;
	}


	void Logger::Info(const char* format, ...)
	{
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		int hours = now->tm_hour > 12 ? now->tm_hour - 12 : now->tm_hour;
		int minute = now->tm_min;
		int second = now->tm_sec;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "INFO";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (hours < 10 ? ("0" + std::to_string(hours)).c_str() : std::to_string(hours).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (minute < 10 ? ("0" + std::to_string(minute)).c_str() : std::to_string(minute).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (second < 10 ? ("0" + std::to_string(second)).c_str() : std::to_string(second).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "Spirit";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << format << "\n";
	}

	void Logger::Chat(const char* format, ...)
	{
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		int hours = now->tm_hour > 12 ? now->tm_hour - 12 : now->tm_hour;
		int minute = now->tm_min;
		int second = now->tm_sec;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "CHAT";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (hours < 10 ? ("0" + std::to_string(hours)).c_str() : std::to_string(hours).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (minute < 10 ? ("0" + std::to_string(minute)).c_str() : std::to_string(minute).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (second < 10 ? ("0" + std::to_string(second)).c_str() : std::to_string(second).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "Spirit";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << format << "\n";
	}


	void Logger::Hooks(const char* format, ...)
	{
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		int hours = now->tm_hour > 12 ? now->tm_hour - 12 : now->tm_hour;
		int minute = now->tm_min;
		int second = now->tm_sec;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "HOOKING";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (hours < 10 ? ("0" + std::to_string(hours)).c_str() : std::to_string(hours).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (minute < 10 ? ("0" + std::to_string(minute)).c_str() : std::to_string(minute).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (second < 10 ? ("0" + std::to_string(second)).c_str() : std::to_string(second).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "Spirit";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << format << "\n";
	}
	void Logger::Invoker(const char* format, ...)
	{
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		int hours = now->tm_hour > 12 ? now->tm_hour - 12 : now->tm_hour;
		int minute = now->tm_min;
		int second = now->tm_sec;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "INVOKER";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (hours < 10 ? ("0" + std::to_string(hours)).c_str() : std::to_string(hours).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (minute < 10 ? ("0" + std::to_string(minute)).c_str() : std::to_string(minute).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (second < 10 ? ("0" + std::to_string(second)).c_str() : std::to_string(second).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "Spirit";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << format << "\n";
	}

	void Logger::Manager(const char* format, ...)
	{
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		int hours = now->tm_hour > 12 ? now->tm_hour - 12 : now->tm_hour;
		int minute = now->tm_min;
		int second = now->tm_sec;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "MANAGER";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (hours < 10 ? ("0" + std::to_string(hours)).c_str() : std::to_string(hours).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (minute < 10 ? ("0" + std::to_string(minute)).c_str() : std::to_string(minute).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (second < 10 ? ("0" + std::to_string(second)).c_str() : std::to_string(second).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "Spirit";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << format << "\n";
	}
	void Logger::Pattern(const char* format, ...)
	{
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		int hours = now->tm_hour > 12 ? now->tm_hour - 12 : now->tm_hour;
		int minute = now->tm_min;
		int second = now->tm_sec;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "PATTERN";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (hours < 10 ? ("0" + std::to_string(hours)).c_str() : std::to_string(hours).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (minute < 10 ? ("0" + std::to_string(minute)).c_str() : std::to_string(minute).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (second < 10 ? ("0" + std::to_string(second)).c_str() : std::to_string(second).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "Spirit";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << format << "\n";
	}
	void Logger::Script(const char* format, ...)
	{
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		int hours = now->tm_hour > 12 ? now->tm_hour - 12 : now->tm_hour;
		int minute = now->tm_min;
		int second = now->tm_sec;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "SCRIPT";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (hours < 10 ? ("0" + std::to_string(hours)).c_str() : std::to_string(hours).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (minute < 10 ? ("0" + std::to_string(minute)).c_str() : std::to_string(minute).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (second < 10 ? ("0" + std::to_string(second)).c_str() : std::to_string(second).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "Spirit";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << format << "\n";
	}
	void Logger::Protections(const char* format, ...)
	{
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		int hours = now->tm_hour > 12 ? now->tm_hour - 12 : now->tm_hour;
		int minute = now->tm_min;
		int second = now->tm_sec;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "PROTEX";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (hours < 10 ? ("0" + std::to_string(hours)).c_str() : std::to_string(hours).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (minute < 10 ? ("0" + std::to_string(minute)).c_str() : std::to_string(minute).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << ":";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << (second < 10 ? ("0" + std::to_string(second)).c_str() : std::to_string(second).c_str());
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "]";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << " [";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "Spirit";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "[";
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << "|";
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		std::cout << "] ";

		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		std::cout << format << "\n";
	}

	void Logger::Error(const char* format, ...)
	{
		std::va_list args{};
		va_start(args, format);
		Log("Error", format, args);
		va_end(args);

		g_Running = false;
	}

	void Logger::Log(const char* type, const char* format, std::va_list args)
	{
		std::lock_guard lock(m_Mutex);

		auto time = std::time(nullptr);
		auto tm = std::localtime(&time);

		char prefix[64] = {};
		std::printf("test");
		std::snprintf(prefix, sizeof(prefix) - 1, "[%02d:%02d:%02d] [%s] ", tm->tm_hour, tm->tm_min, tm->tm_sec, type);

		std::size_t messageLength = std::vsnprintf(nullptr, 0, format, args) + 1;
		auto messageBuffer = std::make_unique<char[]>(messageLength);

		std::uninitialized_fill_n(messageBuffer.get(), messageLength, '\0');
		std::vsnprintf(messageBuffer.get(), messageLength, format, args);

		m_File << prefix << messageBuffer.get() << std::endl;

		m_Messages.push_back(std::move(messageBuffer));
	}

	std::mutex& Logger::GetMutex()
	{
		return m_Mutex;
	}

	std::pair<std::unique_ptr<char[]>*, std::size_t> Logger::GetMessages()
	{
		return std::make_pair(m_Messages.data(), m_Messages.size());
	}

	std::istream& Logger::GetInput()
	{
		return m_Input;
	}
}
