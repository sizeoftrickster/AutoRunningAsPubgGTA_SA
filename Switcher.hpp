#ifndef _SWITCHER_HPP_
#define _SWITCHER_HPP_

class Switcher
{
protected:
	inline static bool& isMenuOpened = *reinterpret_cast<bool*>( 0xBA67A4 );
	inline static bool isEnabled = false;
}; // class Switcher

#endif // !_SWITCHER_HPP_