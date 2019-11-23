#include "util.h"
#include "settings.h"
#include <algorithm>
#include <cctype>
#include <fcntl.h>
#include "Xorstr.hpp"



long Util::GetEpochTime()
{
	using namespace std::chrono;
	milliseconds ms = duration_cast< milliseconds >(
		system_clock::now().time_since_epoch()
		);

	return ms.count();
}

ImColor Util::GetRainbowColor(float speed)
{
	speed = 0.002f * speed;
	long now = Util::GetEpochTime();
	float hue = (now % (int)(1.0f / speed)) * speed;

	return ImColor::HSV(hue, 1.0f, 1.0f);
}

Color Util::GetHealthColor(int hp)
{
	return Color(
		min(510 * (100 - hp) / 100, 255),
		min(510 * hp / 100, 255),
		25
	);
}

//Color Util::GetHealthColor()
//{
//	return Color(
//		min(510 * (100 - 100) / 100, 255),
//		min(510 * 15 / 100, 255),
//		25
//	);
//}

//
//Color Util::GetHealthColor(int hp)
//{
//	return Color(
//		min(510 * (100 - hp) / 100, 255),
//		min(510 * hp / 100, 255),
//		25
//	);
//}