#pragma once
#include "enums.h"
namespace rage
{
	struct scrNativeRegistration;
	struct scrNativeCallContext;
}

namespace Big
{

	class GameVariables
	{
	public:
		explicit GameVariables();
		~GameVariables() noexcept = default;
		GameVariables(GameVariables const&) = delete;
		GameVariables(GameVariables&&) = delete;
		GameVariables& operator=(GameVariables const&) = delete;
		GameVariables& operator=(GameVariables&&) = delete;

		void PostInit();

		bool* m_IsSessionStarted{};
		char m_EventRestore[84] = {};
		char* m_GameBuild;
		char* m_EventHook;
		const int m_EventCount = 78;
		std::uint32_t* GTA_JMP_RBX;
		std::vector<void*> m_EventPtr;

		std::uint32_t* m_GameState;
		std::uint32_t* m_LocalScript;
		std::uint32_t* m_GlobalPTR;

		std::uint32_t* m_FrameCount;
		std::uint64_t** m_GlobalBase;
		std::uint64_t* m_WorldPtr;
		HWND m_GameWindow;
		IDXGISwapChain* m_Swapchain;
		rage::scrNativeRegistration** m_NativeRegistrations;


		using FixVector = void(*)(rage::scrNativeCallContext* context);
		FixVector m_FixVector3{};
	};
	class GameFunctions
	{
	public:
		explicit GameFunctions();
		~GameFunctions() noexcept = default;
		GameFunctions(GameFunctions const&) = delete;
		GameFunctions(GameFunctions&&) = delete;
		GameFunctions& operator=(GameFunctions const&) = delete;
		GameFunctions& operator=(GameFunctions&&) = delete;

		void DefuseEvent(RockstarEvent e, bool toggle);

		using IsDlcPresent = bool(std::uint32_t hash);
		IsDlcPresent* m_IsDlcPresent;

		using WndProc = LRESULT(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		WndProc* m_WndProc;

		using GetEventData = bool(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount);
		GetEventData* m_GetEventData;



		using GetPlayerName = char*	(std::int32_t player);
		GetPlayerName* m_GetPlayerName;

		using FileRegister = std::uint32_t(std::uint32_t* texture_id, const char* path, bool p1, const char* name, bool p2);
		FileRegister* m_fpFileRegister;

		using GetLabelText = const char*(void* unk, const char* label);
		GetLabelText* m_GetLabelText;

		using ChatData = __int64(__int64 a1, __int64 a2, __int64 a3, const char* origText, BOOL a5);
		ChatData* m_GetChatData;

		using ChatPlayer = __int64(__int64 a1, __int64 a2, __int64 a3);
		ChatPlayer* m_GetChatPlayer;

		using TransactionErrorBlocker = void(char* entryHeader, char* entryLine1, int instructionalKey, char* entryLine2, BOOL p4, std::uint32_t p5, std::uint32_t* p6, std::uint32_t* p7, BOOL background);
		TransactionErrorBlocker* m_TransactionErrorBlocker;

		using BypassChat = int(__int64 chat_menu, const char* user_text, const char** output_text);
		BypassChat* m_BypassChat;

		using NetworkEventHandler = void(int64_t event_manager, int64_t sender, int64_t receiver, uint16_t event_id, int32_t event_index, int32_t event_handled_bitset, int64_t bit_buffer_size, int64_t bit_buffer);
		NetworkEventHandler* m_NetworkEventHandler;

		using BlockDrops = bool(void* drop, void* unk, std::uint32_t ped);
		BlockDrops* m_BlockDrops;

		using TriggerScriptEvent = int(bool unk0, uint64_t* args, int argCount, int bitFlags);
		TriggerScriptEvent * m_TriggerScriptEvent;
	};

	inline std::unique_ptr<GameVariables> g_GameVariables;
	inline std::unique_ptr<GameFunctions> g_GameFunctions;
}
