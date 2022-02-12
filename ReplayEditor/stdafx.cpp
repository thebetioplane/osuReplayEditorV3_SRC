#include "stdafx.h"
#include <sstream>

HWND error_message_owner = nullptr;

static void call_fatal(const std::string &msg)
{
	static bool already_called = false;
	if (!already_called)
	{
		already_called = true;
		MessageBoxA(error_message_owner, msg.c_str(), "error in dll (exiting)", MB_ICONERROR);
		exit(1);
	}
}

static void call_not_fatal(const std::string &msg)
{
	MessageBoxA(error_message_owner, msg.c_str(), "warning", MB_ICONINFORMATION);
}

#ifdef _DEBUG
void _fatal(const std::string &msg, const char *file, const int line)
{
	std::stringstream ss;
	ss << "fatal: " << msg << " (" << file << ":" << line << ")";
	call_fatal(ss.str());
}

void _not_fatal(const std::string &msg, const char *file, const int line)
{
	std::stringstream ss;
	ss << "warning: " << msg << " (" << file << ":" << line << ")";
	call_not_fatal(ss.str());
}
#else
void fatal(const std::string &msg)
{
	call_fatal(msg);
}

void not_fatal(const std::string &msg)
{
	call_not_fatal(msg);
}
#endif

void _assert(bool what, const char *file, const int line)
{
	if (!what) {
		std::stringstream ss;
		ss << "assert failed (" << file << ":" << line << ")";
		call_fatal(ss.str());
		exit(1);
	}
}