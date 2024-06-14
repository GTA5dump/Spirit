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
#include "Notification.h"

using namespace Big;
using namespace Big::UserInterface;

int customsellprice = 67000000;
inline int amount = 10000;
void recoverySubmenu(RegularSubmenu* sub)
{
	sub->AddOption<SubOption>("Drop Manager", nullptr, SubmenuDrop);
	sub->AddOption<SubOption>("Tuneables", nullptr, SubmenuTuneableRecovery);//SubmenuTuneableRecovery
	//sub->AddOption<SubOption>("Money", nullptr, SubmenuVehicleRecovery);
	sub->AddOption<SubOption>("Unlocks", nullptr, SubmenuTest);
	sub->AddOption<SubOption>("Level/RP", nullptr, SubmenuRPRecovery);

	// Use a while loop together with the getline() function to read the file line by line
	sub->AddOption<NumberOption<int>>("Amount", nullptr, &amount, 0, 5000000, 100000, 100000, true, "", "", [] {});
	//sub->AddOption<RegularOption>("Hash: " + hash, nullptr, [] {});
	sub->AddOption<RegularOption>("dew it", nullptr, [&] {
		globalHandle(4265412 + 1).As<int>() = 2147483646;
		globalHandle(4265412 + 7).As<int>() = 2147483647;
		globalHandle(4265412 + 6).As<int>() = 0;
		globalHandle(4265412 + 5).As<int>() = 0;
		globalHandle(4265412 + 3).As<int>() = -585718395;
		globalHandle(4265412 + 2).As<int>() = 10;
		globalHandle(4265412 + 80).At(80).As<int>() = 1;
		globalHandle(4265412).As<int>() = 1;

	});
}



void recoverySubmenuSubs()
{
	/* Rename to whatever you like */
	g_UiManager->AddSubmenu<RegularSubmenu>("Drop Manager", SubmenuDrop, [](RegularSubmenu* sub)
	{
		sub->AddOption<SubOption>("Custom Drop", "Create drop with custom location", SubmenuCustomDrop);
		sub->AddOption<RegularOption>("Spawn Single Bag", "Spawn single money bag", []
		{
			g_DropManager->spawn(g_DropManager->SelfDrop);
		});
		sub->AddOption<BoolOption<bool>>("Toggle", "Toggle money bag spawn loop", &g_DropManager->SelfDrop.toggle, BoolDisplay::OnOff);
		sub->AddOption<SubOption>("Location", "Change bag spawn location", SubmenuDropLocation);
		sub->AddOption<SubOption>("Options", "Change drop options", SubmenuDropOptions);
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Custom Drop", SubmenuCustomDrop, [](RegularSubmenu* sub) // Spoiler: They dont really work
	{
		sub->AddOption<SubOption>("Create Drop", "Create custom drop", SubmenuCreateDrop);

		for (auto& drop : g_DropManager->Drops)
		{
			sub->AddOption<SubOption>("Drop", nullptr, SubmenuSelectedDrop, [=] // Drop will always be called "Drop"
			{
				g_SelectedDrop = drop;
			});
		}
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Create Drop", SubmenuCreateDrop, [](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Create", nullptr, []
		{
			g_SelectedDrop = g_DropManager->CreateCustomDrop(Joaat(g_DropManager->getModelName(true)), Joaat(g_DropManager->getPickupName(true)), g_DropManager->defaultValue, g_DropManager->defaultInterval, g_DropManager->defaultPickupPosition, g_DropManager->defaultModelPosition);
			g_UiManager->SubmenuPop(); // little h4x to get back to selected drops when leaving selected one
			g_UiManager->SwitchToSubmenu(SubmenuSelectedDrop);
		});
		sub->AddOption<NumberOption<int>>("Bag Value", nullptr, &g_DropManager->defaultValue, 0, 2500, 100);
		sub->AddOption<NumberOption<int>>("Spawn Interval", nullptr, &g_DropManager->defaultInterval, 0, 5000, 100);
		sub->AddOption<ChooseOption<const char*, std::size_t>>("Model", nullptr, &Lists::DropModelsName, &g_DropManager->defaultModelPosition, true, [] {});
		sub->AddOption<ChooseOption<const char*, std::size_t>>("Pickup", nullptr, &Lists::DropPickupsName, &g_DropManager->defaultPickupPosition, true, [] {});
	});
	g_UiManager->AddSubmenu<DropSubmenu>(&g_SelectedDrop, SubmenuSelectedDrop, [](DropSubmenu* sub)
	{
		g_DropManager->DropInfo(g_SelectedDrop);
		sub->AddOption<RegularOption>("Spawn Single Bag", "Spawn single money bag", []
		{
			g_DropManager->spawn(g_SelectedDrop);
		});
		sub->AddOption<BoolOption<bool>>("Toggle", "Toggle selected drop", &g_SelectedDrop.toggle, BoolDisplay::OnOff);
		/* // crash :cry:
		sub->AddOption<RegularOption>("Delete", "Delete selected drop", []
		{
			g_DropManager->RemoveCustomDrop(g_SelectedDrop.ID);
		});
		*/
		sub->AddOption<SubOption>("Location", "Change selected drop spawn location", SubmenuSelectedDropLocation);
		sub->AddOption<SubOption>("Options", "Change selected drop options", SubmenuSelectedDropOptions);
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Selected Drop Location", SubmenuSelectedDropLocation, [](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Set To My Location", nullptr, []
		{
			NativeVector3 self = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			g_SelectedDrop.location = self;
		});
		sub->AddOption<NumberOption<float>>("X", nullptr, &g_SelectedDrop.location.x, -1000.f, 1000.f, 1.f, 2);
		sub->AddOption<NumberOption<float>>("Y", nullptr, &g_SelectedDrop.location.y, -1000.f, 1000.f, 1.f, 2);
		sub->AddOption<NumberOption<float>>("Z", nullptr, &g_SelectedDrop.location.z, -1000.f, 1000.f, 1.f, 2);
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Selected Drop Options", SubmenuSelectedDropOptions, [](RegularSubmenu* sub)
	{
		sub->AddOption<NumberOption<int>>("Bag Value", nullptr, &g_SelectedDrop.value, 0, 2500, 100);
		sub->AddOption<NumberOption<int>>("Spawn Interval", nullptr, &g_SelectedDrop.interval, 0, 5000, 100);
		sub->AddOption<BoolOption<bool>>("Show Marker", "Show dollar sign on top of drop location", &g_SelectedDrop.marker, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Show Info", "Show drop information", &g_SelectedDrop.info, BoolDisplay::OnOff);
		sub->AddOption<ChooseOption<const char*, std::size_t>>("Model", nullptr, &Lists::DropModelsName, &g_SelectedDrop.modelPosition, true, []
		{
			g_SelectedDrop.model = Joaat(g_DropManager->getCustomModelName(g_SelectedDrop));
		});
		sub->AddOption<ChooseOption<const char*, std::size_t>>("Pickup", nullptr, &Lists::DropPickupsName, &g_SelectedDrop.pickupPosition, true, []
		{
			g_SelectedDrop.pickup = Joaat(g_DropManager->getCustomPickupName(g_SelectedDrop));
		});
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Self Drop Location", SubmenuDropLocation, [](RegularSubmenu* sub)
	{
		sub->AddOption<BoolOption<bool>>("Follow Me", nullptr, &g_DropManager->SelfDrop.followPlayer, BoolDisplay::OnOff);
		sub->AddOption<RegularOption>("Set To My Location", nullptr, []
		{
			NativeVector3 self = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			g_DropManager->SelfDrop.location = self;
		});
		sub->AddOption<NumberOption<float>>("X", nullptr, &g_DropManager->SelfDrop.location.x, 0.f, 1000.f, 1.f, 2);
		sub->AddOption<NumberOption<float>>("Y", nullptr, &g_DropManager->SelfDrop.location.y, 0.f, 1000.f, 1.f, 2);
		sub->AddOption<NumberOption<float>>("Z", nullptr, &g_DropManager->SelfDrop.location.z, 0.f, 1000.f, 1.f, 2);
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Self Drop Options", SubmenuDropOptions, [](RegularSubmenu* sub)
	{
		sub->AddOption<NumberOption<int>>("Bag Value", nullptr, &g_DropManager->SelfDrop.value, 0, 2500, 100);
		sub->AddOption<NumberOption<int>>("Spawn Interval", nullptr, &g_DropManager->SelfDrop.interval, 0, 5000, 100);
		sub->AddOption<BoolOption<bool>>("Show Marker", "Show dollar sign on top of drop location", &g_DropManager->SelfDrop.marker, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Show Info", "Show drop information", &g_DropManager->SelfDrop.info, BoolDisplay::OnOff);
		sub->AddOption<ChooseOption<const char*, std::size_t>>("Model", nullptr, &Lists::DropModelsName, &Lists::DropModelPosition, true, []
		{
			g_DropManager->SelfDrop.model = Joaat(g_DropManager->getModelName(false));
		});
		sub->AddOption<ChooseOption<const char*, std::size_t>>("Pickup", nullptr, &Lists::DropPickupsName, &Lists::DropPickupPosition, true, []
		{
			g_DropManager->SelfDrop.pickup = Joaat(g_DropManager->getPickupName(false));
		});
	});


	g_UiManager->AddSubmenu<RegularSubmenu>("Tuneables", SubmenuTuneableRecovery, [](RegularSubmenu* sub)
	{
		sub->AddOption<BoolOption<bool>>("Xmas Tuneables", nullptr, &g_MainScript->TuneableXmas, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Halloween Tuneables", nullptr, &g_MainScript->TuneableIndependence, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Valentine Tuneables", nullptr, &g_MainScript->TuneableValentine, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Halloween Tuneables", nullptr, &g_MainScript->TuneableHalloween, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Rp Boost Tuneables", nullptr, &g_MainScript->TuneableRPBoost, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("Bunker Multiplier x10", nullptr, &g_MainScript->TuneableBunkerMoney, BoolDisplay::OnOff);

	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Unlocks", SubmenuTest, [&](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Max Stats", nullptr, []
		{
			char* stats_to_change[] = { (char*)"SCRIPT_INCREASE_DRIV", (char*)"SCRIPT_INCREASE_LUNG", (char*)"SCRIPT_INCREASE_SHO", (char*)"SCRIPT_INCREASE_STAM", (char*)"SCRIPT_INCREASE_STL", (char*)"SCRIPT_INCREASE_STRN" };
			for (char* hash : stats_to_change)
			{
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(&((std::string)"MP0_" + hash)[0]), 100, true);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(&((std::string)"MP1_" + hash)[0]), 100, true);
			}
			add_notification("Recovery", "Max stats for character 0 & 1.");
		});
		sub->AddOption<RegularOption>("Tattoos", nullptr, []
		{
			for (int i = 0; i <= 1; i++)
			{
				const char* prefix = (i == 0 ? "MP0" : "MP1");
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_tattoo_fm_unlocks_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_tattoo_fm_unlocks_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_tattoo_fm_unlocks_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_tattoo_fm_unlocks_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_tattoo_fm_unlocks_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_tattoo_fm_unlocks_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_tattoo_fm_unlocks_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_tattoo_fm_unlocks_8").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_tattoo_fm_unlocks_9").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_tattoo_fm_unlocks_10").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_tattoo_fm_unlocks_11").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_tattoo_fm_unlocks_12").c_str()), -1, 1);
			}
			add_notification("Recovery", "Unlocked all tattoos.");
		});
		sub->AddOption<RegularOption>("Hairstyles", nullptr, []
		{
			for (int i = 0; i <= 1; i++)
			{
				const char* prefix = (i == 0 ? "MP0" : "MP1");
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR7").c_str()), -1, 1);
			}
			add_notification("Recovery", "Unlocked all tattoos.");
		});
		sub->AddOption<RegularOption>("Achievements", nullptr, []
		{
			for (int i = 1; i < 100; i++) {
				PLAYER::GIVE_ACHIEVEMENT_TO_PLAYER(i);
			}
			add_notification("Recovery", "Unlocked all achievements.");
		});
		sub->AddOption<RegularOption>("Clothing", nullptr, []
		{
			for (int i = 0; i <= 1; i++)
			{
				const char* prefix = (i == 0 ? "MP0" : "MP1");
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_AWD_FMHORDWAVESSURVIVE").c_str()), 10, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_AWD_FMPICKUPDLCCRATE1ST").c_str()), 1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_AWD_WIN_CAPTURE_DONT_DYING").c_str()), 25, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_AWD_DO_HEIST_AS_MEMBER").c_str()), 25, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_AWD_PICKUP_CAP_PACKAGES").c_str()), 100, 1);
				STATS::STAT_SET_BOOL(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_AWD_FINISH_HEIST_NO_DAMAGE").c_str()), 1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_AWD_WIN_GOLD_MEDAL_HEISTS").c_str()), 25, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_AWD_KILL_TEAM_YOURSELF_LTS").c_str()), 25, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_AWD_KILL_PSYCHOPATHS").c_str()), 100, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_AWD_DO_HEIST_AS_THE_LEADER").c_str()), 25, 1);
				STATS::STAT_SET_BOOL(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_AWD_STORE_20_CAR_IN_GARAGES").c_str()), 1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_FEET_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_HAIR").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_HAIR_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_HAIR_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_HAIR_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_HAIR_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_HAIR_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_HAIR_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_HAIR_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_JBIB").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_JBIB_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_JBIB_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_JBIB_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_JBIB_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_JBIB_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_JBIB_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_JBIB_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_LEGS").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_LEGS_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_LEGS_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_LEGS_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_LEGS_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_LEGS_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_LEGS_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_LEGS_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_FEET").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_FEET_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_FEET_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_FEET_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_FEET_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_FEET_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_FEET_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_FEET_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_BERD").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_BERD_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_BERD_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_BERD_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_BERD_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_BERD_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_BERD_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_BERD_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_PROPS").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_PROPS_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_PROPS_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_PROPS_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_PROPS_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_PROPS_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_PROPS_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_PROPS_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_PROPS_8").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_PROPS_9").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_PROPS_10").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_OUTFIT").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_HAIR").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_HAIR_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_HAIR_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_HAIR_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_HAIR_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_HAIR_5").c_str()), -1, 1);;
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_HAIR_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_HAIR_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_JBIB").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_JBIB_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_JBIB_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_JBIB_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_JBIB_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_JBIB_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_JBIB_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_JBIB_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_LEGS").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_LEGS_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_LEGS_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_LEGS_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_LEGS_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_LEGS_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_LEGS_6").c_str()), -1, 1);;
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_LEGS_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_FEET").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_FEET_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_FEET_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_FEET_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_FEET_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_FEET_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_FEET_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_FEET_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_BERD").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_BERD_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_BERD_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_BERD_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_BERD_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_BERD_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_BERD_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_BERD_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_PROPS").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_PROPS_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_PROPS_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_PROPS_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_PROPS_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_PROPS_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_PROPS_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_PROPS_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_PROPS_8").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_PROPS_9").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_PROPS_10").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_OUTFIT").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_TORSO").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_SPECIAL").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_SPECIAL_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_SPECIAL_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_SPECIAL_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_SPECIAL_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_SPECIAL_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_SPECIAL_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_SPECIAL_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_SPECIAL2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_SPECIAL2_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_DECL").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_TEETH").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_TEETH_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_AVAILABLE_TEETH_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_TORSO").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_SPECIAL").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_SPECIAL_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_SPECIAL_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_SPECIAL_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_SPECIAL_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_SPECIAL_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_SPECIAL_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_SPECIAL_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_SPECIAL2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_SPECIAL2_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_DECL").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_TEETH").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_TEETH_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_CLTHS_ACQUIRED_TEETH_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_0").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_8").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_9").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_10").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_11").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_12").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_13").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_14").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_15").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_16").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_17").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_18").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_19").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_21").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_22").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_23").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_24").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_24").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_25").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_26").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_27").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_28").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_29").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_30").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_31").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_32").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_33").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_34").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_35").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_36").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_37").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_38").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_39").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_DLC_APPAREL_ACQUIRED_40").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_2").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_3").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_4").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_5").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_6").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_7").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_8").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_9").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_10").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_11").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_12").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_13").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_1").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_10").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_11").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "MP0_ADMIN_CLOTHES_GV_BS_12").c_str()), -1, 1);
			}
			add_notification("Recovery", "Unlocked all clothing.");
		});
		sub->AddOption<RegularOption>("LSC Upgrades", nullptr, []
		{
			for (int i = 0; i <= 1; i++)
			{
				const char* prefix = (i == 0 ? "MP0" : "MP1");
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CHAR_FM_CARMOD_1_UNLCK").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CHAR_FM_CARMOD_2_UNLCK").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CHAR_FM_CARMOD_3_UNLCK").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CHAR_FM_CARMOD_4_UNLCK").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CHAR_FM_CARMOD_5_UNLCK").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CHAR_FM_CARMOD_6_UNLCK").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CHAR_FM_CARMOD_7_UNLCK").c_str()), -1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_AWD_FMRALLYWONDRIVE").c_str()), 1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_AWD_FMRALLYWONNAV").c_str()), 1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_AWD_FMWINSEARACE").c_str()), 1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_AWD_FMWINAIRRACE").c_str()), 1, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_NUMBER_TURBO_STARTS_IN_RACE").c_str()), 50, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_USJS_COMPLETED").c_str()), 50, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_AWD_FM_RACES_FASTEST_LAP").c_str()), 50, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_NUMBER_SLIPSTREAMS_IN_RACE").c_str()), 100, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_AWD_WIN_CAPTURES").c_str()), 25, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_AWD_DROPOFF_CAP_PACKAGES").c_str()), 100, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_AWD_KILL_CARRIER_CAPTURE").c_str()), 100, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_AWD_FINISH_HEISTS").c_str()), 50, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_AWD_FINISH_HEIST_SETUP_JOB").c_str()), 50, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_AWD_WIN_LAST_TEAM_STANDINGS").c_str()), 50, 1);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_AWD_ONLY_PLAYER_ALIVE_LTS").c_str()), 50, 1);
			}
			add_notification("Recovery", "Unlocked everything at LSC.");
		});
		sub->AddOption<RegularOption>("Fast Run", nullptr, []
		{
			char* stats_to_change[] = { (char*)"CHAR_ABILITY_1_UNLCK", (char*)"CHAR_ABILITY_2_UNLCK", (char*)"CHAR_ABILITY_3_UNLCK", (char*)"CHAR_FM_ABILITY_1_UNLCK", (char*)"CHAR_FM_ABILITY_2_UNLCK", (char*)"CHA_FMR_ABILITY_3_UNLCK" };
			for (char* hash : stats_to_change)
			{
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(&((std::string)"MP0_" + hash)[0]), -1, true);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(&((std::string)"MP1_" + hash)[0]), -1, true);
			}
			add_notification("Recovery", "Made your characters run 15 Percent faster.");
		});
		sub->AddOption<RegularOption>("Remove Fast Run", nullptr, []
		{
			char* stats_to_change[] = { (char*)"CHAR_ABILITY_1_UNLCK", (char*)"CHAR_ABILITY_2_UNLCK", (char*)"CHAR_ABILITY_3_UNLCK", (char*)"CHAR_FM_ABILITY_1_UNLCK", (char*)"CHAR_FM_ABILITY_2_UNLCK", (char*)"CHA_FMR_ABILITY_3_UNLCK" };
			for (char* hash : stats_to_change)
			{
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(&((std::string)"MP0_" + hash)[0]), 0, true);
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(&((std::string)"MP1_" + hash)[0]), 0, true);
			}
			add_notification("Recovery", "Makes your characters run 15 Percent slower.");
		});
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Level/RP", SubmenuRPRecovery, [](RegularSubmenu* sub)
	{
		sub->AddOption<NumberOption<int>>("Rank", nullptr, &g_MainScript->targetLevel, 0, 8000, 1, 1);
		//sub->AddOption<BreakOption>("Presets", nullptr);
		sub->AddOption<RegularOption>("Rank 120", "Minigun level.", [] {
			g_MainScript->targetLevel = 120;
		});
		sub->AddOption<RegularOption>("Rank 333", nullptr, [] {
			g_MainScript->targetLevel = 333;
		});
		sub->AddOption<RegularOption>("Rank 699", nullptr, [] {
			g_MainScript->targetLevel = 696;
		});
		sub->AddOption<RegularOption>("Rank 1000", nullptr, [] {
			g_MainScript->targetLevel = 1000;
		});
		sub->AddOption<RegularOption>("Rank 1337", "Become a real script kiddy.", [] {
			g_MainScript->targetLevel = 1000;
		});
		sub->AddOption<RegularOption>("Rank 3333", nullptr, [] {
			g_MainScript->targetLevel = 3333;
		});
		sub->AddOption<RegularOption>("Rank 6666", nullptr, [] {
			g_MainScript->targetLevel = 6666;
		});
		sub->AddOption<RegularOption>("Rank 8000 [MAX]", "Go to the max rank.", [] {
			g_MainScript->targetLevel = 8000;
		});

		sub->AddOption<NumberOption<int>>("Character", nullptr, &g_MainScript->targetChar, 0, 1, 1, 1);


		sub->AddOption<RegularOption>("Apply", nullptr, [] {
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string) (g_MainScript->targetChar == 0 ? "MP0_" : "MP1_") + "CHAR_SET_RP_GIFT_ADMIN").c_str()), g_MainScript->Levels[(g_MainScript->targetLevel - 1)], 1);
			add_notification("Recovery", "please join a new session to apply.");
			
		});

	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Extra Money", SubmenuSettingsLanguage, [](RegularSubmenu* sub)
	{
		sub->AddOption<BoolOption<bool>>("Self 2k Money Drop", nullptr, &g_MainScript->moneyDrop, BoolDisplay::OnOff);

	});
}