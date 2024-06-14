#include "Game.hpp"
#include "LogScript.hpp"
#include "UI/UIManager.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "MainScript.hpp"
#include "Natives.hpp"
#include "Lists.hpp"
#include "Fonts.hpp"
#ifdef _WIN32
#define IM_NEWLINE "\r\n"
#else
#define IM_NEWLINE "\n"
#endif
#include "Self.h"
#include "Network.h"
#include "D3DRenderer.hpp"
#include "Notification.h"
#include "Self.h"
#include "MainScript.hpp"
#include "Managers/VehicleManager.hpp"
#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))
namespace Big
{
	bool LogScript::IsInitialized()
	{
		return m_Initialized;
	}

	ScriptType LogScript::GetType()
	{
		return ScriptType::D3D;
	}

	void FixDisplaySize()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 curr = ImVec2(io.DisplaySize.x, io.DisplaySize.y);
		if (curr != g_D3DRenderer->m_windowSize)
		{
			g_D3DRenderer->m_windowSize = curr;
		}

	}

	void LogScript::Initialize()
	{
		ImGuiIO& io = ImGui::GetIO();
		auto font = io.Fonts->AddFontFromFileTTF("ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

		ImGui::PushFont(font);
		io.FontDefault = font;

		FixDisplaySize();

		g_D3DRenderer->m_windowSize = ImVec2(io.DisplaySize.x, io.DisplaySize.y);
		// Jay icon font v1
		// ImGui::PushFont(g_D3DRenderer->m_Spiritfont)
		g_D3DRenderer->m_Spiritfont = io.Fonts->AddFontFromMemoryCompressedBase85TTF(Spirit_compressed_data_base85, 18.f);

		ImGui::StyleColorsClassic();

		m_Initialized = true;
	}

	void LogScript::Destroy()
	{
	}

	ImVec2 resizeVec(ImVec2 vec2)
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImVec2 displaySize = ImVec2(io.DisplaySize.x, io.DisplaySize.y);
		ImVec2 normalSize = ImVec2(1920, 1080);
		float scaleX = displaySize.x < normalSize.x ? displaySize.x / normalSize.x : normalSize.x / displaySize.x;
		float scaleY = displaySize.y < normalSize.y ? displaySize.y / normalSize.y : normalSize.y / displaySize.y;
		float newScale = scaleX < scaleY ? scaleX / scaleY : scaleY / scaleX;

		if (newScale > 1.0f)
		{
			vec2.x = vec2.x / newScale;
			vec2.y = vec2.y / newScale;
		}
		else
		{
			vec2.x = vec2.x * newScale;
			vec2.y = vec2.y * newScale;
		}
		return vec2;
	}
	float resizeX(float x)
	{
		return resizeVec(ImVec2(x, 0.f)).x;
	}
	float resizeY(float y)
	{
		return resizeVec(ImVec2(0.f, y)).y;
	}
	ImVec2 _ImVec2(float x, float y)
	{
		return resizeVec(ImVec2(x, y));
	}

	constexpr std::uint32_t joaat_sub(const char* str) {
		std::uint32_t hash = 0;
		auto Str = const_cast<char*>(str);

		while (*Str)
		{
			hash += ToLower(*Str++);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}

	std::uint32_t CurrentSub = joaat_sub("Self");

	// use ImResizeText instead of ImGui::Text to fix sizes
	// Formatting doesn't work
	void ImResizeText(const char* text)
	{
		ImGui::SetWindowFontScale(resizeX(1.f));
		ImGui::Text(text);
		ImGui::SetWindowFontScale(1.f);
	}

	// Because pasting the same code billion times hurts (and i was kinda lazy to update title Text to ImResizeText)
	void SubTitle(const char* title)
	{
		ImGui::PushFont(g_D3DRenderer->m_titlefont);
		ImGui::Dummy(_ImVec2(1, 1)); ImGui::SameLine(resizeX(265));
		ImResizeText(title);
		ImGui::PopFont();
		ImGui::Separator();
	}
	bool internal_invis, internal_noragdoll;

	int internal_health, internal_armor = 100;
	bool waiting_thread_exists;
	char name_buffer[16];
	bool show_health_options = true;
	void SelfSub() {
		if (ImGui::BeginChild("SelfTab###3", _ImVec2(650, 460), false))
		{
			ImGui::PushFont(g_D3DRenderer->m_titlefont);
			ImGui::Dummy(ImVec2(1, 1)); ImGui::SameLine(265);
			ImGui::Text("Self");
			ImGui::PopFont();
			ImGui::Separator();

			if (ImGui::BeginChild("General", ImVec2(300, 210), true))
			{
				ImGui::Text("General");
				ImGui::Checkbox("Godmode", &GodmodeBool);
				ImGui::Checkbox("Never Wanted", &NeverWantedBool);
				if (!NeverWantedBool)
				{
					ImGui::Text("Wanted Level");
					ImGui::SliderInt("###WantedLevel", &WantedLevelInt, 0, 5);
					PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), WantedLevelInt, false);
					PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), TRUE);
				}
				ImGui::Checkbox("Fake Wanted Level", &FakeWantedLevel);
				if (FakeWantedLevel)
				{
					ImGui::Text("Fake Wanted Level Value");
					ImGui::SliderInt("", &FakeWantedLevelInt, 0, 6);
				}
					
				ImGui::Checkbox("No Ragdoll", &internal_noragdoll);
				ImGui::Checkbox("Off The Radar", &OffTheRadarBool);
				ImGui::Checkbox("Mobile Radio", &MobileRadioBool);
				if (ImGui::Button("Clear Wanted Level"))
					PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
				ImGui::SameLine();
				if (ImGui::Button("Make Player Dry"))
					PED::CLEAR_PED_WETNESS(PLAYER::PLAYER_PED_ID());
			}ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::BeginChild("Visual", ImVec2(300, 210), true))
			{
				ImGui::Text("Visual");
				ImGui::Checkbox("Custom Alpha", &PlayerAlphaBool);
				ImGui::Text("Player Alpha");
				ImGui::SliderInt("###121212", &PlayerAlphaInt, 0, 255);
				ImGui::Text("Invisibility");
				ImGui::ListBox("###2222222", (int*)& Lists::InvisibilitylListPos, Lists::InvisibilitylList, IM_ARRAYSIZE(Lists::InvisibilitylList), 2);
				if (internal_noragdoll)
					Lists::NoRagdollListPos = 1;
				ImGui::Checkbox("Camera Zoom", &CameraZoomBool);
				if (CameraZoomBool)
					ImGui::SliderFloat("Camera Zoom Level", &CamZoomFloat, 0, 100);
				ImGui::Checkbox("Night Vision", &NightVisionBool);
				ImGui::Checkbox("Thermal Vision", &ThermalVisionBool);
				
			}ImGui::EndChild();
			if (ImGui::BeginChild("Misc", ImVec2(300, 190), true))
			{
				ImGui::Text("Misc");
				ImGui::Checkbox("Seatbelt", &SeatBeltBool);
				ImGui::Checkbox("Shrink Player", &ShrinkPlayerBool);
				if (GodmodeBool)
					ImGui::Checkbox("Force Field", &ForceFieldBool);
				ImGui::Checkbox("Fire Breath", &FireBreathBool);
				ImGui::Checkbox("Explosive Melee", &ExplosiveMeleeBool);
			}ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::BeginChild("Movement", ImVec2(300, 190), true))
			{
				ImGui::Text("Movement");
				ImGui::Checkbox("No-Clip", &NoClipBool);
				ImGui::Checkbox("Super Run", &SuperRunBool);
				ImGui::Checkbox("Slow-Motion", &SlomotionBool);
				ImGui::Checkbox("Super Jump", &SuperJumpBool);
			}ImGui::EndChild();
			if (ImGui::BeginChild("PVP Assist", ImVec2(610, 190), true))
			{
				ImGui::Text("PVP Assist");
				ImGui::Checkbox("Increase Health", &HealthIncreaseBool);
				ImGui::SliderInt("Amount (HP)", &HealthIncreaseAmount, 0, 100);
				ImGui::SliderInt("Delay (Ticks)", &HealthIncreaseDelay, 0, 1000);
				ImGui::SliderInt("Health Threshold", &HealthThreshold, 0, 300);
				ImGui::SliderInt("General Accuracy", &PedAccuracy, 0, 100);
			}ImGui::EndChild();
			if (ImGui::BeginChild("Model Changer", ImVec2(610, 190), true))
			{
				ImGui::Text("Model Changer");
				ImGui::InputText("Custom Model", name_buffer, sizeof(name_buffer));
				ImGui::SameLine();
				if (ImGui::Button("Set"))
				{
					g_MainScript->native_stack.push_back([&] {
						Hash hash = MISC::GET_HASH_KEY(name_buffer);
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
				for (const char* model_name : g_MainScript->peds)
				{
					if(ImGui::Button(model_name)) {
						g_MainScript->native_stack.push_back([&] {
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
				}
	
			}ImGui::EndChild();

			



			ImGui::EndChild();
		}
	}
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
	void WeaponSub() {

		if (ImGui::BeginChild("WeaponTab###3", _ImVec2(650, 460), false))
		{
			ImGui::PushFont(g_D3DRenderer->m_titlefont);
			ImGui::Dummy(ImVec2(1, 1)); ImGui::SameLine(265);
			ImGui::Text("Weapon");
			ImGui::PopFont();
			ImGui::Separator();
			if (ImGui::BeginChild("General", ImVec2(220, 180), true))
			{
				ImGui::Text("General");
				ImGui::Checkbox("Infinite Ammo", &g_MainScript->InfiniteAmmoBool);
				ImGui::Checkbox("Explosive Ammo", &g_MainScript->ExplosiveAmmoBool);
				ImGui::Checkbox("Explosive Melee", &g_MainScript->ExplosiveMeleeBool);
				ImGui::Checkbox("Flame Ammo", &g_MainScript->FlameAmmoBool);
				ImGui::Checkbox("One Hit Kill", &g_MainScript->OneHitOneKillBool);
				ImGui::Checkbox("Teleport Gun", &g_MainScript->TeleportGunBool);
				ImGui::Checkbox("Rapid Fire", &g_MainScript->RapidFireBool);
				ImGui::Checkbox("Delete Gun", &g_MainScript->DeleteGunBool);
				ImGui::Checkbox("Rainbow Gun", &g_MainScript->RainbowGunBool);
			}
			ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::BeginChild("Weapon Manager", ImVec2(380, 180), true))
			{
				ImGui::Text("Weapon Manager");
				if (ImGui::Button("Give All Weapons"))
				{
					g_MainScript->native_stack.push_back([&]() {
						for (int i = 0; i < (sizeof(weapons) / 4); i++)
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), weapons[i], 9999, 1);
					});
					
					
				}
				ImGui::SameLine();
				if (ImGui::Button("Give All Normal Weapons"))
				{
					g_MainScript->native_stack.push_back([&]() {
						for (int i = 0; i < (sizeof(mk1_weapons) / 4); i++)
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), mk1_weapons[i], 9999, 1);
					});
				}
				if (ImGui::Button("Give All Mk2 Weapons"))
				{
					g_MainScript->native_stack.push_back([&]() {
						for (int i = 0; i < (sizeof(mk2_weapons) / 4); i++)
							WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), mk2_weapons[i], 9999, 1);
					});
				}
				ImGui::SameLine();
				if (ImGui::Button("Remove All Weapons"))
				{
					g_MainScript->native_stack.push_back([&]() {
						WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), true);
					});
				}
			}
			ImGui::EndChild();
			
			if (ImGui::BeginChild("Money Gun", ImVec2(610, 180), true))
			{
				ImGui::Text("Money Gun");
				ImGui::Checkbox("Enable", &g_MainScript->MoneyGunBool);
				ImGui::SliderInt("Value", &g_MainScript->MoneyGunValue, 0, 2500);
				ImGui::SliderInt("Height", &g_MainScript->MoneyGunHeight, 0, 10);
			}
			ImGui::EndChild();
			if (ImGui::BeginChild("Triggerbot", ImVec2(610, 540), true))
			{
				ImGui::Text("Triggerbot");
				ImGui::Checkbox("Enable", &g_MainScript->TriggerBotBool);
				ImGui::ListBox("Bullet Origin", (int*)& Lists::OriginListPos, Lists::OriginList, 2, 2);
				ImGui::ListBox("Flags", (int*)& Lists::ModeListPos, Lists::ModeList, 2, 2);
				ImGui::ListBox("Selection", (int*)& Lists::SelectListPos, Lists::SelectList, 2, 2);
				ImGui::ListBox("Target", (int*)& Lists::TargetListPos, Lists::TargetList, 2, 2);
				ImGui::Checkbox("Multiple Bullets", &g_MainScript->multiBullets);
				ImGui::ListBox("Auto Shoot", (int*)& Lists::AutoShootListPos, Lists::AutoShootList, 2, 2);
				ImGui::SliderInt("Shoot Delay", &g_MainScript->shootDely, 0, 100);
			}
			ImGui::EndChild();

			
		}
		

		ImGui::EndChild();
	}

	void TeleportSub() {
		if (ImGui::BeginChild("TeleportTab###3", _ImVec2(650, 460), false))
		{
			ImGui::PushFont(g_D3DRenderer->m_titlefont);
			ImGui::Dummy(ImVec2(1, 1)); ImGui::SameLine(265);
			ImGui::Text("Teleport");
			ImGui::PopFont();
			ImGui::Separator();
			if (ImGui::Button("Teleport To Waypoint"))
			{
				g_FiberScript->addTask([]() {
					NativeVector3 coords = get_blip_marker();
					Ped ped = PLAYER::PLAYER_PED_ID();

					if (coords.x == 0 && coords.y == 0)
					{
						return;
					}

					if (PED::IS_PED_IN_ANY_VEHICLE(ped, 0))
						ped = PED::GET_VEHICLE_PED_IS_USING(ped);

					bool isGroundAvailable = false;
					std::vector<float> GROUND_HEIGHT{ 100.0f, 150.0f, 50.0f, 0.0f, 200.0f, 250.0f, 300.0f, 350.0f, 400.0f, 450.0f, 500.0f, 550.0f, 600.0f, 650.0f, 700.0f, 750.0f, 800.0f };

					for (int i = 0; i < GROUND_HEIGHT.size(); i++)
					{
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, coords.x, coords.y, GROUND_HEIGHT[i], 0, 0, 1);

						g_FiberScript->Wait(100);

						if (MISC::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, GROUND_HEIGHT[i], &coords.z, 0))
						{
							isGroundAvailable = true;
							coords.z += 3.0;
							break;
						}
					}
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, coords.x, coords.y, coords.z, 0, 0, 1);
				});
			}
			if (ImGui::BeginChild("Stores", ImVec2(180, 140), true))
			{
				ImGui::Text("Stores");
				if (ImGui::Button("Mask Store"))
					g_MainScript->TP(-1338.16f, -1278.11f, 4.87f);
				if (ImGui::Button("Ponsonbys"))
					g_MainScript->TP(-718.91f, -158.16f, 37.00f);
			}
			ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::BeginChild("Barbers", ImVec2(180, 140), true))
			{
				ImGui::Text("Barbers");
				if (ImGui::Button("Vespucci"))
					g_MainScript->TP(-1294.995239f, -1117.641724f, 6.157444f);
				if (ImGui::Button("Sandy Shores"))
					g_MainScript->TP(1938.357910f, 3717.808350f, 31.607185f);
				
			}
			ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::BeginChild("Cinemas", ImVec2(180, 140), true))
			{
				ImGui::Text("Cinemas");
				if (ImGui::Button("Morningwood"))
					g_MainScript->TP(-1421.111111f, -193.111111f, 46.789345f);
				if (ImGui::Button("Ten Cent"))
					g_MainScript->TP(400.111111f, -711.111111f, 29.752497f);

			}
			ImGui::EndChild();
			
			if (ImGui::BeginChild("LSC", ImVec2(180, 175), true))
			{
				ImGui::Text("LSC");
				if (ImGui::Button("Los Santos Customs"))
					g_MainScript->TP(-361.169098f, -133.35f, 38.68f);
				if (ImGui::Button("Bennys"))
					g_MainScript->TP(-205.3417f, -1305.824f, 31.3527f);
				if (ImGui::Button("Sandy Shores"))
					g_MainScript->TP(1176.822632f, 2657.973145f, 37.370682f);

			}
			ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::BeginChild("Airports", ImVec2(180, 200), true))
			{
				ImGui::Text("Airports");
				if (ImGui::Button("Los Santos Airport"))
					g_MainScript->TP(-1070.906250f, -2972.122803f, 13.773568f);
				if (ImGui::Button("Sandy Shores"))
					g_MainScript->TP(1682.196411f, 3279.987793f, 40.647972f);
				if (ImGui::Button("Fort Zancudo"))
					g_MainScript->TP(-2446.711182f, 3142.811035f, 32.194775f);
				if (ImGui::Button("Flight School"))
					g_MainScript->TP(-1142.084229f, -2697.341553f, 13.324973f);

			}

			ImGui::EndChild();
			


			ImGui::EndChild();
		}
	}
	int target_veh_sub = 0;
	void VehicleSub() {
		if (ImGui::BeginChild("VehicleTab###3", _ImVec2(650, 460), false))
		{
			ImGui::PushFont(g_D3DRenderer->m_titlefont);
			ImGui::Dummy(ImVec2(1, 1)); ImGui::SameLine(265);
			ImGui::Text("Vehicle");
			ImGui::PopFont();
			
			if (ImGui::Button("Spawner", _ImVec2(310, 30)))
			{
				target_veh_sub = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button("Vehicle Options", _ImVec2(310, 30)))
			{
				target_veh_sub = 1;
			}
			ImGui::Separator();
			ImGui::Spacing();
			if (target_veh_sub == 0)
			{
				if (ImGui::BeginChild("VehicleLeftTab###2", _ImVec2(265, 410)))
				{
					if (ImGui::BeginChild("VehicleUpperTabLeft", _ImVec2(260, 150)))
					{
						ImGui::Checkbox("Spawn Maxed", &g_VehicleManager->SpawnMaxed);
						ImGui::Checkbox("Spawn Inside", &g_VehicleManager->SpawnInVehicle);
						if (g_VehicleManager->SpawnInVehicle)
						{
							ImGui::Checkbox("Spawn In Air", &g_VehicleManager->SpawnInAir);
							if (g_VehicleManager->SpawnInAir)
							{
								ImGui::DragFloat("Height", &g_VehicleManager->SpawnInAirZ, 1.f, 0.f, 100.f);
							}
						}
					}
					ImGui::EndChild();

					if (ImGui::BeginChild("VehicleBottomTabLeft", _ImVec2(263, 250), true, ImGuiWindowFlags_NoScrollbar))
					{
						int position = 0;
						for (const auto& dlc : vDLC())
						{
							ImGui::PushID(position);
							if (ImGui::Selectable(Lists::VehicleDLCNames[position]))
							{
								g_VehicleManager->SelectedImGuiDLC = dlc;
							}
							position++;
						}
						position = 0;
					}
					ImGui::EndChild();
				}
				ImGui::EndChild();
				ImGui::SameLine();
				if (ImGui::BeginChild("VehicleTabRight", _ImVec2(0, 400), false, ImGuiWindowFlags_NoScrollbar))
				{
					for (auto& vehicle : g_VehicleManager->GetDLC(g_VehicleManager->SelectedImGuiDLC))
					{
						if (ImGui::Button(vehicle.name, _ImVec2(385, 50)))
						{
							g_MainScript->spawn_vehicle(vehicle.model);
						}
					}
				}
				ImGui::EndChild();
			}
			if (target_veh_sub == 1)
			{
				if (ImGui::BeginChild("Vehicle Fly", ImVec2(310, 210), true))
				{
					ImGui::Text("Vehicle Fly");
					ImGui::Checkbox("Enable", &g_MainScript->IsVehicleFlying);
					ImGui::SliderFloat("Fly Speed", &g_MainScript->VehicleFlySpeed, 0, 10);
					ImGui::Checkbox("Stop After No Input", &g_MainScript->StopAfterNoInput);
					ImGui::Checkbox("No Collision", &g_MainScript->VehicleFlyCollision);
				}
				ImGui::EndChild();
				ImGui::SameLine();
				if (ImGui::BeginChild("General", ImVec2(310, 210), true))
				{
					ImGui::Text("General");
					ImGui::Checkbox("Godmode", &g_MainScript->VehicleGodmodeBool);
					ImGui::Checkbox("Invisible", &g_MainScript->InvisibiltyBool);
					ImGui::Checkbox("Auto Repair", &g_MainScript->AutoFixBool);
					if (ImGui::Button("Clean Vehicle"))
					{
						Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
						{
							GRAPHICS::WASH_DECALS_FROM_VEHICLE(Vehicle, 100.f);
						}
					}
					if (ImGui::Button("Repair Vehicle"))
					{
						Entity Vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), TRUE);

						if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false))
						{
							VEHICLE::SET_VEHICLE_DIRT_LEVEL(Vehicle, 0);
							VEHICLE::SET_VEHICLE_FIXED(Vehicle);
							VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(Vehicle);
							VEHICLE::SET_VEHICLE_ENGINE_ON(Vehicle, true, true, true);
						}
					}
					if (ImGui::Button("Max Vehicle"))
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
					}
				}
				ImGui::EndChild();
				if (ImGui::BeginChild("Movement", ImVec2(240, 210), true))
				{
					ImGui::Text("Movement");
					ImGui::Checkbox("Horn Boost", &g_MainScript->HornBoostBool);
					ImGui::Checkbox("Speed Limit Bypass", &g_MainScript->NoSpeedLimitBool);
					ImGui::Checkbox("Drift Mode", &g_MainScript->DriftModeBool);
				}
				ImGui::EndChild();
				ImGui::SameLine();
				if (ImGui::BeginChild("Multipliers", ImVec2(380, 210), true))
				{
					ImGui::Text("Multipliers");
					ImGui::Text("Air Drag Multiplier");
					ImGui::SliderFloat("###1", &g_MainScript->air_drag_multipler, 0.f, 10000.f);
					ImGui::SameLine();
					if (ImGui::Button("Reset###r1"))
						g_MainScript->air_drag_multipler = 1;
					ImGui::Text("Speed Bump Severity Multiplier");
					ImGui::SliderFloat("###2", &g_MainScript->speed_bump_severity_multipler, 0.f, 10000.f);
					ImGui::SameLine();
					if (ImGui::Button("Reset###r2"))
						g_MainScript->speed_bump_severity_multipler = 1;
					ImGui::Text("Cargobob Magnet Strength Multiplier");
					ImGui::SliderFloat("###3", &g_MainScript->cargobob_pickup_rope_damping_multipler, 0.f, 10000.f);
					ImGui::SameLine();
					if (ImGui::Button("Reset###r3"))
						g_MainScript->cargobob_pickup_rope_damping_multipler = 1;
					ImGui::Text("Headlight Intensity Multiplier");
					ImGui::SliderFloat("###4", &g_MainScript->headlight_multipler, 0.f, 10000.f);
					ImGui::SameLine();
					if (ImGui::Button("Reset###r4"))
						g_MainScript->headlight_multipler = 1;
					ImGui::Text("Level Of Detail Multiplier");
					ImGui::SliderFloat("###5", &g_MainScript->lod_multipler, 0.f, 10000.f);
					ImGui::SameLine();
					if (ImGui::Button("Reset###r5"))
						g_MainScript->lod_multipler = 1;
					ImGui::Text("Acceleration Multiplier");
					ImGui::SliderFloat("###5", &g_MainScript->Acceleration, 0.f, 10000.f);
					ImGui::Text("Brake Multiplier");
					ImGui::SliderFloat("###5", &g_MainScript->Brake, 0.f, 10000.f);
					ImGui::Text("Traction Multiplier");
					ImGui::SliderFloat("###5", &g_MainScript->Traction, 0.f, 10000.f);
					ImGui::Text("Deform Multiplier");
					ImGui::SliderFloat("###5", &g_MainScript->Deform, 0.f, 10000.f);
					ImGui::Text("Suspension Multiplier");
					ImGui::SliderFloat("###5", &g_MainScript->Suspension, 0.f, 10000.f);
					if (ImGui::Button("Apply Vehicle Changes"))
					{
						NanoMemory::set_value<float>({ 0x8, 0xD28, 0x918, 0x6C }, g_MainScript->Brake);
						NanoMemory::set_value<float>({ 0x8, 0xD28, 0x918, 0x90 }, g_MainScript->Traction);
						NanoMemory::set_value<float>({ 0x8, 0xD28, 0x918, 0xF8 }, g_MainScript->Deform);
						NanoMemory::set_value<float>({ 0x8, 0xD28, 0x918, 0xBC }, g_MainScript->Suspension);
					}
				}
				ImGui::EndChild();

			}
			

			
		}
		ImGui::EndChild();
	}

	void NetworkSub() {
		if (ImGui::BeginChild("NetworkTab###3", _ImVec2(650, 460), false))
		{
			SubTitle("Network");

			if (ImGui::BeginChild("NetworkTabLeft", _ImVec2(155, 410), true))
			{
				for (std::uint32_t i = 0; i < 32; ++i)
				{
					if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i))
					{
						if (ImGui::Selectable(PLAYER::GET_PLAYER_NAME(i))) {
							g_SelectedPlayer = i;
						}
					}
				}
			}
			ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::BeginChild("Remotes", ImVec2(300, 400), true))
			{
				ImGui::Text("Remotes");
				ImGui::Checkbox("Sound Spam", &g_MainScript->SoundSpamBool);
				if (ImGui::Button("Kick"))
				{
					Hash Kicks[] = {
						764638896, -345371965
					};
					for (int i = 0; i < 47; i++) {
						uint64_t kick[4] = { Kicks[i],  g_SelectedPlayer, 0, 0 };
						g_GameFunctions->m_TriggerScriptEvent(1, kick, 4, 1 << g_SelectedPlayer);
					}
				}
				if (ImGui::Button("PU Kick"))
				{
					Hash hash = MISC::GET_HASH_KEY("vw_prop_vw_colle_alien");

					STREAMING::REQUEST_MODEL(hash);
					NativeVector3 playerPosition = ENTITY::GET_ENTITY_COORDS(g_SelectedPlayer, true);
					OBJECT::CREATE_AMBIENT_PICKUP(0x2C014CA6, playerPosition.x, playerPosition.y, playerPosition.z, 1, 1, hash, FALSE, TRUE);
				}

				if (ImGui::Button("CEO Kick"))
				{
					uint64_t ceokick[4] = { -1648921703, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0, 0 };
					g_GameFunctions->m_TriggerScriptEvent(1, ceokick, 4, 1 << g_SelectedPlayer);
				}
				if (ImGui::Button("Infinite Loading Screen"))
				{
					uint64_t inv_inf_loading[9] = { -171207973, g_SelectedPlayer, 0, -1, 1, 1, 0, 0, 0 };
					g_GameFunctions->m_TriggerScriptEvent(1, inv_inf_loading, 9, 1 << g_SelectedPlayer);
				}
				if (ImGui::Button("Send To Mission"))
				{
					uint64_t mission[2] = { 1115000764, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer) };
					g_GameFunctions->m_TriggerScriptEvent(1, mission, 2, 1 << g_SelectedPlayer);

					uint64_t mission2[2] = { -545396442 , PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer) };
					g_GameFunctions->m_TriggerScriptEvent(1, mission2, 2, 1 << g_SelectedPlayer);
				}
			}
			ImGui::EndChild();
			if (ImGui::BeginChild("Griefing", ImVec2(300, 400), true))
			{
				ImGui::Text("Griefing");
				ImGui::Checkbox("Water Loop", &g_MainScript->WaterLoopBool);
				ImGui::Checkbox("Fire Loop", &g_MainScript->FireLoopBool);
				if (ImGui::Button("Ram"))
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
				}
				if (ImGui::Button("Airstrike"))
				{
					NativeVector3 StrikeCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1);
					StrikeCoords.z += 15;
					g_CallbackScript->AddCallback<AssetCallback>((MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET")), [=] {
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 35, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 1, MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 30, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 250, 0, MISC::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET"), 0, 1, 1, 500);
						MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(StrikeCoords.x, StrikeCoords.y, StrikeCoords.z + 55, StrikeCoords.x, StrikeCoords.y, StrikeCoords.z, 100, false, 0xF8A3939F, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true, true, 600);
						});
				}
				if (ImGui::Button("Clone"))
				{
					PED::CLONE_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1, 1, 1);
				}
				if (ImGui::Button("Explode"))
				{
					NativeVector3 ExplodeCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 1);
					FIRE::ADD_EXPLOSION(ExplodeCoords.x, ExplodeCoords.y, ExplodeCoords.z, 4, 100, true, 0, 10, 0);
				}
				if (ImGui::Button("Cage"))
				{
					NativeVector3 remotePos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), 0);
					OBJECT::CREATE_OBJECT(959275690, remotePos.x, remotePos.y, remotePos.z - 1.f, true, false, false);
				}
				if (ImGui::Button("Remove Weapons"))
				{
					WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), true);
				}
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();
		if (ImGui::BeginChild("Helpful", ImVec2(300, 400), true))
		{
			ImGui::Text("Helpful");
			if (ImGui::Button("Give All Weapons"))
			{
				Hash NormalWeapons[]{ MISC::GET_HASH_KEY("weapon_revolver_mk2"), MISC::GET_HASH_KEY("weapon_snspistol_mk2"), MISC::GET_HASH_KEY("weapon_smg_mk2"), MISC::GET_HASH_KEY("weapon_pumpshotgun_mk2"), MISC::GET_HASH_KEY("weapon_assaultrifle_mk2"), MISC::GET_HASH_KEY("weapon_carbinerifle_mk2"), MISC::GET_HASH_KEY("weapon_specialcarbine_mk2"), MISC::GET_HASH_KEY("weapon_bullpuprifle_mk2"), MISC::GET_HASH_KEY("weapon_combatmg_mk2"), MISC::GET_HASH_KEY("weapon_heavysniper_mk2"), MISC::GET_HASH_KEY("weapon_marksmanrifle_mk2"), 0xAF3696A1, 0x476BF155, 0xB62D1F670, 0xDFE37640, 0xC1B3C3D1, 0x99B507EA, 0x678B81B1, 0x4E875F73, 0x958A4A8F, 0x440E4788, 0x84BD7BFD, 0x1B06D571, 0x5EF9FEC4, 0x22D8FE39, 0x99AEEB3B, 0x13532244, 0x2BE6766B, 0xEFE7E2DF, 0xBFEFFF6D, 0x83BF0278, 0xAF113F99, 0x9D07F764, 0x7FD62962, 0x1D073A89, 0x7846A318, 0xE284C527, 0x9D61E50F, 0x3656C8C1, 0x05FC3C11, 0x0C472FE2, 0x33058E22, 0xA284510B, 0x4DD2DC56, 0xB1CA77B1, 0x687652CE, 0x42BF8A85, 0x93E220BD, 0x2C3731D9, 0xFDBC8A50, 0x24B17070, 0x060EC506, 0x34A67B97, 0xFDBADCED, 0x23C9F95C, 0x497FACC3, 0xF9E6AA4B, 0x61012683, 0xC0A3098D, 0xD205520E, 0xBFD21232, 0x7F229F94, 0x92A27487, 0x083839C4, 0x7F7497E5, 0xA89CB99E, 0x3AABBBAA, 0xC734385A, 0x787F0BB, 0x47757124, 0xD04C944D, MISC::GET_HASH_KEY("WEAPON_HATCHET"), MISC::GET_HASH_KEY("WEAPON_RAILGUN") };
				for (int i = 0; i < (sizeof(NormalWeapons) / 4); i++)
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), NormalWeapons[i], 9999, 1);
			}
			if (ImGui::Button("Give All Normal"))
			{
				Hash NormalWeapons[]{ 0xAF3696A1, 0x476BF155, 0xB62D1F670, 0xDFE37640, 0xC1B3C3D1, 0x99B507EA, 0x678B81B1, 0x4E875F73, 0x958A4A8F, 0x440E4788, 0x84BD7BFD, 0x1B06D571, 0x5EF9FEC4, 0x22D8FE39, 0x99AEEB3B, 0x13532244, 0x2BE6766B, 0xEFE7E2DF, 0xBFEFFF6D, 0x83BF0278, 0xAF113F99, 0x9D07F764, 0x7FD62962, 0x1D073A89, 0x7846A318, 0xE284C527, 0x9D61E50F, 0x3656C8C1, 0x05FC3C11, 0x0C472FE2, 0x33058E22, 0xA284510B, 0x4DD2DC56, 0xB1CA77B1, 0x687652CE, 0x42BF8A85, 0x93E220BD, 0x2C3731D9, 0xFDBC8A50, 0x24B17070, 0x060EC506, 0x34A67B97, 0xFDBADCED, 0x23C9F95C, 0x497FACC3, 0xF9E6AA4B, 0x61012683, 0xC0A3098D, 0xD205520E, 0xBFD21232, 0x7F229F94, 0x92A27487, 0x083839C4, 0x7F7497E5, 0xA89CB99E, 0x3AABBBAA, 0xC734385A, 0x787F0BB, 0x47757124, 0xD04C944D, MISC::GET_HASH_KEY("WEAPON_HATCHET"), MISC::GET_HASH_KEY("WEAPON_RAILGUN") };
				for (int i = 0; i < (sizeof(NormalWeapons) / 4); i++)
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), NormalWeapons[i], 9999, 1);
			}
			if (ImGui::Button("Give All MK2"))
			{
				Hash MK2Weapons[] = { MISC::GET_HASH_KEY("weapon_revolver_mk2"), MISC::GET_HASH_KEY("weapon_snspistol_mk2"), MISC::GET_HASH_KEY("weapon_smg_mk2"), MISC::GET_HASH_KEY("weapon_pumpshotgun_mk2"), MISC::GET_HASH_KEY("weapon_assaultrifle_mk2"), MISC::GET_HASH_KEY("weapon_carbinerifle_mk2"), MISC::GET_HASH_KEY("weapon_specialcarbine_mk2"), MISC::GET_HASH_KEY("weapon_bullpuprifle_mk2"), MISC::GET_HASH_KEY("weapon_combatmg_mk2"), MISC::GET_HASH_KEY("weapon_heavysniper_mk2"), MISC::GET_HASH_KEY("weapon_marksmanrifle_mk2"), };
				for (int i = 0; i < (sizeof(MK2Weapons) / 4); i++)
					WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_SelectedPlayer), MK2Weapons[i], 9999, 1);
			}
			ImGui::EndChild();
			if (ImGui::BeginChild("Dropping", ImVec2(300, 400), true))
			{
				ImGui::Text("Dropping");
				ImGui::SliderInt("Amount", &g_MainScript->DropAmountInt, 0, 2500);
				ImGui::SliderInt("Height", &g_MainScript->DropHeightInt, 0, 20);
				ImGui::SliderInt("Delay", &g_MainScript->DropDelayInt, 0, 1000);
				ImGui::Checkbox("Enable", &g_MainScript->DropBool);
				ImGui::Separator();
				ImGui::Checkbox("RP Drop", &g_MainScript->RPDrop);
				ImGui::Checkbox("Health Drop", &g_MainScript->HealthDrop);
				ImGui::Checkbox("Armor Drop", &g_MainScript->ArmorDrop);
				ImGui::Checkbox("Vehicle Health Drop", &g_MainScript->VHealthDrop);
				ImGui::Checkbox("Vehicle Armor Drop", &g_MainScript->VArmorDrop);
				if (ImGui::Button("Ammo Drop"))
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
				}
				if (ImGui::Button("Ped Drop 10K"))
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
				}
			}
			ImGui::EndChild();
		}
	}

	void RecoverySub() {
		if (ImGui::BeginChild("RecoveryTab###3", _ImVec2(650, 460), false))
		{
			SubTitle("Recovery");
			ImGui::SliderInt("Character", &g_MainScript->targetChar, 0, 1);
			if (ImGui::BeginChild("Money", ImVec2(300, 300), true))
			{
				ImGui::Text("Money");
				ImGui::Text("[Coming Soon]");
			}
			ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::BeginChild("Levels", ImVec2(300, 300), true))
			{
				ImGui::Text("Levels");
				ImGui::Text("Target Level: " + g_MainScript->targetLevel);
				if (ImGui::Button("Apply")) {
					g_MainScript->native_stack.push_back( [&]() {STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string) (g_MainScript->targetChar == 0 ? "MP0_" : "MP1_") + "CHAR_SET_RP_GIFT_ADMIN").c_str()), g_MainScript->Levels[(g_MainScript->targetLevel - 1)], 1); });
					add_notification("Recovery", "Please join a new session to apply.");
					
				}
				ImGui::SliderInt("Set Level", &g_MainScript->targetLevel, 0, 8000);
				ImGui::Separator();
				ImGui::Text("Presets");
				if (ImGui::Button("Rank 120")) g_MainScript->targetLevel = 120;
				if (ImGui::Button("Rank 333")) g_MainScript->targetLevel = 333;
				if (ImGui::Button("Rank 699")) g_MainScript->targetLevel = 699;
				if (ImGui::Button("Rank 1000")) g_MainScript->targetLevel = 1000;
				if (ImGui::Button("Rank 1337")) g_MainScript->targetLevel = 1337;
				if (ImGui::Button("Rank 3333")) g_MainScript->targetLevel = 3333;
				if (ImGui::Button("Rank 6666")) g_MainScript->targetLevel = 6666;
				if (ImGui::Button("Rank 8000")) g_MainScript->targetLevel = 8000;
			}
			ImGui::EndChild();
			if (ImGui::BeginChild("Unlocks", ImVec2(610, 150), true))
			{
				ImGui::Text("Unlocks");
				const char* prefix = (g_MainScript->targetChar == 0 ? "MP0" : "MP1");
				if (ImGui::Button("Tattoos"))
				{
					g_MainScript->native_stack.push_back([&]() {
						
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
					});
				}
				if (ImGui::Button("Hairstyles"))
				{
					g_MainScript->native_stack.push_back([&]() {
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR").c_str()), -1, 1);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR2").c_str()), -1, 1);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR3").c_str()), -1, 1);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR4").c_str()), -1, 1);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR5").c_str()), -1, 1);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR6").c_str()), -1, 1);
						STATS::STAT_SET_INT(MISC::GET_HASH_KEY(((std::string)prefix + "_CLTHS_AVAILABLE_HAIR7").c_str()), -1, 1);
					});
				}
				if (ImGui::Button("Achievements"))
				{
					g_MainScript->native_stack.push_back([&]() {

						for (int i = 1; i < 100; i++) {
							PLAYER::GIVE_ACHIEVEMENT_TO_PLAYER(i);
						}
					});
				}
				if (ImGui::Button("Clothing"))
				{
					g_MainScript->native_stack.push_back([&]() {

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
					});
				}
				if (ImGui::Button("LSC Upgrades"))
				{
					g_MainScript->native_stack.push_back([&]() {

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
					});
				}
				if (ImGui::Button("Fast Run"))
				{
					g_MainScript->native_stack.push_back([&]() {

						char* stats_to_change[] = { (char*)"CHAR_ABILITY_1_UNLCK", (char*)"CHAR_ABILITY_2_UNLCK", (char*)"CHAR_ABILITY_3_UNLCK", (char*)"CHAR_FM_ABILITY_1_UNLCK", (char*)"CHAR_FM_ABILITY_2_UNLCK", (char*)"CHA_FMR_ABILITY_3_UNLCK" };
						for (char* hash : stats_to_change)
						{
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY(&((std::string)"MP0_" + hash)[0]), -1, true);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY(&((std::string)"MP1_" + hash)[0]), -1, true);
						}
					});
				}
				if (ImGui::Button("Remove Fast Run"))
				{
					g_MainScript->native_stack.push_back([&]() {

						char* stats_to_change[] = { (char*)"CHAR_ABILITY_1_UNLCK", (char*)"CHAR_ABILITY_2_UNLCK", (char*)"CHAR_ABILITY_3_UNLCK", (char*)"CHAR_FM_ABILITY_1_UNLCK", (char*)"CHAR_FM_ABILITY_2_UNLCK", (char*)"CHA_FMR_ABILITY_3_UNLCK" };
						for (char* hash : stats_to_change)
						{
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY(&((std::string)"MP0_" + hash)[0]), 0, true);
							STATS::STAT_SET_INT(MISC::GET_HASH_KEY(&((std::string)"MP1_" + hash)[0]), 0, true);
						}
					});
				}

			}
			ImGui::EndChild();
		}
	}

	void ProtectionsSub() {
		if (ImGui::BeginChild("ProtectionsTab###3", _ImVec2(650, 460), false))
		{
			SubTitle("Protections");
			if (ImGui::BeginChild("Blocks", ImVec2(300, 300), true))
			{
				ImGui::Text("Blocks");
				ImGui::Checkbox("Fire", &g_MainScript->FireProtection);
				ImGui::Checkbox("Time", &g_MainScript->TimeProtection);
				ImGui::Checkbox("Weather", &g_MainScript->WeatherProtection);
				ImGui::Checkbox("Explosion", &g_MainScript->ExplosionProtection);
				ImGui::Checkbox("Give Weapons", &g_MainScript->GiveWeaponProtection);
				ImGui::Checkbox("Remove Weapons", &g_MainScript->RemoveWeaponProtection);
				ImGui::Checkbox("Other", &g_MainScript->OtherProtectionBool);
			}
			ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::BeginChild("Events", ImVec2(300, 300), true))
			{
				ImGui::Text("Events");
				ImGui::Checkbox("Kick", &g_MainScript->KickProtection);
				ImGui::Checkbox("Wanted", &g_MainScript->WantedProtection);
				ImGui::Checkbox("Bounty", &g_MainScript->BountyProtection);
				ImGui::Checkbox("Invite", &g_MainScript->InviteProtectionBool);
				ImGui::Checkbox("CEO Kick", &g_MainScript->Ceo_KickProtection);
				ImGui::Checkbox("CEO Ban", &g_MainScript->CeobanProtection);
				ImGui::Checkbox("CEO Money", &g_MainScript->CeoMoneyProtectionBool);
				ImGui::Checkbox("Sound Spam", &g_MainScript->SoundSpamProtection);
				ImGui::Checkbox("Transaction", &g_MainScript->TransactionProtection);
				ImGui::Checkbox("Send To Job", &g_MainScript->JobProtection);
				ImGui::Checkbox("Loading Screen", &g_MainScript->InfLoadingProtection);
				ImGui::Checkbox("Block Passive Mode", &g_MainScript->PassiveModeProtection);
				ImGui::Checkbox("Remote Off The Radar", &g_MainScript->OfftheradarProtection);
				ImGui::Checkbox("Send To Cayo Perico", &g_MainScript->Cayoperico);

			}
			ImGui::EndChild();
			if (ImGui::BeginChild("Other", ImVec2(610, 300), true))
			{
				ImGui::Text("Other");
				ImGui::Checkbox("Kick On Attack", &g_MainScript->RedirectEvent);
				ImGui::Checkbox("Block Money Pickup", &g_MainScript->MoneyPickup);
				ImGui::Checkbox("Bypass Chat Censor", &g_MainScript->ChatCensor);
				ImGui::Checkbox("Transaction Error Screen", &g_MainScript->TransactionError);

			}
			ImGui::EndChild();
		}
	}

	void MiscSub() {
		if (ImGui::BeginChild("MiscTab###3", _ImVec2(650, 460), false))
		{
			SubTitle("Misc");
			if (ImGui::BeginChild("General", _ImVec2(240, 150), true))
			{
				ImGui::Text("General");
				ImGui::Checkbox("Hide HUD", &g_MainScript->HideHudBool);
				if (ImGui::Button("Skip Current Cutscene"))
				{
					CUTSCENE::STOP_CUTSCENE(true);
					CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
				}
			}
			ImGui::EndChild();
			ImGui::SameLine();
			if (ImGui::BeginChild("Online", _ImVec2(380, 150), true))
			{
				ImGui::Text("Online");

				ImGui::Checkbox("Reset Orbital Canon Cooldown Loop", &g_MainScript->OrbitalCanonBool);
				if (ImGui::Button("Reset Orbital Canon Cooldown"))
				{
					STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP0_ORBITAL_CANNON_COOLDOWN"), 0, 0);
					STATS::STAT_SET_INT(MISC::GET_HASH_KEY("MP1_ORBITAL_CANNON_COOLDOWN"), 0, 0);
				}
			}
			ImGui::EndChild();
			if (ImGui::BeginChild("Drive Utility", _ImVec2(630, 400), true))
			{
				ImGui::Text("Drive Utility");
				ImGui::SliderInt("Drive Speed", &g_MainScript->DriveSpeed, 0, 100);
				ImGui::ListBox("Drive Style", (int*)&Lists::DriveStyleListPos, Lists::DriveStylelList, IM_ARRAYSIZE(Lists::DriveStylelList));
				if (ImGui::Button("Drive To Waypoint"))
				{
					g_MainScript->AutoDriveOn = true;
				}
				ImGui::SameLine();
				if (ImGui::Button("Stop Auto-Drive"))
				{
					BRAIN::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
					ENTITY::SET_ENTITY_HEADING(PLAYER::GET_PLAYERS_LAST_VEHICLE(), ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
					PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), PLAYER::GET_PLAYERS_LAST_VEHICLE(), -1);
				}
			}
		}
	}
	char buffer[30] = {};
	bool internal_manual_save;
	void SettingsSub() {
		if (ImGui::BeginChild("SettingsTab###3", _ImVec2(650, 460), false))
		{
			SubTitle("Settings");
			if (ImGui::BeginChild("SettingsTabLeft", _ImVec2(200, 300), true))
			{
				ImResizeText("General Settings");
				ImGui::Checkbox("Watermark", &g_MainScript->watermark);
				ImGui::Checkbox("ImGui", &g_MainScript->ImGUIMode);
				if (ImGui::Button("Unload"))
				{
					g_Running = false;
				}
			}
			ImGui::EndChild();

			ImGui::SameLine();

			if (ImGui::BeginChild("SettingsTabRight", _ImVec2(420, 300), true))
			{
				ImResizeText("Config Manager");
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Text("Manual Save Name");
				ImGui::InputText("###ManualSaveame", buffer, IM_ARRAYSIZE(buffer));
				ImGui::Checkbox("Manual Save Value", &internal_manual_save);
				if (ImGui::Button("Save"))
					g_MainScript->SaveOption(buffer, internal_manual_save);
				ImGui::Spacing();
				if (ImGui::Button("Reload Config"))
					g_MainScript->loadSavedOptions();
				if (ImGui::Button("Save Config"))
					g_MainScript->SaveWholeMenu();
			}
			ImGui::EndChild();
			ImGui::Separator();
			ImGui::Text("Made with <3. Team Spirit.");
			ImGui::Text("Updated and cracked with <3 - The Unkown Group");
			ImGui::EndChild();
		}
	}


	void LogScript::Tick()
	{
		if (GetTickCount() % 150 == 0) {
			FixDisplaySize();
		}

		if (g_MainScript->watermark) {
			ImGui::SetNextWindowSize(_ImVec2(470.f, 70.f));
			if (ImGui::Begin("Watermark", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
			{
				ImGui::SetWindowPos(_ImVec2((g_D3DRenderer->m_windowSize.x - resizeX(480)), resizeY(10)), true);
				ImVec2 mainWindowPos;
				ImVec2 mainWindowSize;
				ImGui::SetScrollX(0.f);
				mainWindowPos = ImGui::GetWindowPos();
				mainWindowSize = ImGui::GetWindowSize();
				time_t now = time(0); struct tm tstruct;
				tstruct = *localtime(&now); char buf[80];
				strftime(buf, sizeof(buf), "%c", &tstruct);
				ImGui::PushFont(g_D3DRenderer->m_nasafont);
				ImGui::Text("Spiritmenu | %.1f FPS | %s", ImGui::GetIO().Framerate, buf);
				ImGui::Text("Crack by The Unkown Group | Made with love <3");
				ImGui::SetWindowFontScale(1.f);
				ImGui::SetWindowFontScale(resizeX(1.f));
				ImGui::PopFont();
			}
			ImGui::End();
		}
		int count = 0;
		bool shouldclear = true;
		for (Notification notif : notifications)
		{
			if ((GetTickCount() - notif.created_at) < 7150)
			{
				if (!strcmp(notif.title, "") || !strcmp(notif.body_text, ""))
					continue;
				shouldclear = false;
				ImGui::SetNextWindowSize(_ImVec2(350.f, 80.f));
				if (ImGui::Begin(((std::string)std::to_string(notif.created_at).c_str() + notif.body_text).c_str(), NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
				{
					ImGui::SetWindowPos(_ImVec2((g_D3DRenderer->m_windowSize.x - resizeX(350)) - resizeX(20), resizeY(90 + (100 * count))), false);

					ImGui::SetScrollX(0.f);

					ImGui::PushFont(g_D3DRenderer->m_nasafont);
					ImResizeText(notif.title);
					ImGui::Separator();
					ImGui::SetWindowFontScale(resizeX(0.8));
					ImGui::Text(notif.body_text);
					ImGui::PopFont();
					ImGui::End();
					count++;
				}

			}
		}

		if (g_Settings.m_LogWindow && g_MainScript->ImGUIMode && g_UiManager->m_Opened)
		{
			ImGui::SetNextWindowSize(_ImVec2(800, 500));
			if (ImGui::Begin("Main", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
			{
				ImVec2 mainWindowPos;
				ImVec2 mainWindowSize;

				ImGui::SetScrollX(0.f);
				mainWindowPos = ImGui::GetWindowPos();
				mainWindowSize = ImGui::GetWindowSize();
				//https://www.dafont.com/heydings-icons.font
				ImGui::PushFont(g_D3DRenderer->m_iconfont);
				if (ImGui::BeginChild("MainTab###1", _ImVec2(100, 470), false))
				{
					if (ImGui::Button("H", _ImVec2(70, 70))) // Self
					{
						CurrentSub = joaat_sub("Self");
					}

					if (ImGui::Button("@", _ImVec2(70, 70))) // Weapon
					{
						CurrentSub = joaat_sub("Weapon");
					}

					if (ImGui::Button("P", _ImVec2(70, 70))) // Teleport
					{
						CurrentSub = joaat_sub("Teleport");
					}

					if (ImGui::Button("4", _ImVec2(70, 70))) // Vehicle
					{
						CurrentSub = joaat_sub("Vehicle");
					}

					if (ImGui::Button("a", _ImVec2(70, 70))) // Network
					{
						CurrentSub = joaat_sub("Network");
					}

					if (ImGui::Button("t", _ImVec2(70, 70))) // Recovery
					{
						CurrentSub = joaat_sub("Recovery");
					}

					if (ImGui::Button("k", _ImVec2(70, 70))) // Protections
					{
						CurrentSub = joaat_sub("Protections");
					}

					if (ImGui::Button("r", _ImVec2(70, 70))) // Misc
					{
						CurrentSub = joaat_sub("Misc");
					}

					if (ImGui::Button("G", _ImVec2(70, 70))) // Settings
					{
						CurrentSub = joaat_sub("Settings");
					}
					ImGui::EndChild();
				}
				ImGui::PopFont();

				ImGui::SameLine(resizeX(130));

				if (ImGui::BeginChild("Features###2", _ImVec2(650, 470), false))
				{
					switch (CurrentSub)
					{
					case joaat_sub("Self"):
					{ SelfSub(); } break;

					case joaat_sub("Weapon"):
					{ WeaponSub(); } break;

					case joaat_sub("Teleport"):
					{ TeleportSub(); } break;

					case joaat_sub("Vehicle"):
					{ VehicleSub(); } break;

					case joaat_sub("Network"):
					{ NetworkSub(); } break;

					case joaat_sub("Recovery"):
					{ RecoverySub(); } break;

					case joaat_sub("Protections"):
					{ ProtectionsSub(); } break;

					case joaat_sub("Misc"):
					{ MiscSub(); } break;

					case joaat_sub("Settings"):
					{ SettingsSub(); } break;
					}

					ImGui::EndChild();
				}
			}
			ImGui::End();

		}
	}
}
