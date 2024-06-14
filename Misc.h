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



void miscSubmenu(RegularSubmenu* sub)
{
	sub->AddOption<BoolOption<bool>>("Hide HUD", nullptr, &g_MainScript->HideHudBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Reset Orbital Canon Cooldown Loop", nullptr, &g_MainScript->OrbitalCanonBool, BoolDisplay::OnOff);
	sub->AddOption<RegularOption>("Reset Orbital Canon Cooldown", nullptr, []
		{
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ORBITAL_CANNON_COOLDOWN"), 0, 0);
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ORBITAL_CANNON_COOLDOWN"), 0, 0);
		});
	sub->AddOption<RegularOption>("Skip Current Cutscene", nullptr, []
	{
		CUTSCENE::STOP_CUTSCENE(true);
		CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
	});
	sub->AddOption<NumberOption<int>>("Drive Speed", "Adjust the drive speed.", &g_MainScript->DriveSpeed, 0, 100, 1, 3, true, "", "", [] {});
	sub->AddOption<ChooseOption<const char*, std::size_t>>("Drive Style", "Adjust the drive style.", &Lists::DriveStylelList, &Lists::DriveStyleListPos);
	sub->AddOption<RegularOption>("Drive To Waypoint", nullptr, []
		{
			
		});
	sub->AddOption<RegularOption>("Stop Auto-Drive", nullptr, []
		{
			BRAIN::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
			ENTITY::SET_ENTITY_HEADING(PLAYER::GET_PLAYERS_LAST_VEHICLE(), ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), PLAYER::GET_PLAYERS_LAST_VEHICLE(), -1);
		});

}
void miscSubmenuSubs()
{

}
void MiscFeatures(std::uint32_t PlayerId, std::uint32_t PedId) {
	if (g_MainScript->HideHudBool) { HUD::HIDE_HUD_AND_RADAR_THIS_FRAME(); }
	if (g_MainScript->OrbitalCanonBool) { STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ORBITAL_CANNON_COOLDOWN"), 0, 0);  STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ORBITAL_CANNON_COOLDOWN"), 0, 0); }
}