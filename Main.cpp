#include "Game.hpp"
#include "Hooking.hpp"
#include "ScriptManager.hpp"
#include "./Managers/DropManager.hpp"
#include "./Managers/VehicleManager.hpp"
#include "LogScript.hpp"
#include "MainScript.hpp"
#include "ScriptCallback.hpp"
#include "Invoker.hpp"
#include "CustomText.hpp"
#include "Fiber.hpp"
#include "D3DRenderer.hpp"
#include "UI/UIManager.hpp"
#include "Translation.hpp"
#include "Script.hpp"
#include "../Code/callspoofer.h"

/**
 * \brief The entry point of the library
 * \param hInstance The base address of the DLL
 * \param reason The reason for this function being called
 */

bool run_background_threads = true;

BOOL DllMain(HINSTANCE hInstance, DWORD reason, LPVOID)
{
	using namespace Big;
	
		
	if (reason == DLL_PROCESS_ATTACH)
	{
		g_Module = hInstance;
		CreateThread(nullptr, 0, [](LPVOID) -> DWORD
		{
			bool should_crash = false;
			g_Logger = std::make_unique<Logger>();
			g_Logger->Info("Updated and cracked with <3 - The Unkown Group (10/12/2021)");
			g_GameFunctions = std::make_unique<GameFunctions>();
			g_Logger->Hooks("GameFunctions => Loaded");
			g_GameVariables = std::make_unique<GameVariables>();
			g_Logger->Hooks("GameVariables => Loaded");
			g_Logger->Manager("Waiting for GTA to load.");
			while (*g_GameVariables->m_GameState != 0)
			{
				std::this_thread::sleep_for(3ms);
				std::this_thread::yield();
			}
			g_Logger->Manager("Game => Loaded");
			g_GameVariables->PostInit();
			g_Invoker = std::make_unique<Invoker>();
			g_Logger->Manager("Invoker => Loaded");
			g_CustomText = std::make_unique<CustomText>();
			g_Logger->Manager("CustomText => Loaded");
			g_D3DRenderer = std::make_unique<D3DRenderer>();
			g_Logger->Manager("D3DRenderer => Loaded");
			g_UiManager = std::make_unique<UserInterface::UIManager>();
			g_Logger->Manager("UIManager => Loaded");
			g_DropManager = std::make_unique<DropManager>();
			g_Logger->Manager("DropManager => Loaded");
			g_VehicleManager = std::make_unique<VehicleManager>();
			g_Logger->Manager("VehicleManager => Loaded");
			g_ScriptManager = std::make_unique<ScriptManager>();
			g_Logger->Script("ScriptManager => Loaded");
			g_MainScript = std::make_shared<MainScript>();
			g_Logger->Script("MainScript => Loaded");
			g_LogScript = std::make_shared<LogScript>();
			g_Logger->Script("LogScript => Loaded");
			g_CallbackScript = std::make_shared<CallbackScript>();
			g_Logger->Script("CallbackScript => Loaded");
			g_FiberScript = std::make_shared<FiberScript>();
			g_Logger->Script("FiberScript => Loaded");
			g_ScriptManager->AddScript(g_MainScript);
			g_Logger->Script("MainScript => Added");
			g_ScriptManager->AddScript(g_LogScript);
			g_Logger->Script("ScriptManager => Added");
			g_ScriptManager->AddScript(g_CallbackScript);
			g_Logger->Script("CallbackScript => Added");
			g_ScriptManager->AddScript(g_FiberScript);
			g_Logger->Script("FiberScript => Added");
			g_Hooking = std::make_unique<Hooking>();
			g_Logger->Hooks("Hooking => Finished");
			g_Hooking->Hook();
			g_Logger->Info("Spirit => Loaded");
			while (g_Running)
			{
				if (IsKeyPressed(VK_END))
				{
					g_Logger->Hooks("Starting to uninject.");
					g_Running = false;
				}
				std::this_thread::sleep_for(3ms);
				std::this_thread::yield();				
			}

			std::this_thread::sleep_for(500ms);

			g_Hooking->Unhook();
			g_Logger->Hooks("Unhooking finished.");
			g_ScriptManager.reset();
			g_Logger->Hooks("ScriptManager reset.");
			g_LogScript.reset();
			g_Logger->Hooks("LogScript reset.");
			g_MainScript.reset();
			g_Logger->Hooks("MainScript reset.");
			g_CallbackScript.reset();
			g_Logger->Hooks("CallbackScript reset.");
			g_UiManager.reset();
			g_Logger->Hooks("UIManager reset.");
			g_FiberScript.reset();
			g_Logger->Hooks("Script Fiber reset.");
			g_D3DRenderer.reset();
			g_CustomText.reset();
			g_Logger->Hooks("ImGUI reset.");
			g_Invoker.reset();
			g_Logger->Hooks("Invoker reset.");
			std::this_thread::sleep_for(500ms);

			g_Hooking.reset();
			g_Logger->Hooks("Hooking reset.");
			g_TranslationManager.reset();
			g_Logger->Hooks("TranslationManger reset.");
			g_GameVariables.reset();
			g_Logger->Hooks("GameVariables reset.");
			g_GameFunctions.reset();
			g_Logger->Hooks("GameFunctions reset.");
			g_Logger->Hooks("Spirit unloaded.");
			g_Logger.reset();

			FreeLibraryAndExitThread(g_Module, 0);
		}, nullptr, 0, nullptr);
	}

	return true;
}
