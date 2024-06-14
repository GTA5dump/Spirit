#pragma once
#include "MainScript.hpp"
#include "ScriptCallback.hpp"
#include "Lists.hpp"
#include "Natives.hpp"
#include "Hooking.hpp"
#include "Timer.hpp"
#include "Translation.hpp"
#include "CustomText.hpp"
#include "Managers/DropManager.hpp"
#include "Managers/VehicleManager.hpp"
#include "UI/UIManager.hpp"
#include "UI/BoolOption.hpp"
#include "UI/ChooseOption.hpp"
#include "UI/NumberOption.hpp"
#include "UI/RegularOption.hpp"
#include "UI/BreakOption.hpp"
#include "UI/SubOption.hpp"
#include "UI/RegularSubmenu.hpp"
#include "UI/PlayerSubmenu.hpp"
#include "UI/StringOption.hpp"
#include "UI/DropSubmenu.hpp"
#include "Self.h"
#include "Weapon.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "enums.h"
#include "Memory.hpp"
#include "NanoMemory.h"
#include "GlobalOptions.h"
#include "Teleport.h"
#include "Misc.h"
#include "Network.h"
#include "Recovery.h"
#include "Vehicle.h"
#include "Protections.h"
#include "Settings.h"
#include "GlobalOptions.h"
#include "Offsets.h"
#include "Notification.h"
#include "Playerinfo.h"
#include <string>
#include <sstream>
#include <Urlmon.h>


std::vector<std::function<void()>> native_stack = {};

namespace Big
{
	bool MainScript::IsInitialized()
	{
		return m_Initialized;
	}

	std::uint32_t* MainScript::getGameVariables()
	{
		return g_GameVariables->m_GameState;
	}

	void MainScript::loadSavedOptions()
	{
		getSavedOptions();
		GodmodeBool = g_MainScript->getSaved("Godmode").value;
		DisableImGuiInput = g_MainScript->getSaved("Disable All Input (ImGui)").value;
		NeverWantedBool = g_MainScript->getSaved("Never Wanted").value;
		PlayerAlphaBool = g_MainScript->getSaved("Custom Alpha").value;
		FireBreathBool = g_MainScript->getSaved("Fire Breath").value;
		SeatBeltBool = g_MainScript->getSaved("Seatbelt").value;
		NoClipBool = g_MainScript->getSaved("No-Clip").value;
		SuperJumpBool = g_MainScript->getSaved("Super Jump").value;
		ShrinkPlayerBool = g_MainScript->getSaved("Shrink Player").value;
		SuperRunBool = g_MainScript->getSaved("Super Run").value;
		SlomotionBool = g_MainScript->getSaved("Slow-Motion").value;
		OffTheRadarBool = g_MainScript->getSaved("Off The Radar").value;
		ExplosiveMeleeBool = g_MainScript->getSaved("Explosive Melee").value;
		CameraZoomBool = g_MainScript->getSaved("Camera Zoom").value;
		FakeWantedLevel = g_MainScript->getSaved("Fake Wanted Level").value;
		ForceFieldBool = g_MainScript->getSaved("Force Field").value;
		MobileRadioBool = g_MainScript->getSaved("Mobile Radio").value;
		NightVisionBool = g_MainScript->getSaved("Night Vision").value;
		ThermalVisionBool = g_MainScript->getSaved("Thermal Vision").value;
		DrunkPedBool = g_MainScript->getSaved("Drunk").value;
		HealthIncreaseBool = g_MainScript->getSaved("Increase Health").value;
		RockstarDevBool = g_MainScript->getSaved("R* Developer Mode").value;
		AntiIdleKickBook = g_MainScript->getSaved("Anti Idle Kick").value;
		showInfo = g_MainScript->getSaved("Show Playerinfo").value;
		showRID = g_MainScript->getSaved("Show R*ID").value;
		chatCmd = g_MainScript->getSaved("Enable Chat Commands").value;
		chatCmdKick = g_MainScript->getSaved("!kick %name%").value;
		chatCmdSpawn = g_MainScript->getSaved("!spawn %model%").value;
		chatCmdDrop = g_MainScript->getSaved("!drop").value;
		ImGUIMode = g_MainScript->getSaved("ImGui Mode").value;
		VehicleGodmodeBool = g_MainScript->getSaved("Vehicle Godmode").value;
		HornBoostBool = g_MainScript->getSaved("Horn Boost").value;
		AutoFixBool = g_MainScript->getSaved("Auto Fix").value;
		NoSpeedLimitBool = g_MainScript->getSaved("Speed Bypass").value;
		DriftModeBool = g_MainScript->getSaved("Drift Mode").value;
		InvisibiltyBool = g_MainScript->getSaved("Invisible").value;
		watermark = g_MainScript->getSaved("Watermark").value;
		g_VehicleManager->SpawnMaxed = g_MainScript->getSaved("Spawn Maxed").value;
		g_VehicleManager->SpawnInVehicle = g_MainScript->getSaved("Spawn Inside").value;
	}
	void MainScript::SaveWholeMenu()
	{
		getSavedOptions();
		g_MainScript->SaveOption("Godmode", GodmodeBool);
		g_MainScript->SaveOption("Disable All Input (ImGui)", DisableImGuiInput);
		
		g_MainScript->SaveOption("Never Wanted", NeverWantedBool);
		g_MainScript->SaveOption("Custom Alpha", PlayerAlphaBool);
		g_MainScript->SaveOption("Fire Breath", FireBreathBool);
		g_MainScript->SaveOption("Seatbelt", SeatBeltBool);
		g_MainScript->SaveOption("No-Clip", NoClipBool);
		g_MainScript->SaveOption("Super Jump", SuperJumpBool);
		g_MainScript->SaveOption("Shrink Player", ShrinkPlayerBool);
		g_MainScript->SaveOption("Super Run", SuperRunBool);
		g_MainScript->SaveOption("Slow-Motion", SlomotionBool);
		g_MainScript->SaveOption("Off The Radar", OffTheRadarBool);
		g_MainScript->SaveOption("Explosive Melee", ExplosiveMeleeBool);
		g_MainScript->SaveOption("Camera Zoom", CameraZoomBool);
		g_MainScript->SaveOption("Fake Wanted Level", FakeWantedLevel);
		g_MainScript->SaveOption("Force Field", ForceFieldBool);
		g_MainScript->SaveOption("Mobile Radio", MobileRadioBool);
		g_MainScript->SaveOption("Night Vision", NightVisionBool);
		g_MainScript->SaveOption("Thermal Vision", ThermalVisionBool);
		g_MainScript->SaveOption("Drunk", DrunkPedBool);
		g_MainScript->SaveOption("Increase Health", HealthIncreaseBool);
		g_MainScript->SaveOption("R* Developer Mode", RockstarDevBool);
		g_MainScript->SaveOption("Anti Idle Kick", AntiIdleKickBook);
		g_MainScript->SaveOption("Show Playerinfo", showInfo);
		g_MainScript->SaveOption("Show R*ID", showRID);
		g_MainScript->SaveOption("Enable Chat Commands", chatCmd);
		g_MainScript->SaveOption("!kick %name%", chatCmdKick);
		g_MainScript->SaveOption("!spawn %model%", chatCmdSpawn);
		g_MainScript->SaveOption("!drop", chatCmdDrop);
		g_MainScript->SaveOption("ImGui Mode", ImGUIMode);
		g_MainScript->SaveOption("Vehicle Godmode", VehicleGodmodeBool);
		g_MainScript->SaveOption("Horn Boost", HornBoostBool);
		g_MainScript->SaveOption("Auto Fix", AutoFixBool);
		g_MainScript->SaveOption("Speed Bypass", NoSpeedLimitBool);
		g_MainScript->SaveOption("Drift Mode", DriftModeBool);
		g_MainScript->SaveOption("Invisible", InvisibiltyBool);
		g_MainScript->SaveOption("Watermark", watermark);
		g_MainScript->SaveOption("Spawn Maxed", g_VehicleManager->SpawnMaxed);
		g_MainScript->SaveOption("Spawn Inside", g_VehicleManager->SpawnInVehicle);
	}
	ScriptType MainScript::GetType()
	{
		return ScriptType::Game;
	}
	std::vector<std::vector<int>> globals = {};


	void add_to_stack(std::function<void()> f)
	{
		native_stack.push_back(f);
	}

	std::vector<CompressedOption> remove(std::vector<CompressedOption> v)
	{
		std::vector<CompressedOption> checked;

		for (CompressedOption opt : v)
		{
			bool shouldAdd = true;
			for (CompressedOption opt1 : checked)
			{
				if (!shouldAdd) break;
				shouldAdd = std::strcmp(opt.name, opt1.name);
			}
			if (shouldAdd) checked.push_back(opt);
		}
		return checked;
	}

	float lerp(float start, float end, float amount) {
		return start * (1 - amount) + end * amount;
	}

	Vehicle MainScript::spawn_vehicle(const char* toSpawn) {
		Hash model = MISC::GET_HASH_KEY(toSpawn);
		if (STREAMING::IS_MODEL_VALID(model))
		{
			g_CallbackScript->AddCallback<ModelCallback>(model, [model] {

				STREAMING::REQUEST_MODEL(model);
				NativeVector3 ourCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
				float heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
				ourCoords.x + 5;
				Vehicle veh = VEHICLE::CREATE_VEHICLE(model, ourCoords.x, ourCoords.y, ourCoords.z, heading, true, false, NULL);
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh);
				VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, NULL);
				DECORATOR::DECOR_SET_INT(veh, (char*)"MPBitset", 0);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

				if (g_VehicleManager->SpawnInVehicle) { PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1); }

				if (g_VehicleManager->SpawnMaxed) {

					VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
					for (int i = 0; i < 50; i++)
					{
						VEHICLE::SET_VEHICLE_MOD(veh, i, VEHICLE::GET_NUM_VEHICLE_MODS(veh, i) - 1, false);
					}
				}

				if (g_VehicleManager->SpawnInAir)
				{
					NativeVector3 vCoords = ENTITY::GET_ENTITY_COORDS(veh, false);
					ENTITY::SET_ENTITY_COORDS(veh, vCoords.x, vCoords.x, vCoords.z + g_VehicleManager->SpawnInAirZ, false, false, false, false);
				}

				return veh;
			});
		}
		else {
			g_Logger->Info(((std::string)"Invalid model: " + toSpawn).c_str());
		}
		return 0;
	}
	Vehicle spawn_vehicle2(const char* toSpawn, Ped ped) {
		Hash model = MISC::GET_HASH_KEY(toSpawn);
		if (STREAMING::IS_MODEL_VALID(model))
		{
			g_CallbackScript->AddCallback<ModelCallback>(model, [&] {

				STREAMING::REQUEST_MODEL(model);
				int hashplayer = NETWORK::NETWORK_HASH_FROM_PLAYER_HANDLE(g_SelectedPlayer);
				NativeVector3 ourCoords = ENTITY::GET_ENTITY_COORDS(ped, false);
				float heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
				ourCoords.x + 5;
				Vehicle veh = VEHICLE::CREATE_VEHICLE(model, ourCoords.x, ourCoords.y, ourCoords.z, heading, true, false, NULL);
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh);
				VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, NULL);
				DECORATOR::DECOR_SET_INT(veh, (char*)"MPBitset", 0);
				DECORATOR::DECOR_SET_INT(veh, "Player_Vehicle", hashplayer);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
				if (g_VehicleManager->SpawnInVehicle) { PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1); }
				if (g_VehicleManager->SpawnMaxed) {
					VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
					for (int i = 0; i < 50; i++) {
						VEHICLE::SET_VEHICLE_MOD(veh, i, VEHICLE::GET_NUM_VEHICLE_MODS(veh, i) - 1, false);
					}
				}
				if (g_VehicleManager->SpawnInAir)
				{
					NativeVector3 vCoords = ENTITY::GET_ENTITY_COORDS(veh, false);
					ENTITY::SET_ENTITY_COORDS(veh, vCoords.x, vCoords.x, vCoords.z + g_VehicleManager->SpawnInAirZ, false, false, false, false);
				}
				return veh;
			});
		}
		else {
			g_Logger->Info(((std::string)"Invalid model: " + toSpawn).c_str());
		}
		return 0;
	}

#define normal_option(x, y) sub->AddOption<RegularOption>(x, y, []


	void MainScript::Text(const char* text, int red, int green, int blue, int alpha, float x, float y, float width, float height, bool center)
	{
		HUD::SET_TEXT_CENTRE(center);
		HUD::SET_TEXT_COLOUR(red, green, blue, alpha);
		HUD::SET_TEXT_FONT(0);
		HUD::SET_TEXT_SCALE(width, height);
		HUD::SET_TEXT_DROPSHADOW(1, 0, 0, 0, 0);
		HUD::SET_TEXT_EDGE(1, 0, 0, 0, 0);
		HUD::SET_TEXT_OUTLINE();
		HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME((char*)text);
		HUD::END_TEXT_COMMAND_DISPLAY_TEXT(x, y, 0);
	}
	
	void create_vehicle2(const char* toSpawn) {
		Hash hash = Joaat(toSpawn);
		STREAMING::REQUEST_MODEL(hash);
		NativeVector3 ourCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
		float heading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
		ourCoords.x + 5;
		Vehicle veh = VEHICLE::CREATE_VEHICLE(hash, ourCoords.x, ourCoords.y, ourCoords.z, heading, true, false, NULL);
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, NULL);
		DECORATOR::DECOR_SET_INT(veh, (char*)"MPBitset", 0);
		DECORATOR::DECOR_SET_INT(veh, "Player_Vehicle", (int)PLAYER::PLAYER_ID());
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
		if (g_VehicleManager->SpawnInVehicle) {
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
		}
		if (g_VehicleManager->SpawnMaxed) {
			VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
			for (int i = 0; i < 50; i++) {
				VEHICLE::SET_VEHICLE_MOD(veh, i, VEHICLE::GET_NUM_VEHICLE_MODS(veh, i) - 1, false);
			}
			//VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, 5);
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, "SPIRIT");
		}
		if (g_VehicleManager->SpawnInAir) {
			NativeVector3 vCoords = ENTITY::GET_ENTITY_COORDS(veh, false);
			ENTITY::SET_ENTITY_COORDS(veh, vCoords.x, vCoords.x, vCoords.z + g_VehicleManager->SpawnInAirZ, false, false, false, false);
		}
	}

	void MainScript::Initialize()
	{
		g_MainScript->firstFrame = true;
		g_MainScript->current_task = {ChatAction::None, 0, ""};
		const char* m_Header = "textures";
		char buffers[128];
		char mainname[128];
		uint32_t textureID;

		std::sprintf(buffers, "%s//Spirit//UI//textures.ytd", std::getenv("documents"), &m_Header);
		std::sprintf(mainname, "textures.ytd", &m_Header);
		g_GameFunctions->m_fpFileRegister(&textureID, buffers, true, mainname, false);

		struct DropManager::Drop SelfDrop = {
			0, // ID
			PLAYER::PLAYER_PED_ID(), // Player
			false, // Toggle?
			true, // Follow Player?
			false, // Custom?
			false, // Show Info?
			false, // Show Marker?
			g_DropManager->defaultPickup, // Pickup
			g_DropManager->defaultModel,  // Model
			2500, // Value
			1000, // Interval/Cooldown
			g_DropManager->defaultLocation, // Location
			g_DropManager->defaultLocation, // Last Location
			0, // Model Position
			0 // Pickup Position
		};
		g_DropManager->SelfDrop = SelfDrop;

		g_VehicleManager->Init();

		m_Initialized = true;
		using namespace UserInterface;
		loadSavedOptions();
		g_CustomText->AddText(CONSTEXPR_JOAAT("HUD_JOINING"), "Powered By Spirit Engine!");
		g_CustomText->AddText(CONSTEXPR_JOAAT("HUD_QUITTING"), "Powered By Spirit Engine!");
		g_CustomText->AddText(CONSTEXPR_JOAAT("HUD_TRANSP"), "Powered By Spirit Engine!");

		g_CustomText->AddText(CONSTEXPR_JOAAT("PM_EXIT_GAME"), "Exit with Spirit Engine!");

		g_CustomText->AddText(CONSTEXPR_JOAAT("HQRHELP"), "Spirit 1337 Help Center");
		g_CustomText->AddText(CONSTEXPR_JOAAT("PM_GOTO_STORE"), "Only retards buy sharkcards!");
		g_CustomText->AddText(CONSTEXPR_JOAAT("PM_JOBS"), "Imagine doing jobs...");

		g_CustomText->AddText(CONSTEXPR_JOAAT("TOOLTIPS_21"), "Spirit da best rofl");
		g_CustomText->AddText(CONSTEXPR_JOAAT("TOOLTIPS_52"), "Spirit da best rofl");
		g_CustomText->AddText(CONSTEXPR_JOAAT("TOOLTIPS_22"), "Spirit da best rofl");
		g_CustomText->AddText(CONSTEXPR_JOAAT("TOOLTIPS_62"), "Spirit da best rofl");
		g_CustomText->AddText(CONSTEXPR_JOAAT("TOOLTIPS_42"), "Spirit da best rofl");
		g_CustomText->AddText(CONSTEXPR_JOAAT("TOOLTIPS_1"), "Spirit da best rofl");
		g_CustomText->AddText(CONSTEXPR_JOAAT("TOOLTIPS_2"), "Spirit da best rofl");

		g_CustomText->AddText(CONSTEXPR_JOAAT("PM_GO"), "Go Online with Spirit Engine");
		g_CustomText->AddText(CONSTEXPR_JOAAT("PM_FRIEND_FM"), "Join friends with Spirit Engine");
		g_CustomText->AddText(CONSTEXPR_JOAAT("PM_FIND_SESS"), "Join a new session with Spirit Engine");
		g_CustomText->AddText(CONSTEXPR_JOAAT("PM_ENTER_MP"), "Join Online with Spirit Engine");
		add_notification("Welcome Notification", "Welcome to Spirit!");
		

		// Dont forget to fix this, i didnt got though this check in-game
		//if (g_MainScript->SPEEDYHAXORING && g_MainScript->haxorlevel < 0) {
			g_UiManager->AddSubmenu<RegularSubmenu>("Home", SubmenuHome, [](RegularSubmenu* sub)
			{
				//sub->AddOption<SubOption>("Drop Chat Commands", nullptr, SubmenuDropTest);
				sub->AddOption<SubOption>("Vehicle Test", nullptr, SubmenuVehicleTest);
				sub->AddOption<SubOption>("Self", nullptr, SubmenuSelf);
				sub->AddOption<SubOption>("Weapon", nullptr, SubmenuWeapons);
				sub->AddOption<SubOption>("Teleport", nullptr, SubmenuTeleport);
				sub->AddOption<SubOption>("Vehicle", nullptr, SubmenuVehicle);
				sub->AddOption<SubOption>("Network", nullptr, SubmenuNetwork);
				sub->AddOption<SubOption>("Recovery", nullptr, SubmenuRecovery);
				sub->AddOption<SubOption>("Protection", nullptr, SubmenuProtections);
				sub->AddOption<SubOption>("Misc", nullptr, SubmenuMiscellaneous);
				sub->AddOption<SubOption>("Settings", nullptr, SubmenuSettings);
			});
						
			// Sub to test chat commands without going online :KissKiss:
			g_UiManager->AddSubmenu<RegularSubmenu>("Drop Chat Commands", SubmenuDropTest, [](RegularSubmenu* sub)
				{

				sub->AddOption<RegularOption>("!drop on", "", []
						{
							g_MainScript->PlayerToDrop = PLAYER::PLAYER_PED_ID();
							g_MainScript->current_task = { ChatAction::Drop, 0, "on" };
						});
					sub->AddOption<RegularOption>("!drop off", "", []
						{
							g_MainScript->PlayerToDrop = PLAYER::PLAYER_PED_ID();
							g_MainScript->current_task = { ChatAction::Drop, 0, "off" };
						});
					sub->AddOption<RegularOption>("!drop follow on", "", []
						{
							g_MainScript->PlayerToDrop = PLAYER::PLAYER_PED_ID();
							g_MainScript->current_task = { ChatAction::Drop, 0, "follow", "on" };
						});
					sub->AddOption<RegularOption>("!drop follow off", "", []
						{
							g_MainScript->PlayerToDrop = PLAYER::PLAYER_PED_ID();
							g_MainScript->current_task = { ChatAction::Drop, 0, "follow", "off" };
						});
					sub->AddOption<RegularOption>("!drop value 1000", "", []
						{
							g_MainScript->PlayerToDrop = PLAYER::PLAYER_PED_ID();
							g_MainScript->current_task = { ChatAction::Drop, 0, "value", "1000" };
						});
					sub->AddOption<RegularOption>("!drop value 2500", "", []
						{
							g_MainScript->PlayerToDrop = PLAYER::PLAYER_PED_ID();
							g_MainScript->current_task = { ChatAction::Drop, 0, "value", "2500" };
						});
				});

			g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Test", SubmenuVehicleTest, [](RegularSubmenu* sub)
			{
				for (auto& vehicle : g_VehicleManager->GetDLC(vDLC::CayoPerico))
				{
					sub->AddOption<VehicleOption>(vehicle.name, vehicle.model);
				}
				normal_option("Spawn T20", nullptr) {
					create_vehicle2("T20");
				});
			});

			g_UiManager->AddSubmenu<RegularSubmenu>("Self Options", SubmenuSelf, [&](RegularSubmenu* sub) { selfSubmenu(sub); });
			selfSubmenuSubs();

			g_UiManager->AddSubmenu<RegularSubmenu>("Weapons", SubmenuWeapons, [](RegularSubmenu* sub) { weaponSubmenu(sub); });
			weaponSubmenuSubs();

			g_UiManager->AddSubmenu<RegularSubmenu>("Teleport", SubmenuTeleport, [](RegularSubmenu* sub) { teleportSubmenu(sub); });
			teleportSubmenuSubs();

			g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle", SubmenuVehicle, [](RegularSubmenu* sub) { vehicleSubmenu(sub); });
			vehicleSubmenuSubs();

			g_UiManager->AddSubmenu<RegularSubmenu>("Recovery", SubmenuRecovery, [](RegularSubmenu* sub) { recoverySubmenu(sub); });
			recoverySubmenuSubs();

			g_UiManager->AddSubmenu<RegularSubmenu>("Protections", SubmenuProtections, [](RegularSubmenu* sub) { protectionsSubmenu(sub); });
			protectionsSubmenuSubs();

			g_UiManager->AddSubmenu<RegularSubmenu>("Network", SubmenuNetwork, [](RegularSubmenu* sub) { networkSubmenu(sub); });
			networkSubmenuSubs();

			g_UiManager->AddSubmenu<RegularSubmenu>("Misc", SubmenuMiscellaneous, [&](RegularSubmenu* sub) { miscSubmenu(sub); });
			miscSubmenuSubs();

			g_UiManager->AddSubmenu<RegularSubmenu>("Settings", SubmenuSettings, [&](RegularSubmenu* sub) { settingsSubmenu(sub); });
		//}
	}
	void MainScript::Destroy()
	{
		g_UiManager.reset();
	}
	inline int getIDFromName(const char* name)
	{
		for (std::uint32_t i = 0; i < 32; ++i)
		{
			if (!strcmp(PLAYER::GET_PLAYER_NAME(i), name))
				return i;
		}
		return 0;
	}
	bool hasBeennvbefore;
	void MainScript::Tick()
	{
		g_UiManager->OnTick();
		option_list = remove(option_list);
		
		firstFrame = false;
		//--------------------------------------------------------------\\

		SelfFeatures(PLAYER::PLAYER_ID(), PLAYER::PLAYER_PED_ID());
		WeaponFeatures(PLAYER::PLAYER_ID(), PLAYER::PLAYER_PED_ID());
		NetworkFeatures(PLAYER::PLAYER_ID(), PLAYER::PLAYER_PED_ID());
		ProtectionFeatures(PLAYER::PLAYER_ID(), PLAYER::PLAYER_PED_ID());
		MiscFeatures(PLAYER::PLAYER_ID(), PLAYER::PLAYER_PED_ID());
		VehicleFeatures(PLAYER::PLAYER_ID(), PLAYER::PLAYER_PED_ID());

		//--------------------------------------------------------------\\

		// selfdrop works (position may be bugged), custom drops dont get called (modifying to reference/pointer may fix it? not sure so ill leave it)
		// edit: chat command (custom) drop works, dont know about the ones created in "Create Drop" submenu
		g_DropManager->OnTick();

		//--------------------------------------------------------------\\

		if (GetAsyncKeyState(VK_DELETE)) {
			g_MainScript->cursor ^= true;
		}
		if (GetAsyncKeyState(VK_F12)) {
			g_Running = false;
		}
		
		
		if (g_MainScript->cursor) {
			PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
			PAD::DISABLE_ALL_CONTROL_ACTIONS(1);
			PAD::DISABLE_ALL_CONTROL_ACTIONS(2);
			HUD::_SET_CURSOR_SPRITE(5);
			HUD::_SHOW_CURSOR_THIS_FRAME();
		}
		//--------------------------------------------------------------\\

		if (g_MainScript->current_task.action != ChatAction::None)
		{
			if (g_MainScript->current_task.action == ChatAction::SpawnVehicle)
			{
				spawn_vehicle2(modeltospawn.at(1).c_str(), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PlayerToSpawn));
				//spawn_vehicle2(modeltospawn.at(1).c_str(), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(getIDFromName(g_MainScript->playertospawn.at(0))));
			}
			if (g_MainScript->current_task.action == ChatAction::Kick)
			{
				for (int i = 0; i <= NETWORK::NETWORK_GET_NUM_CONNECTED_PLAYERS(); i++)
				{
					if (!strcmp(PLAYER::GET_PLAYER_NAME(i), g_MainScript->current_task.extra))
					{
						std::vector<int> Kicks = { 764638896, -345371965 };
						for (int kickevent : Kicks) {
							uint64_t kick[4] = { kickevent,  i, 0, 0 };
							g_GameFunctions->m_TriggerScriptEvent(1, kick, 4, 1 << i);
							break;
						}
					}
				}
			}
			if (g_MainScript->current_task.action == ChatAction::Drop)
			{
				if (g_MainScript->current_task.extra == "on" && !g_DropManager->CustomDropExists(PlayerToDrop))
					g_DropManager->CreateCustomDrop2(PlayerToDrop);
				for (auto& drop : g_DropManager->Drops)
				{
					if (PlayerToDrop == drop.player) {
						if (g_MainScript->current_task.extra == "on") drop.toggle = true;
						if (g_MainScript->current_task.extra == "off") drop.toggle = false;
						if (g_MainScript->current_task.extra == "value")
						{
							if (g_MainScript->current_task.extra2 != "")
							{
								std::istringstream dropStream(g_MainScript->current_task.extra2);
								unsigned int dropValue;
								dropStream >> dropValue;
								if (dropValue <= 2500 && dropValue >= 0) drop.value = dropValue;
							}
						}
						if (g_MainScript->current_task.extra == "follow")
						{
							if (g_MainScript->current_task.extra2 == "on")
							{
								drop.followPlayer = true;
							}
							else if (g_MainScript->current_task.extra2 == "off")
							{
								drop.location = ENTITY::GET_ENTITY_COORDS(drop.player == PLAYER::PLAYER_PED_ID() ? drop.player : PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(drop.player), true);
								drop.followPlayer = false;
							}
						}
					}
				}
			}

			current_task = { ChatAction::None, 0, "" };
		}


		ready = false;
		for (std::function<void()> f : native_stack)
		{
			f();
		}
		native_stack.clear();
		ready = true;
		g_GameVariables->m_WorldPtr;
	}
}
