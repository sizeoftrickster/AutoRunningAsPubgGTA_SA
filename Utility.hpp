#ifndef _UTILITY_HPP_
#define _UTILITY_HPP_

#include <Windows.h>
#include <Lmcons.h>
#include <string>

class Utility
{
public:
	static std::string GetSystemUsername() {
		char username[UNLEN + 1];
		DWORD username_len = UNLEN + 1;
		GetUserNameA( username, &username_len );
		return username;
	}
}; // class Utility

#endif // !_UTILITY_HPP_