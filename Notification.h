#pragma once
#include "Hooking.hpp"
#include "Game.hpp"
#include "ScriptManager.hpp"
struct Notification
{
	const char* title;
	const char* body_text;
	DWORD created_at;

};
inline std::vector<Notification> notifications = {};

inline long notifications_id = 0;

inline void add_notification(const char* title, const char* body)
{
	notifications.insert(notifications.begin(), { title, body, GetTickCount()} );
	notifications_id++;
}
inline void log_notification(const char* body)
{
	notifications.insert(notifications.begin(), { "Info:", body, GetTickCount() });
	notifications_id++;
}
