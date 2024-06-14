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




void weaponSubmenu(RegularSubmenu* sub)
{
	sub->AddOption<SubOption>("Weapon Manager", nullptr, WeaponManager);
	sub->AddOption<SubOption>("Money Gun", nullptr, MoneyGun);
	sub->AddOption<SubOption>("Triggerbot", nullptr, Triggerbot);
	sub->AddOption<BoolOption<bool>>("Infinite Ammo", nullptr, &g_MainScript->InfiniteAmmoBool, BoolDisplay::YesNo);
	sub->AddOption<BoolOption<bool>>("Explosive Ammo", nullptr, &g_MainScript->ExplosiveAmmoBool, BoolDisplay::YesNo);
	sub->AddOption<BoolOption<bool>>("Explosive Melee", nullptr, &g_MainScript->ExplosiveMeleeBool, BoolDisplay::YesNo);
	sub->AddOption<BoolOption<bool>>("Flame Ammo", nullptr, &g_MainScript->FlameAmmoBool, BoolDisplay::YesNo);
	sub->AddOption<BoolOption<bool>>("One Hit Kill", nullptr, &g_MainScript->OneHitOneKillBool, BoolDisplay::YesNo);
	sub->AddOption<BoolOption<bool>>("Teleport Gun", nullptr, &g_MainScript->TeleportGunBool, BoolDisplay::YesNo);
	sub->AddOption<BoolOption<bool>>("Rapid Fire", nullptr, &g_MainScript->RapidFireBool, BoolDisplay::YesNo);
	sub->AddOption<BoolOption<bool>>("Delete Gun", nullptr, &g_MainScript->DeleteGunBool, BoolDisplay::YesNo);
	sub->AddOption<BoolOption<bool>>("Rainbow Gun", nullptr, &g_MainScript->RainbowGunBool, BoolDisplay::YesNo);
	
}
void weaponSubmenuSubs()
{
	g_UiManager->AddSubmenu<RegularSubmenu>("Triggerbot", Triggerbot, [=](RegularSubmenu* sub)
	{
		sub->AddOption<BoolOption<bool>>("Enabled", nullptr, &g_MainScript->TriggerBotBool, BoolDisplay::OnOff);
		sub->AddOption<ChooseOption<const char*, std::size_t>>("Bullet Origin", nullptr, &Lists::OriginList, &Lists::OriginListPos);
		sub->AddOption<ChooseOption<const char*, std::size_t>>("Flags", nullptr, &Lists::ModeList, &Lists::ModeListPos);
		sub->AddOption<ChooseOption<const char*, std::size_t>>("Selection", nullptr, &Lists::SelectList, &Lists::SelectListPos);
		sub->AddOption<ChooseOption<const char*, std::size_t>>("Target", nullptr, &Lists::TargetList, &Lists::TargetListPos);
		sub->AddOption<ChooseOption<const char*, std::size_t>>("Auto Shoot", nullptr, &Lists::AutoShootList, &Lists::AutoShootListPos);
		sub->AddOption<BoolOption<bool>>("Multiple Bullets", nullptr, &g_MainScript->MultiBulletsBool, BoolDisplay::YesNo);
		sub->AddOption<NumberOption<int>>("Shoot Delay", nullptr, &g_MainScript->shootDelayInt, 0, 100, 5, 1);
	});
	g_UiManager->AddSubmenu<RegularSubmenu>("Money Gun", MoneyGun, [=](RegularSubmenu* sub)
		{
			sub->AddOption<BoolOption<bool>>("Enable", nullptr, &g_MainScript->MoneyGunBool, BoolDisplay::YesNo);
			sub->AddOption<NumberOption<int>>("Value", nullptr, &g_MainScript->MoneyGunValue, 0, 2500, 10, 1);
			sub->AddOption<NumberOption<int>>("Height", nullptr, &g_MainScript->MoneyGunHeight, 0, 10, 1, 1);

		});
	const Hash mk2_weapons[] = {
	0xBFE256D4, 0x78A97CD0, 0xDBBD7280, 0x394F415C, 0xFAD1F1C9, 0xA914799
	};
	const Hash mk1_weapons[] = {
		0x99B507EA, 0x678B81B1, 0x4E875F73, 0x958A4A8F, 0x3813FC08, 0x440E4788, 0x84BD7BFD, 0x1B06D571, 0x5EF9FEC4,
		0x22D8FE39, 0x99AEEB3B, 0x13532244, 0x2BE6766B, 0xEFE7E2DF, 0xBFEFFF6D, 0x83BF0278, 0xAF113F99, 0x9D07F764,
		0x7FD62962, 0x1D073A89, 0x7846A318, 0xE284C527, 0x9D61E50F, 0x3656C8C1, 0x05FC3C11, 0x0C472FE2, 0x33058E22,
		0xA284510B, 0x4DD2DC56, 0xB1CA77B1, 0x687652CE, 0x42BF8A85, 0x93E220BD, 0x2C3731D9, 0xFDBC8A50, 0x24B17070,
		0x060EC506, 0x34A67B97, 0xFDBADCED, 0x23C9F95C, 0x497FACC3, 0xF9E6AA4B, 0x61012683, 0xC0A3098D, 0xD205520E,
		0xBFD21232, 0x7F229F94, 0x92A27487, 0x083839C4, 0x7F7497E5, 0xA89CB99E, 0x3AABBBAA, 0xC734385A, 0x787F0BB,
		0x47757124, 0xD04C944D, 0x3813FC08
	};
	const Hash weapons[] = {
		0x1b06d571, 0x5ef9fec4, 0x22d8fe39, 0x99aeeb3b, 0xd205520e, 0xdc4db296,
		0xc1b3c3d1, 0xbfd21232, 0x83839c4, 0x3656c8c1, 0x47757124,
		0x13532244, 0x2be6766b, 0xefe7e2df, 0x9d07f764, 0x7fd62962, 0xa3d4d34,
		0x61012683, 0xdb1aa450, 0xbd248b55, 0x5307A4EC,
		0xbfefff6d, 0x83bf0278, 0xaf113f99, 0xc0a3098d, 0x7f229f94,
		0x624fe830, 0x97EA20B8, 0xCB96392F, 0x88374054, 0x969C3D67, 0x84D6FAFD, 0x555AF99A, 0x6A6C02E0,
		0x1d073a89, 0x7846a318, 0xe284c527, 0x9d61e50f,
		0x3aabbbaa, 0x12e82d3d, 0xef951fbb, 0xa89cb99e,
		0x5fc3c11, 0xc472fe2, 0xc734385a,
		0xa284510b, 0xb1ca77b1, 0x63ab0442, 0x42bf8a85,
		0x6d544c99, 0x7f7497e5, 0x781fe4a,
		0x93e220bd, 0x2c3731d9, 0xab564b93, 0xfdbc8a50, 0xa0973d5e,
		0x24b17070, 0x60ec506, 0x34a67b97, 0xba45e8b8,
		0x99b507ea, 0x678b81b1, 0x4e875f73, 0x958a4a8f, 0x440e4788, 0x84bd7bfd,
		0x94117305, 0x19044ee0, 0x8bb05fd7, 0xdfe37640, 0xcd274149, 0xf9e6aa4b,
		0x92a27487, 0xf9dcbf2d, 0xdd5df8d9, 0x787f0bb, 0xd8df3c3c,
		0xfbab5776, 0xBFE256D4, 0x78A97CD0, 0x394F415C, 0xFAD1F1C9, 0xDBBD7280, 0xA914799,
		0xAF3696A1, 0x476BF155, 0xB62D1F67
	};
	g_UiManager->AddSubmenu<RegularSubmenu>("Weapon Manager", WeaponManager, [=](RegularSubmenu* sub)
	{
			sub->AddOption<RegularOption>("Give All Weapons", nullptr, [&]
				{
					for (int i = 0; i < (sizeof(weapons) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), weapons[i], 9999, 1);
				});
			sub->AddOption<RegularOption>("Give All Normal", nullptr, [&]
				{
					for (int i = 0; i < (sizeof(mk1_weapons) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), mk1_weapons[i], 9999, 1);
				});
			sub->AddOption<RegularOption>("Give All MK2", nullptr, [&]
				{
					
					for (int i = 0; i < (sizeof(mk2_weapons) / 4); i++)
						WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), mk2_weapons[i], 9999, 1);
				});
			sub->AddOption<RegularOption>("Remove All Weapons", nullptr, []
				{
					WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), true);
				});

	});
}


void WeaponFeatures(std::uint32_t PlayerId, std::uint32_t PedId)
{	
	if (g_MainScript->DeleteGunBool)
	{
		Entity Delete;
		if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &Delete)) {

			if (ENTITY::DOES_ENTITY_EXIST(Delete) && NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Delete) && PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
			{
				ENTITY::DETACH_ENTITY(Delete, 1, 1);
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Delete, 0, 0, 0, 0, 0, 0);
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Delete, 0, 1);
				ENTITY::DELETE_ENTITY(&Delete);
			}
		}
	}
	if (g_MainScript->RainbowGunBool) {
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		static LPCSTR weaponNames[] = {
			"WEAPON_KNIFE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT", "WEAPON_GOLFCLUB", "WEAPON_CROWBAR",
			"WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_MICROSMG", "WEAPON_SMG",
			"WEAPON_ASSAULTSMG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_MG",
			"WEAPON_COMBATMG", "WEAPON_PUMPSHOTGUN", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_BULLPUPSHOTGUN",
			"WEAPON_STUNGUN", "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_GRENADELAUNCHER", "WEAPON_GRENADELAUNCHER_SMOKE",
			"WEAPON_RPG", "WEAPON_MINIGUN", "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_SMOKEGRENADE", "WEAPON_BZGAS",
			"WEAPON_MOLOTOV", "WEAPON_FIREEXTINGUISHER", "WEAPON_PETROLCAN", "WEAPON_SNSPISTOL", "WEAPON_SPECIALCARBINE",
			"WEAPON_HEAVYPISTOL", "WEAPON_BULLPUPRIFLE", "WEAPON_HOMINGLAUNCHER", "WEAPON_PROXMINE", "WEAPON_SNOWBALL",
			"WEAPON_VINTAGEPISTOL", "WEAPON_DAGGER", "WEAPON_FIREWORK", "WEAPON_MUSKET", "WEAPON_MARKSMANRIFLE", "WEAPON_HEAVYSHOTGUN",
			"WEAPON_GUSENBERG", "WEAPON_HATCHET", "WEAPON_RAILGUN", "WEAPON_FLAREGUN",
			"WEAPON_KNUCKLE", "GADGET_NIGHTVISION", "GADGET_PARACHUTE", "WEAPON_MARKSMANPISTOL"
		};
		for (int i = 0; i < sizeof(weaponNames) / sizeof(weaponNames[0]); i++)
		{
			if (WEAPON::HAS_PED_GOT_WEAPON(playerPed, MISC::GET_HASH_KEY((char*)weaponNames[i]), 0))
			{

				WEAPON::SET_PED_WEAPON_TINT_INDEX(playerPed, MISC::GET_HASH_KEY((char*)weaponNames[i]), rand() % 8);

			}
		}
	}

	if (g_MainScript->RapidFireBool)
	{
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
			NativeVector3 gameplayCam = CAM::_GET_GAMEPLAY_CAM_COORDS();
			NativeVector3 gameplayCamRot = CAM::GET_GAMEPLAY_CAM_ROT(0);
			NativeVector3 gameplayCamDirection = RotationToDirectionV3(gameplayCamRot);
			NativeVector3 startCoords = AddV3(gameplayCam, (MultiplyV3(gameplayCamDirection, 1.0f)));
			NativeVector3 endCoords = AddV3(startCoords, MultiplyV3(gameplayCamDirection, 500.0f));
			Hash weaponhash;
			WEAPON::GET_CURRENT_PED_WEAPON(PedId, &weaponhash, 1);
			if (PAD::IS_CONTROL_PRESSED(2, 208) || (GetKeyState(VK_LBUTTON) & 0x8000)) {
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(startCoords.x, startCoords.y, startCoords.z, endCoords.x, endCoords.y, endCoords.z, 50, 1, weaponhash, PedId, 1, 1, 0xbf800000);
			}
		}
	}
	
	
	if (g_MainScript->InfiniteAmmoBool) {
		Hash WeaponHash;
		if (WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &WeaponHash, 1))
		{
			if (WEAPON::IS_WEAPON_VALID(WeaponHash))
			{
				int maxAmmo;
				if (WEAPON::GET_MAX_AMMO(PLAYER::PLAYER_PED_ID(), WeaponHash, &maxAmmo))
				{
					WEAPON::SET_PED_AMMO(PLAYER::PLAYER_PED_ID(), WeaponHash, maxAmmo, NULL);

					maxAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(PLAYER::PLAYER_PED_ID(), WeaponHash, 1);
					if (maxAmmo > 0)
						WEAPON::SET_AMMO_IN_CLIP(PLAYER::PLAYER_PED_ID(), WeaponHash, maxAmmo);
				}
			}
		}
	}
	if (g_MainScript->MoneyGunBool) {
		STREAMING::REQUEST_MODEL(-1666779307);
		Player playerPed = PLAYER::PLAYER_PED_ID();
		if (PAD::IS_CONTROL_PRESSED(2, 208) || (GetKeyState(VK_LBUTTON) & 0x8000))
		{
			g_CallbackScript->AddCallback<ModelCallback>(-1666779307, [=] {
				NativeVector3 ShootCoord;
				WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &ShootCoord);
				OBJECT::CREATE_AMBIENT_PICKUP(0x1E9A99F8, ShootCoord.x, ShootCoord.y, ShootCoord.z + g_MainScript->MoneyGunHeight, 0, g_MainScript->MoneyGunValue, -1666779307, FALSE, TRUE);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(-1666779307);
				});
		}
	}
	if (g_MainScript->TeleportGunBool) {
		if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
		{
			NativeVector3 ShootCoord;
			if(WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), &ShootCoord))
				ENTITY::SET_ENTITY_COORDS(PedId, ShootCoord.x, ShootCoord.y, ShootCoord.z, true, true, true, false);
		}
	}
	if (g_MainScript->ExplosiveAmmoBool) {
		NativeVector3 ExplosiveV3Coords;
		if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), &ExplosiveV3Coords))
		{
			FIRE::ADD_EXPLOSION(ExplosiveV3Coords.x, ExplosiveV3Coords.y, ExplosiveV3Coords.z, 0, 100, true, false, 0, false);
		}
	}
	if (g_MainScript->FlameAmmoBool) {
		NativeVector3 FlameV3Coords;
		if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), &FlameV3Coords))
		{
			FIRE::ADD_EXPLOSION(FlameV3Coords.x, FlameV3Coords.y, FlameV3Coords.z, 3, 100, true, false, 0, false);
		}
	}
	if (g_MainScript->OneHitOneKillBool) {
		Entity EntityTarget;
		if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &EntityTarget))
		{
			RequestControlOfEnt(EntityTarget);
			if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
			{

				if (ENTITY::IS_ENTITY_A_PED(EntityTarget)
					&& PED::IS_PED_IN_ANY_VEHICLE(EntityTarget, 1)) {
					EntityTarget = PED::GET_VEHICLE_PED_IS_IN(EntityTarget, 0);
				}
				VEHICLE::EXPLODE_VEHICLE(EntityTarget, true, false);
				VEHICLE::SET_VEHICLE_OUT_OF_CONTROL(EntityTarget, true, true);
			}
		}
		PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PLAYER::PLAYER_ID(), 9999999.9999f);
		PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(PLAYER::PLAYER_ID(), 9999999.9999f, NULL);
		PLAYER::SET_PLAYER_WEAPON_DEFENSE_MODIFIER(PLAYER::PLAYER_ID(), 9999999.9999f);
		PLAYER::SET_PLAYER_MELEE_WEAPON_DEFENSE_MODIFIER(PLAYER::PLAYER_ID(), 9999999.9999f);
	}
	if (g_MainScript->TriggerBotBool)
	{
		if (IsKeyPressed(VK_RBUTTON) || Lists::AutoShootListPos == 2)
		{
			if (Lists::ModeListPos == 1 && IsKeyPressed(VK_LBUTTON)) PLAYER::DISABLE_PLAYER_FIRING(PlayerId, FALSE);
			const int numElements = 10;
			const int arrSize = numElements * 2 + 2;
			int veh[arrSize];
			veh[0] = numElements;
			int count = PED::GET_PED_NEARBY_PEDS(PedId, veh, -1);
			Ped closest = 0;
			if (veh != NULL)
			{
				for (int i = 0; i < count; i++)
				{
					int offsettedID = i * 2 + 2;
					if (veh[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(veh[offsettedID]))
					{
						for (int j = -1; j <= 2; ++j)
						{

							Any ped = veh[offsettedID];

							NativeVector3 ped_coords = ENTITY::GET_ENTITY_COORDS(ped, TRUE);
							NativeVector3 self_head_coords = PED::GET_PED_BONE_COORDS(PedId, 0x796e, 0, 0, 0);
							NativeVector3 ped_head_coords = PED::GET_PED_BONE_COORDS(ped, 0x796e, 0, 0, 0);
							GRAPHICS::DRAW_LINE(ped_head_coords.x, ped_head_coords.y, ped_head_coords.z, self_head_coords.x, self_head_coords.y, self_head_coords.z, 255, 0, 0, 200);



							NativeVector3 self_coords = ENTITY::GET_ENTITY_COORDS(PedId, TRUE);
							if (closest == 0) closest = ped;
							else if (MISC::GET_DISTANCE_BETWEEN_COORDS(self_coords.x, self_coords.y, self_coords.z, ped_coords.x, ped_coords.y, ped_coords.z, TRUE) < MISC::GET_DISTANCE_BETWEEN_COORDS(self_coords.x, self_coords.y, self_coords.z, ENTITY::GET_ENTITY_COORDS(closest, TRUE).x, ENTITY::GET_ENTITY_COORDS(closest, TRUE).y, ENTITY::GET_ENTITY_COORDS(closest, TRUE).z, TRUE) || Lists::SelectListPos == 1) closest = ped;//                                                                                                                            
							Hash weaponhash;
							WEAPON::GET_CURRENT_PED_WEAPON(PedId, &weaponhash, 1);
							if (!WEAPON::IS_WEAPON_VALID(weaponhash)) continue;
							float screenX, screenY;
							BOOL onScreen = GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD(ENTITY::GET_ENTITY_COORDS(closest, true).x, ENTITY::GET_ENTITY_COORDS(closest, true).y, ENTITY::GET_ENTITY_COORDS(closest, true).z, &screenX, &screenY);
							if (Lists::ModeListPos == 0) onScreen = TRUE;

							

							switch ((unsigned long long)Lists::TargetListPos) {
							case 0:
								if (!PED::IS_PED_A_PLAYER(ped)) continue;
								break;
							case 1:
								if (PED::IS_PED_A_PLAYER(ped)) continue;
								break;
							case 2:

								break;
							}
							PLAYER::DISABLE_PLAYER_FIRING(PlayerId, FALSE);
							int bones[] = { 0x0,0x2e28,0xe39f,0xf9bb,0x3779,0x83c,0xfedd,0xe175,0xb3fe,0xca72,0x9000,0xcc4d,0x512d,0x8aae,0x60e6,0x3fcf,0x5c57,0x192a,0xe0fd,0x5c01,0x60f0,0x60f1,0x60f2,0xfcd9,0xb1c5,0xeeeb,0x49d9,0x67f2, 0xff9, 0xffa, 0x67f3, 0x1049, 0x104a, 0x67f4, 0x1059, 0x105a, 0x67f5, 0x1029, 0x102a, 0x67f6, 0x1039, 0x103a, 0xeb95, 0x8cbd, 0xee4f, 0x1470, 0x58b7, 0x29d2, 0x9d4d, 0x6e5c, 0xdead, 0xe5f2, 0xfa10, 0xfa11, 0xe5f3, 0xfa60, 0xfa61, 0xe5f4, 0xfa70,0xfa71, 0xe5f5,0xfa40, 0xfa41,0xe5f6,0xfa50, 0xfa51,0x6f06, 0x188e,0xab22,0x90ff,0xbb0, 0x9995,0x796e, 0x322c,0xfe2c, 0xe3db,0xb2b6, 0x62ac,0x542e, 0x74ac,0xaa10, 0x6b52,0x4b88, 0x54c,0x2ba6, 0x9149,0x4ed2, 0xf18f,0x4f37, 0x4537,0xb4a0, 0x4324,0x508f,0xb93b,0xc33b,0xb987,0x8b93,0xdd1c };

							if (((closest != NULL && !ENTITY::IS_ENTITY_DEAD(closest, 0) && onScreen) || Lists::SelectListPos == 1))
							{

								if (!ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(PedId, closest, 1000)) continue;
								g_MainScript->Text((const char*)"Target/s Found", 255, 0, 0, 255, 0.5f, 0.5f, 0.3f, 0.3f, true);

								PLAYER::DISABLE_PLAYER_FIRING(PlayerId, TRUE);


								NativeVector3 target_coords = PED::GET_PED_BONE_COORDS(closest, 0x796e, 0, 0, 0);



								if ((IsKeyPressed(VK_LBUTTON) || Lists::AutoShootListPos > 0) && GetTickCount() % (g_MainScript->shootDelayInt > 0 ? g_MainScript->shootDelayInt : 1) == 0)
								{
									if (g_MainScript->MultiBulletsBool)
									{
										for (int bone : bones)
										{
											if (Lists::OriginListPos == 1) self_head_coords = PED::GET_PED_BONE_COORDS(closest, bone, 0, 0, 0);
											PED::SET_PED_SHOOTS_AT_COORD(PLAYER::PLAYER_PED_ID(), target_coords.x, target_coords.y, target_coords.z, true);
											MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(target_coords.x, target_coords.y, target_coords.z, self_head_coords.x, self_head_coords.y, self_head_coords.z, 50, 1, weaponhash, PedId, 1, 1, 0xbf800000);
										}
									}
									else MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(self_head_coords.x, self_head_coords.y, self_head_coords.z, target_coords.x, target_coords.y, target_coords.z, 50, 1, weaponhash, PedId, 1, 1, 0xbf800000);


								}

								GRAPHICS::DRAW_LINE(target_coords.x, target_coords.y, target_coords.z, self_head_coords.x, self_head_coords.y, self_head_coords.z, 0, 255, 0, 200);
								for (int bone : bones)
								{
									NativeVector3 esp_coord = PED::GET_PED_BONE_COORDS(ped, bone, 0, 0, 0);
									GRAPHICS::DRAW_LINE(esp_coord.x, esp_coord.y, esp_coord.z, target_coords.x, target_coords.y, target_coords.z, 255, 0, 0, 100);

								}
								GRAPHICS::DRAW_MARKER(2, target_coords.x, target_coords.y, target_coords.z + 2, 0, 0, 0, 0, 180, 0, 0.5f, 0.5f, 0.5f, 255, 0, 0, 50, 0, 1, 1, 0, 0, 0, 0);

							}
						}
					}
				}
			}
		}
	}
}