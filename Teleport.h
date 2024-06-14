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
#include "Fiber.hpp"
using namespace Big;
using namespace Big::UserInterface;

void teleportSubmenu(RegularSubmenu* sub)
{
	sub->AddOption<RegularOption>("Teleport To Waypoint", nullptr, []
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
		});
	sub->AddOption<SubOption>("Stores", nullptr, Stores);
	sub->AddOption<SubOption>("Barbers", nullptr, Barbers);
	sub->AddOption<SubOption>("Cinemas", nullptr, Cinemas);
	sub->AddOption<SubOption>("Airports", nullptr, Airports);
	sub->AddOption<SubOption>("LSC", nullptr, LSC);
}
void teleportSubmenuSubs()
{
	g_UiManager->AddSubmenu<RegularSubmenu>("Stores", Stores, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Mask Store", nullptr, []
				{
					g_MainScript->TP(-1338.16f, -1278.11f, 4.87f);
				});

			sub->AddOption<RegularOption>("Ponsonbys", nullptr, []
				{
				g_MainScript->TP(-718.91f, -158.16f, 37.00f);
				});
		});
	g_UiManager->AddSubmenu<RegularSubmenu>("Barbers", Barbers, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Vespucci", nullptr, []
				{
				g_MainScript->TP(-1294.995239f, -1117.641724f, 6.157444f);
				});
			sub->AddOption<RegularOption>("Sandy Shores", nullptr, []
				{
				g_MainScript->TP(1938.357910f, 3717.808350f, 31.607185f);
				});
		});
	g_UiManager->AddSubmenu<RegularSubmenu>("Cinemas", Cinemas, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Morningwood", nullptr, []
				{
				g_MainScript->TP(-1421.111111f, -193.111111f, 46.789345f);
				});
			sub->AddOption<RegularOption>("Ten Cent", nullptr, []
				{
				g_MainScript->TP(400.111111f, -711.111111f, 29.752497f);
				});
		});
	g_UiManager->AddSubmenu<RegularSubmenu>("Airports", Airports, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Los Santos Airport", nullptr, []
				{
				g_MainScript->TP(-1070.906250f, -2972.122803f, 13.773568f);
				});
			sub->AddOption<RegularOption>("Sandy Shores", nullptr, []
				{
				g_MainScript->TP(1682.196411f, 3279.987793f, 40.647972f);
				});
			sub->AddOption<RegularOption>("Fort Zancudo", nullptr, []
				{
				g_MainScript->TP(-2446.711182f, 3142.811035f, 32.194775f);
				});
			sub->AddOption<RegularOption>("Flight School", nullptr, []
				{
				g_MainScript->TP(-1142.084229f, -2697.341553f, 13.324973f);
				});


		});
	g_UiManager->AddSubmenu<RegularSubmenu>("LSC", LSC, [](RegularSubmenu* sub)
		{
			sub->AddOption<RegularOption>("Los Santos Customs", nullptr, []
				{
				g_MainScript->TP(-361.169098f, -133.35f, 38.68f);
				});

			sub->AddOption<RegularOption>("Bennys", nullptr, []
				{
				g_MainScript->TP(-205.3417f, -1305.824f, 31.3527f);
				});
			sub->AddOption<RegularOption>("Sandy Shores", nullptr, []
				{
				g_MainScript->TP(1176.822632f, 2657.973145f, 37.370682f);
				});
		});
}