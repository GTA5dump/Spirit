#pragma once
#include "Utility.h"
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
#include "ScriptCallback.hpp"
using namespace Big;
using namespace Big::UserInterface;

inline bool GodmodeBool, FireBreathBool, ShrinkPlayerBool, SuperJumpBool, PlayerAlphaBool, SuperRunBool, SlomotionBool, FastSwimBool,
		SeatBeltBool, NoClipBool, NeverWantedBool, OffTheRadarBool, ForceFieldBool, MobileRadioBool,
			FakeWantedLevel, ExplosiveMeleeBool, NightVisionBool, ThermalVisionBool, DrunkPedBool, CameraZoomBool, HealthIncreaseBool;





inline float SuperRunSpeedFloat = 1.f;
inline int WantedLevelInt;
inline float CamZoomFloat = 4.f;
inline int PlayerAlphaInt = 255;
inline int FakeWantedLevelInt = 0;
inline int PlayerHealth = 200, PlayerArmor = 100;
inline int HealthIncreaseAmount = 1, HealthIncreaseDelay = 1, HealthThreshold = 0;

inline int PedAccuracy = -1;

inline void selfSubmenu(RegularSubmenu* sub)
{
	g_UiManager->AddSubmenu<RegularSubmenu>("Model Changer", ModelChanger, [](RegularSubmenu* sub)
	{
		for (const char* model_name : g_MainScript->peds)
		{
			sub->AddOption<RegularOption>(model_name, nullptr, [&] { 
				Hash hash = MISC::GET_HASH_KEY(model_name);
				if (STREAMING::IS_MODEL_IN_CDIMAGE(hash))
				{
					if (STREAMING::IS_MODEL_VALID(hash))
					{
						g_CallbackScript->AddCallback<ModelCallback>(hash, [&] {
							STREAMING::REQUEST_MODEL(hash);
							if (STREAMING::HAS_MODEL_LOADED(hash))
							{
								PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), hash);
								STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
							}
						});
					}
				}
			});
		}
	});
	sub->AddOption<BoolOption<bool>>("Godmode", nullptr, &GodmodeBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Never Wanted", nullptr, &NeverWantedBool, BoolDisplay::OnOff);
	if (!NeverWantedBool)
	{
		sub->AddOption<NumberOption<int>>("Wanted Level", nullptr, &WantedLevelInt, 0, 5, 1, 3, true, "", "", [] {});
		PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), WantedLevelInt, false);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), TRUE);

	}
	WantedLevelInt = PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
	sub->AddOption<BoolOption<bool>>("Custom Alpha", nullptr, &PlayerAlphaBool, BoolDisplay::OnOff);
	if (PlayerAlphaBool)
	sub->AddOption<NumberOption<int>>("Player Alpha", nullptr, &PlayerAlphaInt, 0, 255, 1, 3, true, "", "", [] { ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), PlayerAlphaInt, 0); });
	sub->AddOption<ChooseOption<const char*, std::size_t>>("Invisibility", nullptr, &Lists::InvisibilitylList, &Lists::InvisibilitylListPos);
	sub->AddOption<ChooseOption<const char*, std::size_t>>("No Ragdoll", nullptr, &Lists::NoRagdollList, &Lists::NoRagdollListPos);
	sub->AddOption<BoolOption<bool>>("Fire Breath", nullptr, &FireBreathBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Seatbelt", nullptr, &SeatBeltBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("No-Clip", nullptr, &NoClipBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Super Jump", nullptr, &SuperJumpBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Fast Swim", nullptr, &FastSwimBool, BoolDisplay::OnOff);

	sub->AddOption<BoolOption<bool>>("Shrink Player", nullptr, &ShrinkPlayerBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Super Run", nullptr, &SuperRunBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Slow-Motion", nullptr, &SlomotionBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Off The Radar", nullptr, &OffTheRadarBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Explosive Melee", nullptr, &ExplosiveMeleeBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Camera Zoom", nullptr, &CameraZoomBool, BoolDisplay::OnOff);
	if (CameraZoomBool)
		sub->AddOption<NumberOption<float>>("Camera Zoom Level", nullptr, &CamZoomFloat, 0, 100, 1, 3, true, "", "", [] {});
	sub->AddOption<BoolOption<bool>>("Fake Wanted Level", nullptr, &FakeWantedLevel, BoolDisplay::OnOff);
	if (FakeWantedLevel)
		sub->AddOption<NumberOption<int>>("Fake Wanted Level Value", nullptr, &FakeWantedLevelInt, 0, 6, 1, 3, true, "", "", [] {});
	if(GodmodeBool)
		sub->AddOption<BoolOption<bool>>("Force Field", nullptr, &ForceFieldBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Mobile Radio", nullptr, &MobileRadioBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Night Vision", nullptr, &NightVisionBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Thermal Vision", nullptr, &ThermalVisionBool, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Drunk", nullptr, &DrunkPedBool, BoolDisplay::OnOff);
	sub->AddOption<NumberOption<int>>("Player Armor", nullptr, &PlayerArmor, 0, PLAYER::GET_PLAYER_MAX_ARMOUR(PLAYER::PLAYER_ID()), 1, 3, true, "", "", [] { PED::SET_PED_ARMOUR(PLAYER::PLAYER_PED_ID(), PlayerArmor); });
	sub->AddOption<NumberOption<int>>("Player Health", nullptr, &PlayerHealth, 0, ENTITY::GET_ENTITY_MAX_HEALTH(PLAYER::PLAYER_PED_ID()), 1, 3, true, "", "", [] { ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), PlayerHealth*2); });
	sub->AddOption<RegularOption>("Clear Wanted Level", nullptr, [] { PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()); });
	sub->AddOption<RegularOption>("Max Player Health", nullptr, [] { ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), ENTITY::GET_ENTITY_MAX_HEALTH(PLAYER::PLAYER_PED_ID())); });
	sub->AddOption<RegularOption>("Max Player Armour", nullptr, [] { PED::SET_PED_ARMOUR(PLAYER::PLAYER_PED_ID(), PLAYER::GET_PLAYER_MAX_ARMOUR(PLAYER::PLAYER_ID())); });
	sub->AddOption<RegularOption>("Make Player Dry", nullptr, [] { PED::CLEAR_PED_WETNESS(PLAYER::PLAYER_PED_ID()); });
	sub->AddOption<RegularOption>("Make Player Dry", nullptr, [] { ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 100); });
	sub->AddOption<BreakOption>("PVP Assist", nullptr);
	sub->AddOption<BoolOption<bool>>("Increase Health", nullptr, &HealthIncreaseBool, BoolDisplay::OnOff);
	sub->AddOption<NumberOption<int>>("Amount (HP)", nullptr, &HealthIncreaseAmount, 1, 300, 1, 3, true, "", "", [] {});
	sub->AddOption<NumberOption<int>>("Delay (Ticks)", nullptr, &HealthIncreaseDelay, 1, 1000, 1, 3, true, "", "", [] {});
	sub->AddOption<NumberOption<int>>("Health Threshold", nullptr, &HealthThreshold, 1, 300, 1, 3, true, "", "", [] {});
	sub->AddOption<NumberOption<int>>("General Accuracy", nullptr, &PedAccuracy, 1, 100, 1, 3, true, "", "", [] {});
}
inline void SelfFeatures(std::uint32_t PlayerId, std::uint32_t PedId) {

	
	
	
	ENTITY::SET_ENTITY_INVINCIBLE(PedId, GodmodeBool);
	ENTITY::SET_ENTITY_PROOFS(PedId, GodmodeBool, GodmodeBool, GodmodeBool, GodmodeBool, GodmodeBool, GodmodeBool, GodmodeBool, GodmodeBool);
	
	GRAPHICS::SET_SEETHROUGH(ThermalVisionBool);
	GRAPHICS::SET_NIGHTVISION(NightVisionBool);
	if (HealthIncreaseAmount == 0) HealthIncreaseAmount = 1;
	if (HealthIncreaseDelay == 0) HealthIncreaseDelay = 1;
	if (GetTickCount() % HealthIncreaseDelay == 0 && HealthIncreaseBool && !PED::IS_PED_DEAD_OR_DYING(PLAYER::PLAYER_PED_ID(), TRUE) && PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID()) )
	{
		ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), ENTITY::GET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID()) + HealthIncreaseAmount);
	}
	if (PedAccuracy == -1)
	{
		PedAccuracy = PED::GET_PED_ACCURACY(PLAYER::PLAYER_PED_ID());
	}

	PED::SET_PED_ACCURACY(PLAYER::PLAYER_PED_ID(), PedAccuracy);

	if(CameraZoomBool) CAM::_ANIMATE_GAMEPLAY_CAM_ZOOM(0.f, CamZoomFloat);


	AUDIO::SET_PED_IS_DRUNK(PedId, DrunkPedBool);

	if (DrunkPedBool)
	{
		STREAMING::REQUEST_ANIM_SET("move_m@drunk@verydrunk");
		if (STREAMING::HAS_ANIM_SET_LOADED("move_m@drunk@verydrunk"))
		{
			PED::SET_PED_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID(), "move_m@drunk@verydrunk", 0x3E800000);
		}
	}

	// Seems not to work in online
	if(ExplosiveMeleeBool) MISC::SET_EXPLOSIVE_MELEE_THIS_FRAME(PlayerId);

	if (FireBreathBool && GetTickCount() % 600 == 0)
	{
		float XPos = 0.02, YPos = 0.2, ZPos = 0.0, XOff = 90.0, YOff = -100.0, ZOff = 90.0;

		STREAMING::REQUEST_NAMED_PTFX_ASSET("core");
		GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL("core");
		GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("ent_sht_flame", PedId, XPos, YPos, ZPos, XOff, YOff, ZOff, 0x796e, 1, 1, 1, 1);
		STREAMING::REMOVE_PTFX_ASSET();
	}

	ENTITY::SET_ENTITY_VISIBLE(PedId, !Lists::InvisibilitylListPos, 0);
	if (Lists::InvisibilitylListPos != 2) {
		NETWORK::SET_PLAYER_VISIBLE_LOCALLY(PlayerId, true);
	}
	
	if(!NoClipBool) ENTITY::SET_ENTITY_ALPHA(PedId, Lists::InvisibilitylListPos == 1 ? 199 : 255, false);


	if(PlayerAlphaBool) ENTITY::SET_ENTITY_ALPHA(PedId, PlayerAlphaInt, false);
	else
		ENTITY::SET_ENTITY_ALPHA(PedId, 1000, false);



	if (FakeWantedLevel) {
		PLAYER::SET_PLAYER_WANTED_LEVEL(PlayerId, 5, true);
		MISC::SET_FAKE_WANTED_LEVEL(FakeWantedLevelInt);
	}
	else {
		MISC::SET_FAKE_WANTED_LEVEL(0);
	}




	if (MobileRadioBool) {
		AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		AUDIO::SET_MOBILE_PHONE_RADIO_STATE(2);
		AUDIO::SET_RADIO_TO_STATION_INDEX(AUDIO::GET_PLAYER_RADIO_STATION_INDEX());
	}
	else {
		AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
		AUDIO::SET_MOBILE_PHONE_RADIO_STATE(0);
	}




	//OFF
	if (Lists::NoRagdollListPos == 0 && !PED::CAN_PED_RAGDOLL(PedId)) PED::SET_PED_CAN_RAGDOLL(PedId, TRUE);
	//NORMAL
	if (Lists::NoRagdollListPos == 1) if (PED::CAN_PED_RAGDOLL(PedId)) PED::SET_PED_CAN_RAGDOLL(PedId, FALSE);


	if (ForceFieldBool) {
		if (GodmodeBool)
		{
			NativeVector3 ForceFieldV3 = ENTITY::GET_ENTITY_COORDS(PedId, true);
			FIRE::ADD_EXPLOSION(ForceFieldV3.x, ForceFieldV3.y, ForceFieldV3.z, 0x08, 640.00, 0, 1, 0, false);
		}
		else {
			g_Logger->Info("Turn godmode on first!");
			add_notification("Spirit", "You need to turn on godmode to use this...");
			ForceFieldBool = false;
		}
	}


	if (NoClipBool) {
		/*CAM::_CLAMP_GAMEPLAY_CAM_PITCH(-90.0f, 90.0f);
		ENTITY::SET_ENTITY_ALPHA(PedId, 199, false);
		NativeVector3 Pos = ENTITY::GET_ENTITY_COORDS(PedId, true);
		NativeVector3 Pos2 = CAM::GET_GAMEPLAY_CAM_COORD();
		float StartDistance = std::distance(&Pos2, &Pos);
		float Heading = ENTITY::GET_ENTITY_HEADING(PedId);

		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PedId, Pos.x, Pos.y, Pos.z, false, false, false);

		ENTITY::SET_ENTITY_HEADING(PedId, Heading >= 360.f ? 0 : Heading + 2.0f);
		if (GetAsyncKeyState(0x53) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 268)) { // S
			if (GetAsyncKeyState(VK_LSHIFT) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 21)) StartDistance -= 3.0;
			if (GetAsyncKeyState(VK_SPACE) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 22)) StartDistance += .5;
			StartDistance -= 1.3f;
			NativeVector3 Coords = AddV3(ENTITY::GET_ENTITY_COORDS(PedId, 1), MultiplyV3(RotationToDirectionV3(CAM::GET_GAMEPLAY_CAM_ROT(0)), StartDistance));

			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PedId, Coords.x, Coords.y, Coords.z, false, false, false);
		}
		if (GetAsyncKeyState(0x57) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 269)) { // W
			if (GetAsyncKeyState(VK_LSHIFT) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 21)) StartDistance += 3.0;
			if (GetAsyncKeyState(VK_SPACE) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, 22)) StartDistance -= .5;
			StartDistance += 2.f;
			NativeVector3 Coords = AddV3(ENTITY::GET_ENTITY_COORDS(PedId, 1), MultiplyV3(RotationToDirectionV3(CAM::GET_GAMEPLAY_CAM_ROT(0)), StartDistance));

			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PedId, Coords.x, Coords.y, Coords.z, false, false, false);
		}*/

	}



	PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PedId, SeatBeltBool);
	PED::SET_PED_CONFIG_FLAG(PedId, 32, SeatBeltBool);


	if (OffTheRadarBool) {
		globalHandle(2425869).At(PlayerId, 443).At(204).As<int>() = 1;
		globalHandle(2440049).At(70).As<int>() = NETWORK::GET_NETWORK_TIME();
	}
	else
	{
		globalHandle(2425869).At(PlayerId, 443).At(204).As<int>() = 0;
		globalHandle(2440049).At(70).As<int>() = NETWORK::GET_NETWORK_TIME() - 60;
	}


	if (SuperJumpBool) MISC::SET_SUPER_JUMP_THIS_FRAME(PlayerId);


	PED::SET_PED_CONFIG_FLAG(PedId, 223, ShrinkPlayerBool);


	if (PLAYER::GET_PLAYER_WANTED_LEVEL(PlayerId) > 0 && NeverWantedBool)
		PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PlayerId);


	PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(PlayerId, SuperRunBool ? 1.5 : 1);
	if (SuperRunBool && PAD::IS_CONTROL_PRESSED(2, 21) && !PED::IS_PED_IN_ANY_VEHICLE(PedId, TRUE) && !PED::IS_PED_RAGDOLL(PedId) && !BRAIN::IS_PED_GETTING_UP(PedId) && !BRAIN::IS_PED_STILL(PedId) && (BRAIN::IS_PED_SPRINTING(PedId) || BRAIN::IS_PED_RUNNING(PedId)))
	{
		ENTITY::APPLY_FORCE_TO_ENTITY(PedId, 1, 0.0f, SuperRunSpeedFloat, 0.0f, 0.0f, 0.0f, 0.0f, 0, 1, 1, 1, 0, 1);
	}



	if (SlomotionBool) {
		MISC::SET_TIME_SCALE(0.2);
	}
	else {
		MISC::SET_TIME_SCALE(1);
	}
}
inline void selfSubmenuSubs()
{

}