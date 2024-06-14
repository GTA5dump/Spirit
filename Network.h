#pragma once
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
#include "GlobalOptions.h"
#include "Playerinfo.h"
#include "ScriptCallback.hpp"
#include "Notification.h"
using namespace Big;
using namespace Big::UserInterface;
//All Define By Dead Eyes
#define PROP_MONEY_BAG_01 0x113FD533
#define PICKUP_MONEY_CASE 0x1E9A99F8 
#define prop_ld_health_pack 0x113FD533
#define PICKUP_HEALTH_STANDARD 0x1E9A99F8
#define PROP_MONEY_BAG_02 -1666779307
#define Rp_Statues_01 1298470051 //alien
#define Rp_Statues_02 437412629 //Princess
#define nocolle 1737847354 //beast
#define colle 1955542584 //beast
#define customscript 738282662 //Custom pickup script skid use 0x2C014CA6
#define Rp_Card_01 1226474661 //queen
#define Rp_Card_02 3642186697 //jack
#define Rp_Card_03 158316850 //king


inline void networkSubmenu(RegularSubmenu* sub)
{
	sub->AddOption<SubOption>("Playerlist", nullptr, SubmenuPlayerList);
	sub->AddOption<SubOption>("Session Weather", nullptr, SessionWeather);
	sub->AddOption<BreakOption>("Misc", nullptr);
	sub->AddOption<BoolOption<bool>>("R* Developer Mode", "Spoofs client into thinking it's a R* Dev.", &g_MainScript->RockstarDevBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Anti Idle Kick", "Pff, imagine being away from your keyboard.", &g_MainScript->AntiIdleKickBook, BoolDisplay::OnOff);
}
inline void networkSubmenuSubs()
{
	g_UiManager->AddSubmenu<RegularSubmenu>("Session Weather", SessionWeather, [](RegularSubmenu* sub)
	{
		if (NETWORK::NETWORK_IS_IN_SESSION())
		{
			sub->AddOption<RegularOption>("[Session Weather Is Online Only]", nullptr, [] {});
		}
		else
			sub->AddOption<RegularOption>("[Session Weather Is Online Only]", nullptr, [] {});
	});

	g_UiManager->AddSubmenu<PlayerSubmenu>(&g_SelectedPlayer, SubmenuSelectedPlayer, [](PlayerSubmenu* sub) {
		Playerinfo(g_SelectedPlayer);
		sub->AddOption<BoolOption<bool>>("Spectate", "Spectate the selected player.", &g_MainScript->SpectatePlayerBool, BoolDisplay::OnOff);
		sub->AddOption<SubOption>("Remotes", nullptr, Remotes);// Dead Eyes
		sub->AddOption<SubOption>("Griefing", nullptr, Griefing); // Dead Eyes
		sub->AddOption<SubOption>("Helpful", nullptr, Helpful);
		sub->AddOption<SubOption>("Dropping", nullptr, Dropping);// Dead Eyes 
		sub->AddOption<SubOption>("Teleport", nullptr, Teleport); // Dead Eyes
		sub->AddOption<SubOption>("Remote Weapons Mods", nullptr, SubmenuSelectedPlayerWeaponsMods); // Dead Eyes 
		sub->AddOption<SubOption>("Remote Vehicle Control", nullptr, SubmenuSelectedPlayerVehicleControl);// Dead Eyes

	});


	g_UiManager->AddSubmenu<RegularSubmenu>("Playerinfo Settings", SubmenuPlayerinfoSettings, [](RegularSubmenu* sub) {
		sub->AddOption<BoolOption<bool>>("Show Playerinfo", "Show's general info about the player.", &g_MainScript->showInfo, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Show R*ID", "Show a unique user identifer.", &g_MainScript->showRID, BoolDisplay::OnOff);
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Chat Commands", SubmenuChatCommands, [](RegularSubmenu* sub) {
		sub->AddOption<BoolOption<bool>>("Enable Chat Commands", nullptr, &g_MainScript->chatCmd, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("!kick %name%", "", &g_MainScript->chatCmdKick, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("!spawn %model%", "", &g_MainScript->chatCmdSpawn, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("!drop", "Options: on/off/value 0-2500/follow on/follow off", &g_MainScript->chatCmdDrop, BoolDisplay::OnOff);
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Playerlist", SubmenuPlayerList, [](RegularSubmenu* sub)
	{
		sub->AddOption<SubOption>("Playerinfo Settings", nullptr, SubmenuPlayerinfoSettings, []{});
		sub->AddOption<SubOption>("Chat Commands", nullptr, SubmenuChatCommands, [] {});
		sub->AddOption<BreakOption>("Players", nullptr);
		
		for (std::uint32_t i = 0; i < 32; ++i)
		{
			if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
			{
				sub->AddOption<SubOption>(PLAYER::GET_PLAYER_NAME(i), nullptr, SubmenuSelectedPlayer, [=]
				{
					g_SelectedPlayer = i;
				});
			}
		}
	});

	g_UiManager->AddSubmenu<RegularSubmenu>("Remotes", Remotes, [](RegularSubmenu* sub)
		{
			Playerinfo(g_SelectedPlayer, g_MainScript->showInfo, !g_MainScript->showRID);
			sub->AddOption<BoolOption<bool>>("Sound Spam", nullptr, &g_MainScript->SoundSpamBool, BoolDisplay::OnOff);
			sub->AddOption<RegularOption>("Kick", nullptr, []
				{
					Hash Kicks[] = { 
						764638896, -345371965, -1252906024
					};
					for (int i = 0; i < 47; i++) {
						uint64_t kick[4] = { Kicks[i],  g_SelectedPlayer, 0, 0 };
						g_GameFunctions->m_TriggerScriptEvent(1, kick, 4, 1 << g_SelectedPlayer);
					}
				});
			sub->AddOption<RegularOption>("CEO Kick", nullptr, []
				{
					uint64_t ceokick[4] = { -1648921703, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0, 0 };
					g_GameFunctions->m_TriggerScriptEvent(1, ceokick, 4, 1 << g_SelectedPlayer);
				});
			sub->AddOption<RegularOption>("CEO Ban", nullptr, []
				{
					uint64_t ban_ceo[4] = { -738295409 , g_SelectedPlayer, 1, 5 };
					g_GameFunctions->m_TriggerScriptEvent(1, ban_ceo, 4, 1 << g_SelectedPlayer);
				});
			sub->AddOption<RegularOption>("Infinite Loading Screen", nullptr, []
				{
					uint64_t inv_inf_loading[9] = { -171207973, g_SelectedPlayer, 0, -1, 1, 1, 0, 0, 0 };
					g_GameFunctions->m_TriggerScriptEvent(1, inv_inf_loading, 9, 1 << g_SelectedPlayer);
				});
			sub->AddOption<RegularOption>("Send To Mission", nullptr, []
				{
					uint64_t mission[2] = { 1115000764, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer) };
					g_GameFunctions->m_TriggerScriptEvent(1, mission, 2, 1 << g_SelectedPlayer);

					uint64_t mission2[2] = { -545396442 , PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer) };
					g_GameFunctions->m_TriggerScriptEvent(1, mission2, 2, 1 << g_SelectedPlayer);
				});
			sub->AddOption<RegularOption>("Send Cayo Perico", nullptr, []
				{
					uint64_t Cayoperico[2] = { 1300962917, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer) };
					g_GameFunctions->m_TriggerScriptEvent(1, Cayoperico, 5, 1 << g_SelectedPlayer);
				});

		});
	g_UiManager->AddSubmenu<RegularSubmenu>("Griefing", Griefing, [](RegularSubmenu* sub)
		{
			Playerinfo(g_SelectedPlayer);
			sub->AddOption<BoolOption<bool>>("Water Loop", nullptr, &g_MainScript->WaterLoopBool, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("Fire Loop", nullptr, &g_MainScript->FireLoopBool, BoolDisplay::OnOff);
			sub->AddOption<RegularOption>("Ram", nullptr, []
				{
					Hash model = MISC::GET_HASH_KEY("INSURGENT");

					g_CallbackScript->AddCallback<ModelCallback>(model, [model] {
						float offset;
						STREAMING::REQUEST_MODEL(model);
						NativeVector3 pCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0.0, -10.0, 0.0);
						if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model))
						{
							NativeVector3 dim1, dim2;
							MISC::GET_MODEL_DIMENSIONS(model, &dim1, &dim2);

							offset = dim2.y * 1.6;

							NativeVector3 dir = ENTITY::GET_ENTITY_FORWARD_VECTOR(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer));
							float rot = (ENTITY::GET_ENTITY_ROTATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0)).z;
							pCoords.x + (dir.x * offset);
							pCoords.y + (dir.y * offset);
							Vehicle veh = VEHICLE::CREATE_VEHICLE(model, pCoords.x, pCoords.y, pCoords.z, rot, true, false, NULL);

							VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, NULL);
							ENTITY::SET_ENTITY_VISIBLE(veh, true, 0);
							VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 1000.0);
						}
						});
				});
			sub->AddOption<RegularOption>("Airstrike", nullptr, []
				{
					NativeVector3 StrikeCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1);
					StrikeCoords.z += 15;
					g_CallbackScript->AddCallback<AssetCallback>((MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET")), [=] {
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 35, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 1, MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 30, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 0, MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), 0, 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 55, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 100, false, 0xF8A3939F, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true, true, 600);
						});
				});
			sub->AddOption<RegularOption>("Taze Him V1", nullptr, []
				{
					NativeVector3 StrikeCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1);
					StrikeCoords.z += 0;
					g_CallbackScript->AddCallback<AssetCallback>((MISC::GET_HASH_KEY("WEAPON_STUNGUN")), [=] {
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 2, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 1, MISC::GET_HASH_KEY("WEAPON_STUNGUN"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 2, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 0, MISC::GET_HASH_KEY("WEAPON_STUNGUN"), 0, 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 2, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 100, false, MISC::GET_HASH_KEY("WEAPON_STUNGUN"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true, true, 600);
						});
				});
			sub->AddOption<RegularOption>("Taze Him V2", nullptr, []
				{
					NativeVector3 StrikeCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1);
					StrikeCoords.z += 0;
					g_CallbackScript->AddCallback<AssetCallback>((MISC::GET_HASH_KEY("weapon_electric_fence")), [=] {
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 0, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 1, MISC::GET_HASH_KEY("weapon_electric_fence"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 0, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 0, MISC::GET_HASH_KEY("weapon_electric_fence"), 0, 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 0, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 100, false, MISC::GET_HASH_KEY("weapon_electric_fence"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true, true, 600);
						});
				});
			sub->AddOption<RegularOption>("Pipe Bom Him", nullptr, []
				{
					NativeVector3 StrikeCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1);
					StrikeCoords.z += 0;
					g_CallbackScript->AddCallback<AssetCallback>((MISC::GET_HASH_KEY("WEAPON_PIPEBOMB")), [=] {
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 5, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 1, MISC::GET_HASH_KEY("WEAPON_PIPEBOMB"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 3, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 0, MISC::GET_HASH_KEY("WEAPON_PIPEBOMB"), 0, 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 2, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 100, false, MISC::GET_HASH_KEY("WEAPON_PIPEBOMB"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true, true, 600);
						});
				});
			sub->AddOption<RegularOption>("Ray Pistol Him", nullptr, []
				{
					NativeVector3 StrikeCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1);
					StrikeCoords.z += 0;
					g_CallbackScript->AddCallback<AssetCallback>((MISC::GET_HASH_KEY("WEAPON_RAYPISTOL")), [=] {
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 10, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 1, MISC::GET_HASH_KEY("WEAPON_RAYPISTOL"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 10, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 0, MISC::GET_HASH_KEY("WEAPON_RAYPISTOL"), 0, 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 10, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 100, false, MISC::GET_HASH_KEY("WEAPON_RAYPISTOL"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true, true, 600);
						});
				});
			sub->AddOption<RegularOption>("Add Gasoline on him", nullptr, []
				{
					NativeVector3 StrikeCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1);
					StrikeCoords.z += 0;
					g_CallbackScript->AddCallback<AssetCallback>((MISC::GET_HASH_KEY("WEAPON_PETROLCAN")), [=] {
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 2, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 1, MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 1, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 0, MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), 0, 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 0, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 100, false, 0xF8A3939F, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true, true, 600);
						});
				});


			sub->AddOption<RegularOption>("Clone", nullptr, []
				{
					PED::CLONE_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1, 1, 1);
				});
			sub->AddOption<RegularOption>("Explode", nullptr, []
				{
					NativeVector3 ExplodeCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1);
					FIRE::ADD_EXPLOSION(ExplodeCoords.x, ExplodeCoords.y, ExplodeCoords.z, 4, 1000.f, true, 0, 10, 0);
				});
			sub->AddOption<RegularOption>("Cage", nullptr, []
				{
					NativeVector3 remotePos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0);
					OBJECT::CREATE_OBJECT(959275690, remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, false);
				});
			sub->AddOption<RegularOption>("Cage 2", nullptr, []
				{
					NativeVector3 remotePos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0);
					OBJECT::CREATE_OBJECT(1396140175, remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, false);
				});
			sub->AddOption<RegularOption>("Cage 3", nullptr, []
				{
					NativeVector3 remotePos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0);
					OBJECT::CREATE_OBJECT(-1326042488, remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, false);
				});
			sub->AddOption<RegularOption>("Cage 4", nullptr, []
				{
					NativeVector3 remotePos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0);
					OBJECT::CREATE_OBJECT(-1363719163, remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, false);
				});
			sub->AddOption<RegularOption>("Cage 5", nullptr, []
				{
					NativeVector3 remotePos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0);
					OBJECT::CREATE_OBJECT(1688773919, remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, false);
				});
			sub->AddOption<RegularOption>("Cage 6", nullptr, []
				{
					NativeVector3 remotePos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0);
					OBJECT::CREATE_OBJECT(3334677549, remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, false);
				});
			sub->AddOption<RegularOption>("Remove All Cage", nullptr, []
				{
					int modelhash = MISC::GET_HASH_KEY("w_at_scope_small");
					uint64_t BadObjects[] = { 959275690, 1396140175, -1326042488, -1363719163, 1688773919, 3334677549, }; // Add pipe hash from 2take1 
					NativeVector3 SelectedPlayer = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(g_SelectedPlayer), true);
					for (Hash hash : BadObjects) {
						ENTITY::CREATE_MODEL_SWAP(SelectedPlayer.x, SelectedPlayer.y, SelectedPlayer.z, 10.0f, hash, modelhash, 0);
					}
				});
			sub->AddOption<RegularOption>("Remove Weapons", nullptr, []
				{
					WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true);
				});
		});
	g_UiManager->AddSubmenu<RegularSubmenu>("Helpful", Helpful, [](RegularSubmenu* sub)
		{
			Playerinfo(g_SelectedPlayer);
			sub->AddOption<RegularOption>("Give All Weapons", nullptr, []
				{
					Hash NormalWeapons[]{ MISC::GET_HASH_KEY("weapon_revolver_mk2"), MISC::GET_HASH_KEY("weapon_snspistol_mk2"), MISC::GET_HASH_KEY("weapon_smg_mk2"), MISC::GET_HASH_KEY("weapon_pumpshotgun_mk2"), MISC::GET_HASH_KEY("weapon_assaultrifle_mk2"), MISC::GET_HASH_KEY("weapon_carbinerifle_mk2"), MISC::GET_HASH_KEY("weapon_specialcarbine_mk2"), MISC::GET_HASH_KEY("weapon_bullpuprifle_mk2"), MISC::GET_HASH_KEY("weapon_combatmg_mk2"), MISC::GET_HASH_KEY("weapon_heavysniper_mk2"), MISC::GET_HASH_KEY("weapon_marksmanrifle_mk2"), 0xAF3696A1, 0x476BF155, 0xB62D1F670, 0xDFE37640, 0xC1B3C3D1, 0x99B507EA, 0x678B81B1, 0x4E875F73, 0x958A4A8F, 0x440E4788, 0x84BD7BFD, 0x1B06D571, 0x5EF9FEC4, 0x22D8FE39, 0x99AEEB3B, 0x13532244, 0x2BE6766B, 0xEFE7E2DF, 0xBFEFFF6D, 0x83BF0278, 0xAF113F99, 0x9D07F764, 0x7FD62962, 0x1D073A89, 0x7846A318, 0xE284C527, 0x9D61E50F, 0x3656C8C1, 0x05FC3C11, 0x0C472FE2, 0x33058E22, 0xA284510B, 0x4DD2DC56, 0xB1CA77B1, 0x687652CE, 0x42BF8A85, 0x93E220BD, 0x2C3731D9, 0xFDBC8A50, 0x24B17070, 0x060EC506, 0x34A67B97, 0xFDBADCED, 0x23C9F95C, 0x497FACC3, 0xF9E6AA4B, 0x61012683, 0xC0A3098D, 0xD205520E, 0xBFD21232, 0x7F229F94, 0x92A27487, 0x083839C4, 0x7F7497E5, 0xA89CB99E, 0x3AABBBAA, 0xC734385A, 0x787F0BB, 0x47757124, 0xD04C944D, MISC::GET_HASH_KEY("WEAPON_HATCHET"), MISC::GET_HASH_KEY("WEAPON_RAILGUN"),	0xAF3696A1, 0x476BF155, 0xB62D1F67, 2441047180, 3126027122, 727643628, 94989220, 1470379660, 2636060646, };
					for (int i = 0; i < (sizeof(NormalWeapons) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), NormalWeapons[i], 9999, 1);
				});
			sub->AddOption<RegularOption>("Give All Normal", nullptr, []
				{
					Hash NormalWeapons[]{ 0xAF3696A1, 0x476BF155, 0xB62D1F670, 0xDFE37640, 0xC1B3C3D1, 0x99B507EA, 0x678B81B1, 0x4E875F73, 0x958A4A8F, 0x440E4788, 0x84BD7BFD, 0x1B06D571, 0x5EF9FEC4, 0x22D8FE39, 0x99AEEB3B, 0x13532244, 0x2BE6766B, 0xEFE7E2DF, 0xBFEFFF6D, 0x83BF0278, 0xAF113F99, 0x9D07F764, 0x7FD62962, 0x1D073A89, 0x7846A318, 0xE284C527, 0x9D61E50F, 0x3656C8C1, 0x05FC3C11, 0x0C472FE2, 0x33058E22, 0xA284510B, 0x4DD2DC56, 0xB1CA77B1, 0x687652CE, 0x42BF8A85, 0x93E220BD, 0x2C3731D9, 0xFDBC8A50, 0x24B17070, 0x060EC506, 0x34A67B97, 0xFDBADCED, 0x23C9F95C, 0x497FACC3, 0xF9E6AA4B, 0x61012683, 0xC0A3098D, 0xD205520E, 0xBFD21232, 0x7F229F94, 0x92A27487, 0x083839C4, 0x7F7497E5, 0xA89CB99E, 0x3AABBBAA, 0xC734385A, 0x787F0BB, 0x47757124, 0xD04C944D, MISC::GET_HASH_KEY("WEAPON_HATCHET"), MISC::GET_HASH_KEY("WEAPON_RAILGUN"),	0xAF3696A1, 0x476BF155, 0xB62D1F67,2441047180, 3126027122, 727643628, 94989220, 1470379660, 2636060646,  };
					for (int i = 0; i < (sizeof(NormalWeapons) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), NormalWeapons[i], 9999, 1);
				});
			sub->AddOption<RegularOption>("Give All MK2", nullptr, []
				{
					Hash MK2Weapons[] = { MISC::GET_HASH_KEY("weapon_revolver_mk2"), MISC::GET_HASH_KEY("weapon_snspistol_mk2"), MISC::GET_HASH_KEY("weapon_smg_mk2"), MISC::GET_HASH_KEY("weapon_pumpshotgun_mk2"), MISC::GET_HASH_KEY("weapon_assaultrifle_mk2"), MISC::GET_HASH_KEY("weapon_carbinerifle_mk2"), MISC::GET_HASH_KEY("weapon_specialcarbine_mk2"), MISC::GET_HASH_KEY("weapon_bullpuprifle_mk2"), MISC::GET_HASH_KEY("weapon_combatmg_mk2"), MISC::GET_HASH_KEY("weapon_heavysniper_mk2"), MISC::GET_HASH_KEY("weapon_marksmanrifle_mk2"), };
					for (int i = 0; i < (sizeof(MK2Weapons) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), MK2Weapons[i], 9999, 1);
				});
		});
	g_UiManager->AddSubmenu<RegularSubmenu>("Dropping", Dropping, [](RegularSubmenu* sub)
		{
			Playerinfo(g_SelectedPlayer);
			sub->AddOption<BoolOption<bool>>("Money Drop [2.5K GTA$]", nullptr, &g_MainScript->TwoFiveKBool, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("RP Drop [2K XP]", nullptr, &g_MainScript->RPDrop, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("Health Drop", nullptr, &g_MainScript->HealthDrop, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("Armor Drop", nullptr, &g_MainScript->ArmorDrop, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("Vehicle Health Drop", nullptr, &g_MainScript->VHealthDrop, BoolDisplay::OnOff);
			sub->AddOption<BoolOption<bool>>("Vehicle Armor Drop", nullptr, &g_MainScript->VArmorDrop, BoolDisplay::OnOff);
			sub->AddOption<RegularOption>("Ammo Drop", nullptr, []
				{
					int modelhash = MISC::GET_HASH_KEY("prop_ld_ammo_pack_01");
					Ped iPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
					STREAMING::REQUEST_MODEL(modelhash);
					if (!STREAMING::HAS_MODEL_LOADED(modelhash)) {
						
					}
					else {
						NativeVector3 playerPosition = ENTITY::GET_ENTITY_COORDS(iPed, FALSE);
						Hash Ammodrop[] = { MISC::GET_HASH_KEY("PICKUP_AMMO_FIREWORK"), MISC::GET_HASH_KEY("PICKUP_AMMO_FLAREGUN"), MISC::GET_HASH_KEY("PICKUP_AMMO_MINIGUN"), MISC::GET_HASH_KEY("PICKUP_AMMO_RPG"), MISC::GET_HASH_KEY("PICKUP_AMMO_GRENADELAUNCHER"), MISC::GET_HASH_KEY("PICKUP_AMMO_SNIPER"), MISC::GET_HASH_KEY("PICKUP_AMMO_SHOTGUN"), MISC::GET_HASH_KEY("PICKUP_AMMO_MG"), MISC::GET_HASH_KEY("PICKUP_AMMO_RIFLE"), MISC::GET_HASH_KEY("PICKUP_AMMO_SMG"), MISC::GET_HASH_KEY("PICKUP_AMMO_PISTOL"), };
						for (int i = 0; i < (sizeof(Ammodrop) / 4); i++)
						OBJECT::CREATE_AMBIENT_PICKUP(Ammodrop[i], playerPosition.x, playerPosition.y, playerPosition.z + 5, 0, 2500, modelhash, FALSE, TRUE);
						STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(modelhash);
					}
				});
			sub->AddOption<RegularOption>("Ped Drop", nullptr, []
				{
					STREAMING::REQUEST_MODEL(MISC::GET_HASH_KEY("A_C_Panther"));
					NativeVector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), false);
					if (STREAMING::HAS_MODEL_LOADED(MISC::GET_HASH_KEY("A_C_Panther")))
					{
						int Ped = PED::CREATE_PED(21, MISC::GET_HASH_KEY("A_C_Panther"), coords.x, coords.y, coords.z, 0, 1, 0);
						if (ENTITY::DOES_ENTITY_EXIST(Ped))
						{
							STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(MISC::GET_HASH_KEY("A_C_Panther"));
							RequestControlOfEnt(Ped);
							PED::SET_PED_MONEY(Ped, 10000);
							ENTITY::SET_ENTITY_HEALTH(Ped, 0);

						}

					}
				});
		
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Teleport", Teleport, [](RegularSubmenu* sub)
		{
			Playerinfo(g_SelectedPlayer);
			sub->AddOption<RegularOption>("To Player", nullptr, []
				{
					Entity Handle;
					NativeVector3 Coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), false);
					PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), false) ? Handle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID())) : Handle = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID());
					ENTITY::SET_ENTITY_COORDS(Handle, Coords.x, Coords.y, Coords.z, false, false, false, false);
				});
			sub->AddOption<RegularOption>("In Vehicle", nullptr, []
				{
					Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), false);
					if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), false)) {
						for (int i = -1; i < 16; i++)
						{
							if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, i, NULL))
							{
								PED::SET_PED_INTO_VEHICLE(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), veh, i);
							}
							else {
								add_notification("Error", "No seats are free.");
							}
						}
					}
					else {
						add_notification("Error", "Player not in vehicle.");
					}
				});
			sub->AddOption<RegularOption>("Vehicle To You", nullptr, []
				{
					NativeVector3 playerCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), FALSE);
					if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
					{
						int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
						RequestControlOfEnt(vehicle);
						if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
						{
							ENTITY::SET_ENTITY_COORDS(vehicle, playerCoords.x, playerCoords.y, playerCoords.z, true, false, false, true);//
						}
						add_notification("Error", "Spam it");
					}
					else
					{
						add_notification("Error", "Player not in vehicle or can't sync");
					}
				});
			sub->AddOption<RegularOption>("Vehicle To Waypoint", nullptr, []
				{
					int WaypointHandle = HUD::GET_FIRST_BLIP_INFO_ID(8);
					if (HUD::DOES_BLIP_EXIST(WaypointHandle))
					{
						NativeVector3 WaypointPos = HUD::GET_BLIP_COORDS(WaypointHandle);
						if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
						{
							int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
							RequestControlOfEnt(vehicle);
							if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
							{
								ENTITY::SET_ENTITY_COORDS(vehicle, WaypointPos.x, WaypointPos.y, WaypointPos.z + 35.0f, 0, 0, 0, 1);
							}
							add_notification("Error", "Spam it");
						}
						else
						{
							add_notification("Error", "Player not in vehicle or can't sync");
						}
					}
					else
					{
						add_notification("Error", "Set waypoint");
					}
				});

			g_UiManager->AddSubmenu<RegularSubmenu>("Remote Vehicle Control", Teleport, [](RegularSubmenu* sub)
				{
					Playerinfo(g_SelectedPlayer);
					sub->AddOption<RegularOption>("Kick Flip", nullptr, []
						{
							if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
							{
								int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
								RequestControlOfEnt(vehicle);
								if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
								{
									ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, 1, 0, 0, 10.0f, 20.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
								}
								add_notification("Error", "Force Apllied");
							}
							else
							{
								add_notification("Error", "Player not in vehicle or can't sync");
							}
						});

					sub->AddOption<RegularOption>("Heel Flip", nullptr, []
						{
							if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
							{
								int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
								RequestControlOfEnt(vehicle);
								if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
								{
									ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, true, 0, 0, 5.0f, -2.0f, 0, 0, true, true, true, true, false, true);
								}
								add_notification("Error", "Force Apllied");
							}
							else
							{
								add_notification("Error", "Player not in vehicle or can't sync");
							}
						});

					sub->AddOption<RegularOption>("Back Flip", nullptr, []
						{
							if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
							{
								int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
								RequestControlOfEnt(vehicle);
								if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
								{
									ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, true, 0, 0, 6.0f, 0, 2.0f, 0, true, true, true, true, false, true);
								}
								add_notification("Error", "Force Apllied");
							}
							else
							{
								add_notification("Error", "Player not in vehicle or can't sync");
							}
						});
					sub->AddOption<RegularOption>("Double Back Flip", nullptr, []
						{
							if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
							{
								int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
								RequestControlOfEnt(vehicle);
								if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
								{
									ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, true, 0, 0, 12.0f, 0, 4.0f, 0, true, true, true, true, false, true);
								}
								add_notification("Error", "Force Apllied");
							}
							else
							{
								add_notification("Error", "Player not in vehicle or can't sync");
							}
						});
					sub->AddOption<RegularOption>("360 | Donut", nullptr, []
						{
							if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
							{
								int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
								RequestControlOfEnt(vehicle);
								if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
								{
									ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, 1, 0, 20.f, 0.0f, 20.f, 0.f, 0.f, 0, 1, 1, 1, 0, 1);
								}
								add_notification("Error", "Force Apllied");
							}
							else
							{
								add_notification("Error", "Player not in vehicle or can't sync");
							}
						});
					sub->AddOption<RegularOption>("Jump", nullptr, []
						{
							if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
							{
								int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
								RequestControlOfEnt(vehicle);
								if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
								{
									ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, true, 0, 0, 7.0f, 0, 0, 0, true, true, true, true, false, true);
								}
								add_notification("Error", "Force Apllied");
							}
							else
							{
								add_notification("Error", "Player not in vehicle or can't sync");
							}
						});
					sub->AddOption<RegularOption>("Send To The Moon", nullptr, []
						{
							if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
							{
								int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
								RequestControlOfEnt(vehicle);
								if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
								{
									ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, true, 0, 0, 999.0f, 0, 0, 0, true, true, true, true, false, true);
								}
								add_notification("Error", "Force Apllied");
							}
							else
							{
								add_notification("Error", "Player not in vehicle or can't sync");
							}
						});
					sub->AddOption<RegularOption>("Crush Him", nullptr, []
						{
							if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
							{
								int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
								RequestControlOfEnt(vehicle);
								if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
								{
									ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, true, 0, 0, -400.0f, 0, 0, 0, true, true, true, true, false, true);
								}
								add_notification("Error", "Force Apllied");
							}
							else
							{
								add_notification("Error", "Player not in vehicle or can't sync");
							}
						});
					sub->AddOption<RegularOption>("Spinning Jump", nullptr, []
						{
							if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
							{
								int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
								RequestControlOfEnt(vehicle);
								if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
								{
									ENTITY::APPLY_FORCE_TO_ENTITY(vehicle, 1, 0, 20.f, 20.0f, 20.f, 0.f, 0.f, 0, 1, 1, 1, 0, 1);
								}
								add_notification("Error", "Force Apllied");
							}
							else
							{
								add_notification("Error", "Player not in vehicle or can't sync");
							}
						});
					sub->AddOption<RegularOption>("Boost", nullptr, []
						{
							if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
							{
								int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
								RequestControlOfEnt(vehicle);
								if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
								{
									VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle, 500.f);
								}
								add_notification("Error", "Force Apllied");
							}
							else
							{
								add_notification("Error", "Player not in vehicle or can't sync");
							}
						});
					sub->AddOption<RegularOption>("Backward Boost", nullptr, []
						{
							if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
							{
								int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
								RequestControlOfEnt(vehicle);
								if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
								{
									VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle, -500.0f);
								}
								add_notification("Error", "Force Apllied");
							}
							else
							{
								add_notification("Error", "Player not in vehicle or can't sync");
							}
						});
					sub->AddOption<RegularOption>("Stop Vehicle", nullptr, []
						{
							if (PED::IS_PED_IN_ANY_VEHICLE((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), true))
							{
								int vehicle = PED::GET_VEHICLE_PED_IS_IN((PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer)), false);
								RequestControlOfEnt(vehicle);
								if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehicle))
								{
									VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle, 0);
								}
								add_notification("Error", "Force Apllied");
							}
							else
							{
								add_notification("Error", "Player not in vehicle or can't sync");
							}
						});


				});

		});

	g_UiManager->AddSubmenu<RegularSubmenu>("Remote Weapons Mods", SubmenuSelectedPlayerWeaponsMods, [](RegularSubmenu* sub) {

		Playerinfo(g_SelectedPlayer);
		sub->AddOption<BoolOption<bool>>("Money Gun", "Selected Player Shoot 2,5k $", &g_MainScript->Shootmoney, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Rp Gun", "Selected Player Shoot 2k RP", &g_MainScript->ShootRp, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Delete Gun", "Delete Object Shooted", &g_MainScript->Deletegunr, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Fire Gun", "Selected Player Shoot Flames", &g_MainScript->FireGun, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Water Gun", "Selected Player Shoot Hydrant", &g_MainScript->WaterGun, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Ped Gun", "Selected Player Shoot Pedestrian", &g_MainScript->PedGunv2, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Adder Gun", "Selected Player Shoot Adder", &g_MainScript->Shootcars, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Cargo Plane Gun", "Selected Player Shoot Cargo Planes", &g_MainScript->Shootcargo, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Kosatka Gun", "Selected Player Shoot Submarines", &g_MainScript->Shootsub, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Airstrike Gun", "Selected Player Shoot Airstrikes", &g_MainScript->Shootairstrike, BoolDisplay::OnOff);
		//sub->AddOption<BoolOption<bool>>("Weapon_Option_Name", "Description", &g_MainScript->OptionBool, BoolDisplay::OnOff); My Example 


		});



}

inline void NetworkFeatures(std::uint32_t PlayerId, std::uint32_t PedId) {
	NETWORK::SPECTATE(g_MainScript->SpectatePlayerBool, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer));




	if (g_MainScript->AntiIdleKickBook)
	{
		globalHandle(1377236).At(1165).As<int>() = -99999999;
	}

	if (g_MainScript->SoundSpamBool) {
		uint64_t sound_spam[3] = { 552065831, g_SelectedPlayer, 1 };
		g_GameFunctions->m_TriggerScriptEvent(1, sound_spam, 3, 1 << g_SelectedPlayer);
	}


	if (g_MainScript->WaterLoopBool) {
		Player waterloopedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
		NativeVector3 Pos = ENTITY::GET_ENTITY_COORDS(waterloopedplayer, true);
		FIRE::ADD_EXPLOSION(Pos.x, Pos.y, Pos.z - 2, 13, 5, true, 0, 0, 0);
	}

	if (g_MainScript->FireLoopBool) {
		Player fireloopedplayer = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
		NativeVector3 Pos = ENTITY::GET_ENTITY_COORDS(fireloopedplayer, true);
		FIRE::ADD_EXPLOSION(Pos.x, Pos.y, Pos.z - 2, 12, 5, true, 0, 0, 0);
	}

	if (g_MainScript->DropBool) {
		if ((timeGetTime() - g_MainScript->TimeM) > g_MainScript->DropDelayInt)
		{
			g_CallbackScript->AddCallback<ModelCallback>(-1666779307, [=] {
				Ped iPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
				NativeVector3 pCoords = ENTITY::GET_ENTITY_COORDS(iPed, 0);
				OBJECT::CREATE_AMBIENT_PICKUP(0x1E9A99F8, pCoords.x, pCoords.y, pCoords.z + g_MainScript->DropHeightInt, 0, g_MainScript->DropAmountInt, -1666779307, FALSE, TRUE);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(-1666779307);
				});

			g_MainScript->TimeM = timeGetTime();
		}
	}
	if (g_MainScript->RPDrop)
	{
		for (int i = 0; i <= 201; i++)
		{
			Hash hash = MISC::GET_HASH_KEY("vw_prop_vw_colle_imporage");
			STREAMING::REQUEST_MODEL(hash);
			g_CallbackScript->AddCallback<ModelCallback>(hash, [=] {
				NativeVector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true);
				OBJECT::CREATE_AMBIENT_PICKUP(738282662, playerPosition.x, playerPosition.y, playerPosition.z, 1, 1, hash, FALSE, TRUE);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(738282662);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
			});
		}
	}
	if (g_MainScript->TwoFiveKBool) {
		if ((timeGetTime() - g_MainScript->TimeM) > g_MainScript->DropDelayInt)
		{
			g_CallbackScript->AddCallback<ModelCallback>(-1666779307, [=] {
				Ped iPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
				NativeVector3 pCoords = ENTITY::GET_ENTITY_COORDS(iPed, 0);
				OBJECT::CREATE_AMBIENT_PICKUP(0x1E9A99F8, pCoords.x, pCoords.y, pCoords.z + g_MainScript->DropHeightInt, 0, 2500, -1666779307, FALSE, TRUE);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(-1666779307);
				});

			g_MainScript->TimeM = timeGetTime();
		}
	}

	if (g_MainScript->HealthDrop)
	{
		Ped iPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
		NativeVector3 playerPosition = ENTITY::GET_ENTITY_COORDS(iPed, FALSE);
		OBJECT::CREATE_AMBIENT_PICKUP(MISC::GET_HASH_KEY("pickup_health_standard"), playerPosition.x, playerPosition.y, playerPosition.z, 0, 1, 1, false, true);
	}

	if (g_MainScript->ArmorDrop)
	{
		Ped iPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
		NativeVector3 playerPosition = ENTITY::GET_ENTITY_COORDS(iPed, FALSE);
		OBJECT::CREATE_AMBIENT_PICKUP(MISC::GET_HASH_KEY("pickup_armour_standard"), playerPosition.x, playerPosition.y, playerPosition.z, 0, 1, 1, false, true);
	}


	if (g_MainScript->VHealthDrop)
	{
		Ped iPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
		NativeVector3 playerPosition = ENTITY::GET_ENTITY_COORDS(iPed, FALSE);
		OBJECT::CREATE_AMBIENT_PICKUP(MISC::GET_HASH_KEY("pickup_vehicle_health_standard"), playerPosition.x, playerPosition.y, playerPosition.z, 0, 1, 1, false, true);
	}

	if (g_MainScript->VArmorDrop)
	{
		Ped iPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer);
		NativeVector3 playerPosition = ENTITY::GET_ENTITY_COORDS(iPed, FALSE);
		OBJECT::CREATE_AMBIENT_PICKUP(MISC::GET_HASH_KEY("pickup_vehicle_armour_standard"), playerPosition.x, playerPosition.y, playerPosition.z, 0, 1, 1, false, true);
	}








	if (g_MainScript->TuneableXmas)
	{
		//toggle gift 
		globalHandle(262145).At(8984).As<int>() = true;
		globalHandle(262145 + 8984).As<int>() = true;
		globalHandle(262145).At(8985).As<int>() = true;
		globalHandle(262145 + 8985).As<int>() = true;
		globalHandle(262145).At(8986).As<int>() = true;
		globalHandle(262145 + 8986).As<int>() = true;

		//2019  
		globalHandle(262145).At(27973).As<int>() = true;
		globalHandle(262145 + 27973).As<int>() = true;
		globalHandle(262145).At(27974).As<int>() = true;
		globalHandle(262145 + 27974).As<int>() = true;
		globalHandle(262145).At(27975).As<int>() = true;
		globalHandle(262145 + 27975).As<int>() = true;
		globalHandle(262145).At(27976).As<int>() = true;
		globalHandle(262145 + 27976).As<int>() = true;


		//2018
		globalHandle(262145).At(25122).As<int>() = true;
		globalHandle(262145 + 25122).As<int>() = true;
		globalHandle(262145).At(25123).As<int>() = true;
		globalHandle(262145 + 25123).As<int>() = true;
		globalHandle(262145).At(25124).As<int>() = true;
		globalHandle(262145 + 25124).As<int>() = true;
		globalHandle(262145).At(25125).As<int>() = true;
		globalHandle(262145 + 25125).As<int>() = true;

		//2016
		globalHandle(262145).At(18651).As<int>() = true;
		globalHandle(262145 + 18651).As<int>() = true;
		globalHandle(262145).At(18652).As<int>() = true;
		globalHandle(262145 + 18652).As<int>() = true;
		globalHandle(262145).At(18653).As<int>() = true;
		globalHandle(262145 + 18653).As<int>() = true;
		globalHandle(262145).At(18654).As<int>() = true;
		globalHandle(262145 + 18654).As<int>() = true;


		//2015
		globalHandle(262145).At(12349).As<int>() = true;
		globalHandle(262145 + 12349).As<int>() = true;
		globalHandle(262145).At(12350).As<int>() = true;
		globalHandle(262145 + 12350).As<int>() = true;
		globalHandle(262145).At(12351).As<int>() = true;
		globalHandle(262145 + 12351).As<int>() = true;
	}

	if (g_MainScript->TuneableIndependence)
	{
		globalHandle(262145).At(7795).As<int>() = true;
		globalHandle(262145 + 7795).As<int>() = true;


		//unlock
		globalHandle(262145).At(7833).As<int>() = true;
		globalHandle(262145 + 7833).As<int>() = true;
		globalHandle(262145).At(7834).As<int>() = true;
		globalHandle(262145 + 7834).As<int>() = true;
		globalHandle(262145).At(7835).As<int>() = true;
		globalHandle(262145 + 7835).As<int>() = true;
		globalHandle(262145).At(7836).As<int>() = true;
		globalHandle(262145 + 7836).As<int>() = true;
		globalHandle(262145).At(7837).As<int>() = true;
		globalHandle(262145 + 7837).As<int>() = true;
		globalHandle(262145).At(7838).As<int>() = true;
		globalHandle(262145 + 7838).As<int>() = true;
		globalHandle(262145).At(7839).As<int>() = true;
		globalHandle(262145 + 7839).As<int>() = true;


		//firework
		globalHandle(262145).At(7800).As<int>() = true;
		globalHandle(262145 + 7800).As<int>() = true;
		globalHandle(262145).At(7801).As<int>() = true;
		globalHandle(262145 + 7801).As<int>() = true;
		globalHandle(262145).At(7802).As<int>() = true;
		globalHandle(262145 + 7802).As<int>() = true;
		globalHandle(262145).At(7803).As<int>() = true;
		globalHandle(262145 + 7803).As<int>() = true;



		//Vehicle
		globalHandle(262145).At(7814).As<int>() = true;
		globalHandle(262145 + 7814).As<int>() = true;
		globalHandle(262145).At(7815).As<int>() = true;
		globalHandle(262145 + 7815).As<int>() = true;


		//Weapons
		globalHandle(262145).At(7821).As<int>() = true;
		globalHandle(262145 + 7821).As<int>() = true;
		globalHandle(262145).At(7822).As<int>() = true;
		globalHandle(262145 + 7822).As<int>() = true;
		globalHandle(262145).At(7823).As<int>() = true;
		globalHandle(262145 + 7823).As<int>() = true;
		globalHandle(262145).At(7824).As<int>() = true;
		globalHandle(262145 + 7824).As<int>() = true;
	}

	if (g_MainScript->TuneableValentine)
	{
		globalHandle(262145).At(12981).As<int>() = true;
		globalHandle(262145).At(11563).As<int>() = true;
		globalHandle(262145).At(12929).As<int>() = true;
		globalHandle(262145).At(12930).As<int>() = true;
		globalHandle(262145).At(11564).As<int>() = true;
		globalHandle(262145).At(11565).As<int>() = true;
		globalHandle(262145).At(11566).As<int>() = true;
		globalHandle(262145).At(11567).As<int>() = true;
		globalHandle(262145).At(12982).As<int>() = true;
		globalHandle(262145).At(12984).As<int>() = true;
		globalHandle(262145 + 12981).As<int>() = true;
		globalHandle(262145 + 12929).As<int>() = true;
		globalHandle(262145 + 12930).As<int>() = true;
		globalHandle(262145 + 11563).As<int>() = true;
		globalHandle(262145 + 11564).As<int>() = true;
		globalHandle(262145 + 11565).As<int>() = true;
		globalHandle(262145 + 11566).As<int>() = true;
		globalHandle(262145 + 11567).As<int>() = true;
		globalHandle(262145 + 12982).As<int>() = true;
		globalHandle(262145 + 12984).As<int>() = true;
		globalHandle(262145 + 6596).As<int>() = true;
		globalHandle(262145 + 6597).As<int>() = true;
		globalHandle(262145 + 6598).As<int>() = true;
		globalHandle(262145 + 6599).As<int>() = true;
		globalHandle(262145 + 6600).As<int>() = true;
		globalHandle(262145 + 6601).As<int>() = true;
		globalHandle(262145 + 6602).As<int>() = true;
		globalHandle(262145 + 6603).As<int>() = true;
	}

	if (g_MainScript->TuneableHalloween)
	{
		//Halloween
		globalHandle(262145).At(12235).As<int>() = true;
		globalHandle(262145 + 12235).As<int>() = true;
		globalHandle(262145).At(11529).As<int>() = true;
		globalHandle(262145 + 11529).As<int>() = true;


		//Turn On
		globalHandle(262145).At(11574).As<int>() = true;
		globalHandle(262145 + 11574).As<int>() = true;
		globalHandle(262145).At(11575).As<int>() = true;
		globalHandle(262145 + 11575).As<int>() = true;
		globalHandle(262145).At(11576).As<int>() = true;
		globalHandle(262145 + 11576).As<int>() = true;
		globalHandle(262145).At(11578).As<int>() = true;
		globalHandle(262145 + 11578).As<int>() = true;
		globalHandle(262145).At(11584).As<int>() = true;
		globalHandle(262145 + 11584).As<int>() = true;
		globalHandle(262145).At(11585).As<int>() = true;
		globalHandle(262145 + 11585).As<int>() = true;
		globalHandle(262145).At(11586).As<int>() = true;
		globalHandle(262145 + 11586).As<int>() = true;

		//Bubble head
		globalHandle(262145).At(12096).As<int>() = true;
		globalHandle(262145).At(12097).As<int>() = true;
		globalHandle(262145).At(12098).As<int>() = true;
		globalHandle(262145).At(12099).As<int>() = true;
		globalHandle(262145).At(12100).As<int>() = true;
		globalHandle(262145).At(12101).As<int>() = true;
		globalHandle(262145).At(12102).As<int>() = true;
		globalHandle(262145).At(12103).As<int>() = true;
		globalHandle(262145).At(12104).As<int>() = true;
		globalHandle(262145).At(12105).As<int>() = true;
		globalHandle(262145).At(12106).As<int>() = true;
		globalHandle(262145).At(12107).As<int>() = true;
		globalHandle(262145).At(12108).As<int>() = true;
		globalHandle(262145).At(12109).As<int>() = true;
		globalHandle(262145).At(12110).As<int>() = true;
		globalHandle(262145).At(12111).As<int>() = true;
		globalHandle(262145).At(12112).As<int>() = true;
		globalHandle(262145).At(12113).As<int>() = true;
		globalHandle(262145).At(12114).As<int>() = true;
		globalHandle(262145).At(12115).As<int>() = true;
		globalHandle(262145).At(12116).As<int>() = true;
		globalHandle(262145).At(12117).As<int>() = true;
		globalHandle(262145 + 12096).As<int>() = true;
		globalHandle(262145 + 12097).As<int>() = true;
		globalHandle(262145 + 12098).As<int>() = true;
		globalHandle(262145 + 12099).As<int>() = true;
		globalHandle(262145 + 12100).As<int>() = true;
		globalHandle(262145 + 12101).As<int>() = true;
		globalHandle(262145 + 12102).As<int>() = true;
		globalHandle(262145 + 12103).As<int>() = true;
		globalHandle(262145 + 12104).As<int>() = true;
		globalHandle(262145 + 12105).As<int>() = true;
		globalHandle(262145 + 12106).As<int>() = true;
		globalHandle(262145 + 12107).As<int>() = true;
		globalHandle(262145 + 12108).As<int>() = true;
		globalHandle(262145 + 12109).As<int>() = true;
		globalHandle(262145 + 12110).As<int>() = true;
		globalHandle(262145 + 12111).As<int>() = true;
		globalHandle(262145 + 12112).As<int>() = true;
		globalHandle(262145 + 12113).As<int>() = true;
		globalHandle(262145 + 12114).As<int>() = true;
		globalHandle(262145 + 12115).As<int>() = true;
		globalHandle(262145 + 12116).As<int>() = true;
		globalHandle(262145 + 12117).As<int>() = true;
	}

	if (g_MainScript->TuneableRPBoost)
	{
		globalHandle(262145).At(6615).As<int>() = true;
		globalHandle(262145 + 6615).As<int>() = true;
	}

	if (g_MainScript->TuneableBunkerMoney)
	{
		globalHandle(262145).At(21045).As<float>() = 10.f;
		globalHandle(262145).At(21046).As<float>() = 10.f;
		globalHandle(262145).At(21044).As<float>() = 10.f;
		globalHandle(262145).At(21047).As<float>() = 10.f;
		globalHandle(262145).At(21048).As<float>() = 10.f;
	}

	if (g_MainScript->Shootcars)
	{

		if (PED::IS_PED_SHOOTING(PLAYER::GET_PLAYER_PED(g_SelectedPlayer)))
		{
			NativeVector3 PlayerPos;
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::GET_PLAYER_PED(g_SelectedPlayer), &PlayerPos);
			int Hashs = MISC::GET_HASH_KEY("ADDER");
			STREAMING::REQUEST_MODEL(Hashs);
			if (STREAMING::HAS_MODEL_LOADED(Hashs))
			{
				VEHICLE::CREATE_VEHICLE(Hashs, PlayerPos.x, PlayerPos.y, PlayerPos.z, 0.0f, 1, 0, 0);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hashs);
			}
		}
	}
	if (g_MainScript->Shootmoney)
	{

		if (PED::IS_PED_SHOOTING(PLAYER::GET_PLAYER_PED(g_SelectedPlayer)))
		{
			NativeVector3 PlayerPos;
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::GET_PLAYER_PED(g_SelectedPlayer), &PlayerPos);
			STREAMING::REQUEST_MODEL((PROP_MONEY_BAG_02));
			while (!STREAMING::HAS_MODEL_LOADED((PROP_MONEY_BAG_02))) {
				OBJECT::CREATE_AMBIENT_PICKUP((PICKUP_MONEY_CASE), PlayerPos.x, PlayerPos.y, PlayerPos.z, 0, 2500, (PROP_MONEY_BAG_02), FALSE, TRUE);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED((PROP_MONEY_BAG_02)); }

		}
	}
	if (g_MainScript->Shootexp)
	{

		if (PED::IS_PED_SHOOTING(PLAYER::GET_PLAYER_PED(g_SelectedPlayer)))
		{
			NativeVector3 PlayerPos;
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::GET_PLAYER_PED(g_SelectedPlayer), &PlayerPos);
			FIRE::ADD_EXPLOSION(PlayerPos.x, PlayerPos.y, PlayerPos.z, 18, 1000.f, true, 0, 10, 0);

		}
	}
	if (g_MainScript->Shootairstrike)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::GET_PLAYER_PED(g_SelectedPlayer))) {
			{
				NativeVector3 PlayerPos;
				WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::GET_PLAYER_PED(g_SelectedPlayer), &PlayerPos);

				Hash airStrike = MISC::GET_HASH_KEY("WEAPON_AIRSTRIKE_ROCKET");
				WEAPON::REQUEST_WEAPON_ASSET(airStrike, 31, false);
				while (!WEAPON::HAS_WEAPON_ASSET_LOADED(airStrike))
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(PlayerPos.x, PlayerPos.y, PlayerPos.z + 50.f, PlayerPos.x, PlayerPos.y, PlayerPos.z, 250, 1, airStrike, g_SelectedPlayer, 1, 0, -1.0);
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(PlayerPos.x, PlayerPos.y + 20.f, PlayerPos.z + 50.f, PlayerPos.x, PlayerPos.y, PlayerPos.z, 250, 1, airStrike, g_SelectedPlayer, 1, 0, -1.0);
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(PlayerPos.x + 20.f, PlayerPos.y, PlayerPos.z + 50.f, PlayerPos.x, PlayerPos.y, PlayerPos.z, 250, 1, airStrike, g_SelectedPlayer, 1, 0, -1.0);



			}
		}

	}
	if (g_MainScript->Deletegunr)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::GET_PLAYER_PED(g_SelectedPlayer))) {
			{
				NativeVector3 PlayerPos;
				WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::GET_PLAYER_PED(g_SelectedPlayer), &PlayerPos);
				MISC::_CLEAR_AREA_OF_EVERYTHING(PlayerPos.x, PlayerPos.y, PlayerPos.z, 2, 0, 0, 0, 0);
			}
		}
	}
	if (g_MainScript->PedGunv2)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::GET_PLAYER_PED(g_SelectedPlayer))) {
			{
				NativeVector3 PlayerPos;
				WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::GET_PLAYER_PED(g_SelectedPlayer), &PlayerPos);
				PED::CREATE_RANDOM_PED(PlayerPos.x, PlayerPos.y, PlayerPos.z);
			}
		}
	}
	if (g_MainScript->FireGun)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::GET_PLAYER_PED(g_SelectedPlayer))) {
			{
				NativeVector3 PlayerPos;
				WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::GET_PLAYER_PED(g_SelectedPlayer), &PlayerPos);
				FIRE::ADD_EXPLOSION(PlayerPos.x, PlayerPos.y, PlayerPos.z, 12, 5.f, true, 0, 10, 0);
			}
		}
	}
	if (g_MainScript->WaterGun)
	{
		if (PED::IS_PED_SHOOTING(PLAYER::GET_PLAYER_PED(g_SelectedPlayer))) {
			{
				NativeVector3 PlayerPos;
				WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::GET_PLAYER_PED(g_SelectedPlayer), &PlayerPos);
				FIRE::ADD_EXPLOSION(PlayerPos.x, PlayerPos.y, PlayerPos.z, 13, 5.f, true, 0, 10, 0);
			}
		}
	}
	if (g_MainScript->Shootcargo)
	{

		if (PED::IS_PED_SHOOTING(PLAYER::GET_PLAYER_PED(g_SelectedPlayer)))
		{
			NativeVector3 PlayerPos;
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::GET_PLAYER_PED(g_SelectedPlayer), &PlayerPos);
			int Hashs = MISC::GET_HASH_KEY("CARGOPLANE");
			STREAMING::REQUEST_MODEL(Hashs);
			if (STREAMING::HAS_MODEL_LOADED(Hashs))
			{
				VEHICLE::CREATE_VEHICLE(Hashs, PlayerPos.x, PlayerPos.y, PlayerPos.z, 0.0f, 1, 0, 0);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hashs);
			}
		}
	}
	if (g_MainScript->Shootsub)
	{

		if (PED::IS_PED_SHOOTING(PLAYER::GET_PLAYER_PED(g_SelectedPlayer)))
		{
			NativeVector3 PlayerPos;
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::GET_PLAYER_PED(g_SelectedPlayer), &PlayerPos);
			int Hashs = MISC::GET_HASH_KEY("KOSATKA");
			STREAMING::REQUEST_MODEL(Hashs);
			if (STREAMING::HAS_MODEL_LOADED(Hashs))
			{
				VEHICLE::CREATE_VEHICLE(Hashs, PlayerPos.x, PlayerPos.y, PlayerPos.z, 0.0f, 1, 0, 0);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Hashs);
			}
		}
	}

	if (g_MainScript->ShootRp)
	{

		if (PED::IS_PED_SHOOTING(PLAYER::GET_PLAYER_PED(g_SelectedPlayer)))
		{
			NativeVector3 PlayerPos;
			WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::GET_PLAYER_PED(g_SelectedPlayer), &PlayerPos);
			Hash RPHASH = MISC::GET_HASH_KEY("vw_prop_vw_colle_prbubble");


			STREAMING::REQUEST_MODEL(RPHASH);
			if (!STREAMING::HAS_MODEL_LOADED(RPHASH)) {

			}
			else {
				OBJECT::CREATE_AMBIENT_PICKUP(0x2C014CA6, PlayerPos.x, PlayerPos.y, PlayerPos.z, 0, 1, RPHASH, FALSE, TRUE);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(RPHASH);
			}

		}
	}





}
