#include "UI/UIManager.hpp"
#include "UI/BoolOption.hpp"
#include "UI/ChooseOption.hpp"
#include "UI/NumberOption.hpp"
#include "UI/RegularOption.hpp"
#include "UI/BreakOption.hpp"
#include "UI/BreakOption.hpp"
#include "UI/SubOption.hpp"
#include "UI/RegularSubmenu.hpp"
#include "UI/PlayerSubmenu.hpp"
#include "UI/StringOption.hpp"
#include "GlobalOptions.h"
#include "Fiber.hpp"
#include "ScriptCallback.hpp"
#include "NanoMemory.h"


using namespace Big;
using namespace Big::UserInterface;

void create_vehicle(const char* toSpawn) {
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


class VehicleOption : public BaseOption<VehicleOption>
{
public:
	explicit VehicleOption() = default;
	explicit VehicleOption(const char* text, const char* vehicle, const char* description = nullptr, bool shouldAdd = true)
	{
		SetLeftText(text);
		if (description)
			SetDescription(description);
		std::function<void()> action = [vehicle] { create_vehicle(vehicle); };
		SetAction(std::move(action));
		if (shouldAdd) option_list.push_back({ text, std::move(action),  amount_of_options });
		amount_of_options++;
	}

	~VehicleOption() noexcept = default;
	VehicleOption(VehicleOption const&) = default;
	VehicleOption& operator=(VehicleOption const&) = default;
	VehicleOption(VehicleOption&&) = default;
	VehicleOption& operator=(VehicleOption&&) = default;
};

void vehicleSubmenu(RegularSubmenu* sub)
{

	sub->AddOption<SubOption>("Spawner", nullptr, Spawner);
	sub->AddOption<SubOption>("Vehicle Fly", nullptr, VehicleFly);
	sub->AddOption<BoolOption<bool>>("Vehicle Godmode", nullptr, &g_MainScript->VehicleGodmodeBool, BoolDisplay::OnOff, false, [] {});
	sub->AddOption<BoolOption<bool>>("Horn Boost", nullptr, &g_MainScript->HornBoostBool, BoolDisplay::OnOff, false, [] {});
	sub->AddOption<BoolOption<bool>>("Auto Fix", nullptr, &g_MainScript->AutoFixBool, BoolDisplay::OnOff, false, [] {});
	sub->AddOption<BoolOption<bool>>("Speed Bypass", nullptr, &g_MainScript->NoSpeedLimitBool, BoolDisplay::OnOff, false, [] {});
	sub->AddOption<BoolOption<bool>>("Drift Mode", nullptr, &g_MainScript->DriftModeBool, BoolDisplay::OnOff, false, [] {});
	sub->AddOption<BoolOption<bool>>("Invisible", nullptr, &g_MainScript->InvisibiltyBool, BoolDisplay::OnOff, false, [] {});
	sub->AddOption<BoolOption<bool>>("Vehicle Rockets", nullptr, &g_MainScript->VehicleRocketsBool, BoolDisplay::OnOff, false, [] {});
	sub->AddOption<NumberOption<float>>("Acceleration", nullptr, &g_MainScript->Acceleration, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
		NanoMemory::set_value<float>({ 0x8, 0xD28, 0x918, 0x4C }, g_MainScript->Acceleration);
	});
	sub->AddOption<NumberOption<float>>("Brake", nullptr, &g_MainScript->Brake, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
		NanoMemory::set_value<float>({ 0x8, 0xD28, 0x918, 0x6C }, g_MainScript->Brake);

	});
	sub->AddOption<NumberOption<float>>("Traction", nullptr, &g_MainScript->Traction, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
		NanoMemory::set_value<float>({ 0x8, 0xD28, 0x918, 0x90 }, g_MainScript->Traction);

	});
	sub->AddOption<NumberOption<float>>("Deform", nullptr, &g_MainScript->Deform, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
		NanoMemory::set_value<float>({ 0x8, 0xD28, 0x918, 0xF8 }, g_MainScript->Deform);

	});
	sub->AddOption<NumberOption<float>>("Suspension", nullptr, &g_MainScript->Suspension, 0.f, 1000.f, 0.1f, 1, true, "", "", [] {
		NanoMemory::set_value<float>({ 0x8, 0xD28, 0x918, 0xBC }, g_MainScript->Suspension);
	});
	
	sub->AddOption<RegularOption>("Max Vehicle", nullptr, []
	{
		Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
		{
			VEHICLE::SET_VEHICLE_MOD_KIT(Vehicle, 0);
			for (int i = 0; i < 50; i++)
			{
				VEHICLE::SET_VEHICLE_MOD(Vehicle, i, VEHICLE::GET_NUM_VEHICLE_MODS(Vehicle, i) - 1, false);
			}
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle, 7, 84, 61);
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle, 7, 84, 61);
			VEHICLE::SET_VEHICLE_FIXED(Vehicle);
			VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(Vehicle);
			VEHICLE::SET_VEHICLE_DIRT_LEVEL(Vehicle, 0);
		}
	});
	sub->AddOption<RegularOption>("Clean Vehicle", nullptr, []
	{
		Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
		{
			GRAPHICS::WASH_DECALS_FROM_VEHICLE(Vehicle, 100.f);
		}
	});
	sub->AddOption<RegularOption>("Fix Vehicle", nullptr, []
		{
			Entity Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			{
				VEHICLE::SET_VEHICLE_DIRT_LEVEL(Vehicle, 0);
				VEHICLE::SET_VEHICLE_FIXED(Vehicle);
				VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(Vehicle);
			}
		});
	sub->AddOption<RegularOption>("Open Doors", nullptr, []
		{
			Entity Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			{
				VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 0, true, false);
				VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 1, true, false);
				VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 2, true, false);
				VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 3, true, false);
				VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 4, true, false);
				VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 5, true, false);
				VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 6, true, false);
				VEHICLE::SET_VEHICLE_DOOR_OPEN(Vehicle, 7, true, false);
			}
		});
	sub->AddOption<RegularOption>("Close Doors", nullptr, []
		{
			Entity Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			{
				VEHICLE::SET_VEHICLE_DOORS_SHUT(Vehicle, false);
			}
		});
	sub->AddOption<RegularOption>("Attach Ramp", nullptr, []
	{
		Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
		{
			NativeVector3 CoordsV3 = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			STREAMING::REQUEST_MODEL(MISC::GET_HASH_KEY("prop_mp_ramp_01"));
			if (STREAMING::HAS_MODEL_LOADED(MISC::GET_HASH_KEY("prop_mp_ramp_01")))
			{
				Entity Ramp = OBJECT::CREATE_OBJECT(MISC::GET_HASH_KEY("prop_mp_ramp_01"), CoordsV3.x, CoordsV3.y, CoordsV3.z, 1, 1, 0);
				if (ENTITY::DOES_ENTITY_EXIST(Ramp))
				{
					ENTITY::SET_ENTITY_COLLISION(Ramp, true, true);
					ENTITY::SET_ENTITY_ALPHA(Ramp, 130, false);
					ENTITY::ATTACH_ENTITY_TO_ENTITY(Ramp, Vehicle, 0, 0, 6.3, -0.3, 13.5, 0, 177.5, 0, 0, true, 0, 2, 1);
				}
			}
		}
	});
	sub->AddOption<RegularOption>("Random Color", nullptr, []
	{
		Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
		{
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Vehicle, MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255));
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Vehicle, MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255), MISC::GET_RANDOM_INT_IN_RANGE(0, 255));
		}
	});
	sub->AddOption<RegularOption>("Launch Up", nullptr, []
		{
			Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(Vehicle, true, 0, 0, 40.0f, 0, 0, 0, true, true, true, true, false, true);
			}
		});
	sub->AddOption<RegularOption>("Launch Forward", nullptr, []
		{
			Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			{
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(Vehicle, 80);
			}
		});
	sub->AddOption<RegularOption>("Flip Vehicle", nullptr, []
		{
			Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
			{
				VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(Vehicle, NULL);
			}
		});
	sub->AddOption<RegularOption>("Own Vehicle", "So you can keep it if you have a tracker.", []
	{
		Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);
		if (ENTITY::DOES_ENTITY_EXIST(Vehicle))
		{
			DECORATOR::DECOR_SET_INT(Vehicle, "Player_Vehicle", NETWORK::NETWORK_HASH_FROM_PLAYER_HANDLE(PLAYER::PLAYER_ID()));
			g_Logger->Info("Set current vehicle as storable.");
		}
		else
		{
			g_Logger->Info("Vehicle doesn't exist");
		}
	});
	sub->AddOption<BreakOption>("Multiplers");
	sub->AddOption<NumberOption<float>>("Air Drag Multiplier", nullptr, &g_MainScript->air_drag_multipler, 0.f, 10000.f, 1.f, 5.f);
	sub->AddOption<NumberOption<float>>("Speed Bump Severity Multiplier", nullptr, &g_MainScript->speed_bump_severity_multipler, 0.f, 10000.f, 1.f, 5.f);
	sub->AddOption<NumberOption<float>>("Cargobob Magnet Strength Multiplier", nullptr, &g_MainScript->cargobob_pickup_rope_damping_multipler, 0.f, 10000.f, 1.f, 5.f);
	sub->AddOption<NumberOption<float>>("Headlight Intensity Multiplier", nullptr, &g_MainScript->headlight_multipler, 0.f, 10000.f, 1.f, 5.f);
	sub->AddOption<NumberOption<float>>("Level Of Detail (LOD) Multiplier", nullptr, &g_MainScript->lod_multipler, 0.f, 10000.f, 1.f, 5.f);
}


const char* capitalize(const char* str)
{
	char* a = (char*)str;
	int len = strlen(a);
	for (int i = 0; i < len; ++i)
	{
		if (i == 0)
		{
			if (islower(a[i]))
				a[i] = toupper(a[i]);
		}
		else
		{
			if (a[i] != ' ')
			{
				if (isupper(a[i]))
					a[i] = tolower(a[i]);
			}
			else
			{
				i++;
				if (islower(a[i]))
					a[i] = toupper(a[i]);
			}
		}
	}
	return &a[0];
}

void addVehicleOption(const char* model_name, RegularSubmenu* sub)
{
	const char* test = model_name;

}
const char* getVehicleName(const char* model_name)
{
	return capitalize(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(Joaat(model_name)));
}
const char* getVehicleNameFromHash(Hash hash)
{
	return capitalize(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(hash));
}

void vehicleSubmenuSubs()
{
	g_UiManager->AddSubmenu<RegularSubmenu>("Spawner", Spawner, [](RegularSubmenu* sub)
	{
		sub->AddOption<BoolOption<bool>>("Spawn Maxed", nullptr, &g_VehicleManager->SpawnMaxed, BoolDisplay::OnOff, false, [] {});
		sub->AddOption<BoolOption<bool>>("Spawn Inside", nullptr, &g_VehicleManager->SpawnInVehicle, BoolDisplay::OnOff, false, [] {});
		if (g_VehicleManager->SpawnInVehicle)
		{
			sub->AddOption<BoolOption<bool>>("Spawn In Air", nullptr, &g_VehicleManager->SpawnInAir, BoolDisplay::OnOff);
			if (g_VehicleManager->SpawnInAir)
			{
				sub->AddOption<NumberOption<float>>("Height", nullptr, &g_VehicleManager->SpawnInAirZ, 0.f, 100.f, 1.f, 5.f);
			}
		}

		sub->AddOption<SubOption>("DLC", nullptr, VehicleDLC);
		sub->AddOption<SubOption>("Sports", nullptr, VehicleSports);
		sub->AddOption<SubOption>("Muscle", nullptr, VehicleMuscle);
		sub->AddOption<SubOption>("Super", nullptr, VehicleSuper);
		sub->AddOption<SubOption>("Sports Classics", nullptr, VehicleSportsClassics);
		sub->AddOption<SubOption>("Sedans", nullptr, VehicleSedans);
		sub->AddOption<SubOption>("SUVs", nullptr, VehicleSUVs);
		sub->AddOption<SubOption>("Compacts", nullptr, VehicleCompacts);
		sub->AddOption<SubOption>("Motorcycles", nullptr, VehicleMotorcycles);
		sub->AddOption<SubOption>("Off-Road", nullptr, VehicleOffRoad);
		sub->AddOption<SubOption>("Open Wheel", nullptr, VehicleOpenWheel);
		sub->AddOption<SubOption>("Cycles", nullptr, VehicleCycles);
		sub->AddOption<SubOption>("Vans", nullptr, VehicleVans);
		sub->AddOption<SubOption>("Utility", nullptr, VehicleUtility);
		sub->AddOption<SubOption>("Industrial", nullptr, VehicleIndustrial);
		sub->AddOption<SubOption>("Service", nullptr, VehicleService);
		sub->AddOption<SubOption>("Commercial", nullptr, VehicleCommercial);
		sub->AddOption<SubOption>("Emergency", nullptr, VehicleEmergency);
		sub->AddOption<SubOption>("Military", nullptr, VehicleMilitary);
		sub->AddOption<SubOption>("Boats", nullptr, VehicleBoats);
		sub->AddOption<SubOption>("Planes", nullptr, VehiclePlanes);
		sub->AddOption<SubOption>("Helicopters", nullptr, VehicleHelicopters);
	});

	g_UiManager->AddSubmenu<RegularSubmenu>("DLC", VehicleDLC, [=](RegularSubmenu* sub)
	{
		sub->AddOption<SubOption>("The Cayo Perico", nullptr, VehicleCayoPerico);
		sub->AddOption<SubOption>("Los Santos: Summer Special", nullptr, VehicleLosSantosSummerSpecial);
		sub->AddOption<SubOption>("The Diamond Casino Heist", nullptr, VehicleCasinoHeist);
		sub->AddOption<SubOption>("Arena Wars", nullptr, VehicleArenaWars);
		sub->AddOption<SubOption>("After Hours", nullptr, VehicleAfterHours);
		sub->AddOption<SubOption>("Southern SA", nullptr, VehicleSouthernSA);
		sub->AddOption<SubOption>("The Doomsday Heist", nullptr, VehicleDoomsday);
		sub->AddOption<SubOption>("Smuggler's Run", nullptr, VehicleSmugglers);
		sub->AddOption<SubOption>("Gunnrunning Pack", nullptr, VehicleGunrunning);
		sub->AddOption<SubOption>("Special Vehicle Circuit", nullptr, VehicleCircuit);
		sub->AddOption<SubOption>("Import/Export", nullptr, VehicleIE);
		sub->AddOption<SubOption>("Bikers DLC", nullptr, VehicleBikers);
		sub->AddOption<SubOption>("Cunning Stunts", nullptr, VehicleCunning);
		sub->AddOption<SubOption>("Futher Adventures", nullptr, VehicleAdventures);
		sub->AddOption<SubOption>("Lowriders Customs", nullptr, VehicleLowridersC);
		sub->AddOption<SubOption>("Be My Valentine", nullptr, VehicleBeMyValentine);
		sub->AddOption<SubOption>("January 2016", nullptr, VehicleJanuary2016);
		sub->AddOption<SubOption>("Festive Surprise 2015", nullptr, VehicleFestive2015);
		sub->AddOption<SubOption>("Executive And Other Criminals", nullptr, VehicleExecutive);
		sub->AddOption<SubOption>("Halloween Surprise", nullptr, VehicleHalloween);
		sub->AddOption<SubOption>("Lowriders", nullptr, VehicleLowriders);
		sub->AddOption<SubOption>("Gotten Gains Part 2", nullptr, VehicleGotten2);
		sub->AddOption<SubOption>("Gotten Gains Part 1", nullptr, VehicleGotten1);
		sub->AddOption<SubOption>("Heists DLC", nullptr, VehicleHeistsDLC);
		sub->AddOption<SubOption>("Festive Surprise 2014", nullptr, VehicleFestive2014);
		sub->AddOption<SubOption>("Next Gen DLC", nullptr, VehicleNextGen);
		sub->AddOption<SubOption>("Last Team Standing", nullptr, VehicleLLS);
		sub->AddOption<SubOption>("SA Flight School", nullptr, VehicleFlightSchool);
		sub->AddOption<SubOption>("Independence Day", nullptr, VehicleIndependence);
		sub->AddOption<SubOption>("I'm Not Hipster", nullptr, VehicleHipster);
		sub->AddOption<SubOption>("High Life", nullptr, VehicleHighLife);
		sub->AddOption<SubOption>("Business", nullptr, VehicleBusiness);
		sub->AddOption<SubOption>("Valentine's Day Massacre", nullptr, VehicleValentinesDay);
		sub->AddOption<SubOption>("Beach Bum", nullptr, VehicleBeachBum);
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("The Cayo Perico", VehicleCayoPerico, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Alkonost RO-86", "alkonost");
		sub->AddOption<VehicleOption>("Annihilator Stealth", "annihilator2");
		sub->AddOption<VehicleOption>("BF Weevil", "weevil");
		sub->AddOption<VehicleOption>("Dinka Verus", "verus");
		sub->AddOption<VehicleOption>("Dinka Veto Classic", "veto");
		sub->AddOption<VehicleOption>("Dinka Veto Modern", "veto2");
		sub->AddOption<VehicleOption>("Grotti Brioso 300", "brioso2");
		sub->AddOption<VehicleOption>("Grotti Itali RSX", "italirsx");
		sub->AddOption<VehicleOption>("Kosatka", "kosatka");
		sub->AddOption<VehicleOption>("Kraken Avisa", "avisa");
		sub->AddOption<VehicleOption>("Kurtz 31 Patrol Boat", "patrolboat");
		sub->AddOption<VehicleOption>("Maibatsu Manchez Scout", "manchez2");
		sub->AddOption<VehicleOption>("Mammoth Squaddie", "squaddie");
		sub->AddOption<VehicleOption>("Nagasaki Weaponized Dinghy", "dinghy5");
		sub->AddOption<VehicleOption>("Pegassi Toreador", "toreador");
		sub->AddOption<VehicleOption>("Shitzu Longfin", "longfin");
		sub->AddOption<VehicleOption>("Sparrow (update type)", "seasparrow2");
		sub->AddOption<VehicleOption>("Sparrow (update type)", "seasparrow3");
		sub->AddOption<VehicleOption>("Vapid Slamtruck", "slamtruck");
		sub->AddOption<VehicleOption>("Vapid Winky", "winky");
		sub->AddOption<VehicleOption>("Vetir", "vetir");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Los Santos Summer Special", VehicleLosSantosSummerSpecial, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("BF Club", "club");
		sub->AddOption<VehicleOption>("Benefactor BR8", "openwheel1");
		sub->AddOption<VehicleOption>("Beater Dukes", "dukes3");
		sub->AddOption<VehicleOption>("Coquette D10", "coquette4");
		sub->AddOption<VehicleOption>("Declasse DR1", "openwheel2");
		sub->AddOption<VehicleOption>("Gauntlet Classic Custom", "gauntlet5");
		sub->AddOption<VehicleOption>("Glendale Custom", "glendale2");
		sub->AddOption<VehicleOption>("Landstalker XS", "landstalker2");
		sub->AddOption<VehicleOption>("Manana Custom", "manana2");
		sub->AddOption<VehicleOption>("Penumbra FF", "penumbra2");
		sub->AddOption<VehicleOption>("Peyote Custom", "peyote3");
		sub->AddOption<VehicleOption>("Semilone Frontier", "semilone2");
		sub->AddOption<VehicleOption>("Tigon", "tigon");
		sub->AddOption<VehicleOption>("Yosemite Rancher", "yosemite3");
		sub->AddOption<VehicleOption>("Youga Classic 4x4", "youga3");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("The Diamond Casino Heist", VehicleCasinoHeist, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Asbo", "asbo");
		sub->AddOption<VehicleOption>("Drift Yosemite", "yasomite2");
		sub->AddOption<VehicleOption>("Everon", "everon");
		sub->AddOption<VehicleOption>("Furia", "furia");
		sub->AddOption<VehicleOption>("Imorgon", "imorgon");
		sub->AddOption<VehicleOption>("JB 7002", "jb7002");
		sub->AddOption<VehicleOption>("Komoda", "komoda");
		sub->AddOption<VehicleOption>("Outlaw", "outlaw");
		sub->AddOption<VehicleOption>("PR4", "formula");
		sub->AddOption<VehicleOption>("R88", "formula2");
		sub->AddOption<VehicleOption>("RC Tank", "minitank");
		sub->AddOption<VehicleOption>("Rebla", "rebla");
		sub->AddOption<VehicleOption>("Retinue MK2", "retinue2");
		sub->AddOption<VehicleOption>("Stryder", "stryder");
		sub->AddOption<VehicleOption>("Sugoi", "sugoi");
		sub->AddOption<VehicleOption>("Sultan Classic", "sultan2");
		sub->AddOption<VehicleOption>("V-STR", "vstr");
		sub->AddOption<VehicleOption>("Vagrant", "vagrant");
		sub->AddOption<VehicleOption>("Zhaba", "zhaba");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("The Diamond Casino & Resort", VehicleCasinoResort, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("8F Drafter", "drafter");
		sub->AddOption<VehicleOption>("Caracara 4x4", "caracara2");
		sub->AddOption<VehicleOption>("Caracara 4x4", "caracara2");
		sub->AddOption<VehicleOption>("Dynasty", "dynasty");
		sub->AddOption<VehicleOption>("Emerus", "emerus");
		sub->AddOption<VehicleOption>("Gauntlet Classic", "gauntlet3");
		sub->AddOption<VehicleOption>("Gauntlet Hellfire", "gauntlet4");
		sub->AddOption<VehicleOption>("Hellion", "hellion");
		sub->AddOption<VehicleOption>("Issi Sport", "issi7");
		sub->AddOption<VehicleOption>("Jagular", "jagular");
		sub->AddOption<VehicleOption>("Krieger", "krieger");
		sub->AddOption<VehicleOption>("Locust", "locust");
		sub->AddOption<VehicleOption>("Nebula Turbo", "nebula");
		sub->AddOption<VehicleOption>("Neo", "neo");
		sub->AddOption<VehicleOption>("Novak", "novak");
		sub->AddOption<VehicleOption>("Paragon R (Armored)", "paragon2");
		sub->AddOption<VehicleOption>("Paragon R", "paragon");
		sub->AddOption<VehicleOption>("Peyote Gasser", "peyote2");
		sub->AddOption<VehicleOption>("Rampant Rocket Tricycle", "rrocket");
		sub->AddOption<VehicleOption>("S80RR", "s80");
		sub->AddOption<VehicleOption>("Thrax", "thrax");
		sub->AddOption<VehicleOption>("Zion Classic", "zion3");
		sub->AddOption<VehicleOption>("Zorusso", "zorusso");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Arena Wars", VehicleArenaWars, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Apocalypse Bruiser", "Bruiser");
		sub->AddOption<VehicleOption>("Apocalypse Brutus", "Brutus");
		sub->AddOption<VehicleOption>("Apocalypse Cerberus", "Cerberus");
		sub->AddOption<VehicleOption>("Apocalypse Deathbike", "Deathbike");
		sub->AddOption<VehicleOption>("Apocalypse Dominator", "Dominator");
		sub->AddOption<VehicleOption>("Apocalypse Impaler", "Impaler2");
		sub->AddOption<VehicleOption>("Apocalypse Imperator", "Imperator");
		sub->AddOption<VehicleOption>("Apocalypse Issi", "Issi4");
		sub->AddOption<VehicleOption>("Apocalypse Sasquatch", "Sasquatch");
		sub->AddOption<VehicleOption>("Apocalypse Scarab", "Scarab");
		sub->AddOption<VehicleOption>("Apocalypse Slamvan", "Slamvan4");
		sub->AddOption<VehicleOption>("Apocalypse ZR 380", "ZR380");
		sub->AddOption<VehicleOption>("Clique", "Clique");
		sub->AddOption<VehicleOption>("Deveste", "Deveste");
		sub->AddOption<VehicleOption>("Deviant", "Deviant");
		sub->AddOption<VehicleOption>("Future Shock Bruiser", "Bruiser2");
		sub->AddOption<VehicleOption>("Future Shock Brutus", "Brutus2");
		sub->AddOption<VehicleOption>("Future Shock Cerberus", "Cerberus2");
		sub->AddOption<VehicleOption>("Future Shock Cerberus", "Cerberus2");
		sub->AddOption<VehicleOption>("Future Shock Deathbike", "Deathbike2");
		sub->AddOption<VehicleOption>("Future Shock Dominator", "Dominator2");
		sub->AddOption<VehicleOption>("Future Shock Impaler", "Impaler3");
		sub->AddOption<VehicleOption>("Future Shock Imperator", "Imperator2");
		sub->AddOption<VehicleOption>("Future Shock Issi", "Issi5");
		sub->AddOption<VehicleOption>("Future Shock Sasquatch", "Sasquatch2");
		sub->AddOption<VehicleOption>("Future Shock Scarab", "Scarab2");
		sub->AddOption<VehicleOption>("Future Shock Slamvan", "Slamvan5");
		sub->AddOption<VehicleOption>("Future Shock ZR 380", "ZR3802");
		sub->AddOption<VehicleOption>("GT500", "GT500");
		sub->AddOption<VehicleOption>("Impaler", "Impaler");
		sub->AddOption<VehicleOption>("Itali GTO", "GTO");
		sub->AddOption<VehicleOption>("Nightmare Bruiser", "Bruiser3");
		sub->AddOption<VehicleOption>("Nightmare Brutus", "Brutus3");
		sub->AddOption<VehicleOption>("Nightmare Cerberus", "Cerberus");
		sub->AddOption<VehicleOption>("Nightmare Cerberus", "Cerberus3");
		sub->AddOption<VehicleOption>("Nightmare Deathbike", "Deathbike3");
		sub->AddOption<VehicleOption>("Nightmare Dominator", "Dominator3");
		sub->AddOption<VehicleOption>("Nightmare Impaler", "Impaler4");
		sub->AddOption<VehicleOption>("Nightmare Imperator", "Imperator3");
		sub->AddOption<VehicleOption>("Nightmare Issi", "Issi6");
		sub->AddOption<VehicleOption>("Nightmare Sasquatch", "Sasquatch3");
		sub->AddOption<VehicleOption>("Nightmare Scarab", "Scarab3");
		sub->AddOption<VehicleOption>("Nightmare Slamvan", "Slamvan6");
		sub->AddOption<VehicleOption>("Nightmare ZR 380", "ZR3803");
		sub->AddOption<VehicleOption>("RC Bandito", "Bandito");
		sub->AddOption<VehicleOption>("Toros", "Toros");
		sub->AddOption<VehicleOption>("Tulip", "Tulip");
		sub->AddOption<VehicleOption>("Vamos", "Vamos");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("After Hours", VehicleAfterHours, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Blimp", "blimp3");
		sub->AddOption<VehicleOption>("Festival Bus", "pbus2");
		sub->AddOption<VehicleOption>("Freecrawler", "freecrawler");
		sub->AddOption<VehicleOption>("Menacer", "menacer");
		sub->AddOption<VehicleOption>("Mule Custom", "mule4");
		sub->AddOption<VehicleOption>("Opressor MK2", "oppressor2");
		sub->AddOption<VehicleOption>("Patriot Stretch", "patriot2");
		sub->AddOption<VehicleOption>("Pounder Custom", "pounder2");
		sub->AddOption<VehicleOption>("Scramjet", "scramjet");
		sub->AddOption<VehicleOption>("Speedo Custom", "speedo4");
		sub->AddOption<VehicleOption>("Stafford", "stafford");
		sub->AddOption<VehicleOption>("Strikeforce", "strikeforce");
		sub->AddOption<VehicleOption>("Swinger", "swinger");
		sub->AddOption<VehicleOption>("Terrorbyte", "terbyte");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Southern SA", VehicleSouthernSA, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Caracara", "caracara");
		sub->AddOption<VehicleOption>("Cheburek", "cheburek");
		sub->AddOption<VehicleOption>("Dominator GTX", "dominator3");
		sub->AddOption<VehicleOption>("Ellie", "ellie");
		sub->AddOption<VehicleOption>("Entity XXR", "entity2");
		sub->AddOption<VehicleOption>("Fagaloa", "fagaloa");
		sub->AddOption<VehicleOption>("Flash GT", "flashgt");
		sub->AddOption<VehicleOption>("GB200", "gb200");
		sub->AddOption<VehicleOption>("Hotring Sabre", "hotring");
		sub->AddOption<VehicleOption>("Issi Classic", "issi3");
		sub->AddOption<VehicleOption>("Michelli", "michelli");
		sub->AddOption<VehicleOption>("Sea Sparrow", "seasparrow");
		sub->AddOption<VehicleOption>("Taipan", "taipan");
		sub->AddOption<VehicleOption>("Tezeract", "tezeract");
		sub->AddOption<VehicleOption>("Tyrant", "tyrant");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("The Doomsday Heist", VehicleDoomsday, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Akula", "akula");
		sub->AddOption<VehicleOption>("Autarch", "autarch");
		sub->AddOption<VehicleOption>("Avenger", "avenger");
		sub->AddOption<VehicleOption>("Barrage", "barrage");
		sub->AddOption<VehicleOption>("Chernobog", "chernobog");
		sub->AddOption<VehicleOption>("Comet SR", "comet4");
		sub->AddOption<VehicleOption>("Comet Safari", "comet3");
		sub->AddOption<VehicleOption>("Deluxo", "deluxo");
		sub->AddOption<VehicleOption>("GT500", "gt500");
		sub->AddOption<VehicleOption>("Hermes", "hermes");
		sub->AddOption<VehicleOption>("Hustler", "hustler");
		sub->AddOption<VehicleOption>("Kamacho", "kamacho");
		sub->AddOption<VehicleOption>("Neon", "neon");
		sub->AddOption<VehicleOption>("Pariah", "pariah");
		sub->AddOption<VehicleOption>("Raiden", "raiden");
		sub->AddOption<VehicleOption>("Revolter", "revolter");
		sub->AddOption<VehicleOption>("Riata", "riata");
		sub->AddOption<VehicleOption>("Riot", "riot2");
		sub->AddOption<VehicleOption>("SC1", "sc1");
		sub->AddOption<VehicleOption>("Savestra", "savestra");
		sub->AddOption<VehicleOption>("Sentinel Classic", "sentinel3");
		sub->AddOption<VehicleOption>("Streiter", "streiter");
		sub->AddOption<VehicleOption>("Stromberg", "stromberg");
		sub->AddOption<VehicleOption>("TM-02 Khanjali Tank", "khanjali");
		sub->AddOption<VehicleOption>("Thruster", "thruster");
		sub->AddOption<VehicleOption>("Viseris", "viseris");
		sub->AddOption<VehicleOption>("Volatol", "volatol");
		sub->AddOption<VehicleOption>("Yosemite", "yosemite");
		sub->AddOption<VehicleOption>("190Z", "z190");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Smuggler's Run", VehicleSmugglers, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Alpha-Z1", "ALPHAZ1");
		sub->AddOption<VehicleOption>("Bombushka", "BOMBUSHKA");
		sub->AddOption<VehicleOption>("Cyclone", "CYCLONE");
		sub->AddOption<VehicleOption>("Havok", "HAVOK");
		sub->AddOption<VehicleOption>("Howard", "HOWARD");
		sub->AddOption<VehicleOption>("Hunter", "HUNTER");
		sub->AddOption<VehicleOption>("Microlight", "MICROLIGHT");
		sub->AddOption<VehicleOption>("Mogul", "MOGUL");
		sub->AddOption<VehicleOption>("Molotok", "MOLOTOK");
		sub->AddOption<VehicleOption>("Nokota", "NOKOTA");
		sub->AddOption<VehicleOption>("Pyro", "PYRO");
		sub->AddOption<VehicleOption>("Rapid GT Classic", "RAPIDGT3");
		sub->AddOption<VehicleOption>("Retinue", "RETINUE");
		sub->AddOption<VehicleOption>("Rogue", "ROGUE");
		sub->AddOption<VehicleOption>("Seabreeze", "SEABREEZE");
		sub->AddOption<VehicleOption>("Starling", "STARLING");
		sub->AddOption<VehicleOption>("Tula", "TULA");
		sub->AddOption<VehicleOption>("Vigilante", "VIGILANTE");
		sub->AddOption<VehicleOption>("Visione", "VISIONE");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Gunnrunning Pack", VehicleGunrunning, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("APC Tank", "apc");
		sub->AddOption<VehicleOption>("Anti-Aircraft Trailer", "trailers4");
		sub->AddOption<VehicleOption>("Ardent", "ardent");
		sub->AddOption<VehicleOption>("Caddy (Bunker)", "caddy3");
		sub->AddOption<VehicleOption>("Cheetah Classic", "cheetah2");
		sub->AddOption<VehicleOption>("Dune FAV", "dune3");
		sub->AddOption<VehicleOption>("Halftrack", "halftrack");
		sub->AddOption<VehicleOption>("Hauler Custom", "hauler2");
		sub->AddOption<VehicleOption>("Insurgent Pick-Up Custom", "insurgent3");
		sub->AddOption<VehicleOption>("Mobile Operation Center", "trailerlarge");
		sub->AddOption<VehicleOption>("Nightshark", "nightshark");
		sub->AddOption<VehicleOption>("Oppressor", "oppressor");
		sub->AddOption<VehicleOption>("Phantom Custom", "phantom3");
		sub->AddOption<VehicleOption>("Torero", "torero");
		sub->AddOption<VehicleOption>("Vagner", "vagner");
		sub->AddOption<VehicleOption>("Weaponized Tampa", "tampa3");
		sub->AddOption<VehicleOption>("XA-21", "xa21");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Special Vehicle Circuit", VehicleCircuit, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("GP1", "gp1");
		sub->AddOption<VehicleOption>("Infernus Classic", "infernus2");
		sub->AddOption<VehicleOption>("Ruston", "ruston");
		sub->AddOption<VehicleOption>("Turismo R", "turismor");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Import/Export", VehicleIE, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Blazer Aqua", "blazer5");
		sub->AddOption<VehicleOption>("Boxville (Armored)", "boxville5");
		sub->AddOption<VehicleOption>("Comet Retro Custom", "comet3");
		sub->AddOption<VehicleOption>("Diabolus Custom", "diabolus2");
		sub->AddOption<VehicleOption>("Diabolus", "diabolus");
		sub->AddOption<VehicleOption>("Elegy Retro Custom", "elegy2");
		sub->AddOption<VehicleOption>("FCR 1000 Custom", "fcr2");
		sub->AddOption<VehicleOption>("FCR 1000", "fcr");
		sub->AddOption<VehicleOption>("Itali GTB Custom", "italigtb2");
		sub->AddOption<VehicleOption>("Itali GTB", "italigtb");
		sub->AddOption<VehicleOption>("Nero Custom", "nero2");
		sub->AddOption<VehicleOption>("Nero", "nero");
		sub->AddOption<VehicleOption>("Penetrator", "penetrator");
		sub->AddOption<VehicleOption>("Phantom Wedge", "phantom2");
		sub->AddOption<VehicleOption>("Ramp Buggy", "dune5");
		sub->AddOption<VehicleOption>("Rocket Voltic", "voltic2");
		sub->AddOption<VehicleOption>("Ruiner 2000", "ruiner2");
		sub->AddOption<VehicleOption>("Specter Custom", "specter2");
		sub->AddOption<VehicleOption>("Specter", "specter");
		sub->AddOption<VehicleOption>("Technical Aqua", "technical2");
		sub->AddOption<VehicleOption>("Tempesta", "tempesta");
		sub->AddOption<VehicleOption>("Wastelander", "wastelander");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Bikers DLC", VehicleBikers, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Avarus", "avarus");
		sub->AddOption<VehicleOption>("Chimera", "chimera");
		sub->AddOption<VehicleOption>("Daemon Custom", "daemon2");
		sub->AddOption<VehicleOption>("Defiler", "defiler");
		sub->AddOption<VehicleOption>("Esskey", "esskey");
		sub->AddOption<VehicleOption>("Faggio Mod", "faggio2");
		sub->AddOption<VehicleOption>("Faggio Sport", "faggio3");
		sub->AddOption<VehicleOption>("Hakuchou Drag Bike", "hakuchou2");
		sub->AddOption<VehicleOption>("Manchez", "manchez");
		sub->AddOption<VehicleOption>("Nightblade", "nightblade");
		sub->AddOption<VehicleOption>("Raptor", "raptor");
		sub->AddOption<VehicleOption>("Ratbike", "ratbike");
		sub->AddOption<VehicleOption>("Sanctus", "sanctus");
		sub->AddOption<VehicleOption>("Shotaro", "shotaro");
		sub->AddOption<VehicleOption>("Street Blazer", "blazer4");
		sub->AddOption<VehicleOption>("Tornado Rat Rod", "tornado6");
		sub->AddOption<VehicleOption>("Vortex", "vortex");
		sub->AddOption<VehicleOption>("Wolfsbane", "wolfsbane");
		sub->AddOption<VehicleOption>("Youga Classic", "youga2");
		sub->AddOption<VehicleOption>("Zombie Bobber", "zombiea");
		sub->AddOption<VehicleOption>("Zombie Chopper", "zombieb");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Cunning Stunts", VehicleCunning, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("BF400", "bf400");
		sub->AddOption<VehicleOption>("Brioso", "brioso");
		sub->AddOption<VehicleOption>("Cliffhanger", "cliffhanger");
		sub->AddOption<VehicleOption>("Contender", "contender");
		sub->AddOption<VehicleOption>("Drift Tampa", "tampa2");
		sub->AddOption<VehicleOption>("Gargoyle", "gargoyle");
		sub->AddOption<VehicleOption>("Lynx", "lynx");
		sub->AddOption<VehicleOption>("Omnis", "omnis");
		sub->AddOption<VehicleOption>("RE-7B", "re7b");
		sub->AddOption<VehicleOption>("Rallytruck", "rallytruck");
		sub->AddOption<VehicleOption>("Sheava", "sheava");
		sub->AddOption<VehicleOption>("Trophy Truck Custom", "trophytruck");
		sub->AddOption<VehicleOption>("Trophy Truck", "trophytruck2");
		sub->AddOption<VehicleOption>("Tropos Rallye", "tropos");
		sub->AddOption<VehicleOption>("Tyrus", "tyrus");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Futher Adventures", VehicleAdventures, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Bestia GTS", "bestiagts");
		sub->AddOption<VehicleOption>("Brickade", "brickade");
		sub->AddOption<VehicleOption>("FMJ", "fmj");
		sub->AddOption<VehicleOption>("Nimbus", "nimbus");
		sub->AddOption<VehicleOption>("Pfister 811", "pfister811");
		sub->AddOption<VehicleOption>("Reaper", "reaper");
		sub->AddOption<VehicleOption>("Rumpo Custom", "rumpo3");
		sub->AddOption<VehicleOption>("Seven-70", "seven70");
		sub->AddOption<VehicleOption>("Tug", "tug");
		sub->AddOption<VehicleOption>("Volatus", "volatus");
		sub->AddOption<VehicleOption>("Windsor Drop", "windsor2");
		sub->AddOption<VehicleOption>("X80 Proto", "prototipo");
		sub->AddOption<VehicleOption>("XLS (Armored)", "xls2");
		sub->AddOption<VehicleOption>("XLS", "xls");
		sub->AddOption<VehicleOption>("Yosemite", "yosemite");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Lowriders Customs", VehicleLowridersC, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Faction Custom Donk", "faction3");
		sub->AddOption<VehicleOption>("Minivan Custom", "minivan2");
		sub->AddOption<VehicleOption>("Sabre Turbo Custom", "sabregt2");
		sub->AddOption<VehicleOption>("Slamvan Custom", "slamvan3");
		sub->AddOption<VehicleOption>("Tornado Custom", "tornado2");
		sub->AddOption<VehicleOption>("Virgo Classic Custom", "virgo3");
		sub->AddOption<VehicleOption>("Virgo Classic", "virgo2");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Be My Valentine", VehicleBeMyValentine, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Roosevelt Valor", "btype3");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("January 2016", VehicleJanuary2016, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Banshee 900R", "banshee2");
		sub->AddOption<VehicleOption>("Sultan RS", "sultanrs");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Festive Surprise 2015", VehicleFestive2015, [=](RegularSubmenu* sub)
		{
			sub->AddOption<VehicleOption>("Tampa", "tampa");
		});
	g_UiManager->AddSubmenu<RegularSubmenu>("Executive And Other Criminals", VehicleExecutive, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Baller LE (Armored)", "baller5");
		sub->AddOption<VehicleOption>("Baller LE LWB (Armored)", "baller6");
		sub->AddOption<VehicleOption>("Baller LE LWB", "baller4");
		sub->AddOption<VehicleOption>("Baller LE", "baller3");
		sub->AddOption<VehicleOption>("Cognoscenti (Armored)", "cognoscenti2");
		sub->AddOption<VehicleOption>("Cognoscenti 55 (Armored)", "cog552");
		sub->AddOption<VehicleOption>("Cognoscenti 55", "cog55");
		sub->AddOption<VehicleOption>("Cognoscenti", "cognoscenti");
		sub->AddOption<VehicleOption>("Mamba", "mamba");
		sub->AddOption<VehicleOption>("Nightshade", "nightshade");
		sub->AddOption<VehicleOption>("Schafter LWB (Armored)", "schafter6");
		sub->AddOption<VehicleOption>("Schafter LWB", "schafter4");
		sub->AddOption<VehicleOption>("Schafter V12 (Armored)", "schafter5");
		sub->AddOption<VehicleOption>("Schafter V12", "schafter3");
		sub->AddOption<VehicleOption>("SuperVolito Carbon", "supervolito2");
		sub->AddOption<VehicleOption>("SuperVolito", "supervolito");
		sub->AddOption<VehicleOption>("Turreted Limo", "limo2");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Halloween Surprise", VehicleHalloween, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Lurcher", "lurcher");
		sub->AddOption<VehicleOption>("Roosevelt Valor", "btype2");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Lowriders", VehicleLowriders, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Buccaneer Custom", "buccaneer");
		sub->AddOption<VehicleOption>("Chino Custom", "chino2");
		sub->AddOption<VehicleOption>("Faction Custom", "faction2");
		sub->AddOption<VehicleOption>("Faction", "faction");
		sub->AddOption<VehicleOption>("Moonbeam Custom", "moonbeam2");
		sub->AddOption<VehicleOption>("Moonbeam", "moonbeam");
		sub->AddOption<VehicleOption>("Primo Custom", "primo2");
		sub->AddOption<VehicleOption>("Voodoo ", "voodoo");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Gotten Gains Part 2", VehicleGotten2, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Brawler", "brawler");
		sub->AddOption<VehicleOption>("Chino", "chino");
		sub->AddOption<VehicleOption>("Coquette BlackFin", "coquette");
		sub->AddOption<VehicleOption>("T20", "t20");
		sub->AddOption<VehicleOption>("Toro", "toro");
		sub->AddOption<VehicleOption>("Vindicator", "vindicator");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Gotten Gains Part 1", VehicleGotten1, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Luxor Deluxe", "luxor2");
		sub->AddOption<VehicleOption>("Osiris", "osiris");
		sub->AddOption<VehicleOption>("Stirling GT", "feltzer3");
		sub->AddOption<VehicleOption>("Swift Deluxe", "swift2");
		sub->AddOption<VehicleOption>("Virgo", "virgo");
		sub->AddOption<VehicleOption>("Windsor", "windsor");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Heists DLC", VehicleHeistsDLC, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Casco", "casco");
		sub->AddOption<VehicleOption>("Enduro", "enduro");
		sub->AddOption<VehicleOption>("Gang Burrito", "mule3");
		sub->AddOption<VehicleOption>("Guardian", "guardian");
		sub->AddOption<VehicleOption>("Hydra", "hydra");
		sub->AddOption<VehicleOption>("Insurgent Pick-Up", "insurgent2");
		sub->AddOption<VehicleOption>("Insurgent", "insurgent");
		sub->AddOption<VehicleOption>("Kuruma (Armored)", "kuruma");
		sub->AddOption<VehicleOption>("Kuruma", "kuruma2");
		sub->AddOption<VehicleOption>("Lectro", "lectro");
		sub->AddOption<VehicleOption>("Primo Custom", "hydra");
		sub->AddOption<VehicleOption>("Savage", "savage");
		sub->AddOption<VehicleOption>("Technical", "technical");
		sub->AddOption<VehicleOption>("Valkyrie", "Valkyrie");
		sub->AddOption<VehicleOption>("Velum 5-Seater", "velum2");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Festive Surprise 2014", VehicleFestive2014, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Rat Truck", "ratloader2");
		sub->AddOption<VehicleOption>("Slamvan", "slamvan");
		sub->AddOption<VehicleOption>("Jester (Racercar)", "jester2");
		sub->AddOption<VehicleOption>("Massacro (Racercar)", "massacro2");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Next Gen DLC", VehicleNextGen, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Blista Compact", "blista2");
		sub->AddOption<VehicleOption>("Burger Shot Stallion", "stallion2");
		sub->AddOption<VehicleOption>("Dodo", "dodo");
		sub->AddOption<VehicleOption>("Duke O'Death", "dukes2");
		sub->AddOption<VehicleOption>("Dukes", "dukes");
		sub->AddOption<VehicleOption>("Go Go Monkey Blista", "blista3");
		sub->AddOption<VehicleOption>("Kraken", "kraken");
		sub->AddOption<VehicleOption>("Marshall", "marshall");
		sub->AddOption<VehicleOption>("Pi�wasser Dominator", "dominator2");
		sub->AddOption<VehicleOption>("Redwood Gauntlet", "gauntlet2");
		sub->AddOption<VehicleOption>("Sprunk Buffalo", "buffalo3");
		sub->AddOption<VehicleOption>("Stallion", "stallion");
		sub->AddOption<VehicleOption>("Xero Blimp", "blimp2");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Last Team Standing", VehicleLLS, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Furore GT", "furoregt");
		sub->AddOption<VehicleOption>("Hakuchou", "hakuchou");
		sub->AddOption<VehicleOption>("Innovation", "innovation");
		sub->AddOption<VehicleOption>("Kalahari (Topless)", "kalakari2");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("SA Flight School", VehicleFlightSchool, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Bestra", "besra");
		sub->AddOption<VehicleOption>("Coquette Classic (Topless)", "coquette3");
		sub->AddOption<VehicleOption>("Coquette Classic", "coquette2");
		sub->AddOption<VehicleOption>("Miljet", "miljet");
		sub->AddOption<VehicleOption>("Miljet", "miljet");
		sub->AddOption<VehicleOption>("Swift", "swift");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Independence Day", VehicleIndependence, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Liberator", "monster");
		sub->AddOption<VehicleOption>("Sovereign", "sovereign");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("I'm Not Hipster", VehicleHipster, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Blade", "blade");
		sub->AddOption<VehicleOption>("Dubsta 8x8", "dubsta3");
		sub->AddOption<VehicleOption>("Glendale", "glendale");
		sub->AddOption<VehicleOption>("Panto", "panto");
		sub->AddOption<VehicleOption>("Phapsody", "phapsody");
		sub->AddOption<VehicleOption>("Pigalle", "pigalle");
		sub->AddOption<VehicleOption>("Warrener", "warrener");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("High Life", VehicleHighLife, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Huntley S", "huntleys");
		sub->AddOption<VehicleOption>("Massacro", "massacro");
		sub->AddOption<VehicleOption>("Thrust", "thrust");
		sub->AddOption<VehicleOption>("Zentoro", "zentoro");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Business", VehicleBusiness, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Alpha", "alpha");
		sub->AddOption<VehicleOption>("Jester", "jester");
		sub->AddOption<VehicleOption>("Vestra", "vestra");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Valentine's Day Massacre", VehicleValentinesDay, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Roosevelt", "btype");
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Beach Bum", VehicleBeachBum, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Bifta", "bifta");
		sub->AddOption<VehicleOption>("Kalahari", "kalahari");
		sub->AddOption<VehicleOption>("Paradise", "paradise");
	});

	g_UiManager->AddSubmenu<RegularSubmenu>("Commercial", VehicleCommercial, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Apocalypse Cerberus", nullptr, [&] { create_vehicle("�cerberus"); });
		sub->AddOption<RegularOption>("Benson", nullptr, [&] { create_vehicle("�benson"); });
		sub->AddOption<RegularOption>("Biff", nullptr, [&] { create_vehicle("�biff"); });
		sub->AddOption<RegularOption>("Future Shock Cerberus", nullptr, [&] { create_vehicle("�cerberus2"); });
		sub->AddOption<RegularOption>("Hauler", nullptr, [&] { create_vehicle("�hauler"); });
		sub->AddOption<RegularOption>("Hauler Custom", nullptr, [&] { create_vehicle("�hauler2"); });
		sub->AddOption<RegularOption>("Mule", nullptr, [&] { create_vehicle("�mule"); });
		sub->AddOption<RegularOption>("Mule Custom", nullptr, [&] { create_vehicle("�mule4"); });
		sub->AddOption<RegularOption>("Nightmare Cerberus", nullptr, [&] { create_vehicle("�cerberus3"); });
		sub->AddOption<RegularOption>("Packer", nullptr, [&] { create_vehicle("�packer"); });
		sub->AddOption<RegularOption>("Phantom", nullptr, [&] { create_vehicle("�phantom"); });
		sub->AddOption<RegularOption>("Phantom Custom", nullptr, [&] { create_vehicle("�phantom3"); });
		sub->AddOption<RegularOption>("Phantom Wedge", nullptr, [&] { create_vehicle("�phantom2"); });
		sub->AddOption<RegularOption>("Pounder", nullptr, [&] { create_vehicle("�pounder"); });
		sub->AddOption<RegularOption>("Pounder Custom", nullptr, [&] { create_vehicle("�pounder2"); });
		sub->AddOption<RegularOption>("Terrorbyte", nullptr, [&] { create_vehicle("��terbyte"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Service", VehicleService, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Airport Bus", nullptr, [&] { create_vehicle("airbus"); });
		sub->AddOption<RegularOption>("Brickade", nullptr, [&] { create_vehicle("�brickade"); });
		sub->AddOption<RegularOption>("Bus", nullptr, [&] { create_vehicle("bus"); });
		sub->AddOption<RegularOption>("Dashound", nullptr, [&] { create_vehicle("dashound"); });
		sub->AddOption<RegularOption>("Dune", nullptr, [&] { create_vehicle("�rallytruck"); });
		sub->AddOption<RegularOption>("Festival Bus", nullptr, [&] { create_vehicle("pbus2"); });
		sub->AddOption<RegularOption>("Rental Shuttle Bus", nullptr, [&] { create_vehicle("rentalbus"); });
		sub->AddOption<RegularOption>("Taxi", nullptr, [&] { create_vehicle("taxi"); });
		sub->AddOption<RegularOption>("Tourbus", nullptr, [&] { create_vehicle("tourbus"); });
		sub->AddOption<RegularOption>("Trashmaster", nullptr, [&] { create_vehicle("trash"); });
		sub->AddOption<RegularOption>("Wastelander", nullptr, [&] { create_vehicle("wastelander"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Industrial", VehicleIndustrial, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Bulldozer", nullptr, [&] { create_vehicle("dozer"); });
		sub->AddOption<RegularOption>("Cutter", nullptr, [&] { create_vehicle("cutter"); });
		sub->AddOption<RegularOption>("Dock Handler", nullptr, [&] { create_vehicle("handler"); });
		sub->AddOption<RegularOption>("Dump Truck", nullptr, [&] { create_vehicle("dump"); });
		sub->AddOption<RegularOption>("Flatbed", nullptr, [&] { create_vehicle("flatbed"); });
		sub->AddOption<RegularOption>("Guardian", nullptr, [&] { create_vehicle("guardian"); });
		sub->AddOption<RegularOption>("Mixer", nullptr, [&] { create_vehicle("mixer"); });
		sub->AddOption<RegularOption>("Rubble", nullptr, [&] { create_vehicle("rubble"); });
		sub->AddOption<RegularOption>("Tipper", nullptr, [&] { create_vehicle("tiptruck"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Utility", VehicleUtility, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Airtug", nullptr, [&] { create_vehicle("airtug"); });
		sub->AddOption<RegularOption>("Caddy", nullptr, [&] { create_vehicle("caddy"); });
		sub->AddOption<RegularOption>("Docktug", nullptr, [&] { create_vehicle("docktug"); });
		sub->AddOption<RegularOption>("Fieldmaster", nullptr, [&] { create_vehicle("tractor2"); });
		sub->AddOption<RegularOption>("Fieldmaster (Snow)", nullptr, [&] { create_vehicle("tractor3"); });
		sub->AddOption<RegularOption>("Forklift", nullptr, [&] { create_vehicle("forklift"); });
		sub->AddOption<RegularOption>("Lawn Mower", nullptr, [&] { create_vehicle("mower"); });
		sub->AddOption<RegularOption>("Ripley", nullptr, [&] { create_vehicle("ripley"); });
		sub->AddOption<RegularOption>("Sadler", nullptr, [&] { create_vehicle("sadler"); });
		sub->AddOption<RegularOption>("Sadler (Snow)", nullptr, [&] { create_vehicle("sadler2"); });
		sub->AddOption<RegularOption>("Scrap Truck", nullptr, [&] { create_vehicle("scrap"); });
		sub->AddOption<RegularOption>("Towtruck", nullptr, [&] { create_vehicle("towtruck2"); });
		sub->AddOption<RegularOption>("Tractor", nullptr, [&] { create_vehicle("tractor"); });
		sub->AddOption<RegularOption>("Utility Truck", nullptr, [&] { create_vehicle("�utillitruck"); });
		sub->AddOption<BreakOption>("Trailers", nullptr);
		sub->AddOption<RegularOption>("Army Trailer", nullptr, [&] { create_vehicle("�armytrailer"); });
		sub->AddOption<RegularOption>("Baletrailer", nullptr, [&] { create_vehicle("baletrailer"); });
		sub->AddOption<RegularOption>("Boat Trailer", nullptr, [&] { create_vehicle("boattrailer"); });
		sub->AddOption<RegularOption>("Dock Trailer", nullptr, [&] { create_vehicle("docktrailer"); });
		sub->AddOption<RegularOption>("Graintrailer", nullptr, [&] { create_vehicle("graintrailer"); });
		sub->AddOption<RegularOption>("Mobile Operations Center", nullptr, [&] { create_vehicle("trailerlarge"); });
		sub->AddOption<RegularOption>("Trailer", nullptr, [&] { create_vehicle("tr3"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Emergency", VehicleEmergency, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Ambulance", nullptr, [&] { create_vehicle("Ambulance"); });
		sub->AddOption<RegularOption>("FBI", nullptr, [&] { create_vehicle("FBI"); });
		sub->AddOption<RegularOption>("Police", nullptr, [&] { create_vehicle("Police"); });
		sub->AddOption<RegularOption>("Predator", nullptr, [&] { create_vehicle("Predator"); });
		sub->AddOption<RegularOption>("Sheriff", nullptr, [&] { create_vehicle("Sheriff"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Military", VehicleMilitary, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("APC", nullptr, [&] { create_vehicle("APC"); });
		sub->AddOption<RegularOption>("Barracks", nullptr, [&] { create_vehicle("Barracks"); });
		sub->AddOption<RegularOption>("Crusader", nullptr, [&] { create_vehicle("Crusader"); });
		sub->AddOption<RegularOption>("Halftrack", nullptr, [&] { create_vehicle("Halftrack"); });
		sub->AddOption<RegularOption>("Rhino", nullptr, [&] { create_vehicle("Rhino"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Boats", VehicleBoats, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Dinghy", nullptr, [&] { create_vehicle("Dinghy"); });
		sub->AddOption<RegularOption>("Jetmax", nullptr, [&] { create_vehicle("Jetmax"); });
		sub->AddOption<VehicleOption>("Kurtz 31 Patrol Boat", "patrolboat");
		sub->AddOption<VehicleOption>("Kosatka", "kosatka");
		sub->AddOption<VehicleOption>("Kraken Avisa", "avisa");
		sub->AddOption<RegularOption>("Marquis", nullptr, [&] { create_vehicle("Marquis"); });
		sub->AddOption<RegularOption>("Seashark", nullptr, [&] { create_vehicle("Seashark"); });
		sub->AddOption<RegularOption>("Speeder", nullptr, [&] { create_vehicle("Speeder"); });
		sub->AddOption<RegularOption>("Squalo", nullptr, [&] { create_vehicle("Squalo"); });
		sub->AddOption<RegularOption>("Submersible", nullptr, [&] { create_vehicle("Submersible"); });
		sub->AddOption<RegularOption>("Suntrap", nullptr, [&] { create_vehicle("Suntrap"); });
		sub->AddOption<RegularOption>("Toro", nullptr, [&] { create_vehicle("Toro"); });
		sub->AddOption<RegularOption>("Tropic", nullptr, [&] { create_vehicle("Tropic"); });
		sub->AddOption<RegularOption>("Tug", nullptr, [&] { create_vehicle("Tug"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Planes", VehiclePlanes, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Alkonost RO-86", "alkonost");
		sub->AddOption<RegularOption>("Besra", nullptr, [&] { create_vehicle("Besra"); });
		sub->AddOption<RegularOption>("Blimp", nullptr, [&] { create_vehicle("Blimp"); });
		sub->AddOption<RegularOption>("Cargo Plane", nullptr, [&] { create_vehicle("CargoPlane"); });
		sub->AddOption<RegularOption>("Cuban 800", nullptr, [&] { create_vehicle("Cuban800"); });
		sub->AddOption<RegularOption>("Dodo", nullptr, [&] { create_vehicle("Dodo"); });
		sub->AddOption<RegularOption>("Duster", nullptr, [&] { create_vehicle("Duster"); });
		sub->AddOption<RegularOption>("Hydra", nullptr, [&] { create_vehicle("Hydra"); });
		sub->AddOption<RegularOption>("Jet", nullptr, [&] { create_vehicle("Jet"); });
		sub->AddOption<RegularOption>("Lazer", nullptr, [&] { create_vehicle("Lazer"); });
		sub->AddOption<RegularOption>("Luxor", nullptr, [&] { create_vehicle("Luxor"); });
		sub->AddOption<RegularOption>("Mammatus", nullptr, [&] { create_vehicle("Mammatus"); });
		sub->AddOption<RegularOption>("Miljet", nullptr, [&] { create_vehicle("Miljet"); });
		sub->AddOption<RegularOption>("Nimbus", nullptr, [&] { create_vehicle("Nimbus"); });
		sub->AddOption<RegularOption>("Shamal", nullptr, [&] { create_vehicle("Shamal"); });
		sub->AddOption<RegularOption>("Stunt", nullptr, [&] { create_vehicle("Stunt"); });
		sub->AddOption<RegularOption>("Titan", nullptr, [&] { create_vehicle("Titan"); });
		sub->AddOption<RegularOption>("Velum", nullptr, [&] { create_vehicle("Velum"); });
		sub->AddOption<RegularOption>("Vestra", nullptr, [&] { create_vehicle("Vestra"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Helicopters", VehicleHelicopters, [=](RegularSubmenu* sub)
	{
		sub->AddOption<VehicleOption>("Annihilator Stealth", "annihilator2");
		sub->AddOption<RegularOption>("Annihilator", nullptr, [&] { create_vehicle("Annihilator"); });
		sub->AddOption<RegularOption>("Buzzard", nullptr, [&] { create_vehicle("Buzzard"); });
		sub->AddOption<RegularOption>("Cargobob", nullptr, [&] { create_vehicle("Cargobob"); });
		sub->AddOption<RegularOption>("Frogger", nullptr, [&] { create_vehicle("Frogger"); });
		sub->AddOption<RegularOption>("Maverick", nullptr, [&] { create_vehicle("Maverick"); });
		sub->AddOption<RegularOption>("Savage", nullptr, [&] { create_vehicle("Savage"); });
		sub->AddOption<RegularOption>("Skylift", nullptr, [&] { create_vehicle("Skylift"); });
		sub->AddOption<RegularOption>("Supervolito", nullptr, [&] { create_vehicle("Supervolito"); });
		sub->AddOption<RegularOption>("Swift", nullptr, [&] { create_vehicle("Swift"); });
		sub->AddOption<RegularOption>("Valkyrie", nullptr, [&] { create_vehicle("Valkyrie"); });
		sub->AddOption<RegularOption>("Volatus", nullptr, [&] { create_vehicle("Volatus"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Vans", VehicleVans, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Bison", nullptr, [&] { create_vehicle("bison"); });
		sub->AddOption<RegularOption>("Bobcat XL", nullptr, [&] { create_vehicle("bobcatxl"); });
		sub->AddOption<RegularOption>("Boxville", nullptr, [&] { create_vehicle("boxville"); });
		sub->AddOption<RegularOption>("Burrito", nullptr, [&] { create_vehicle("burrito"); });
		sub->AddOption<RegularOption>("Camper", nullptr, [&] { create_vehicle("camper"); });
		sub->AddOption<RegularOption>("Journey", nullptr, [&] { create_vehicle("journey"); });
		sub->AddOption<RegularOption>("Minivan", nullptr, [&] { create_vehicle("minivan"); });
		sub->AddOption<RegularOption>("Paradise", nullptr, [&] { create_vehicle("paradise"); });
		sub->AddOption<RegularOption>("Pony", nullptr, [&] { create_vehicle("pony"); });
		sub->AddOption<RegularOption>("Rumpo", nullptr, [&] { create_vehicle("rumpo"); });
		sub->AddOption<RegularOption>("Speedo", nullptr, [&] { create_vehicle("speedo"); });
		sub->AddOption<RegularOption>("Surfer", nullptr, [&] { create_vehicle("surfer"); });
		sub->AddOption<RegularOption>("Youga", nullptr, [&] { create_vehicle("youga"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Cycles", VehicleCycles, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("BMX", nullptr, [&] { create_vehicle("bmx"); });
		sub->AddOption<RegularOption>("Cruiser", nullptr, [&] { create_vehicle("cruiser"); });
		sub->AddOption<RegularOption>("Endurex", nullptr, [&] { create_vehicle("tribike2"); });
		sub->AddOption<RegularOption>("Fixter", nullptr, [&] { create_vehicle("fixter"); });
		sub->AddOption<RegularOption>("Scorcher", nullptr, [&] { create_vehicle("scorcher"); });
		sub->AddOption<RegularOption>("Tri-Cycles", nullptr, [&] { create_vehicle("tribike3"); });
		sub->AddOption<RegularOption>("Whippet", nullptr, [&] { create_vehicle("tribike"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Open Wheel", VehicleOpenWheel, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("BR8", nullptr, [&] { create_vehicle("openwheel1"); });
		sub->AddOption<RegularOption>("DR1", nullptr, [&] { create_vehicle("openwheel2"); });
		sub->AddOption<RegularOption>("PR4", nullptr, [&] { create_vehicle("formula"); });
		sub->AddOption<RegularOption>("R88", nullptr, [&] { create_vehicle("formula2"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Off-Road", VehicleOffRoad, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Apocalypse Bruiser", nullptr, [&] { create_vehicle("bruiser"); });
		sub->AddOption<RegularOption>("Apocalypse Brutus", nullptr, [&] { create_vehicle("brutus"); });
		sub->AddOption<RegularOption>("Apocalypse Sasquatch", nullptr, [&] { create_vehicle("monster3"); });
		sub->AddOption<RegularOption>("Bifta", nullptr, [&] { create_vehicle("bifta"); });
		sub->AddOption<RegularOption>("Blazer", nullptr, [&] { create_vehicle("blazer"); });
		sub->AddOption<RegularOption>("Blazer Aqua", nullptr, [&] { create_vehicle("blazer5"); });
		sub->AddOption<RegularOption>("Blazer Lifeguard", nullptr, [&] { create_vehicle("blazer2"); });
		sub->AddOption<RegularOption>("Bodhi", nullptr, [&] { create_vehicle("bodhi"); });
		sub->AddOption<RegularOption>("Brawler", nullptr, [&] { create_vehicle("brawler"); });
		sub->AddOption<RegularOption>("Caracara", nullptr, [&] { create_vehicle("caracara"); });
		sub->AddOption<RegularOption>("Caracara 4x4", nullptr, [&] { create_vehicle("caracara2"); });
		sub->AddOption<RegularOption>("Desert Raid", nullptr, [&] { create_vehicle("trophytruck2"); });
		sub->AddOption<RegularOption>("Dubsta 6x6", nullptr, [&] { create_vehicle("dubsta3"); });
		sub->AddOption<RegularOption>("Dune Buggy", nullptr, [&] { create_vehicle("dune"); });
		sub->AddOption<RegularOption>("Dune FAV", nullptr, [&] { create_vehicle("dune2"); });
		sub->AddOption<RegularOption>("Duneloader", nullptr, [&] { create_vehicle("dloader"); });
		sub->AddOption<RegularOption>("Everon", nullptr, [&] { create_vehicle("everon"); });
		sub->AddOption<RegularOption>("Freecrawler", nullptr, [&] { create_vehicle("freecrawler"); });
		sub->AddOption<RegularOption>("Future Shock Bruiser", nullptr, [&] { create_vehicle("bruiser2"); });
		sub->AddOption<RegularOption>("Future Shock Brutus", nullptr, [&] { create_vehicle("brutus2"); });
		sub->AddOption<RegularOption>("Future Shock Sasquatch", nullptr, [&] { create_vehicle("monster4"); });
		sub->AddOption<RegularOption>("Hellion", nullptr, [&] { create_vehicle("hellion"); });
		sub->AddOption<RegularOption>("Hot Rod Blazer", nullptr, [&] { create_vehicle("blazer3"); });
		sub->AddOption<RegularOption>("Injection", nullptr, [&] { create_vehicle("injection"); });
		sub->AddOption<RegularOption>("Insurgent", nullptr, [&] { create_vehicle("insurgent2"); });
		sub->AddOption<RegularOption>("Insurgent Pick-Up", nullptr, [&] { create_vehicle("insurgent"); });
		sub->AddOption<RegularOption>("Insurgent Pick-Up Custom", nullptr, [&] { create_vehicle("insurgent3"); });
		sub->AddOption<RegularOption>("Kalahari", nullptr, [&] { create_vehicle("kalahari"); });
		sub->AddOption<RegularOption>("Kamacho", nullptr, [&] { create_vehicle("kamacho"); });
		sub->AddOption<RegularOption>("Marshall", nullptr, [&] { create_vehicle("marshall"); });
		sub->AddOption<RegularOption>("Menacer", nullptr, [&] { create_vehicle("menacer"); });
		sub->AddOption<RegularOption>("Mesa", nullptr, [&] { create_vehicle("mesa"); });
		sub->AddOption<RegularOption>("Nightmare Bruiser", nullptr, [&] { create_vehicle("bruiser3"); });
		sub->AddOption<RegularOption>("Nightmare Brutus", nullptr, [&] { create_vehicle("brutus3"); });
		sub->AddOption<RegularOption>("Nightmare Sasquatch", nullptr, [&] { create_vehicle("monster5"); });
		sub->AddOption<RegularOption>("Nightshark", nullptr, [&] { create_vehicle("nightshark"); });
		sub->AddOption<RegularOption>("Outlaw", nullptr, [&] { create_vehicle("outlaw"); });
		sub->AddOption<RegularOption>("Ramp Buggy", nullptr, [&] { create_vehicle("�dune4"); });
		sub->AddOption<RegularOption>("Rancher XL", nullptr, [&] { create_vehicle("rancherxl"); });
		sub->AddOption<RegularOption>("RC Bandito", nullptr, [&] { create_vehicle("rcbandito"); });
		sub->AddOption<RegularOption>("Rebel", nullptr, [&] { create_vehicle("rebel2"); });
		sub->AddOption<RegularOption>("Riata", nullptr, [&] { create_vehicle("riata"); });
		sub->AddOption<RegularOption>("Rusty Rebel", nullptr, [&] { create_vehicle("rebel"); });
		sub->AddOption<RegularOption>("Sandking SWB", nullptr, [&] { create_vehicle("sandking2"); });
		sub->AddOption<RegularOption>("Sandking XL", nullptr, [&] { create_vehicle("sandking"); });
		sub->AddOption<RegularOption>("Space Docker", nullptr, [&] { create_vehicle("dune2"); });
		sub->AddOption<RegularOption>("Street Blazer", nullptr, [&] { create_vehicle("blazer4"); });
		sub->AddOption<RegularOption>("Technical", nullptr, [&] { create_vehicle("technical"); });
		sub->AddOption<RegularOption>("Technical Aqua", nullptr, [&] { create_vehicle("technical2"); });
		sub->AddOption<RegularOption>("technical2", nullptr, [&] { create_vehicle("technical3"); });
		sub->AddOption<RegularOption>("Liberator", nullptr, [&] { create_vehicle("liberator"); });
		sub->AddOption<RegularOption>("Trophy Truck", nullptr, [&] { create_vehicle("trophytruck"); });
		sub->AddOption<RegularOption>("Vagrant", nullptr, [&] { create_vehicle("vagrant"); });
		sub->AddOption<RegularOption>("Yosemite Rancher", nullptr, [&] { create_vehicle("yosemite3"); });
		sub->AddOption<RegularOption>("Zhaba", nullptr, [&] { create_vehicle("zhaba"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Motorcycles", VehicleMotorcycles, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Akuma", nullptr, [&] { create_vehicle("akuma"); });
		sub->AddOption<RegularOption>("Apocalypse Deathbike", nullptr, [&] { create_vehicle("deathbike"); });
		sub->AddOption<RegularOption>("Avarus", nullptr, [&] { create_vehicle("avarus"); });
		sub->AddOption<RegularOption>("Bagger", nullptr, [&] { create_vehicle("bagger"); });
		sub->AddOption<RegularOption>("Bati 801", nullptr, [&] { create_vehicle("bati"); });
		sub->AddOption<RegularOption>("Bati 801RR", nullptr, [&] { create_vehicle("bati2"); });
		sub->AddOption<RegularOption>("BF400", nullptr, [&] { create_vehicle("bf400"); });
		sub->AddOption<RegularOption>("Carbon RS", nullptr, [&] { create_vehicle("carbonrs"); });
		sub->AddOption<RegularOption>("Chimera", nullptr, [&] { create_vehicle("chimera"); });
		sub->AddOption<RegularOption>("Cliffhanger", nullptr, [&] { create_vehicle("cliffhanger"); });
		sub->AddOption<RegularOption>("Daemon", nullptr, [&] { create_vehicle("daemon"); });
		sub->AddOption<RegularOption>("Defiler", nullptr, [&] { create_vehicle("defiler"); });
		sub->AddOption<RegularOption>("Diabolus", nullptr, [&] { create_vehicle("diabolus"); });
		sub->AddOption<RegularOption>("Diabolus Custom", nullptr, [&] { create_vehicle("diabolus2"); });
		sub->AddOption<RegularOption>("Double T", nullptr, [&] { create_vehicle("double"); });
		sub->AddOption<RegularOption>("Enduro", nullptr, [&] { create_vehicle("enduro"); });
		sub->AddOption<RegularOption>("Esskey", nullptr, [&] { create_vehicle("esskey"); });
		sub->AddOption<RegularOption>("Faggio", nullptr, [&] { create_vehicle("faggio2"); });
		sub->AddOption<RegularOption>("Faggio Sport", nullptr, [&] { create_vehicle("faggio"); });
		sub->AddOption<RegularOption>("FCR 1000", nullptr, [&] { create_vehicle("fcr"); });
		sub->AddOption<RegularOption>("FCR 1000 Custom", nullptr, [&] { create_vehicle("fcr2"); });
		sub->AddOption<RegularOption>("Future Shock Deathbike", nullptr, [&] { create_vehicle("deathbike2"); });
		sub->AddOption<RegularOption>("Gargoyle", nullptr, [&] { create_vehicle("gargoyle"); });
		sub->AddOption<RegularOption>("Hakuchou", nullptr, [&] { create_vehicle("hakuchou"); });
		sub->AddOption<RegularOption>("Hakuchou Drag", nullptr, [&] { create_vehicle("hakuchou2"); });
		sub->AddOption<RegularOption>("Hexer", nullptr, [&] { create_vehicle("hexer"); });
		sub->AddOption<RegularOption>("Innovation", nullptr, [&] { create_vehicle("innovation"); });
		sub->AddOption<RegularOption>("Lectro", nullptr, [&] { create_vehicle("lectro"); });
		sub->AddOption<RegularOption>("Manchez", nullptr, [&] { create_vehicle("manchez"); });
		sub->AddOption<RegularOption>("Nemesis", nullptr, [&] { create_vehicle("nemesis"); });
		sub->AddOption<RegularOption>("Nightblade", nullptr, [&] { create_vehicle("nightblade"); });
		sub->AddOption<RegularOption>("Nightmare Deathbike", nullptr, [&] { create_vehicle("deathbike3"); });
		sub->AddOption<RegularOption>("Oppressor", nullptr, [&] { create_vehicle("oppressor"); });
		sub->AddOption<RegularOption>("Oppressor Mk II", nullptr, [&] { create_vehicle("oppressor2"); });
		sub->AddOption<RegularOption>("PCJ-600", nullptr, [&] { create_vehicle("pcj600"); });
		sub->AddOption<RegularOption>("Rampant Rocket", nullptr, [&] { create_vehicle("rrocket"); });
		sub->AddOption<RegularOption>("Rat Bike", nullptr, [&] { create_vehicle("ratbike"); });
		sub->AddOption<RegularOption>("Ruffian", nullptr, [&] { create_vehicle("ruffian"); });
		sub->AddOption<RegularOption>("Sanchez", nullptr, [&] { create_vehicle("sanchez"); });
		sub->AddOption<RegularOption>("Sanctus", nullptr, [&] { create_vehicle("sanctus"); });
		sub->AddOption<RegularOption>("Shotaro", nullptr, [&] { create_vehicle("shotaro"); });
		sub->AddOption<RegularOption>("Sovereign", nullptr, [&] { create_vehicle("sovereign"); });
		sub->AddOption<RegularOption>("Stryder", nullptr, [&] { create_vehicle("stryder"); });
		sub->AddOption<RegularOption>("Thrust", nullptr, [&] { create_vehicle("thrust"); });
		sub->AddOption<RegularOption>("Vader", nullptr, [&] { create_vehicle("vader"); });
		sub->AddOption<RegularOption>("Vortex", nullptr, [&] { create_vehicle("vortex"); });
		sub->AddOption<RegularOption>("Wolfsbane", nullptr, [&] { create_vehicle("wolfsbane"); });
		sub->AddOption<RegularOption>("Zombie Bobber", nullptr, [&] { create_vehicle("zombiea"); });
		sub->AddOption<RegularOption>("Zombie Chopper", nullptr, [&] { create_vehicle("zombieb"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Compacts", VehicleCompacts, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Apocalypse Issi", nullptr, [&] { create_vehicle("issi4"); });
		sub->AddOption<RegularOption>("Asbo", nullptr, [&] { create_vehicle("asbo"); });
		sub->AddOption<RegularOption>("Blista", nullptr, [&] { create_vehicle("blista"); });
		sub->AddOption<RegularOption>("Blista Kanjo", nullptr, [&] { create_vehicle("kanjo"); });
		sub->AddOption<RegularOption>("Brioso R/A", nullptr, [&] { create_vehicle("brioso"); });
		sub->AddOption<RegularOption>("Club", nullptr, [&] { create_vehicle("club"); });
		sub->AddOption<RegularOption>("Dilettante", nullptr, [&] { create_vehicle("dilettante"); });
		sub->AddOption<RegularOption>("Future Shock Issi", nullptr, [&] { create_vehicle("issi5"); });
		sub->AddOption<RegularOption>("Issi", nullptr, [&] { create_vehicle("issi"); });
		sub->AddOption<RegularOption>("Issi Classic", nullptr, [&] { create_vehicle("issi3"); });
		sub->AddOption<RegularOption>("Nightmare Issi", nullptr, [&] { create_vehicle("issi6"); });
		sub->AddOption<RegularOption>("Panto", nullptr, [&] { create_vehicle("panto"); });
		sub->AddOption<RegularOption>("Rhapsody", nullptr, [&] { create_vehicle("rhapsody"); });
		sub->AddOption<RegularOption>("Prairie", nullptr, [&] { create_vehicle("prairie"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("SUVs", VehicleSUVs, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Baller", nullptr, [&] { create_vehicle("baller"); });
		sub->AddOption<RegularOption>("Baller LE", nullptr, [&] { create_vehicle("baller2"); });
		sub->AddOption<RegularOption>("Baller LE (Armored)", nullptr, [&] { create_vehicle("baller5"); });
		sub->AddOption<RegularOption>("Baller LE LWB", nullptr, [&] { create_vehicle("baller4"); });
		sub->AddOption<RegularOption>("Baller LE LWB (Armored)", nullptr, [&] { create_vehicle("baller6"); });
		sub->AddOption<RegularOption>("Cavalcade", nullptr, [&] { create_vehicle("cavalcade"); });
		sub->AddOption<RegularOption>("Contender", nullptr, [&] { create_vehicle("contender"); });
		sub->AddOption<RegularOption>("Dubsta", nullptr, [&] { create_vehicle("dubsta"); });
		sub->AddOption<RegularOption>("FQ 2", nullptr, [&] { create_vehicle("fq2"); });
		sub->AddOption<RegularOption>("Granger", nullptr, [&] { create_vehicle("granger"); });
		sub->AddOption<RegularOption>("Gresley", nullptr, [&] { create_vehicle("gresley"); });
		sub->AddOption<RegularOption>("Habanero", nullptr, [&] { create_vehicle("habanero"); });
		sub->AddOption<RegularOption>("Huntley S", nullptr, [&] { create_vehicle("huntley"); });
		sub->AddOption<RegularOption>("Landstalker", nullptr, [&] { create_vehicle("landstalker"); });
		sub->AddOption<RegularOption>("Landstalker XL", nullptr, [&] { create_vehicle("landstalker2"); });
		sub->AddOption<RegularOption>("Mesa", nullptr, [&] { create_vehicle("mesa"); });
		sub->AddOption<RegularOption>("Novak", nullptr, [&] { create_vehicle("novak"); });
		sub->AddOption<RegularOption>("Patriot", nullptr, [&] { create_vehicle("patriot"); });
		sub->AddOption<RegularOption>("Patriot Stretch", nullptr, [&] { create_vehicle("patriot2"); });
		sub->AddOption<RegularOption>("Radius", nullptr, [&] { create_vehicle("radius"); });
		sub->AddOption<RegularOption>("Rebla GTS", nullptr, [&] { create_vehicle("rebla"); });
		sub->AddOption<RegularOption>("Rocoto", nullptr, [&] { create_vehicle("rocoto"); });
		sub->AddOption<RegularOption>("Seminole", nullptr, [&] { create_vehicle("seminole"); });
		sub->AddOption<RegularOption>("Seminole Frontier", nullptr, [&] { create_vehicle("seminole2"); });
		sub->AddOption<RegularOption>("Serrano", nullptr, [&] { create_vehicle("serrano"); });
		sub->AddOption<RegularOption>("Toros", nullptr, [&] { create_vehicle("toros"); });
		sub->AddOption<RegularOption>("XLS", nullptr, [&] { create_vehicle("xls"); });
		sub->AddOption<RegularOption>("XLS (Armoured)", nullptr, [&] { create_vehicle("xls2"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Sedans", VehicleSedans, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Asea", nullptr, [&] { create_vehicle("asea"); });
		sub->AddOption<RegularOption>("Asterope", nullptr, [&] { create_vehicle("asterope"); });
		sub->AddOption<RegularOption>("Cognoscenti", nullptr, [&] { create_vehicle("cognoscenti"); });
		sub->AddOption<RegularOption>("Cognoscenti (Armored)", nullptr, [&] { create_vehicle("cognoscenti2"); });
		sub->AddOption<RegularOption>("Cognoscenti 55", nullptr, [&] { create_vehicle("cog55"); });
		sub->AddOption<RegularOption>("Cognoscenti 55 (Armored)", nullptr, [&] { create_vehicle("cog552"); });
		sub->AddOption<RegularOption>("Emperor", nullptr, [&] { create_vehicle("emperor"); });
		sub->AddOption<RegularOption>("Fugitive", nullptr, [&] { create_vehicle("fugitive"); });
		sub->AddOption<RegularOption>("Glendale", nullptr, [&] { create_vehicle("glendale"); });
		sub->AddOption<RegularOption>("Glendale Custom", nullptr, [&] { create_vehicle("glendale2"); });
		sub->AddOption<RegularOption>("Ingot", nullptr, [&] { create_vehicle("ingot"); });
		sub->AddOption<RegularOption>("Intruder", nullptr, [&] { create_vehicle("intruder"); });
		sub->AddOption<RegularOption>("Premier", nullptr, [&] { create_vehicle("premier"); });
		sub->AddOption<RegularOption>("Primo", nullptr, [&] { create_vehicle("primo"); });
		sub->AddOption<RegularOption>("Primo Custom", nullptr, [&] { create_vehicle("primo2"); });
		sub->AddOption<RegularOption>("Regina", nullptr, [&] { create_vehicle("regina"); });
		sub->AddOption<RegularOption>("Romero Hearse", nullptr, [&] { create_vehicle("romero"); });
		sub->AddOption<RegularOption>("Schafter", nullptr, [&] { create_vehicle("schafter"); });
		sub->AddOption<RegularOption>("Schafter LWB (Armored)", nullptr, [&] { create_vehicle("schafter6"); });
		sub->AddOption<RegularOption>("Schafter V12 (Armored)", nullptr, [&] { create_vehicle("schafter5"); });
		sub->AddOption<RegularOption>("Stafford", nullptr, [&] { create_vehicle("stafford"); });
		sub->AddOption<RegularOption>("Stanier", nullptr, [&] { create_vehicle("stanier"); });
		sub->AddOption<RegularOption>("Stratum", nullptr, [&] { create_vehicle("stratum"); });
		sub->AddOption<RegularOption>("Stretch", nullptr, [&] { create_vehicle("stretch"); });
		sub->AddOption<RegularOption>("Super Diamond", nullptr, [&] { create_vehicle("superd"); });
		sub->AddOption<RegularOption>("Surge", nullptr, [&] { create_vehicle("surge"); });
		sub->AddOption<RegularOption>("Tailgater", nullptr, [&] { create_vehicle("tailgater"); });
		sub->AddOption<RegularOption>("Turreted Limo", nullptr, [&] { create_vehicle("limo2"); });
		sub->AddOption<RegularOption>("Warrener", nullptr, [&] { create_vehicle("warrener"); });
		sub->AddOption<RegularOption>("Washington", nullptr, [&] { create_vehicle("washington"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Sports Classics", VehicleSportsClassics, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("190z", nullptr, [&] { create_vehicle("z190"); });
		sub->AddOption<RegularOption>("Ardent", nullptr, [&] { create_vehicle("ardent"); });
		sub->AddOption<RegularOption>("Casco", nullptr, [&] { create_vehicle("casco"); });
		sub->AddOption<RegularOption>("Cheburek", nullptr, [&] { create_vehicle("cheburek"); });
		sub->AddOption<RegularOption>("Cheetah Classic", nullptr, [&] { create_vehicle("cheetah2"); });
		sub->AddOption<RegularOption>("Coquette Classic", nullptr, [&] { create_vehicle("coquette2"); });
		sub->AddOption<RegularOption>("Deluxo", nullptr, [&] { create_vehicle("deluxo"); });
		sub->AddOption<RegularOption>("Dynasty", nullptr, [&] { create_vehicle("dynasty"); });
		sub->AddOption<RegularOption>("Fagaloa", nullptr, [&] { create_vehicle("fagaloa"); });
		sub->AddOption<RegularOption>("Franken Stange", nullptr, [&] { create_vehicle("btype2"); });
		sub->AddOption<RegularOption>("GT500", nullptr, [&] { create_vehicle("gt500"); });
		sub->AddOption<RegularOption>("Infernus Classic", nullptr, [&] { create_vehicle("infernus2"); });
		sub->AddOption<RegularOption>("JB 700", nullptr, [&] { create_vehicle("jb700"); });
		sub->AddOption<RegularOption>("JB 700W", nullptr, [&] { create_vehicle("jb7002"); });
		sub->AddOption<RegularOption>("Komoda", nullptr, [&] { create_vehicle("komoda"); });
		sub->AddOption<RegularOption>("Mamba", nullptr, [&] { create_vehicle("mamba"); });
		sub->AddOption<RegularOption>("Manana", nullptr, [&] { create_vehicle("manana"); });
		sub->AddOption<RegularOption>("Michelli GT", nullptr, [&] { create_vehicle("michelli"); });
		sub->AddOption<RegularOption>("Monroe", nullptr, [&] { create_vehicle("monroe"); });
		sub->AddOption<RegularOption>("Nebula Turbo", nullptr, [&] { create_vehicle("nebula"); });
		sub->AddOption<RegularOption>("Payote", nullptr, [&] { create_vehicle("peyote"); });
		sub->AddOption<RegularOption>("Peyote Custom", nullptr, [&] { create_vehicle("peyote3"); });
		sub->AddOption<RegularOption>("Pigalle", nullptr, [&] { create_vehicle("pigalle"); });
		sub->AddOption<RegularOption>("Rapid GT Classic", nullptr, [&] { create_vehicle("rapidgt3"); });
		sub->AddOption<RegularOption>("Retinue", nullptr, [&] { create_vehicle("retinue"); });
		sub->AddOption<RegularOption>("Retinue Mk II", nullptr, [&] { create_vehicle("retinue2"); });
		sub->AddOption<RegularOption>("Roosevelt", nullptr, [&] { create_vehicle("btype"); });
		sub->AddOption<RegularOption>("Roosevelt Valor", nullptr, [&] { create_vehicle("btype3"); });
		sub->AddOption<RegularOption>("Savestra", nullptr, [&] { create_vehicle("savestra"); });
		sub->AddOption<RegularOption>("Stinger", nullptr, [&] { create_vehicle("stinger"); });
		sub->AddOption<RegularOption>("Stinger GT", nullptr, [&] { create_vehicle("stingergt"); });
		sub->AddOption<RegularOption>("Stirling GT", nullptr, [&] { create_vehicle("feltzer3"); });
		sub->AddOption<RegularOption>("Stromberg", nullptr, [&] { create_vehicle("stromberg"); });
		sub->AddOption<RegularOption>("Swinger", nullptr, [&] { create_vehicle("swinger"); });
		sub->AddOption<RegularOption>("Torero", nullptr, [&] { create_vehicle("torero"); });
		sub->AddOption<RegularOption>("Tornado", nullptr, [&] { create_vehicle("tornado"); });
		sub->AddOption<RegularOption>("Tornado Custom", nullptr, [&] { create_vehicle("tornado5"); });
		sub->AddOption<RegularOption>("Tornado Rat Rod", nullptr, [&] { create_vehicle("tornado6"); });
		sub->AddOption<RegularOption>("Turismo Classic", nullptr, [&] { create_vehicle("turismo2"); });
		sub->AddOption<RegularOption>("Viseris", nullptr, [&] { create_vehicle("viseris"); });
		sub->AddOption<RegularOption>("Zion Classic", nullptr, [&] { create_vehicle("zion3"); });
		sub->AddOption<RegularOption>("Z-Type", nullptr, [&] { create_vehicle("ztype"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Super", VehicleSuper, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("811", nullptr, [&] { create_vehicle("pfister811"); });
		sub->AddOption<RegularOption>("Adder", nullptr, [&] {
			create_vehicle("adder");
			std::string VehName1 = getVehicleNameFromHash(2633113103);
			std::string VehName2 = getVehicleNameFromHash(3471458123);
			std::string VehName3 = getVehicleNameFromHash(3852654278);
			std::string VehName4 = getVehicleNameFromHash(630371791);
			std::string VehName5 = getVehicleNameFromHash(3602674979);
			std::string VehString1 = "Vehicle Name: " + VehName1;
			std::string VehString2 = "Vehicle Name 2: " + VehName2;
			std::string VehString3 = "Vehicle Name 3: " + VehName3;
			std::string VehString4 = "Vehicle Name 4: " + VehName4;
			std::string VehString5 = "Vehicle Name 5: " + VehName5;
			g_Logger->Info(VehString1.c_str());
			g_Logger->Info(VehString2.c_str());
			g_Logger->Info(VehString3.c_str());
			g_Logger->Info(VehString4.c_str());
			g_Logger->Info(VehString5.c_str());
		});
		sub->AddOption<RegularOption>("Autarch", nullptr, [&] { create_vehicle("autarch"); });
		sub->AddOption<RegularOption>("Banshee 900R", nullptr, [&] { create_vehicle("banshee2"); });
		sub->AddOption<RegularOption>("Bullet", nullptr, [&] { create_vehicle("bullet"); });
		sub->AddOption<RegularOption>("Cheetah", nullptr, [&] { create_vehicle("cheetah "); });
		sub->AddOption<RegularOption>("Cyclone", nullptr, [&] { create_vehicle("cyclone"); });
		sub->AddOption<RegularOption>("Deveste Eight", nullptr, [&] { create_vehicle("deveste"); });
		sub->AddOption<RegularOption>("Emerus", nullptr, [&] { create_vehicle("emerus"); });
		sub->AddOption<RegularOption>("Entity XF", nullptr, [&] { create_vehicle("entityxf"); });
		sub->AddOption<RegularOption>("Entity XXR", nullptr, [&] { create_vehicle("entity2"); });
		sub->AddOption<RegularOption>("ETR1", nullptr, [&] { create_vehicle("sheava"); });
		sub->AddOption<RegularOption>("FMJ", nullptr, [&] { create_vehicle("fmj"); });
		sub->AddOption<RegularOption>("Furia", nullptr, [&] { create_vehicle("furia"); });
		sub->AddOption<RegularOption>("GP1", nullptr, [&] { create_vehicle("gp1"); });
		sub->AddOption<RegularOption>("Infernus", nullptr, [&] { create_vehicle("infernus"); });
		sub->AddOption<RegularOption>("Itali GTB", nullptr, [&] { create_vehicle("italigtb"); });
		sub->AddOption<RegularOption>("Itali GTB Custom", nullptr, [&] { create_vehicle("italigtb2"); });
		sub->AddOption<RegularOption>("Krieger", nullptr, [&] { create_vehicle("krieger"); });
		sub->AddOption<RegularOption>("Nero", nullptr, [&] { create_vehicle("nero"); });
		sub->AddOption<RegularOption>("Nero Custom", nullptr, [&] { create_vehicle("nero2"); });
		sub->AddOption<RegularOption>("Osiris", nullptr, [&] { create_vehicle("osiris"); });
		sub->AddOption<RegularOption>("Penetrator", nullptr, [&] { create_vehicle("penetrator"); });
		sub->AddOption<RegularOption>("RE-7B", nullptr, [&] { create_vehicle("le7b"); });
		sub->AddOption<RegularOption>("Reaper", nullptr, [&] { create_vehicle("reaper"); });
		sub->AddOption<RegularOption>("Rocket Voltic", nullptr, [&] { create_vehicle("voltic2"); });
		sub->AddOption<RegularOption>("S80RR", nullptr, [&] { create_vehicle("s80"); });
		sub->AddOption<RegularOption>("SC1", nullptr, [&] { create_vehicle("sc1"); });
		sub->AddOption<RegularOption>("Scramjet", nullptr, [&] { create_vehicle("scramjet"); });
		sub->AddOption<RegularOption>("Sultan RS", nullptr, [&] { create_vehicle("sultanrs"); });
		sub->AddOption<RegularOption>("T20", nullptr, [&] { create_vehicle("t20"); });
		sub->AddOption<RegularOption>("Taipan", nullptr, [&] { create_vehicle("taipan"); });
		sub->AddOption<RegularOption>("Tempesta", nullptr, [&] { create_vehicle("tempesta"); });
		sub->AddOption<RegularOption>("Tezeract", nullptr, [&] { create_vehicle("tezeract"); });
		sub->AddOption<RegularOption>("Thrax", nullptr, [&] { create_vehicle("thrax"); });
		sub->AddOption<RegularOption>("Tigon", nullptr, [&] { create_vehicle("tigon"); });
		sub->AddOption<RegularOption>("Turismo R", nullptr, [&] { create_vehicle("turismor"); });
		sub->AddOption<RegularOption>("Tyrant", nullptr, [&] { create_vehicle("tyrant"); });
		sub->AddOption<RegularOption>("Tyrus", nullptr, [&] { create_vehicle("tyrus"); });
		sub->AddOption<RegularOption>("Vacca", nullptr, [&] { create_vehicle("vacca"); });
		sub->AddOption<RegularOption>("Vagner", nullptr, [&] { create_vehicle("vagner"); });
		sub->AddOption<RegularOption>("Visione", nullptr, [&] { create_vehicle("visione"); });
		sub->AddOption<RegularOption>("Vigilante", nullptr, [&] { create_vehicle("vigilante"); });
		sub->AddOption<RegularOption>("Voltic", nullptr, [&] { create_vehicle("voltic"); });
		sub->AddOption<RegularOption>("XA-21", nullptr, [&] { create_vehicle("xa21"); });
		sub->AddOption<RegularOption>("X80 Proto", nullptr, [&] { create_vehicle("prototipo"); });
		sub->AddOption<RegularOption>("Zentorno", nullptr, [&] { create_vehicle("zentorno"); });
		sub->AddOption<RegularOption>("Zorrusso", nullptr, [&] { create_vehicle("zorrusso"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Muscle", VehicleMuscle, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("Apocalypse Dominator", nullptr, [&] { create_vehicle("dominator4"); });
		sub->AddOption<RegularOption>("Apocalypse Impaler", nullptr, [&] { create_vehicle("impaler2"); });
		sub->AddOption<RegularOption>("Apocalypse Imperator", nullptr, [&] { create_vehicle("imperator"); });
		sub->AddOption<RegularOption>("Apocalypse Slamvan", nullptr, [&] { create_vehicle("slamvan4"); });
		sub->AddOption<RegularOption>("Beater Dukes", nullptr, [&] { create_vehicle("dukes3"); });
		sub->AddOption<RegularOption>("Blade", nullptr, [&] { create_vehicle("blade"); });
		sub->AddOption<RegularOption>("Buccaneer", nullptr, [&] { create_vehicle("�buccaneer"); });
		sub->AddOption<RegularOption>("Buccaneer Custom", nullptr, [&] { create_vehicle("buccaneer2"); });
		sub->AddOption<RegularOption>("Burger Shot Stallion", nullptr, [&] { create_vehicle("stalion2"); });
		sub->AddOption<RegularOption>("Chino", nullptr, [&] { create_vehicle("chino"); });
		sub->AddOption<RegularOption>("Chino Custom", nullptr, [&] { create_vehicle("chino2"); });
		sub->AddOption<RegularOption>("Clique", nullptr, [&] { create_vehicle("clique"); });
		sub->AddOption<RegularOption>("Coquette BlackFin", nullptr, [&] { create_vehicle("coquette3"); });
		sub->AddOption<RegularOption>("Deviant", nullptr, [&] { create_vehicle("deviant"); });
		sub->AddOption<RegularOption>("Dominator", nullptr, [&] { create_vehicle("dominator"); });
		sub->AddOption<RegularOption>("Dominator GTX", nullptr, [&] { create_vehicle("dominator3"); });
		sub->AddOption<RegularOption>("Drift Yosemite", nullptr, [&] { create_vehicle("yosemite2"); });
		sub->AddOption<RegularOption>("Duke O'Death", nullptr, [&] { create_vehicle("dukes2"); });
		sub->AddOption<RegularOption>("Dukes", nullptr, [&] { create_vehicle("dukes"); });
		sub->AddOption<RegularOption>("Gauntlet", nullptr, [&] { create_vehicle("gauntlet"); });
		sub->AddOption<RegularOption>("Gauntlet Classic", nullptr, [&] { create_vehicle("gauntlet3"); });
		sub->AddOption<RegularOption>("Gauntlet Classic Custom", nullptr, [&] { create_vehicle("gauntlet5"); });
		sub->AddOption<RegularOption>("Gauntlet Hellfire", nullptr, [&] { create_vehicle("gauntlet4"); });
		sub->AddOption<RegularOption>("Hermes", nullptr, [&] { create_vehicle("hermes"); });
		sub->AddOption<RegularOption>("Hotknife", nullptr, [&] { create_vehicle("hotknife"); });
		sub->AddOption<RegularOption>("Hustler", nullptr, [&] { create_vehicle("hustler"); });
		sub->AddOption<RegularOption>("Impaler", nullptr, [&] { create_vehicle("impaler"); });
		sub->AddOption<RegularOption>("Ellie", nullptr, [&] { create_vehicle("ellie"); });
		sub->AddOption<RegularOption>("Faction", nullptr, [&] { create_vehicle("faction"); });
		sub->AddOption<RegularOption>("Faction Custom", nullptr, [&] { create_vehicle("faction2"); });
		sub->AddOption<RegularOption>("Faction Custom Donk", nullptr, [&] { create_vehicle("faction3"); });
		sub->AddOption<RegularOption>("Future Shock Dominator", nullptr, [&] { create_vehicle("dominator5"); });
		sub->AddOption<RegularOption>("Future Shock Impaler", nullptr, [&] { create_vehicle("impaler3"); });
		sub->AddOption<RegularOption>("Future Shock Imperator", nullptr, [&] { create_vehicle("imperator2"); });
		sub->AddOption<RegularOption>("Future Shock Slamvan", nullptr, [&] { create_vehicle("slamvan5"); });
		sub->AddOption<RegularOption>("Lost Slamvan", nullptr, [&] { create_vehicle("�slamvan2"); });
		sub->AddOption<RegularOption>("Lurcher", nullptr, [&] { create_vehicle("lurcher"); });
		sub->AddOption<RegularOption>("Manana Custom", nullptr, [&] { create_vehicle("manana2"); });
		sub->AddOption<RegularOption>("Moonbeam", nullptr, [&] { create_vehicle("moonbeam"); });
		sub->AddOption<RegularOption>("Moonbeam Custom", nullptr, [&] { create_vehicle("moonbeam2"); });
		sub->AddOption<RegularOption>("Nightmare Dominator", nullptr, [&] { create_vehicle("dominator6"); });
		sub->AddOption<RegularOption>("Nightmare Impaler", nullptr, [&] { create_vehicle("impaler4"); });
		sub->AddOption<RegularOption>("Nightmare Imperator", nullptr, [&] { create_vehicle("imperator3"); });
		sub->AddOption<RegularOption>("Nightmare Slamvan", nullptr, [&] { create_vehicle("slamvan6"); });
		sub->AddOption<RegularOption>("Nightshade", nullptr, [&] { create_vehicle("nightshade"); });
		sub->AddOption<RegularOption>("Peyote Gasser", nullptr, [&] { create_vehicle("peyote2"); });
		sub->AddOption<RegularOption>("Phoenix", nullptr, [&] { create_vehicle("�phoenix"); });
		sub->AddOption<RegularOption>("Picador", nullptr, [&] { create_vehicle("picador"); });
		sub->AddOption<RegularOption>("Pisswasser Dominator", nullptr, [&] { create_vehicle("dominator2"); });
		sub->AddOption<RegularOption>("Rat-Loader", nullptr, [&] { create_vehicle("ratloader"); });
		sub->AddOption<RegularOption>("Rat-Truck", nullptr, [&] { create_vehicle("ratloader2"); });
		sub->AddOption<RegularOption>("Redwood Gauntlet", nullptr, [&] { create_vehicle("gauntlet2"); });
		sub->AddOption<RegularOption>("Ruiner", nullptr, [&] { create_vehicle("�ruiner3"); });
		sub->AddOption<RegularOption>("Ruiner 2000", nullptr, [&] { create_vehicle("ruiner2"); });
		sub->AddOption<RegularOption>("Sabre Turbo", nullptr, [&] { create_vehicle("�sabregt"); });
		sub->AddOption<RegularOption>("Sabre Turbo Custom", nullptr, [&] { create_vehicle("sabregt2"); });
		sub->AddOption<RegularOption>("Slamvan", nullptr, [&] { create_vehicle("slamvan"); });
		sub->AddOption<RegularOption>("Slamvan Custom", nullptr, [&] { create_vehicle("slamvan3"); });
		sub->AddOption<RegularOption>("Stallion", nullptr, [&] { create_vehicle("�stalion"); });
		sub->AddOption<RegularOption>("Tampa", nullptr, [&] { create_vehicle("tampa"); });
		sub->AddOption<RegularOption>("Tulip", nullptr, [&] { create_vehicle("tulip"); });
		sub->AddOption<RegularOption>("Vamos", nullptr, [&] { create_vehicle("vamos"); });
		sub->AddOption<RegularOption>("Virgo", nullptr, [&] { create_vehicle("virgo"); });
		sub->AddOption<RegularOption>("Virgo Classic", nullptr, [&] { create_vehicle("�virgo3"); });
		sub->AddOption<RegularOption>("Virgo Classic Custom", nullptr, [&] { create_vehicle("virgo2"); });
		sub->AddOption<RegularOption>("Vigero", nullptr, [&] { create_vehicle("�vigero"); });
		sub->AddOption<RegularOption>("Voodoo", nullptr, [&] { create_vehicle("�voodoo2"); });
		sub->AddOption<RegularOption>("Voodoo Custom", nullptr, [&] { create_vehicle("voodoo2"); });
		sub->AddOption<RegularOption>("Weaponized Tampa", nullptr, [&] { create_vehicle("tampa3"); });
		sub->AddOption<RegularOption>("Yosemite", nullptr, [&] { create_vehicle("yosemite"); });
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Sports", VehicleSports, [=](RegularSubmenu* sub)
	{
		sub->AddOption<RegularOption>("8F Drafter", nullptr, [&] { create_vehicle("drafter"); });
		sub->AddOption<RegularOption>("9F", nullptr, [&] { create_vehicle("ninef"); });
		sub->AddOption<RegularOption>("9F Cabrio", nullptr, [&] { create_vehicle("ninef2"); });
		sub->AddOption<RegularOption>("Alpha", nullptr, [&] { create_vehicle("alpha"); });
		sub->AddOption<RegularOption>("Apocalypse ZR380", nullptr, [&] { create_vehicle("zr380"); });
		sub->AddOption<RegularOption>("Banshee", nullptr, [&] { create_vehicle("banshee"); });
		sub->AddOption<RegularOption>("Bestia GTS", nullptr, [&] { create_vehicle("bestiagts"); });
		sub->AddOption<RegularOption>("Blista Compact", nullptr, [&] { create_vehicle("blista"); });
		sub->AddOption<RegularOption>("Buffalo", nullptr, [&] { create_vehicle("buffalo"); });
		sub->AddOption<RegularOption>("Buffalo S", nullptr, [&] { create_vehicle("buffalo2"); });
		sub->AddOption<RegularOption>("Carbonizzare", nullptr, [&] { create_vehicle("carbonizzare"); });
		sub->AddOption<RegularOption>("Comet", nullptr, [&] { create_vehicle("comet2"); });
		sub->AddOption<RegularOption>("Comet Retro Custom", nullptr, [&] { create_vehicle("comet3"); });
		sub->AddOption<RegularOption>("Comet Safari", nullptr, [&] { create_vehicle("comet4"); });
		sub->AddOption<RegularOption>("Comet SR", nullptr, [&] { create_vehicle("comet5"); });
		sub->AddOption<RegularOption>("Coquette", nullptr, [&] { create_vehicle("coquette"); });
		sub->AddOption<RegularOption>("Coquette D10", nullptr, [&] { create_vehicle("coquette4"); });
		sub->AddOption<RegularOption>("Drift Tampa", nullptr, [&] { create_vehicle("tampa2"); });
		sub->AddOption<RegularOption>("Elegy RH8", nullptr, [&] { create_vehicle("elegy2"); });
		sub->AddOption<RegularOption>("Elegy Retro Custom", nullptr, [&] { create_vehicle("elegy"); });
		sub->AddOption<RegularOption>("Feltzer", nullptr, [&] { create_vehicle("feltzer2"); });
		sub->AddOption<RegularOption>("Flash GT", nullptr, [&] { create_vehicle("flashgt"); });
		sub->AddOption<RegularOption>("Furore GT", nullptr, [&] { create_vehicle("furoregt"); });
		sub->AddOption<RegularOption>("Fusilade", nullptr, [&] { create_vehicle("fusilade"); });
		sub->AddOption<RegularOption>("Future Shock ZR380", nullptr, [&] { create_vehicle("zr3802"); });
		sub->AddOption<RegularOption>("Futo", nullptr, [&] { create_vehicle("futo"); });
		sub->AddOption<RegularOption>("GB200", nullptr, [&] { create_vehicle("gb200"); });
		sub->AddOption<RegularOption>("Go Go Monkey Blista", nullptr, [&] { create_vehicle("blista3"); });
		sub->AddOption<RegularOption>("Hotring Sabre", nullptr, [&] { create_vehicle("hotring"); });
		sub->AddOption<RegularOption>("Imorgon", nullptr, [&] { create_vehicle("imorgon"); });
		sub->AddOption<RegularOption>("Itali GTO", nullptr, [&] { create_vehicle("italigto"); });
		sub->AddOption<RegularOption>("Issi Sport", nullptr, [&] { create_vehicle("issi7"); });
		sub->AddOption<RegularOption>("Jester", nullptr, [&] { create_vehicle("jester"); });
		sub->AddOption<RegularOption>("Jester (Racecar)", nullptr, [&] { create_vehicle("jester2"); });
		sub->AddOption<RegularOption>("Jester Classic", nullptr, [&] { create_vehicle("jester3"); });
		sub->AddOption<RegularOption>("Jugular", nullptr, [&] { create_vehicle("jugular"); });
		sub->AddOption<RegularOption>("Khamelion", nullptr, [&] { create_vehicle("khamelion"); });
		sub->AddOption<RegularOption>("Kuruma", nullptr, [&] { create_vehicle("kuruma"); });
		sub->AddOption<RegularOption>("Kuruma (Armored)", nullptr, [&] { create_vehicle("kuruma2"); });
		sub->AddOption<RegularOption>("Locust", nullptr, [&] { create_vehicle("locust"); });
		sub->AddOption<RegularOption>("Lynx", nullptr, [&] { create_vehicle("lynx"); });
		sub->AddOption<RegularOption>("Massacro", nullptr, [&] { create_vehicle("massacro"); });
		sub->AddOption<RegularOption>("Massacro (Racecar)", nullptr, [&] { create_vehicle("massacro2"); });
		sub->AddOption<RegularOption>("Neo", nullptr, [&] { create_vehicle("neo"); });
		sub->AddOption<RegularOption>("Neon", nullptr, [&] { create_vehicle("neon"); });
		sub->AddOption<RegularOption>("Nightmare ZR380", nullptr, [&] { create_vehicle("zr3803"); });
		sub->AddOption<RegularOption>("Omnis", nullptr, [&] { create_vehicle("omnis"); });
		sub->AddOption<RegularOption>("Paragon R", nullptr, [&] { create_vehicle("paragon"); });
		sub->AddOption<RegularOption>("Paragon R (Armored)", nullptr, [&] { create_vehicle("paragon2"); });
		sub->AddOption<RegularOption>("Pariah", nullptr, [&] { create_vehicle("pariah"); });
		sub->AddOption<RegularOption>("Penumbra", nullptr, [&] { create_vehicle("penumbra"); });
		sub->AddOption<RegularOption>("Penumbra FF", nullptr, [&] { create_vehicle("penumbra2"); });
		sub->AddOption<RegularOption>("Raiden", nullptr, [&] { create_vehicle("raiden"); });
		sub->AddOption<RegularOption>("Rapid GT", nullptr, [&] { create_vehicle("rapidgt"); });
		sub->AddOption<RegularOption>("Rapid GT (Convertible)", nullptr, [&] { create_vehicle("rapidgt2"); });
		sub->AddOption<RegularOption>("Raptor", nullptr, [&] { create_vehicle("raptor"); });
		sub->AddOption<RegularOption>("Revolter", nullptr, [&] { create_vehicle("revolter"); });
		sub->AddOption<RegularOption>("Ruston", nullptr, [&] { create_vehicle("ruston"); });
		sub->AddOption<RegularOption>("Schafter LWB", nullptr, [&] { create_vehicle("schafter4"); });
		sub->AddOption<RegularOption>("Schafter V12", nullptr, [&] { create_vehicle("schafter3"); });
		sub->AddOption<RegularOption>("Schlagen GT", nullptr, [&] { create_vehicle("schlagen"); });
		sub->AddOption<RegularOption>("Schwartzer", nullptr, [&] { create_vehicle("schwarzer"); });
		sub->AddOption<RegularOption>("Sentinel Classic", nullptr, [&] { create_vehicle("sentinel3"); });
		sub->AddOption<RegularOption>("Seven-70", nullptr, [&] { create_vehicle("seven70"); });
		sub->AddOption<RegularOption>("Specter", nullptr, [&] { create_vehicle("specter"); });
		sub->AddOption<RegularOption>("Specter Custom", nullptr, [&] { create_vehicle("specter2"); });
		sub->AddOption<RegularOption>("Sprunk Buffalo", nullptr, [&] { create_vehicle("buffalo3"); });
		sub->AddOption<RegularOption>("Streiter", nullptr, [&] { create_vehicle("streiter"); });
		sub->AddOption<RegularOption>("Sugoi", nullptr, [&] { create_vehicle("sugoi"); });
		sub->AddOption<RegularOption>("Sultan", nullptr, [&] { create_vehicle("sultan"); });
		sub->AddOption<RegularOption>("Sultan Classic", nullptr, [&] { create_vehicle("sultan2"); });
		sub->AddOption<RegularOption>("Surano", nullptr, [&] { create_vehicle("surano"); });
		sub->AddOption<RegularOption>("Tropos Rallye", nullptr, [&] { create_vehicle("tropos"); });
		sub->AddOption<RegularOption>("Verlierer", nullptr, [&] { create_vehicle("verlierer2"); });
		sub->AddOption<RegularOption>("V-STR", nullptr, [&] { create_vehicle("vstr"); });
	});

	g_UiManager->AddSubmenu<RegularSubmenu>("Vehicle Fly", VehicleFly, [](RegularSubmenu* sub)
	{
		sub->AddOption<BoolOption<bool>>("Vehicle Fly", nullptr, &g_MainScript->IsVehicleFlying, BoolDisplay::OnOff);
		sub->AddOption<NumberOption<float>>("Fly Speed", nullptr, &g_MainScript->VehicleFlySpeed, 0.f, 10.f, 0.1f, 1);
		sub->AddOption<BoolOption<bool>>("Stop After No Input", nullptr, &g_MainScript->StopAfterNoInput, BoolDisplay::OnOff);
		sub->AddOption<BoolOption<bool>>("No Collision: ", nullptr, &g_MainScript->VehicleFlyCollision, BoolDisplay::YesNo);
	});
}
void VehicleFeatures(std::uint32_t PlayerId, std::uint32_t PedId) {

	Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PedId, false);
	if (!ENTITY::DOES_ENTITY_EXIST(veh))
		return;

	if (g_MainScript->AutoDriveOn)
	{
		NativeVector3 DriveCoords = get_blip_marker();
		if (Lists::DriveStyleListPos == 0) {
			BRAIN::TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), DriveCoords.x, DriveCoords.y, DriveCoords.z, g_MainScript->DriveSpeed, 1, 0);
		}
		else if (Lists::DriveStyleListPos == 1) {
			BRAIN::TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), DriveCoords.x, DriveCoords.y, DriveCoords.z, g_MainScript->DriveSpeed, 4, 0);
		}
		else if (Lists::DriveStyleListPos == 2) {
			BRAIN::TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), DriveCoords.x, DriveCoords.y, DriveCoords.z, g_MainScript->DriveSpeed, 2, 0);
		}
		else if (Lists::DriveStyleListPos == 3) {
			BRAIN::TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), DriveCoords.x, DriveCoords.y, DriveCoords.z, g_MainScript->DriveSpeed, 5, 0);
		}
		else if (Lists::DriveStyleListPos == 4) {
			BRAIN::TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), DriveCoords.x, DriveCoords.y, DriveCoords.z, g_MainScript->DriveSpeed, 1074528293, 0);
		}
		else if (Lists::DriveStyleListPos == 5) {
			BRAIN::TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), DriveCoords.x, DriveCoords.y, DriveCoords.z, g_MainScript->DriveSpeed, 786468, 0);
		}
		else if (Lists::DriveStyleListPos == 6) {
			BRAIN::TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), DriveCoords.x, DriveCoords.y, DriveCoords.z, g_MainScript->DriveSpeed, 2883621, 0);
		}
		g_MainScript->AutoDriveOn = false;
	}


	if (ENTITY::DOES_ENTITY_EXIST(veh))
	{
		VEHICLE::_SET_CAR_HIGH_SPEED_BUMP_SEVERITY_MULTIPLIER(g_MainScript->speed_bump_severity_multipler);
		VEHICLE::SET_CARGOBOB_PICKUP_MAGNET_STRENGTH(veh, g_MainScript->cargobob_pickup_rope_damping_multipler);
		PLAYER::SET_AIR_DRAG_MULTIPLIER_FOR_PLAYERS_VEHICLE(PLAYER::PLAYER_ID(), g_MainScript->air_drag_multipler);
		VEHICLE::SET_VEHICLE_LIGHT_MULTIPLIER(veh, g_MainScript->headlight_multipler);
		VEHICLE::SET_VEHICLE_LOD_MULTIPLIER(veh, g_MainScript->speed_bump_severity_multipler);
	}

	if (g_MainScript->VehicleRocketsBool)
	{
		if (IsKeyPressed(0x6B) && GetTickCount() % 50 == 0 && PED::IS_PED_IN_ANY_VEHICLE(PedId, 0))
		{
			NativeVector3 min, max;
			MISC::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &min, &max);
			NativeVector3 coords1 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(max.x + 0.25f), max.y + 1.25f, 0.1);
			NativeVector3 coords2 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (max.x + 0.25f), max.y + 1.25f, 0.1);
			NativeVector3 coords3 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -max.x, max.y + 100.0f, 0.1f);
			NativeVector3 coords4 = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, max.x, max.y + 100.0f, 0.1f);

			MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1.x, coords1.y, coords1.z,
				coords3.x, coords3.y, coords4.z,
				250, 1, MISC::GET_HASH_KEY("WEAPON_VEHICLE_ROCKET"), PedId, 1, 0, -1.0);
			MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1.x, coords2.y, coords2.z,
				coords3.x, coords3.y, coords4.z,
				250, 1, MISC::GET_HASH_KEY("WEAPON_VEHICLE_ROCKET"), PedId, 1, 0, -1.0);
		}
	}

	if (g_MainScript->IsVehicleFlying)
	{
		if (!g_MainScript->StopAfterNoInput) VEHICLE::SET_VEHICLE_GRAVITY(veh, true);
		else VEHICLE::SET_VEHICLE_GRAVITY(veh, false);


		NativeVector3 cam_pos = CAM::GET_GAMEPLAY_CAM_ROT(0);
		ENTITY::SET_ENTITY_ROTATION(veh, cam_pos.x, cam_pos.y, cam_pos.z, 1, TRUE);
		ENTITY::SET_ENTITY_COLLISION(veh, !g_MainScript->VehicleFlyCollision, TRUE);


		if (PAD::IS_CONTROL_PRESSED(2, 71))
			!g_MainScript->StopAfterNoInput ? ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, 0.0f, g_MainScript->VehicleFlySpeed, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1) : VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, !IsKeyPressed(VK_SHIFT) ? g_MainScript->VehicleFlySpeed * 200 : g_MainScript->VehicleFlySpeed * 200 * 2);
		if (PAD::IS_CONTROL_PRESSED(2, 72))
			!g_MainScript->StopAfterNoInput ? ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, 0.0f, 0 - (!IsKeyPressed(VK_SHIFT) ? g_MainScript->VehicleFlySpeed : g_MainScript->VehicleFlySpeed * 2), 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1) : VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 0 - (!IsKeyPressed(VK_SHIFT) ? g_MainScript->VehicleFlySpeed * 200 : g_MainScript->VehicleFlySpeed * 200 * 2));
		if (PAD::IS_CONTROL_PRESSED(2, 63))
			!g_MainScript->StopAfterNoInput ? ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, (!IsKeyPressed(VK_SHIFT) ? 0 - g_MainScript->VehicleFlySpeed : (0 - g_MainScript->VehicleFlySpeed) * 2), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1) : ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, 0 - (!IsKeyPressed(VK_SHIFT) ? g_MainScript->VehicleFlySpeed * 200 : g_MainScript->VehicleFlySpeed * 200 * 2), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
		if (PAD::IS_CONTROL_PRESSED(2, 64))
			!g_MainScript->StopAfterNoInput ? ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, !IsKeyPressed(VK_SHIFT) ? g_MainScript->VehicleFlySpeed : g_MainScript->VehicleFlySpeed * 2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1) : ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, !IsKeyPressed(VK_SHIFT) ? g_MainScript->VehicleFlySpeed * 200 : g_MainScript->VehicleFlySpeed * 200 * 2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
		if (g_MainScript->StopAfterNoInput && !PAD::IS_CONTROL_PRESSED(2, 71) && !PAD::IS_CONTROL_PRESSED(2, 72))
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 0.0f);
	}
	else
	{
		VEHICLE::SET_VEHICLE_GRAVITY(veh, true);
		ENTITY::SET_ENTITY_COLLISION(veh, TRUE, TRUE);
	}

	RequestControlOfEnt(veh);
	int var;
	DECORATOR::DECOR_REGISTER("Player_Vehicle", 3);
	DECORATOR::DECOR_REGISTER("Veh_Modded_By_Player", 3);
	DECORATOR::DECOR_SET_INT(veh, "Player_Vehicle", NETWORK::NETWORK_HASH_FROM_PLAYER_HANDLE(PlayerId));
	DECORATOR::DECOR_SET_INT(veh, "Veh_Modded_By_Player", MISC::GET_HASH_KEY(PLAYER::GET_PLAYER_NAME(PlayerId)));
	DECORATOR::DECOR_SET_INT(veh, "Not_Allow_As_Saved_Veh", 0);
	DECORATOR::DECOR_SET_INT(veh, "MPBitset", 1 << 10);
	VEHICLE::SET_VEHICLE_IS_STOLEN(veh, false);

	ENTITY::SET_ENTITY_INVINCIBLE(veh, g_MainScript->VehicleGodmodeBool);
	VEHICLE::SET_VEHICLE_ENGINE_CAN_DEGRADE(veh, !g_MainScript->VehicleGodmodeBool);
	ENTITY::SET_ENTITY_PROOFS(veh, g_MainScript->VehicleGodmodeBool, g_MainScript->VehicleGodmodeBool, g_MainScript->VehicleGodmodeBool, g_MainScript->VehicleGodmodeBool, g_MainScript->VehicleGodmodeBool, g_MainScript->VehicleGodmodeBool, g_MainScript->VehicleGodmodeBool, g_MainScript->VehicleGodmodeBool);
	VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 6969.f);
	VEHICLE::SET_VEHICLE_EXPLODES_ON_HIGH_EXPLOSION_DAMAGE(veh, !g_MainScript->VehicleGodmodeBool);
	VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 6969.f);
	VEHICLE::SET_DISABLE_VEHICLE_PETROL_TANK_DAMAGE(veh, g_MainScript->VehicleGodmodeBool);
	VEHICLE::SET_VEHICLE_CAN_BREAK(veh, !g_MainScript->VehicleGodmodeBool);
	VEHICLE::SET_DISABLE_VEHICLE_PETROL_TANK_FIRES(veh, g_MainScript->VehicleGodmodeBool);
	VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(veh, 6969.f);
	VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, !g_MainScript->VehicleGodmodeBool);
	VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, !g_MainScript->VehicleGodmodeBool);
	VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(veh, 6969.f);
	VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, !g_MainScript->VehicleGodmodeBool);
	if (VEHICLE::_IS_VEHICLE_DAMAGED(veh) && g_MainScript->VehicleGodmodeBool)
	{
		VEHICLE::SET_VEHICLE_FIXED(veh);
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0.0f);
		VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 2000.0f);
		VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(veh, 2000.0f);
		VEHICLE::SET_VEHICLE_BODY_HEALTH(veh, 2000.0f);
		VEHICLE::SET_VEHICLE_UNDRIVEABLE(veh, false);
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
	}

	if (g_MainScript->NoSpeedLimitBool) ENTITY::SET_ENTITY_MAX_SPEED(veh, 99999999);
	ENTITY::SET_ENTITY_VISIBLE(veh, !g_MainScript->InvisibiltyBool, 0);
	VEHICLE::SET_VEHICLE_REDUCE_GRIP(veh, g_MainScript->DriftModeBool);
	if (g_MainScript->AutoFixBool) {
		if (VEHICLE::_IS_VEHICLE_DAMAGED(veh)) {
			VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0);
			VEHICLE::SET_VEHICLE_FIXED(veh);
			VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(veh);
		}
	}


	if (g_MainScript->HornBoostBool && PAD::IS_CONTROL_PRESSED(2, INPUT_VEH_HORN))
	{
		if (!ENTITY::DOES_ENTITY_EXIST(veh))
			return;
		if (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(veh))
		{
			NETWORK::SET_NETWORK_ID_CAN_MIGRATE(veh, true);
			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(veh);
		}
		AUDIO::SET_VEHICLE_BOOST_ACTIVE(veh, true);

		ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, 0.0f, 1.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
		ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, 0.0f, 1.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
		ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, 0.0f, 1.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
	}
}
