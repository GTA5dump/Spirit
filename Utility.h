#pragma once
#include "Game.hpp"
#include "Fiber.hpp"
#include <codecvt>
#include "NanoMemory.h"

namespace Big
{
	inline void RequestControlOfid(Entity netid)
	{
		int tick = 0;

		while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(netid) && tick <= 25)
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(netid);
			tick++;
		}
	}
	inline void RequestControlOfEnt(Entity entity)
	{
		int tick = 0;
		while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(entity) && tick <= 25)
		{
			NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(entity);
			tick++;
		}
		if (NETWORK::NETWORK_IS_SESSION_STARTED())
		{
			int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(entity);
			RequestControlOfid(netID);
			NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, 1);
		}
	}
	inline NativeVector3 RotationToDirectionV3(NativeVector3 rot) {
		NativeVector3 dir;
		float radiansZ = rot.z * 0.0174532924f;
		float radiansX = rot.x * 0.0174532924f;
		float num = abs((float)cos((double)radiansX));
		dir.x = (float)((double)((float)(-(float)sin((double)radiansZ))) * (double)num);
		dir.y = (float)((double)((float)cos((double)radiansZ)) * (double)num);
		dir.z = (float)sin((double)radiansX);
		return dir;
	}

	inline NativeVector3 AddV3(NativeVector3 vectorA, NativeVector3 vectorB) {
		NativeVector3 result;
		result.x = vectorA.x;
		result.y = vectorA.y;
		result.z = vectorA.z;
		result.x += vectorB.x;
		result.y += vectorB.y;
		result.z += vectorB.z;
		return result;
	}

	inline NativeVector3 MultiplyV3(NativeVector3 vector, float x) {
		NativeVector3 result;
		result.x = vector.x;
		result.y = vector.y;
		result.z = vector.z;
		result.x *= x;
		result.y *= x;
		result.z *= x;
		return result;
	}

	inline float GetDistanceFloat(NativeVector3 pointA, NativeVector3 pointB) {
		float a_x = pointA.x;
		float a_y = pointA.y;
		float a_z = pointA.z;
		float b_x = pointB.x;
		float b_y = pointB.y;
		float b_z = pointB.z;
		double x_ba = (double)(b_x - a_x);
		double y_ba = (double)(b_y - a_y);
		double z_ba = (double)(b_z - a_z);
		double y_2 = y_ba * y_ba;
		double x_2 = x_ba * x_ba;
		double sum_2 = y_2 + x_2;
		return(float)sqrt(sum_2 + z_ba);
	}

	inline float GetVectorLengthFloat(NativeVector3* vector) {
		double x = (double)vector->x;
		double y = (double)vector->y;
		double z = (double)vector->z;
		return(float)sqrt(x * x + y * y + z * z);
	}


	inline NativeVector3 get_blip_marker()
	{
		static NativeVector3 zero;
		NativeVector3 coords;

		bool blipFound = false;
		// search for marker blip
		int blipIterator = HUD::_GET_BLIP_INFO_ID_ITERATOR();
		for (Blip i = HUD::GET_FIRST_BLIP_INFO_ID(blipIterator); HUD::DOES_BLIP_EXIST(i) != 0; i = HUD::GET_NEXT_BLIP_INFO_ID(blipIterator))
		{
			if (HUD::GET_BLIP_INFO_ID_TYPE(i) == 4)
			{
				coords = HUD::GET_BLIP_INFO_ID_COORD(i);
				blipFound = true;
				break;
			}
		}
		if (blipFound)
		{
			return coords;
		}

		return zero;
	}

	inline void WriteStringToIni(std::string string, std::string file, std::string app, std::string key)
	{
		WritePrivateProfileStringA(app.c_str(), key.c_str(), string.c_str(), file.c_str());
	}

	inline std::string ReadStringFromIni(std::string file, std::string app, std::string key)
	{
		char buf[100];
		GetPrivateProfileStringA(app.c_str(), key.c_str(), "NULL", buf, 100, file.c_str());
		return (std::string)buf;
	}

	inline void WriteIntToIni(int intValue, std::string file, std::string app, std::string key)
	{
		WriteStringToIni(std::to_string(intValue), file, app, key);
	}

	inline int ReadIntFromIni(std::string file, std::string app, std::string key)
	{
		return std::stoi(ReadStringFromIni(file, app, key));
	}

	inline void WriteFloatToIni(float floatValue, std::string file, std::string app, std::string key)
	{
		WriteStringToIni((std::to_string(floatValue)), file, app, key);
	}

	inline float ReadFloatFromIni(std::string file, std::string app, std::string key)
	{
		return std::stof(ReadStringFromIni(file, app, key));
	}

	inline void WriteBoolToIni(bool b00l, std::string file, std::string app, std::string key)
	{
		WriteStringToIni(b00l ? "true" : "false", file, app, key);
	}

	inline bool ReadBoolFromIni(std::string file, std::string app, std::string key)
	{
		return ReadStringFromIni(file, app, key) == "true" ? true : false;
	}

	inline std::string read_configfile(std::string App, std::string Key)
	{
		static uint32_t* (*pRegisterFile)(int*, const char*, bool, const char*, bool) = reinterpret_cast<decltype(pRegisterFile)>(NanoMemory::pattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 50 48 8B EA 4C 8B FA 48 8B D9 4D 85 C9").count(1).get(0).get<void>(0));

		std::string  pathapp;
		pathapp = getenv("appdata");

		std::string path = pathapp + "\\Spirit\\Login.ini";


		return ReadStringFromIni(path, App, Key);
	}

	inline std::wstring s2ws(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}

	inline wchar_t* GetWC(const char* c)
	{
		size_t cSize = strlen(c) + 1;
		wchar_t* wc = new wchar_t[cSize];
		mbstowcs(wc, c, cSize);

		return wc;
	}
}