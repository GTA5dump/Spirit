#include "Hooking.hpp"
#include "Game.hpp"
#include "ScriptManager.hpp"
#include "CustomText.hpp"
#include "Util.hpp"
#include <string>
#include <shlwapi.h>
#include <fstream>
#include <thread>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <array>
#include "Common.hpp"
#include <signal.h>
#include <iostream>
#include <stdio.h>
#include "D3DRenderer.hpp"
#include "UI/UIManager.hpp"
#include <MinHook/MinHook.h>
#include "Natives.hpp"
#include "MainScript.hpp"
#include "Notification.h"
#include "ScriptCallback.hpp"
#include "Network.h"
//#include "encrypt.h"
#include "include/xorstr.h"


#ifdef BIGBASE_DEBUG
#  pragma comment(lib, "MinHook-Debug.lib")
#else
#  pragma comment(lib, "MinHook-Release.lib")
#endif

namespace Big
{

	namespace
	{
		std::uint32_t g_HookFrameCount{};
	}
	void* GED_Nullpointer = nullptr;
    std::vector<int> kicks = { -240491041,//1 7
	-351876592,//1 6
	-1633299070,//1 24
	-638863591,//1 23
	416104391,//1 23
	-1595515659,//1 4
	489321898,//1 23
	-1705723814,//1 3
	-1336925360,//1 20
	-1372360252,//1 10
	1664114676,//1 8
	-87967637,//1 3
	1665678974,//1 5
	-1032273438,//1 3
	-1544167759,//1 4
	-730739328,//1 23
	-620075057,//1 5
	741525574,//1 3
	-1542610935,//1 5
	1967155575,//1 5
	-1542610935,//1 4
	1843603007,//1 3
	74423716,//1 5
	473998728,//1 3
	-961190199,//1 5
	1217352219,//1 5
	-1245024507,//1 3
	1394622857,//1 4
	127054992,//1 4
	-158787995,//1 10
	-262341369,//1 2
	-948169674,//1 2
	-1882923979,//1 27
	960916034,//1 5
	-1730227041,//1 8
	314706334,//1 3
	-365271546,//1 5
	1376408207,//1 5
	-253630609,//1 4
	1428412924,//1 3
	-1212832151};
	std::vector<int> vehicle_kick = { -1333236192 }; //1
	std::vector<int> infinite_loading_screen = { -171207973 }; //1
	std::vector<int> transaction_error = { 1302185744 }; //1
	std::vector<int> ceo_kick = { -1648921703,-738295409 }; //1
	std::vector<int> set_bounty = { -116602735 }; //1
	std::vector<int> remote_teleport = { -171207973 }; //1
	std::vector<int> block_passive = { };
	std::vector<int> remote_blind_eye = { };
	std::vector<int> remote_otr = { 575518757 }; //1
	std::vector<int> send_to_job = { -545396442 };
	std::vector<int> sound_spam = { 1097312011 }; //1
	std::vector<int> clear_wanted = { 393068387 }; //1
    std::vector<int> ceo_money = {  };
    std::vector<int> invite = { 1097312011}; //1
    std::vector<int> force_camera_forward = { 1120313136 }; //1
	std::vector<int> force_cayo_perico = { 1300962917 }; //1

	uint64_t netcat_insert_dedupe_addr = 0;
	typedef void(__fastcall* netcat_insert_direct_t)(uint64_t catalog, uint64_t* key, uint64_t** item);
	netcat_insert_direct_t netcat_insert_direct = NULL;

	typedef size_t(__cdecl* strlen_t)(const char* str);
	strlen_t builtin_strlen = NULL;
	std::vector<const char*> attackers = {};

	uint64_t strlen_addr = 0;
	size_t strlen_cacher(char* str)
	{
		static char* start = NULL;
		static char* end = NULL;
		size_t len = 0;
		const size_t cap = 20000;

		// if we have a "cached" string and current pointer is within it
		if (start && str >= start && str <= end) {
			// calculate the new strlen
			len = end - str;

			// if we're near the end, unload self
			// we don't want to mess something else up
			if (len < cap / 2)
				MH_DisableHook((LPVOID)strlen_addr);

			// super-fast return!
			return len;
		}

		// count the actual length
		// we need at least one measurement of the large JSON
		// or normal strlen for other strings
		len = builtin_strlen(str);

		// if it was the really long string
		// save it's start and end addresses
		if (len > cap) {
			start = str;
			end = str + len;
		}

		// slow, boring return
		return len;
	}


	void IconNotification(char *text, char *text2, char *Subject)
	{
		HUD::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		HUD::_SET_NOTIFICATION_MESSAGE_CLAN_TAG("CHAR_GANGAPP", "CHAR_GANGAPP", true, 7, text2, Subject, 1.0, "___SPRT");
		HUD::_DRAW_NOTIFICATION(1, 1);
	}
	const char* CharKeyboard(const char* windowName = "", int maxInput = 21, const char* defaultText = "") {
		MISC::DISPLAY_ONSCREEN_KEYBOARD(0, "", "", defaultText, "", "", "", maxInput);
		while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0);

		if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT()) return "";
		return MISC::GET_ONSCREEN_KEYBOARD_RESULT();
	}
	BOOL addProtection(std::vector<std::vector<int>> events, char* name, bool shouldAddToModderListOnDetect, std::int64_t* args, std::function<void()> onDetect = []() {})
	{
		Player player = args[1];
		const char* sender = PLAYER::GET_PLAYER_NAME(player);
		if (strcmp(sender, "**Invalid**") == 0) return true;
		if (std::count(attackers.begin(), attackers.end(), sender))
		{
			return false;
		}
		for (std::vector<int> event : events)
		{
            if (std::count(event.begin(), event.end(), args[0]))
            {
                std::vector<int>::iterator itr = std::find(event.begin(), event.end(), args[0]);
                IconNotification(&("Sender: " + (std::string)sender + (std::string)" | Event: " + (std::string)name + (std::string)" [" + std::to_string(std::distance(event.begin(), itr)) + (std::string)"]")[0], (char*)"Spirit Protections", &((std::string)"Spirit Protections")[0]);
                //g_Logger->Protections(&("Status: Blocked    Sender: " + (std::string)sender + "    Event: " + name + (std::string)" [" + std::to_string(std::distance(event.begin(), itr)) + (std::string)"]")[0]);
                onDetect();
                return false;

            }
		}
		return true;
	}
    BOOL addProtectionSingle(int event, char* name, bool shouldAddToModderListOnDetect, std::int64_t* args, std::function<void()> onDetect = []() {})
    {
        Player player = args[1];
        const char* sender = PLAYER::GET_PLAYER_NAME(player);
        auto SenderID = *reinterpret_cast<std::int8_t*>(player + 0x2D);

        if (strcmp(sender, "**Invalid**") == 0) return true;
        if (std::count(attackers.begin(), attackers.end(), sender))
        {
            return false;
        }

        if (args[0] == event)
        {
            IconNotification(&("Sender: " + (std::string)sender + " | Event: " + (std::string)name)[0], (char*)"Spirit Protections", &((std::string)"Spirit Protections")[0]);
            g_Logger->Protections(&("Status: Blocked  |  Sender: " + (std::string)sender + "  |  Event: " + name)[0]);
            onDetect();

            return false;
        }


        return true;
    }






	BOOL __cdecl HK_GET_EVENT_DATA(std::int32_t EventGruppe, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount)
	{
		auto HandlerBool = static_cast<decltype(&HK_GET_EVENT_DATA)>(GED_Nullpointer)(EventGruppe, eventIndex, args, argCount);
		if (HandlerBool)
		{
			if (!addProtection({ kicks }, (char*)"Kick", false, args) && g_MainScript->KickProtection) return false;
			if (!addProtection({ infinite_loading_screen }, (char*)"Loading Screen", true, args) && g_MainScript->InfLoadingProtection) return false;
			if (!addProtection({ transaction_error }, (char*)"Transaction Error", true, args) && g_MainScript->TransactionProtection) return false;
			if (!addProtection({ ceo_kick }, (char*)"CEO Kick", true, args) && g_MainScript->Ceo_KickProtection) return false;
			if (!addProtection({ remote_teleport }, (char*)"Remote Teleport", true, args) && g_MainScript->CeobanProtection) return false;
			if (!addProtection({ block_passive }, (char*)"Block Passive Mode", true, args) && g_MainScript->PassiveModeProtection) return false;
			if (!addProtection({ remote_blind_eye }, (char*)"Remote Blind Eye", true, args) && g_MainScript->BlindEyeProtection) return false;
			if (!addProtection({ remote_otr }, (char*)"Remote Off The Radar", true, args) && g_MainScript->OfftheradarProtection) return false;
			if (!addProtection({ send_to_job }, (char*)"Send To Job", true, args) && g_MainScript->JobProtection) return false;
			if (!addProtection({ sound_spam }, (char*)"Sound Spam", true, args) && g_MainScript->SoundSpamProtection) return false;
			if (!addProtection({ clear_wanted }, (char*)"Clear Wanted", true, args) && g_MainScript->WantedProtection) return false;
            if (!addProtection({ set_bounty }, (char*)"Bounty", true, args) && g_MainScript->BountyProtection) return false;
            if (!addProtection({ ceo_money }, (char*)"CEO Money", true, args) && g_MainScript->BountyProtection) return false;
            if (!addProtection({ invite }, (char*)"Invite", true, args) && g_MainScript->BountyProtection) return false;
			if (!addProtection({ force_cayo_perico }, (char*)"Send To Cayo Perico", true, args) && g_MainScript->Cayoperico) return false;
            if (!addProtectionSingle(1097312011, (char*)"TP To Apartment", true, args) && g_MainScript->RemoteTeleportProtection) return false;
            if (!addProtectionSingle(-738295409, (char*)"CEO Ban", true, args) && g_MainScript->CeobanProtection) return false;

			return true;
		}
	}



	bool Hooks::IsDlcPresent(std::uint32_t hash)
	{
		if (g_Running && g_HookFrameCount != *g_GameVariables->m_FrameCount)
		{
			g_HookFrameCount = *g_GameVariables->m_FrameCount;
			//if (g_MainScript->decrpytion_code != "" && encrypt(xorstr("Spirit_menu_da_best"), decryption_key) == "jcKAwjg 3pkt4gQbh4Zy4fQvjY==")
			//{
				g_ScriptManager->OnGameTick();
			//}
			if (hash == 2532323046UL && g_MainScript->RockstarDevBool)
				return true;
		}

		return static_cast<decltype(&IsDlcPresent)>(g_Hooking->m_OriginalIsDlcPresent)(hash);
	}

    void Hooks::ReceivedEventHandler(int64_t event_manager, int64_t sender, int64_t receiver, uint16_t event_id, int32_t event_index, int32_t event_handled_bitset, int64_t bit_buffer_size, int64_t bit_buffer)
    {
        BYTE sender_id = *reinterpret_cast<BYTE*>(sender + 45);
        BYTE receiver_id = *reinterpret_cast<BYTE*>(receiver + 45);
        const std::string event_name = reinterpret_cast<char*>(*reinterpret_cast<__int64*>(event_manager + (__int64)8 * event_id + 241328));
        const char* Sender_Name = PLAYER::GET_PLAYER_NAME(sender_id);

        if ((int)receiver_id == PLAYER::PLAYER_ID())
        {
            int enumId = event_index - 3;

            switch (enumId)
            {
            case KICK_VOTES_EVENT:
                if (g_MainScript->OtherProtectionBool) {
                    //g_Logger->Protections(&("Status: Blocked  |  Sender: " + (std::string)Sender_Name + "  |  Event: " + "Vote Kick")[0]);
                    //IconNotification(&("Status: Blocked\nSender: " + (std::string)Sender_Name)[0], (char*)"Spirit Protections", &((std::string)"Vote Kick")[0]);
                }
                break;

            case GAME_WEATHER_EVENT:
                if (g_MainScript->WeatherProtection) {
                    //g_Logger->Protections(&("Status: Blocked  |  Sender: " + (std::string)Sender_Name + "  |  Event: " + "Weather")[0]);
                    //IconNotification(&("Status: Blocked\nSender: " + (std::string)Sender_Name)[0], (char*)"Spirit Protections", &((std::string)"Weather")[0]);
                }
                break;

            case GAME_CLOCK_EVENT:
                if (g_MainScript->TimeProtection)
                {
                    //g_Logger->Protections(&("Status: Blocked  |  Sender: " + (std::string)Sender_Name + "  |  Event: " + "Time")[0]);
                    //IconNotification(&("Status: Blocked\nSender: " + (std::string)Sender_Name)[0], (char*)"Spirit Protections", &((std::string)"Time")[0]);
                }
                break;
            case NETWORK_CLEAR_PED_TASKS_EVENT:
                if (g_MainScript->OtherProtectionBool) {
                    //g_Logger->Protections(&("Status: Blocked  |  Sender: " + (std::string)Sender_Name + "  |  Event: " + "Freeze")[0]);
                    //IconNotification(&("Status: Blocked\nSender: " + (std::string)Sender_Name)[0], (char*)"Spirit Protections", &((std::string)"Freeze")[0]);
                }
                break;

            case GIVE_PED_SCRIPTED_TASK_EVENT:
                if (g_MainScript->OtherProtectionBool)
                {
                    //g_Logger->Protections(&("Status: Blocked  |  Sender: " + (std::string)Sender_Name + "  |  Event: " + "Task")[0]);
                    //IconNotification(&("Status: Blocked\nSender: " + (std::string)Sender_Name)[0], (char*)"Spirit Protections", &((std::string)"Task")[0]);
                }
                break;

            case GIVE_PED_SEQUENCE_TASK_EVENT:
                if (g_MainScript->OtherProtectionBool)
                {
                    //g_Logger->Protections(&("Status: Blocked  |  Sender: " + (std::string)Sender_Name + "  |  Event: " + "Task")[0]);
                    //IconNotification(&("Status: Blocked\nSender: " + (std::string)Sender_Name)[0], (char*)"Spirit Protections", &((std::string)"Task")[0]);
                }
                break;

            case REQUEST_CONTROL_EVENT:
                if (g_MainScript->OtherProtectionBool)
                {
                    //g_Logger->Protections(&("Status: Blocked  |  Sender: " + (std::string)Sender_Name + "  |  Event: " + "Request")[0]);
                    //IconNotification(&("Status: Blocked\nSender: " + (std::string)Sender_Name)[0], (char*)"Spirit Protections", &((std::string)"Request")[0]);
                }
                break;

            case EXPLOSION_EVENT:
                if (g_MainScript->ExplosionProtection)
                {
                    //g_Logger->Protections(&("Status: Blocked  |  Sender: " + (std::string)Sender_Name + "  |  Event: " + "Explosion")[0]);
                    //IconNotification(&("Status: Blocked\nSender: " + (std::string)Sender_Name)[0], (char*)"Spirit Protections", &((std::string)"Explosion")[0]);
                }
                break;

            case FIRE_EVENT:
                if (g_MainScript->FireProtection)
                {
                    //g_Logger->Protections(&("Status: Blocked  |  Sender: " + (std::string)Sender_Name + "  |  Event: " + "Fire")[0]);
                    //IconNotification(&("Status: Blocked\nSender: " + (std::string)Sender_Name)[0], (char*)"Spirit Protections", &((std::string)"Fire")[0]);
                }

            case NETWORK_PTFX_EVENT:
                if (g_MainScript->OtherProtectionBool)
                {
                    //g_Logger->Protections(&("Status: Blocked  |  Sender: " + (std::string)Sender_Name + "  |  Event: " + "PTFX")[0]);
                    //IconNotification(&("Status: Blocked\nSender: " + (std::string)Sender_Name)[0], (char*)"Spirit Protections", &((std::string)"PTFX")[0]);
                }
                break;

            }
        }


        return static_cast<decltype(&ReceivedEventHandler)>(g_Hooking->m_OriginalReceievedEvent)(event_manager, sender, receiver, event_id, event_index, event_handled_bitset, bit_buffer_size, bit_buffer);
    }

	std::vector<std::string> split(std::string str, std::string sep) {
		char* cstr = const_cast<char*>(str.c_str());
		char* current;
		std::vector<std::string> arr;
		current = strtok(cstr, sep.c_str());
		while (current != NULL) {
			arr.push_back(current);
			current = strtok(NULL, sep.c_str());
		}
		return arr;
	}
	void spawn_vehicle(const char* toSpawn, Ped ped) {
		Hash model = MISC::GET_HASH_KEY(toSpawn);
		if (STREAMING::IS_MODEL_VALID(model))
		{
			STREAMING::REQUEST_MODEL(model);
			g_CallbackScript->AddCallback<ModelCallback>(model, [&] {
				NativeVector3 ourCoords = ENTITY::GET_ENTITY_COORDS(ped, false);
				float heading = ENTITY::GET_ENTITY_HEADING(ped);
				ourCoords.x + 5;
				Vehicle veh = VEHICLE::CREATE_VEHICLE(model, ourCoords.x, ourCoords.y, ourCoords.z, heading, true, false, NULL);
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh);
				VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, NULL);
				DECORATOR::DECOR_SET_INT(veh, (char*)"MPBitset", 0);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
			});
		}
		else {
			g_Logger->Info(((std::string)"Invalid model: " + toSpawn).c_str());
		}
	}
	void Hooks::processChatCommands(__int64 p, std::string message)
	{
		std::vector<std::string> list = split(message, " ");
		if (list.size() == 2)
		{
			if (!strcmp(list.at(0).c_str(), "!spawn") && g_MainScript->chatCmdSpawn)
			{
				g_MainScript->modeltospawn = list;
				//g_MainScript->playertospawn = { PLAYER::GET_PLAYER_NAME(p) };
				g_MainScript->PlayerToSpawn = p;
				g_MainScript->current_task = { ChatAction::SpawnVehicle, p, list.at(1).c_str() };
			}
			if (!strcmp(list.at(0).c_str(), "!kick") && g_MainScript->chatCmdKick)
			{
				g_MainScript->current_task = { ChatAction::Kick, 0, list.at(1).c_str() };
			}
			if (!strcmp(list.at(0).c_str(), "!drop") && g_MainScript->chatCmdDrop)
			{
				g_MainScript->PlayerToDrop = p;
				g_MainScript->current_task = { ChatAction::Drop, p, list.at(1).c_str(), list.at(2).c_str() };
			}
		}

	}

    __int64 Hooks::ChatHook(__int64 a1, __int64 a2, __int64 a3, const char* origText, BOOL isTeam)
    {
        std::string Message(origText);
        __int8 PlayerID = *(std::int8_t*)(g_GameFunctions->m_GetChatPlayer(a3, a2, a3) + 0x21); //0x21 is the 1.57 offset for player id, not usable in everything but it is in this.
		std::vector<std::string> words;
		for (int x = 1; x < Message.length(); x++) {
			std::string t = "";
			if (Message.at(x) == ' ') {
				words.push_back(t);
				t.erase();
			}
			else {
				t += Message.at(x);
				if (x == Message.length() - 1) words.push_back(t);

			}
		}
        if (PlayerID >= 0 && PlayerID < 32)
        {
			//Check if your in a team or not, instead of generalizing it, like a lazy piece of shit... stop being a lazy fuck, I ain't here to do this for you 24/7.
			if (!isTeam) {
				g_Logger->Chat("%s [All]: %s", g_GameFunctions->m_GetPlayerName(PlayerID), Message.c_str()); //Prints only the global chat
			}
			else {
				g_Logger->Chat("%s [Team]: %s", g_GameFunctions->m_GetPlayerName(PlayerID), Message.c_str()); //Prints only the team chat
			}
			if (g_MainScript->chatCmd) processChatCommands(PlayerID, Message);
        }
        else {
            g_Logger->Chat("Error");
        }

        return static_cast<decltype(&ChatHook)>(g_Hooking->m_OriginalChat)( a1,  a2,  a3, origText,  isTeam);
    }

	const char* Hooks::GetLabelText(void* unk, const char* label)
	{
		if (g_Running)
			if (auto text = g_CustomText->GetText(Joaat(label)))
				return text;

		return static_cast<decltype(&GetLabelText)>(g_Hooking->m_OriginalGetLabelText)(unk, label);
	}

    int Hooks::TriggerScriptEvent(bool unk0, uint64_t* args, int argCount, int bitFlags)
    {
        return static_cast<decltype(&TriggerScriptEvent)>(g_Hooking->m_OriginalTriggerScriptEvent)(unk0, args, argCount, bitFlags);
    }

    void Hooks::TransactionErrorBlocker(char* entryHeader, char* entryLine1, int instructionalKey, char* entryLine2, BOOL p4, Any p5, Any* p6, Any* p7, BOOL background)
    {
        if (SCRIPT::GET_HASH_OF_THIS_SCRIPT_NAME() == MISC::GET_HASH_KEY("shop_controller") && g_MainScript->TransactionError)
        {
            return;
        }
        return static_cast<decltype(&TransactionErrorBlocker)>(g_Hooking->m_OriginalDisableErrorScreen)(entryHeader, entryLine1, instructionalKey, entryLine2, p4, p5, p6, p7, background);
    }
    int Hooks::ChatBypasser(__int64 chat_menu, const char* user_text, const char** output_text)
    {
        if (g_MainScript->ChatCensor)
        {
            return -1;
        }
        return static_cast<decltype(&ChatBypasser)>(g_Hooking->m_OriginalCensorChat)(chat_menu, user_text, output_text);
    }
    bool Hooks::MoneyDropBlocker(void* drop, void* unk, std::uint32_t* ped)
    {
        if (g_MainScript->MoneyPickup)
        {
            return false;
        }
        return static_cast<decltype(&MoneyDropBlocker)>(g_Hooking->m_OriginalDropBlocker)(drop, unk, ped);
    }
	bool Hooks::GetEventData(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount)
	{
        bool result = static_cast<decltype(&GetEventData)>(g_Hooking->m_OriginalGetEventData)(eventGroup, eventIndex, args, argCount);
        if (result)
        {
            if (!addProtection({ kicks }, (char*)"Kick", false, args) && g_MainScript->KickProtection) return false;
            if (!addProtection({ infinite_loading_screen }, (char*)"Loading Screen", true, args) && g_MainScript->InfLoadingProtection) return false;
            if (!addProtection({ transaction_error }, (char*)"Transaction Error", true, args) && g_MainScript->TransactionProtection) return false;
            if (!addProtection({ ceo_kick }, (char*)"CEO Kick", true, args) && g_MainScript->Ceo_KickProtection) return false;
            if (!addProtection({ remote_teleport }, (char*)"Remote Teleport", true, args) && g_MainScript->CeobanProtection) return false;
            if (!addProtection({ block_passive }, (char*)"Block Passive Mode", true, args) && g_MainScript->PassiveModeProtection) return false;
            if (!addProtection({ remote_blind_eye }, (char*)"Remote Blind Eye", true, args) && g_MainScript->BlindEyeProtection) return false;
            if (!addProtection({ remote_otr }, (char*)"Remote Off The Radar", true, args) && g_MainScript->OfftheradarProtection) return false;
            if (!addProtection({ send_to_job }, (char*)"Send To Job", true, args) && g_MainScript->JobProtection) return false;
            if (!addProtection({ sound_spam }, (char*)"Sound Spam", true, args) && g_MainScript->SoundSpamProtection) return false;
            if (!addProtection({ clear_wanted }, (char*)"Clear Wanted", true, args) && g_MainScript->WantedProtection) return false;
            if (!addProtection({ set_bounty }, (char*)"Bounty", true, args) && g_MainScript->BountyProtection) return false;
            if (!addProtection({ ceo_money }, (char*)"CEO Money", true, args) && g_MainScript->BountyProtection) return false;
            if (!addProtection({ invite }, (char*)"Invite", true, args) && g_MainScript->BountyProtection) return false;
			if (!addProtection({ force_cayo_perico }, (char*)"Send To Cayo Perico", true, args) && g_MainScript->Cayoperico) return false;
            if (!addProtectionSingle(-171207973, (char*)"TP To Apartment", true, args) && g_MainScript->RemoteTeleportProtection) return false;
            if (!addProtectionSingle(-738295409, (char*)"CEO Ban", true, args) && g_MainScript->CeobanProtection) return false;
        }

        return static_cast<decltype(&GetEventData)>(g_Hooking->m_OriginalGetEventData)(eventGroup, eventIndex, args, argCount);

	}

	LRESULT Hooks::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		g_D3DRenderer->WndProc(hWnd, msg, wParam, lParam);
		return static_cast<decltype(&WndProc)>(g_Hooking->m_OriginalWndProc)(hWnd, msg, wParam, lParam);
	}

	HRESULT Hooks::Present(IDXGISwapChain* dis, UINT syncInterval, UINT flags)
	{
		if (g_Running)
		{
			g_D3DRenderer->BeginFrame();
			g_ScriptManager->OnD3DTick();
			g_D3DRenderer->EndFrame();
		}

		return g_Hooking->m_D3DHook.GetOriginal<decltype(&Present)>(PresentIndex)(dis, syncInterval, flags);
	}

	HRESULT Hooks::ResizeBuffers(IDXGISwapChain* dis, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
	{
		if (g_Running)
		{
			g_D3DRenderer->PreResize();
			auto hr = g_Hooking->m_D3DHook.GetOriginal<decltype(&ResizeBuffers)>(ResizeBuffersIndex)(dis, bufferCount, width, height, newFormat, swapChainFlags);
			if (SUCCEEDED(hr))
			{
				g_D3DRenderer->PostResize();
			}

			return hr;
		}

		return g_Hooking->m_D3DHook.GetOriginal<decltype(&ResizeBuffers)>(ResizeBuffersIndex)(dis, bufferCount, width, height, newFormat, swapChainFlags);
	}
	void* m_BetterObjectBypass;



	static void* getObjectBypass()
	{
		//Yet still decected, go use BBv2 bypass lazy shit
		return m_BetterObjectBypass;
	}

	Hooking::Hooking() :
		m_D3DHook(g_GameVariables->m_Swapchain, 18)
	{
		//g_Logger->Hooks("OB => Loaded");
		//uint64_t base_addr = (uint64_t)GetModuleHandleA(NULL);

		//if (GetModuleHandleA("steam_api64.dll")) {
		//	// addresses hardcoded for Steam version 2215/1.53-57
		//	netcat_insert_dedupe_addr = base_addr + 0x10AA918;
		//	strlen_addr = base_addr + 0x17C01A0;

		//	netcat_insert_direct = (netcat_insert_direct_t)(base_addr + 0x5BB07C);
		//}
		//else if (GetModuleHandleA("EOSSDK-Win64-Shipping.dll")) {
		//	// addresses hardcoded for Social Club version 2215/1.53-56 (1.57 now dead)
		//	netcat_insert_dedupe_addr = base_addr + 0x10A9664;
		//	strlen_addr = base_addr + 0x17BD600;

		//	netcat_insert_direct = (netcat_insert_direct_t)(base_addr + 0x24EA8C);
		//}
		//else {
		//	// addresses hardcoded for Social Club version 2215/1.53-56 (1.57 now dead)
		//	netcat_insert_dedupe_addr = base_addr + 0x10A9664;
		//	strlen_addr = base_addr + 0x17BD600;

		//	netcat_insert_direct = (netcat_insert_direct_t)(base_addr + 0x24EA8C);
		//}
		MH_Initialize();



		g_Logger->Hooks("MH => Initialized");
		MH_CreateHook(g_GameFunctions->m_IsDlcPresent, &Hooks::IsDlcPresent, &m_OriginalIsDlcPresent);
		g_Logger->Hooks("IDP => Hooked");
		MH_CreateHook(g_GameFunctions->m_GetLabelText, &Hooks::GetLabelText, &m_OriginalGetLabelText);
		g_Logger->Hooks("GLT => Hooked");
		MH_CreateHook(g_GameFunctions->m_TriggerScriptEvent, &Hooks::TriggerScriptEvent, &m_OriginalTriggerScriptEvent);
		g_Logger->Hooks("TSE => Hooked");
		MH_CreateHook(g_GameFunctions->m_GetEventData, &Hooks::GetEventData, &m_OriginalGetEventData);
		g_Logger->Hooks("GED => Hooked");
		MH_CreateHook(g_GameFunctions->m_WndProc, &Hooks::WndProc, &m_OriginalWndProc);
		g_Logger->Hooks("BD => Hooked");
		MH_CreateHook(g_GameFunctions->m_BlockDrops, &Hooks::MoneyDropBlocker, &m_OriginalDropBlocker);
		g_Logger->Hooks("BC => Hooked");
		MH_CreateHook(g_GameFunctions->m_BypassChat, &Hooks::ChatBypasser, &m_OriginalCensorChat);
		g_Logger->Hooks("TEB => Hooked");
		MH_CreateHook(g_GameFunctions->m_TransactionErrorBlocker, &Hooks::TransactionErrorBlocker, &m_OriginalDisableErrorScreen);
		MH_CreateHook(g_GameFunctions->m_NetworkEventHandler, &Hooks::ReceivedEventHandler, &m_OriginalReceievedEvent);
		g_Logger->Hooks("NEH => Hooked");
		g_Logger->Hooks("TEB => Hooked");
		MH_CreateHook(g_GameFunctions->m_GetChatData, &Hooks::ChatHook, &m_OriginalChat);
		g_Logger->Hooks("GCD => Hooked");

		g_Logger->Hooks("WP => Hooked");
		m_D3DHook.Hook(&Hooks::Present, Hooks::PresentIndex);
		m_D3DHook.Hook(&Hooks::ResizeBuffers, Hooks::ResizeBuffersIndex);
		g_Logger->Hooks("ImGui => Hooked");

		//if (GetModuleHandleA("steam_api64.dll")) {
		//	MH_CreateHook((LPVOID)strlen_addr, &strlen_cacher, (LPVOID*)&builtin_strlen);
		//	g_Logger->Hooks("SA => Hooked");
		//	MH_EnableHook((LPVOID)strlen_addr);
		//	MH_EnableHook((LPVOID)netcat_insert_dedupe_addr);
		//}
	}

	Hooking::~Hooking() noexcept
	{
		MH_RemoveHook(g_GameFunctions->m_WndProc);
		g_Logger->Hooks("WP => Unhooked");
		MH_RemoveHook(g_GameFunctions->m_GetEventData);
		g_Logger->Hooks("GED => Unhooked");
		MH_RemoveHook(g_GameFunctions->m_GetLabelText);
		g_Logger->Hooks("GLT => Unhooked");
        MH_RemoveHook(g_GameFunctions->m_TriggerScriptEvent);
        g_Logger->Hooks("TSE => Unhooked");
		MH_RemoveHook(g_GameFunctions->m_IsDlcPresent);
		g_Logger->Hooks("IDP => Unhooked");
        MH_RemoveHook(g_GameFunctions->m_BlockDrops);
        g_Logger->Hooks("BD => Unhooked");
        MH_RemoveHook(g_GameFunctions->m_BypassChat);
        g_Logger->Hooks("BC => Unhooked");
        MH_RemoveHook(g_GameFunctions->m_TransactionErrorBlocker);
        g_Logger->Hooks("TEB => Unhooked");
        MH_RemoveHook(g_GameFunctions->m_NetworkEventHandler);
        g_Logger->Hooks("NEH => Unhooked");
        MH_RemoveHook(g_GameFunctions->m_GetChatData);
        g_Logger->Hooks("GCD => Unhooked");

		MH_Uninitialize();
		g_Logger->Hooks("MH => Uninitialized");
	}


	void Hooking::Hook()
	{
		m_D3DHook.Enable();
		MH_EnableHook(MH_ALL_HOOKS);
	}

	void Hooking::Unhook()
	{
		g_Logger->Hooks("OB => Reverted");
		m_D3DHook.Disable();
		g_Logger->Hooks("ImGUI => Reverted");
		MH_DisableHook(MH_ALL_HOOKS);
		g_Logger->Hooks("Hooks => Reverted");
	}
}
