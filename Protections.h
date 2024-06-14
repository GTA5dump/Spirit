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

using namespace Big;
using namespace Big::UserInterface;

bool NotifyOnJoinBool, NotifyOnLeaveBool = false;

void protectionsSubmenu(RegularSubmenu* sub)
{
	sub->AddOption<SubOption>("Events", nullptr, EventProtection);
	sub->AddOption<SubOption>("Blocks", nullptr, BlockProtection);
	sub->AddOption<SubOption>("Other", nullptr, OtherProtection);
	
}
void protectionsSubmenuSubs()
{
	g_UiManager->AddSubmenu<RegularSubmenu>("Events", EventProtection, [](RegularSubmenu* sub)
	{
		sub->AddOption<BoolOption<bool>>("Kick", nullptr, &g_MainScript->KickProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Wanted", nullptr, &g_MainScript->WantedProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Bounty", nullptr, &g_MainScript->BountyProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Invite", nullptr, &g_MainScript->InviteProtectionBool, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("CEO Kick", nullptr, &g_MainScript->Ceo_KickProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("CEO Ban", nullptr, &g_MainScript->CeobanProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("CEO Money", nullptr, &g_MainScript->CeoMoneyProtectionBool, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Sound Spam", nullptr, &g_MainScript->SoundSpamProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Transaction", nullptr, &g_MainScript->TransactionProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Send To Job", nullptr, &g_MainScript->JobProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Loading Screen", nullptr, &g_MainScript->InfLoadingProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Remote Teleport", nullptr, &g_MainScript->RemoteTeleportProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Block Passive Mode", nullptr, &g_MainScript->PassiveModeProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Remote Off The Radar", nullptr, &g_MainScript->OfftheradarProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Send To Cayo Perico", nullptr, &g_MainScript->Cayoperico, BoolDisplay::OnOff);

	});

	g_UiManager->AddSubmenu<RegularSubmenu>("Blocks", BlockProtection, [](RegularSubmenu* sub)
	{
		sub->AddOption<BoolOption<bool>>("Fire", nullptr, &g_MainScript->FireProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Time", nullptr, &g_MainScript->TimeProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Weather", nullptr, &g_MainScript->WeatherProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Explosion", nullptr, &g_MainScript->ExplosionProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Give Weapons", nullptr, &g_MainScript->GiveWeaponProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Remove Weapons", nullptr, &g_MainScript->RemoveWeaponProtection, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Other", nullptr, &g_MainScript->OtherProtectionBool, BoolDisplay::OnOff);
	});

	g_UiManager->AddSubmenu<RegularSubmenu>("Other", OtherProtection, [](RegularSubmenu* sub)
	{
		sub->AddOption<BoolOption<bool>>("Joining Players", nullptr, &NotifyOnJoinBool, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Leaving Players", nullptr, &NotifyOnLeaveBool, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Kick On Attack", nullptr, &g_MainScript->RedirectEvent, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Block Money Pickup", nullptr, &g_MainScript->MoneyPickup, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Bypass Chat Censor", nullptr, &g_MainScript->ChatCensor, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Transaction Error Screen", nullptr, &g_MainScript->TransactionError, BoolDisplay::OnOff);
	});
}
void ProtectionFeatures(std::uint32_t PlayerId, std::uint32_t PedId)
{

	if (NotifyOnLeaveBool) {
		for (int i = 0; i < 33; i++) {
			if (auto Leaving = NETWORK::NETWORK_LEAVE_TRANSITION())
			{
				std::string PlayerName = PLAYER::GET_PLAYER_NAME(Leaving);
				std::string Message = " is leaving your session";
				std::string Combined = PlayerName + Message;
				const char* uwu = (Combined).c_str();

				IconNotification(&((char*)uwu)[0], (char*)"Spirit", &((char*)"Player Leaving")[0]);
			}
		}
	}

	if (NotifyOnJoinBool) {
		for (int i = 0; i < 33; i++) {
			if (auto Leaving = NETWORK::NETWORK_JOIN_TRANSITION(i))
			{
				std::string PlayerName = PLAYER::GET_PLAYER_NAME(Leaving);
				std::string Message = " is joining your session";
				std::string Combined = PlayerName + Message;
				const char* uwu = (Combined).c_str();

				IconNotification(&((char*)uwu)[0], (char*)"Spirit", &((char*)"Player Joining")[0]);
			}
		}
	}

	g_GameFunctions->DefuseEvent(SCRIPT_ARRAY_DATA_VERIFY_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(REQUEST_CONTROL_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(GIVE_CONTROL_EVENT, g_MainScript->GiveWeaponProtection);
	g_GameFunctions->DefuseEvent(WEAPON_DAMAGE_EVENT, g_MainScript->RemoveWeaponProtection);
	g_GameFunctions->DefuseEvent(REQUEST_PICKUP_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(REQUEST_MAP_PICKUP_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(GAME_CLOCK_EVENT, g_MainScript->TimeProtection);
	g_GameFunctions->DefuseEvent(GAME_WEATHER_EVENT, g_MainScript->WeatherProtection);
	g_GameFunctions->DefuseEvent(RESPAWN_PLAYER_PED_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(GIVE_WEAPON_EVENT, g_MainScript->GiveWeaponProtection);
	g_GameFunctions->DefuseEvent(REMOVE_WEAPON_EVENT, g_MainScript->RemoveWeaponProtection);
	g_GameFunctions->DefuseEvent(REMOVE_ALL_WEAPONS_EVENT, g_MainScript->RemoveWeaponProtection);
	g_GameFunctions->DefuseEvent(VEHICLE_COMPONENT_CONTROL_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(FIRE_EVENT, g_MainScript->FireProtection);
	g_GameFunctions->DefuseEvent(EXPLOSION_EVENT, g_MainScript->ExplosionProtection);
	g_GameFunctions->DefuseEvent(START_PROJECTILE_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(UPDATE_PROJECTILE_TARGET_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(BREAK_PROJECTILE_TARGET_LOCK_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(REMOVE_PROJECTILE_ENTITY_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(ALTER_WANTED_LEVEL_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(CHANGE_RADIO_STATION_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(RAGDOLL_REQUEST_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(PLAYER_TAUNT_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(PLAYER_CARD_STAT_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(DOOR_BREAK_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(SCRIPTED_GAME_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(REMOTE_SCRIPT_INFO_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(REMOTE_SCRIPT_LEAVE_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(MARK_AS_NO_LONGER_NEEDED_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(CONVERT_TO_SCRIPT_ENTITY_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(SCRIPT_WORLD_STATE_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(INCIDENT_ENTITY_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(CLEAR_AREA_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(CLEAR_RECTANGLE_AREA_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_REQUEST_SYNCED_SCENE_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_START_SYNCED_SCENE_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_UPDATE_SYNCED_SCENE_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_STOP_SYNCED_SCENE_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(GIVE_PED_SCRIPTED_TASK_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(GIVE_PED_SEQUENCE_TASK_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_CLEAR_PED_TASKS_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_START_PED_ARREST_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_START_PED_UNCUFF_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_SOUND_CAR_HORN_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_ENTITY_AREA_STATUS_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_GARAGE_OCCUPIED_STATUS_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(PED_CONVERSATION_LINE_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(SCRIPT_ENTITY_STATE_CHANGE_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_PLAY_SOUND_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_STOP_SOUND_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_PLAY_AIRDEFENSE_FIRE_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_BANK_REQUEST_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_AUDIO_BARK_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(REQUEST_DOOR_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_TRAIN_REQUEST_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_TRAIN_REPORT_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_INCREMENT_STAT_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(MODIFY_VEHICLE_LOCK_WORD_STATE_DATA, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(MODIFY_PTFX_WORD_STATE_DATA_SCRIPTED_EV, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(REQUEST_PHONE_EXPLOSION_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(REQUEST_DETACHMENT_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(KICK_VOTES_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(GIVE_PICKUP_REWARDS_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_CRC_HASH_CHECK_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(BLOW_UP_VEHICLE_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_SPECIAL_FIRE_EQUIPPED_WEAPON, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_RESPONDED_TO_THREAT_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_SHOUT_TARGET_POSITION, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(VOICE_DRIVEN_MOUTH_MOVEMENT_FINISHED_EV, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(PICKUP_DESTROYED_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(UPDATE_PLAYER_SCARS_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_CHECK_EXE_SIZE_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_PTFX_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_PED_SEEN_DEAD_PED_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(REMOVE_STICKY_BOMB_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_CHECK_CODE_CRCS_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(INFORM_SILENCED_GUNSHOT_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(PED_PLAY_PAIN_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(CACHE_PLAYER_HEAD_BLEND_DATA_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(REMOVE_PED_FROM_PEDGROUP_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(REPORT_MYSELF_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(REPORT_CASH_SPAWN_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(ACTIVATE_VEHICLE_SPECIAL_ABILITY_EVENT, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(BLOCK_WEAPON_SELECTION, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(NETWORK_CHECK_CATALOG_CRC, g_MainScript->OtherProtectionBool);
	g_GameFunctions->DefuseEvent(REMOVE_ALL_PED_WEAPONS, g_MainScript->OtherProtectionBool);
}