#pragma once
#include "ConcurrencyPCH.h"

static  const std::byte ESC{ 'H' };
static const std::byte SOF{ 0x10 };

template<typename T>
void ProcessNextByte(std::byte b, T&& frameCompleted)
{
	static std::string frame{};
	static bool inHeader{ false };
	static bool wasESC{ false };
	static bool lookingForSOF{ false };

	if (inHeader)
	{
		if ((ESC == b) && not wasESC)
		{
			wasESC = true;
			return;
		} 
		else if (wasESC)
		{
			wasESC = false;
			if ((SOF == b) || (ESC != b)) 
			{ 
				if(SOF==b) frameCompleted(frame); 
				// if b is not SOF discard the frame
				frame.clear();
				inHeader = false;
				return;
			}
		}
		
		frame += static_cast<char>(b);
	}
	else if((ESC == b) && !lookingForSOF)
	{
		lookingForSOF = true;
	}
	else if ((SOF == b) && lookingForSOF)
	{
		inHeader = true;
		lookingForSOF = false;
	}
	else
	{
		lookingForSOF = false;
	}
}