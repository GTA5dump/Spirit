#include "Game.hpp"
#include "Invoker.hpp"
#include "Memory.hpp"
#include "MainScript.hpp"


namespace Big
{

	GameVariables::GameVariables() :
		m_GameState(Signature("48 85 C9 74 4B 83 3D").Scan().Add(7).Rip().As<decltype(m_GameState)>()), //48 85 C9 74 4B 83 3D
		m_LocalScript(Signature("48 8B 05 ? ? ? ? 8B CF 48 8B 0C C8 39 59 68").Scan().As<std::uint32_t*>()), //48 8B 05 ? ? ? ? 8B CF 48 8B 0C C8 39 59 68
		m_GlobalPTR(Signature("4C 8D 05 ? ? ? ? 4D 8B 08 4D 85 C9 74 11").Scan().As<std::uint32_t*>()), //4C 8D 05 ? ? ? ? 4D 8B 08 4D 85 C9 74 11
		GTA_JMP_RBX(Signature("23 C3 C1 E0 0C").Scan().Sub(1).As<std::uint32_t*>()), //23 C3 C1 E0 0C
		m_FrameCount(Signature("F3 0F 10 0D ? ? ? ? 44 89 6B 08").Scan().Add(4).Rip().Sub(8).As<decltype(m_GameState)>()), //F3 0F 10 0D ? ? ? ? 44 89 6B 08
		m_GameWindow(FindWindowA("grcWindow", "Grand Theft Auto V")), //Grand Theft Auto V
		m_Swapchain(Signature("48 8B 0D ? ? ? ? 48 8D 55 A0 48 8B 01").Scan().Add(3).Rip().As<decltype(m_Swapchain)&>()), //48 8B 0D ? ? ? ? 48 8D 55 A0 48 8B 01
		m_NativeRegistrations(Signature("48 83 EC 20 48 8D 0D ? ? ? ? E8 ? ? ? ? 0F B7 15 ? ? ? ? 33 FF").Scan().Add(7).Rip().As<decltype(m_NativeRegistrations)>()), //48 83 EC 20 48 8D 0D ? ? ? ? E8 ? ? ? ? 0F B7 15 ? ? ? ? 33 FF
		m_FixVector3(Signature(("83 79 18 00 48 8B D1 74 4A FF 4A 18 48 63 4A 18 48 8D 41 04 48 8B 4C CA")).Scan().As<decltype(m_FixVector3)>()), //83 79 18 00 48 8B D1 74 4A FF 4A 18 48 63 4A 18 48 8D 41 04 48 8B 4C CA
		m_EventHook(Signature(("48 83 EC 28 E8 ? ? ? ? 48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? 4C 8D 05 ? ? ? ? BA 03")).Scan().As<decltype(m_EventHook)>()), //48 83 EC 28 E8 ? ? ? ? 48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? 4C 8D 05 ? ? ? ? BA 03
		m_GlobalBase(Signature("4C 8D 4D 08 48 8D 15 ? ? ? ? 4C 8B C0").Scan().Add(7).Rip().As<decltype(m_GlobalBase)>()), //4C 8D 4D 08 48 8D 15 ? ? ? ? 4C 8B C0
		m_WorldPtr(Signature("48 8B 05 ? ? ? ? 45 ? ? ? ? 48 8B 48 08 48 85 C9 74 07").Scan().As<decltype(m_WorldPtr)>()), //48 8B 05 ? ? ? ? 45 ? ? ? ? 48 8B 48 08 48 85 C9 74 07
		m_IsSessionStarted(Signature("44 38 1D ? ? ? ? 0F 84 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 8B 85").Scan().Add(3).Rip().As<bool*>()) //44 38 1D ? ? ? ? 0F 84 ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 48 8B 85
		
	{
		g_Logger->Pattern("RBX => Found");
		g_Logger->Pattern("LCPTR => Found");
		g_Logger->Pattern("GPTR => Found");
		g_Logger->Pattern("GS => Found");
		g_Logger->Pattern("FC => Found");
		g_Logger->Pattern("GW => Found");
		g_Logger->Pattern("GS => Found");
		g_Logger->Pattern("SC => Found");
		g_Logger->Pattern("NR => Found");
		g_Logger->Pattern("GB => Found");
		g_Logger->Pattern("WP => Found");

		auto sig = Signature("48 83 EC 60 48 8D 0D ? ? ? ? E8").Scan().Sub(17); //48 83 EC 60 48 8D 0D ? ? ? ? E8
		m_GameBuild = sig.Add(265 + 3).Rip().As<decltype(m_GameBuild)>();
		g_Logger->Pattern("GB => Found");
	
		if (m_EventHook)
		{
			int i = 0, j = 0, matches = 0, found = 0;
			const char* pattern = "\x4C\x8D\x05";
			while (found != m_EventCount)
			{
				if (m_EventHook[i] == pattern[j])
				{
					if (++matches == 3)
					{
						m_EventPtr.push_back((void*)(reinterpret_cast<uint64_t>(m_EventHook + i - j) + *reinterpret_cast<int*>(m_EventHook + i + 1) + 7));
						found++;
						j = matches = 0;
					}
					j++;
				}
				else
				{
					matches = j = 0;
				}
				i++;
			}
		}
	}
	void GameVariables::PostInit()
	{
	}

	GameFunctions::GameFunctions():
		m_IsDlcPresent(Signature("84 C0 74 0D 8D 53 0C").Scan().Sub(63).As<decltype(m_IsDlcPresent)>()),
		m_WndProc(Signature("48 8B 0D ? ? ? ? 48 8D 55 EF FF 15").Scan().Sub(853).As<decltype(m_WndProc)>()),
		m_GetEventData(Signature("48 85 C0 74 14 4C 8B 10").Scan().Sub(28).As<decltype(m_GetEventData)>()),
		m_GetPlayerName(Signature("40 53 48 83 EC 20 80 3D ? ? ? ? ? 8B D9 74 22").Scan().As<decltype(m_GetPlayerName)>()),
		m_TriggerScriptEvent(Signature("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 81 EC ? ? ? ? 45 8B F0 41 8B F9").Scan().As<decltype(m_TriggerScriptEvent)>()),
		m_fpFileRegister(Signature("C3 48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 50").Scan().Add(1).As<decltype(m_fpFileRegister)>()),
		m_GetLabelText(Signature("48 89 5C 24 ? 57 48 83 EC 20 48 8B DA 48 8B F9 48 85 D2 75 44 E8").Scan().Sub(19).As<decltype(m_GetLabelText)>()),
		m_BlockDrops(Signature("49 8B 80 ? ? ? ? 48 85 C0 74 0C F6 80 ? ? ? ? ? 75 03").Scan().As<decltype(m_BlockDrops)>()),
		m_NetworkEventHandler(Signature("66 83 FA ? 73 13 0F B7 C2 4C 89 84 C1 ? ? ? ? 4C 89 8C C1").Scan().As<decltype(m_NetworkEventHandler)>())


		/*
        

		Sig missing
		Get chat
		Bypass chat
		Block Drop/Pickup
		Get Label text
		Transaction
		NetworkEventHandler

		*/

	{}

	void GameFunctions::DefuseEvent(RockstarEvent event, bool toggle)
	{
		static const unsigned char retn = 0xC3;
		char* p = (char*)g_GameVariables->m_EventPtr[event];
		if (toggle)
		{
			if (g_GameVariables->m_EventRestore[event] == 0)
				g_GameVariables->m_EventRestore[event] = p[0];
			*p = retn;
		}
		else
		{
			if (g_GameVariables->m_EventRestore[event] != 0)
				*p = g_GameVariables->m_EventRestore[event];
		}
	}
}
