#pragma once
#include "UI/UIManager.hpp"

namespace Big::Lists
{
	inline std::size_t DropModelPosition = 0;
	inline const char* DropModels[] = { "PROP_MONEY_BAG_01", "PROP_POLY_BAG_01", "PROP_PAPER_BAG_01" };
	inline const char* DropModelsName[]{ "Money Bag", "Poly Bag", "Paper Bag" };
	inline std::size_t DropPickupPosition = 0;
	inline const char* DropPickups[] = { "PICKUP_MONEY_PAPER_BAG", "PICKUP_MONEY_CASE", "PICKUP_MONEY_PURSE", "PICKUP_MONEY_SECURITY_CASE", "PICKUP_MONEY_VARIABLE", "PICKUP_MONEY_WALLET" };
	inline const char* DropPickupsName[]{ "Money Paper Bag", "Money Case", "Money Purse", "Security Case", "Money Variable", "Money Wallet" };

	inline const char* VehicleDLCNames[]
	{
		"The Cayo Perico",
		"Los Santos Summer Special",
		"The Diamond Casino Heist",
		"The Diamond Casino & Resort",
		"Arena Wars",
		"After Hours",
		"Southern SA",
		"The Doomsday Heist",
		"Smuggler's Run",
		"Gunnrunning Pack",
		"Special Vehicle Circuit",
		"Import/Export",
		"Bikers DLC",
		"Cunning Stunts",
		"Futher Adventures",
		"Lowriders Customs",
		"Be My Valentine",
		"January 2016",
		"Festive Surprise 2015",
		"Executive And Other Criminals",
		"Halloween Surprise",
		"Lowriders",
		"Gotten Gains Part 2",
		"Gotten Gains Part 1",
		"Heists DLC",
		"Festive Surprise 2014",
		"Next Gen DLC",
		"Last Team Standing",
		"SA Flight School",
		"Independence Day",
		"I'm Not Hipster",
		"High Life",
		"Business",
		"Valentine's Day Massacre",
		"Beach Bum",
	};

	inline const char* HeaderTypesFrontend[]
	{
		"STATIC",
		"GRADIENT"
	};

	inline UserInterface::HeaderType HeaderTypesBackend[]
	{
		UserInterface::HeaderType::Static,
		UserInterface::HeaderType::Gradient
	};

	inline std::size_t HeaderTypesPosition = 1;

	inline const char* DemoList[10]
	{
		"ONE",
		"TWO",
		"THREE",
		"FOUR",
		"FIVE",
		"SIX",
		"SEVEN",
		"EIGHT",
		"NINE",
		"TEN"
	};
	inline const char* TargetList[3]
	{
		"PLAYER",
		"PED",
		"BOTH"
	};
	inline const char* SelectList[2]
	{
		"SINGULAR",
		"EVERYONE"
	};
	inline const char* ModeList[2]
	{
		"IN & OUT FOV",
		"IN FOV"
	};
	inline const char* OriginList[2]
	{
		"FROM SELF",
		"FROM TARGET BODY"
	};

	inline const char* AutoShootList[3]
	{
		"OFF",
		"WHILE AIMING",
		"ALWAYS"
	};

	inline const char* MesurementList[3]
	{
		"METRIC",
		"IMPERIAL"
	};

	inline const char* NoRagdollList[2]
	{
		"OFF",
		"NORMAL"
	};
	inline std::size_t NoRagdollListPos = 0;
	inline const char* InvisibilitylList[3]
	{
		"OFF",
		"LOCAL",
		"EVERYONE"
	};
	inline std::size_t DriveStyleListPos = 0;
	inline const char* DriveStylelList[7]
	{
		"NORMAL",
		"AVOID VEHICLES",
		"AVOID PEDS",
		"OVERTAKE",
		"RUSHED",
		"AVOID TRAFFIC",
		"IGNORE LIGHTS"
	};
	inline std::size_t InvisibilitylListPos = 0;
	inline const char* VisionlList[3]
	{
		"OFF",
		"NIGHT",
		"THERMAL"
	};
	inline std::size_t MesurementListPos = 0;
	inline std::size_t VisionlListPos = 0;
	inline std::size_t AutoShootListPos = 0;
	inline std::size_t ModeListPos = 0;
	inline std::size_t DemoListPos = 0;
	inline std::size_t TargetListPos = 2;
	inline std::size_t SelectListPos = 1;
	inline std::size_t OriginListPos = 1;
}
