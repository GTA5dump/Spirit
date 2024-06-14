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
#include "Utility.h"

using namespace Big;
using namespace Big::UserInterface;

inline std::string GetPlayerWeapon(Player player)
{
	Hash weaponHash;
	WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player), &weaponHash, 0);
	switch (weaponHash)
	{
	case -1569615261:
		return "Unarmed";
		break;
	case -1716189206:
		return "Knife";
		break;
	case 1737195953:
		return "Nightstick";
		break;
	case 1317494643:
		return "Hammer";
		break;
	case -1786099057:
		return "Bat";
		break;
	case -2067956739:
		return "Crowbar";
		break;
	case 1141786504:
		return "Golfclub";
		break;
	case -102323637:
		return "Bottle";
		break;
	case -1834847097:
		return "Dagger";
		break;
	case -102973651:
		return "Hatchet";
		break;
	case -656458692:
		return "Knuckle Duster";
		break;
	case -581044007:
		return "Machete";
		break;
	case -1951375401:
		return "Flashlight";
		break;
	case -538741184:
		return "Switch Blade";
		break;
	case -1810795771:
		return "Poolcue";
		break;
	case 419712736:
		return "Wrench";
		break;
	case -853065399:
		return "Battle Axe";
		break;
	case 0x3813FC08:
		return "Stone Hatchet";
		break;
	case 453432689:
		return "Pistol";
		break;
	case 3219281620:
		return "Pistol Mk2";
		break;
	case 1593441988:
		return "Combat Pistol";
		break;
	case -1716589765:
		return "Pistol 50";
		break;
	case -1076751822:
		return "SNS Pistol";
		break;
	case 0x88374054:
		return "SNS Pistol Mk2";
		break;
	case -771403250:
		return "Heavy Pistol";
		break;
	case 137902532:
		return "Vintage Pistol";
		break;
	case -598887786:
		return "Marksman Pistol";
		break;
	case -1045183535:
		return "Revolver";
		break;
	case 0xCB96392F:
		return "Revolver Mk2";
		break;
	case 584646201:
		return "AP Pistol";
		break;
	case 911657153:
		return "Stun Gun";
		break;
	case 1198879012:
		return "Flare Gun";
		break;
	case 0x97EA20B8:
		return "Double Action Revolver";
		break;
	case 0xAF3696A1:
		return "Up-n-Atomizer";
		break;
	case 324215364:
		return "Micro SMG";
		break;
	case -619010992:
		return "Machine Pistol";
		break;
	case 736523883:
		return "SMG";
		break;
	case 2024373456:
		return "SMG Mk2";
		break;
	case -270015777:
		return "Assault SMG";
		break;
	case 171789620:
		return "Combat PDW";
		break;
	case -1660422300:
		return "MG";
		break;
	case 2144741730:
		return "Combat MG";
		break;
	case 368662592:
		return "Combat MG Mk2";
		break;
	case 1627465347:
		return "Gusenberg";
		break;
	case -1121678507:
		return "Mini SMG";
		break;
	case 0x476BF155:
		return "Unholy Hellbringer";
		break;
	case -1074790547:
		return "Assault Rifle";
		break;
	case 961495388:
		return "Assault Rifle Mk2";
		break;
	case -2084633992:
		return "Carbine Rifle";
		break;
	case 4208062921:
		return "Carbine Rifle Mk2";
		break;
	case -1357824103:
		return "Advanced Rifle";
		break;
	case -1063057011:
		return "Special Carbine";
		break;
	case 0x969C3D67:
		return "Special Carbine Mk2";
		break;
	case 2132975508:
		return "Bullpup Rifle";
		break;
	case 0x84D6FAFD:
		return "Bullpup Rifle Mk2";
		break;
	case 1649403952:
		return "Compact Rifle";
		break;
	case 100416529:
		return "Sniper Rifle";
		break;
	case 205991906:
		return "Heavy Sniper";
		break;
	case 177293209:
		return "Heavy Sniper Mk2";
		break;
	case -952879014:
		return "Marksman Rifle";
		break;
	case 0x6A6C02E0:
		return "Marksman Rifle Mk2";
		break;
	case 487013001:
		return "Pump Shotgun";
		break;
	case 0x555AF99A:
		return "Pump Shotgun Mk2";
		break;
	case 2017895192:
		return "Sawnoff Shotgun";
		break;
	case -1654528753:
		return "Bullpup Shotgun";
		break;
	case -494615257:
		return "Assault Shotgun";
		break;
	case -1466123874:
		return "Musket";
		break;
	case 984333226:
		return "Heavy Shotgun";
		break;
	case -275439685:
		return "Double Barrel Shotgun";
		break;
	case 317205821:
		return "Sweeper Shotgun";
		break;
	case -1568386805:
		return "Grenade Launcher";
		break;
	case -1312131151:
		return "RPG";
		break;
	case 1119849093:
		return "Minigun";
		break;
	case 2138347493:
		return "Firework";
		break;
	case 1834241177:
		return "Railgun";
		break;
	case 1672152130:
		return "Homing Launcher";
		break;
	case 1305664598:
		return "Grenade Launcher Smoke";
		break;
	case 125959754:
		return "Compact Launcher";
		break;
	case 0xB62D1F67:
		return "Widowmaker";
		break;
	case -1813897027:
		return "Grenade";
		break;
	case 741814745:
		return "Sticky Bomb";
		break;
	case -1420407917:
		return "Proximity Mine";
		break;
	case -1600701090:
		return "BZ Gas";
		break;
	case 615608432:
		return "Molotov";
		break;
	case 101631238:
		return "Fire Extinguisher";
		break;
	case 883325847:
		return "Petrol Can";
		break;
	case 1233104067:
		return "Flare";
		break;
	case 600439132:
		return "Ball";
		break;
	case 126349499:
		return "Snowball";
		break;
	case -37975472:
		return "Smoke Grenade";
		break;
	case -1169823560:
		return "Pipebomb";
		break;
	case -72657034:
		return "Parachute";
		break;

	default:
		return "None Found";
		break;
	} 
	return "Unknown";
}
inline bool ModderCheck(int player) {
	Ped PlayerPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);

	if (!INTERIOR::IS_VALID_INTERIOR(INTERIOR::GET_INTERIOR_FROM_ENTITY(PlayerPed))) {
		if (PLAYER::GET_PLAYER_INVINCIBLE(player)) {
			return true;
		}
		else {
			return false;
		}
	}


	return false;
}



inline void Playerinfo(int player, bool show = true, bool hideRID = false) {
	if (!show) return;
	Hash Street[2]{};
	float RoundedDistance{};
	Ped PlayerPed = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);
	Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(PlayerPed, false);
	NativeVector3 PedCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
	NativeVector3 Coords = ENTITY::GET_ENTITY_COORDS(PlayerPed, false);
	Hash VehicleHash = ENTITY::GET_ENTITY_MODEL(PED::GET_VEHICLE_PED_IS_IN(PlayerPed, 0));
	float distance = GetDistanceFloat(PedCoords, Coords);
	bool Connected = NETWORK::NETWORK_IS_SESSION_ACTIVE();
	bool InVehicle = PED::IS_PED_IN_ANY_VEHICLE(PlayerPed, true);
	PATHFIND::GET_STREET_NAME_AT_COORD(Coords.x, Coords.y, Coords.z, &Street[0], &Street[1]);
	
	std::ostringstream Distance; Distance << "~s~";
	if (distance > 1000)
	{
		distance = round((distance / 1000) * 100) / 100;
		Distance << distance << " Kilometers";
	}
	else
	{
		distance = round(distance * 1000) / 100;
		Distance << distance << " Meters";
	}
	

	int netHandle[13];
	NETWORK::NETWORK_HANDLE_FROM_PLAYER(player, netHandle, 13);
	const char* RID = NETWORK::NETWORK_MEMBER_ID_FROM_GAMER_HANDLE(&netHandle[0]);

	float Health = (float)ENTITY::GET_ENTITY_HEALTH(PlayerPed);
	float MaxHealth = (float)ENTITY::GET_ENTITY_MAX_HEALTH(PlayerPed);
	float HealthProgress = Health * 100 / MaxHealth;


	float Armor = (float)PED::GET_PED_ARMOUR(PlayerPed);
	float MaxArmor = (float)PLAYER::GET_PLAYER_MAX_ARMOUR(player);
	float ArmorProgress = Armor * 100 / MaxArmor;
	std::ostringstream WantedLevel; WantedLevel << "~s~" << PLAYER::GET_PLAYER_WANTED_LEVEL(player);

	long long money = globalHandle(1590682).At(player, 883).At(211).At(3).As<long long>();
	int rp = globalHandle(1590682).At(player, 883).At(211).At(5).As<int>();
	int rank = globalHandle(1590682).At(player, 883).At(211).At(6).As<int>();
	int kills = globalHandle(1590682).At(player, 883).At(211).At(28).As<int>();
	int deaths = globalHandle(1590682).At(player, 883).At(211).At(29).As<int>();
	float kd = globalHandle(1590682).At(player, 883).At(211).At(26).As<float>();
	int handle = globalHandle(1590682).At(player, 883).At(211).At(56).As<int>();
	int handle2 = globalHandle(1590682).At(player, 883).At(211).At(3).As<int>();
	int bankMoney = handle - handle2;

	float speed = ENTITY::GET_ENTITY_SPEED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player));
	int currentInterior = INTERIOR::GET_INTERIOR_FROM_ENTITY(PlayerPed);

	if (ENTITY::DOES_ENTITY_EXIST(PlayerPed)) {
		if (!NETWORK::NETWORK_IS_SESSION_STARTED()) {
			g_UiManager->Box(PLAYER::GET_PLAYER_NAME(player));
			g_UiManager->LeftRightInfo("Godmode", PLAYER::GET_PLAYER_INVINCIBLE(player) ? "~s~Yes" : "~s~No", "Distance", (char*)Distance.str().c_str());
			g_UiManager->LeftRightInfo("Alive", ENTITY::IS_ENTITY_DEAD(player, NULL) ? "~s~Yes" : "~s~No", "Inside", INTERIOR::IS_VALID_INTERIOR(currentInterior) ? "~s~Yes" : "~s~No");
			g_UiManager->LeftRightInfo("R*ID", RID, "Speed", std::to_string(speed));
			g_UiManager->LeftRightInfo("Health", std::to_string(Health), "Percent", std::to_string(HealthProgress));
			g_UiManager->LeftRightInfo("Armor", std::to_string(Armor), "Percent", std::to_string(ArmorProgress));
			g_UiManager->LeftRightInfo("Wanted", WantedLevel.str().c_str(), "Weapon", GetPlayerWeapon(player).c_str());
			g_UiManager->LeftRightInfo("KD", "Offline", "Rank", "Offline");
			g_UiManager->LeftRightInfo("Kills", "Offline", "Deaths", "Offline");
			g_UiManager->LeftRightInfo("Cash", "Offline", "Bank", "Offline");
			g_UiManager->LeftRightInfo("X", std::to_string(Coords.x), "Modder", "Offline");
			g_UiManager->LeftRightInfo("Y", std::to_string(Coords.y), "Z", std::to_string(Coords.z));
			g_UiManager->MiddleInfo("Street", &("~s~" + (std::string)HUD::GET_STREET_NAME_FROM_HASH_KEY(Street[0]))[0]);
			g_UiManager->MiddleInfo("Zone", &("~s~" + (std::string)HUD::_GET_LABEL_TEXT(ZONE::GET_NAME_OF_ZONE(Coords.x, Coords.y, Coords.z)))[0]);
		}
		else if (NETWORK::NETWORK_IS_SESSION_STARTED()){
			g_UiManager->Box(PLAYER::GET_PLAYER_NAME(player));
			g_UiManager->LeftRightInfo("Godmode", PLAYER::GET_PLAYER_INVINCIBLE(player) ? "~s~Yes" : "~s~No", "Distance", (char*)Distance.str().c_str());
			g_UiManager->LeftRightInfo("Alive", ENTITY::IS_ENTITY_DEAD(player, NULL) ? "~s~Yes" : "~s~No", "Inside", INTERIOR::IS_VALID_INTERIOR(currentInterior) ? "~s~Yes" : "~s~No");
			g_UiManager->LeftRightInfo("R*ID", !hideRID ? RID : "Hidden", "Speed", std::to_string(speed));
			g_UiManager->LeftRightInfo("Health", std::to_string(Health), "Percent", std::to_string(HealthProgress));
			g_UiManager->LeftRightInfo("Armor", std::to_string(Armor), "Percent", std::to_string(ArmorProgress));
			g_UiManager->LeftRightInfo("Wanted", WantedLevel.str().c_str(), "Weapon", GetPlayerWeapon(player).c_str());
			g_UiManager->LeftRightInfo("KD", std::to_string(kd), "Rank", std::to_string(rank));
			g_UiManager->LeftRightInfo("Kills", std::to_string(kills), "Deaths", std::to_string(deaths));
			g_UiManager->LeftRightInfo("Cash", std::to_string(money), "Bank", std::to_string(bankMoney));
			g_UiManager->LeftRightInfo("X", std::to_string(Coords.x), "Modder", ModderCheck(player) ? "~s~Yes" : "~s~No");
			g_UiManager->LeftRightInfo("Y", std::to_string(Coords.y), "Z", std::to_string(Coords.z));
			g_UiManager->MiddleInfo("Street", &("~s~" + (std::string)HUD::GET_STREET_NAME_FROM_HASH_KEY(Street[0]))[0]);
			g_UiManager->MiddleInfo("Zone", &("~s~" + (std::string)HUD::_GET_LABEL_TEXT(ZONE::GET_NAME_OF_ZONE(Coords.x, Coords.y, Coords.z)))[0]);
		}
	}
}