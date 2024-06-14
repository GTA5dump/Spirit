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

using namespace Big;
using namespace Big::UserInterface;

const char* manual_option;
bool manual_value;

void settingsSubmenu(RegularSubmenu* sub)
{
	sub->AddOption<BoolOption<bool>>("ImGui Mode", nullptr, &g_MainScript->ImGUIMode, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Watermark", nullptr, &g_MainScript->watermark, BoolDisplay::OnOff);
	sub->AddOption<BoolOption<bool>>("Disable All Input (ImGui)", nullptr, &g_MainScript->DisableImGuiInput, BoolDisplay::OnOff);
	sub->AddOption<BreakOption>("Config Manager", nullptr);
	sub->AddOption<RegularOption>("Reload Config", "Reloads whole menu with current config", [] { 
		g_MainScript->loadSavedOptions();
	});
	sub->AddOption<RegularOption>("Save Config", "Saves whole menu with current config", [] {
		g_MainScript->SaveWholeMenu();
	});
	sub->AddOption<BreakOption>("--------------", nullptr);

	sub->AddOption<RegularOption>("Unload", "Uninjects menu.", [] { g_Running = false; });
}
void settingsSubmenuSubs()
{


}