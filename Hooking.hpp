#pragma once
#include "VMTHook.hpp"

namespace Big
{
	struct Hooks
	{
		static bool IsDlcPresent(std::uint32_t hash);
		static const char* GetLabelText(void* unk, const char* label);
		static bool GetEventData(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount);
		static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static int TriggerScriptEvent(bool unk0, uint64_t* args, int argCount, int bitFlags);
		static void TransactionErrorBlocker(char* entryHeader, char* entryLine1, int instructionalKey, char* entryLine2, BOOL p4, std::uint32_t p5, std::uint32_t* p6, std::uint32_t* p7, BOOL background);
		static int ChatBypasser(__int64 chat_menu, const char* user_text, const char** output_text);
		static bool MoneyDropBlocker(void* drop, void* unk, std::uint32_t* ped);
		static void ReceivedEventHandler(int64_t event_manager, int64_t sender, int64_t receiver, uint16_t event_id, int32_t event_index, int32_t event_handled_bitset, int64_t bit_buffer_size, int64_t bit_buffer);
		static __int64 ChatHook(__int64 a1, __int64 a2, __int64 a3, const char* origText, BOOL isTeam);
		static constexpr auto PresentIndex = 8;
		static HRESULT Present(IDXGISwapChain* dis, UINT syncInterval, UINT flags);
		static void processChatCommands(__int64 p, std::string message);
		static constexpr auto ResizeBuffersIndex = 13;
		static HRESULT ResizeBuffers(IDXGISwapChain* dis, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
	};

	class Hooking
	{
	public:
		explicit Hooking();
		~Hooking() noexcept;
		Hooking(Hooking const&) = delete;
		Hooking(Hooking&&) = delete;
		Hooking& operator=(Hooking const&) = delete;
		Hooking& operator=(Hooking&&) = delete;
		static void * getObjectBypass();
		void Hook();
		void Unhook();

		friend struct Hooks;
	private:
		void* m_NtQueryVirtualMemory{};

		void* m_OriginalIsDlcPresent{};
		void* m_OriginalGetLabelText{};
		void* m_OriginalGetEventData{};
		void* m_OriginalTriggerScriptEvent{};
		void* m_OriginalWndProc{};
		void* m_OriginalDisableErrorScreen{};
		void* m_OriginalCensorChat{};
		void* m_OriginalDropBlocker{};
		void* m_OriginalReceievedEvent{};
		void* m_OriginalChat{};

		VMTHook m_D3DHook;

		char* m_ObjectBypass;

		void* m_BetterObjectBypass;
		//static inline void* m_nativeReturn = Signature("FF E3").Scan().As<void*>();

		char m_OriginalObjectBypass[2];
	};

	inline std::unique_ptr<Hooking> g_Hooking;
	
	void IconNotification(char * text, char * text2, char * Subject);

}
