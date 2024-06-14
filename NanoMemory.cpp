//Memory.cpp
#include "NanoMemory.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Library Files:
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Winmm.lib")

// Windows Header Files:
#include <windows.h>
#include <Mmsystem.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <Psapi.h>
#include <timeapi.h>
#include <time.h>

namespace Big
{



	namespace Memory {

		void TransformPattern(const std::string & pattern, std::string & data, std::string & mask);

		class executable_meta {
		private:

			uintptr_t	m_begin;
			uintptr_t	m_end;
			DWORD		m_size;
		public:

			executable_meta()
				: m_begin(0), m_end(0) {
			}

			void EnsureInit();

			inline uintptr_t begin() { return m_begin; }
			inline uintptr_t end() { return m_end; }
			inline DWORD size() { return m_size; }
		};

		class pattern_match {
		private:
			void*	m_pVoid = nullptr;
			void*	m_pBegin = nullptr;
			void*	m_pEnd = nullptr;
			void * m_pointer;

		public:

			inline pattern_match(void * pointer) {

				m_pointer = pointer;
			}
			template <typename rT>
			rT*	section_begin()
			{
				return reinterpret_cast<rT*>(m_pBegin);
			}

			template <typename rT>
			rT*	section_end()
			{
				return reinterpret_cast<rT*>(m_pEnd);
			}
			template<typename T>
			T * get(int offset) {

				if (m_pointer == nullptr) {
					return nullptr;
				}

				char * ptr = reinterpret_cast<char*>(m_pointer);
				return reinterpret_cast<T*>(ptr + offset);
			}
			template<typename T>
			T * get() {

				return get<T>(0);
			}
		};

		typedef std::vector<pattern_match> matchVec;

		class pattern {
		private:

			std::string			m_bytes;
			std::string			m_mask;

			uint64_t			m_hash;

			size_t				m_size;

			matchVec			m_matches;

			bool				m_matched;

		private:

			void Initialize(const char* pattern, size_t length);

			bool ConsiderMatch(uintptr_t offset);

			void EnsureMatches(int maxCount);

		public:

			template<size_t Len>
			pattern(const char(&pattern)[Len]) {

				Initialize(pattern, Len);
			}

			inline pattern & count(int expected) {

				if (!m_matched) {
					EnsureMatches(expected);
				}

				return *this;
			}

			inline size_t size() {

				if (!m_matched) {
					EnsureMatches(INT_MAX);
				}

				return m_matches.size();
			}

			inline pattern_match & get(int index) {

				if (!m_matched) {
					EnsureMatches(INT_MAX);
				}

				if (m_matches.size() == 0) {

					m_matches.push_back(pattern_match(nullptr));
					return m_matches[0];
				}

				return m_matches[index];
			}

		public:
			// define a hint
			static void hint(uint64_t hash, uintptr_t address);
		};

		// for link /DYNAMICBASE executables
		static ptrdiff_t baseAddressDifference;

		// sets the base address difference based on an obtained pointer
		inline void set_base(uintptr_t address)
		{
#ifdef _M_IX86
			uintptr_t addressDiff = (address - 0x400000);
#elif defined(_M_AMD64)
			uintptr_t addressDiff = (address - 0x140000000);
#endif

			// pointer-style cast to ensure unsigned overflow ends up copied directly into a signed value
			baseAddressDifference = *(ptrdiff_t*)&addressDiff;
		}

		// gets the main base of the process
		uintptr_t get_base();
		DWORD64 GetBase();
		DWORD get_size();
		// gets the main base of the process with Offset
		uintptr_t get_base_offsetted(DWORD offset);
		struct module
		{
			DWORD64 dwBase, dwSize;
		};
		// gets a multilayer pointer address
		uintptr_t get_multilayer_pointer(uintptr_t base_address, std::vector<DWORD> offsets);
		uintptr_t FindPointer(HANDLE hproc, uintptr_t ptr, std::vector<unsigned int> offsets);
		DWORD64 FindSignature(DWORD64 start, DWORD64 size, const char* sig, const char* mask);
		bool compare(const uint8_t* pData, const uint8_t* bMask, const char* sMask);
		// sets the base to the process main base
		inline void set_base()
		{
			set_base((uintptr_t)GetModuleHandle(NULL));
		}

		// adjusts the address passed to the base as set above
		template<typename T>
		inline void adjust_base(T& address)
		{
			*(uintptr_t*)&address += baseAddressDifference;
		}
		// returns the adjusted address to the stated base
		template<typename T>
		inline uintptr_t get_adjusted(T address)
		{
			return (uintptr_t)address + baseAddressDifference;
		}

		template<typename ValueType, typename AddressType>
		inline void put(AddressType address, ValueType value)
		{
			adjust_base(address);

			memcpy((void*)address, &value, sizeof(value));
		}

		template<typename ValueType, typename AddressType>
		inline void putVP(AddressType address, ValueType value)
		{
			adjust_base(address);

			DWORD oldProtect;
			VirtualProtect((void*)address, sizeof(value), PAGE_EXECUTE_READWRITE, &oldProtect);

			memcpy((void*)address, &value, sizeof(value));

			VirtualProtect((void*)address, sizeof(value), oldProtect, &oldProtect);
		}

		template<typename AddressType>
		inline void nop(AddressType address, size_t length)
		{
			adjust_base(address);

			memset((void*)address, 0x90, length);
		}

		template<typename AddressType>
		inline void return_function(AddressType address, uint16_t stackSize = 0)
		{
			if (stackSize == 0)
			{
				put<uint8_t>(address, 0xC3);
			}
			else
			{
				put<uint8_t>(address, 0xC2);
				put<uint16_t>((uintptr_t)address + 1, stackSize);
			}
		}

		template<typename T>
		inline T* getRVA(uintptr_t rva)
		{
#ifdef _M_IX86
			return (T*)(baseAddressDifference + 0x400000 + rva);
#elif defined(_M_AMD64)
			return (T*)(0x140000000 + rva);
#endif
		}

		template<typename T, typename AT>
		inline void jump(AT address, T func)
		{
			put<uint8_t>(address, 0xE9);
			put<int>((uintptr_t)address + 1, (intptr_t)func - (intptr_t)get_adjusted(address) - 5);
		}

		template<typename T, typename AT>
		inline void call(AT address, T func)
		{
			put<uint8_t>(address, 0xE8);
			put<int>((uintptr_t)address + 1, (intptr_t)func - (intptr_t)get_adjusted(address) - 5);
		}

		template<typename T>
		inline T get_call(T address)
		{
			intptr_t target = *(uintptr_t*)(get_adjusted(address) + 1);
			target += (get_adjusted(address) + 5);

			return (T)target;
		}

		template<typename TTarget, typename T>
		inline void set_call(TTarget* target, T address)
		{
			*(T*)target = get_call(address);
		}

		namespace vp
		{
			template<typename T, typename AT>
			inline void jump(AT address, T func)
			{
				putVP<uint8_t>(address, 0xE9);
				putVP<int>((uintptr_t)address + 1, (intptr_t)func - (intptr_t)get_adjusted(address) - 5);
			}

			template<typename T, typename AT>
			inline void call(AT address, T func)
			{
				putVP<uint8_t>(address, 0xE8);
				putVP<int>((uintptr_t)address + 1, (intptr_t)func - (intptr_t)get_adjusted(address) - 5);
			}
		}

		std::vector<DWORD64> get_string_addresses(std::string str);
		//std::unique_ptr<Big::GameVariables> g_GameVariables;
		//std::unique_ptr<Big::GameFunctions> g_GameFunctions;
		template <typename T>
		T get_value(std::vector<DWORD> offsets) {

			uintptr_t Addr = get_multilayer_pointer((uintptr_t)g_GameVariables->m_WorldPtr, offsets);
			if (Addr == NULL) {
				return NULL;
			}

			return *((T*)Addr);
		}
		template <typename T>
		void set_value(std::vector<DWORD> offsets, T value) {
			uintptr_t Addr = get_multilayer_pointer((uintptr_t)g_GameVariables->m_WorldPtr, offsets);
			if (Addr == NULL) {
				return;
			}

			*reinterpret_cast<T*>(Addr) = value;
		}

	}
}


namespace NanoMemory
{
	static std::multimap<uint64_t, uintptr_t> g_hints;

	void NanoMemory::executable_meta::EnsureInit() {

		if (m_begin) {
			return;
		}

		HMODULE gModule = GetModuleHandle(NULL);

		m_begin = reinterpret_cast<uintptr_t>(gModule);
		const IMAGE_DOS_HEADER * dosHeader = reinterpret_cast<const IMAGE_DOS_HEADER*>(gModule);
		const IMAGE_NT_HEADERS * ntHeader = reinterpret_cast<const IMAGE_NT_HEADERS64*>(reinterpret_cast<const uint8_t*>(dosHeader) + dosHeader->e_lfanew);
		m_end = m_begin + ntHeader->OptionalHeader.SizeOfCode;
		m_size = ntHeader->OptionalHeader.SizeOfImage;
	}

	void NanoMemory::TransformPattern(const std::string & pattern, std::string & data, std::string & mask) {

		std::stringstream dataStr;
		std::stringstream maskStr;

		uint8_t tempDigit = 0;
		bool tempFlag = false;

		for (auto & ch : pattern) {

			if (ch == ' ') {
				continue;
			}
			else if (ch == '?') {

				dataStr << '\x00';
				maskStr << '?';
			}
			else if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f')) {

				char str[] = { ch, 0 };
				int thisDigit = strtol(str, nullptr, 16);

				if (!tempFlag) {

					tempDigit = (thisDigit << 4);
					tempFlag = true;
				}
				else {

					tempDigit |= thisDigit;
					tempFlag = false;

					dataStr << tempDigit;
					maskStr << 'x';
				}
			}
		}

		data = dataStr.str();
		mask = maskStr.str();
	}

	void NanoMemory::pattern::Initialize(const char* pattern, size_t length) {

	}

	uintptr_t NanoMemory::get_base()
	{
		executable_meta executable;
		executable.EnsureInit();
		return executable.begin();
	}

	DWORD NanoMemory::get_size()
	{
		executable_meta executable;
		executable.EnsureInit();
		return executable.size();
	}

	uintptr_t NanoMemory::get_base_offsetted(DWORD offset)
	{
		return get_base() + offset;
	}

	uintptr_t NanoMemory::get_multilayer_pointer(uintptr_t base_address, std::vector<DWORD> offsets)
	{
		uintptr_t ptr = *(uintptr_t*)(base_address);
		if (!ptr) {

			return NULL;
		}
		auto level = offsets.size();

		for (auto i = 0; i < level; i++)
		{
			if (i == level - 1)
			{
				ptr += offsets[i];
				if (!ptr) return NULL;
				return ptr;
			}
			else
			{
				ptr = *(uint64_t*)(ptr + offsets[i]);
				if (!ptr) return NULL;
			}
		}

		return ptr;
	}

	static bool compare(const uint8_t* pData, const uint8_t* bMask, const char* sMask)
	{
		for (; *sMask; ++sMask, ++pData, ++bMask)
			if (*sMask == 'x' && *pData != *bMask)
				return false;

		return *sMask == NULL;
	}

	std::vector<DWORD64> NanoMemory::get_string_addresses(std::string str)
	{
		std::string currentMask;
		const char* to_scan = str.c_str();
		for (uint8_t i = 0; i < strlen(to_scan); i++) currentMask += "x";
		const char *mask = currentMask.c_str();
		std::vector<DWORD64> foundAddrs;
		for (uint32_t i = 0; i < get_size(); ++i)
		{
			auto address = get_base() + i;
			if (compare((BYTE *)(address), (BYTE *)to_scan, mask))
			{
				foundAddrs.push_back((address));
			}
		}

		return foundAddrs;

	}

	bool NanoMemory::pattern::ConsiderMatch(uintptr_t offset) {

		const char * pattern = m_bytes.c_str();
		const char * mask = m_mask.c_str();

		char * ptr = reinterpret_cast<char*>(offset);

		for (size_t i = 0; i < m_size; i++) {

			if (mask[i] == '?') {
				continue;
			}

			if (pattern[i] != ptr[i]) {
				return false;
			}
		}

		m_matches.push_back(pattern_match(ptr));

		return true;
	}

	void NanoMemory::pattern::EnsureMatches(int maxCount) {

		if (m_matched) {
			return;
		}

		static executable_meta executable;

		executable.EnsureInit();

		int cpuid[4];
		__cpuid(cpuid, 0);

		bool sse42 = false;

		if (m_mask.size() <= 16) {

			if (cpuid[0] >= 1) {

				__cpuidex(cpuid, 1, 0);

				sse42 = (cpuid[2] & (1 << 20)) == 1;
			}
		}

		auto matchSuccess = [&](uintptr_t address) {

			g_hints.insert(std::make_pair(m_hash, address));

			return (m_matches.size() == maxCount);
		};

		LARGE_INTEGER ticks;
		QueryPerformanceCounter(&ticks);

		uint64_t startTicksOld = ticks.QuadPart;

		if (!sse42) {

			for (uintptr_t i = executable.begin(); i <= executable.end(); i++) {

				if (ConsiderMatch(i)) {

					if (matchSuccess(i)) {
						break;
					}
				}
			}
		}
		else {

			__declspec(align(16)) char desiredMask[16] = { 0 };

			for (int i = 0; i < m_mask.size(); i++) {
				desiredMask[i / 8] |= ((m_mask[i] == '?') ? 0 : 1) << (i % 8);
			}

			__m128i mask = _mm_load_si128(reinterpret_cast<const __m128i*>(desiredMask));
			__m128i comparand = _mm_loadu_si128(reinterpret_cast<const __m128i*>(m_bytes.c_str()));

			for (uintptr_t i = executable.begin(); i <= executable.end(); i++) {

				__m128i value = _mm_loadu_si128(reinterpret_cast<const __m128i*>(i));
				__m128i result = _mm_cmpestrm(value, 16, comparand, (int)m_bytes.size(), _SIDD_CMP_EQUAL_EACH);

				__m128i matches = _mm_and_si128(mask, result);
				__m128i equivalence = _mm_xor_si128(mask, matches);

				if (_mm_test_all_zeros(equivalence, equivalence)) {

					m_matches.push_back(pattern_match((void*)i));

					if (matchSuccess(i)) {
						break;
					}
				}
			}
		}

		m_matched = true;
	}

	void NanoMemory::pattern::hint(uint64_t hash, uintptr_t address) {

		auto range = g_hints.equal_range(hash);

		for (auto it = range.first; it != range.second; it++) {

			if (it->second == address) {
				return;
			}
		}

		g_hints.insert(std::make_pair(hash, address));
	}
}