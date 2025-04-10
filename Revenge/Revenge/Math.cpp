#include "defines.h"
#include "Rectangle.h"
#include "Math.h"
#include "Manager.h"
#include "Sprite.h"


float Lerp(float start, float end, float percent)
{
	return (start + (percent * (end - start)));
}

float LerpClamped(float start, float end, float percent)
{
	float* lower, * higher;
	if (end > start)
	{
		higher = &end; lower = &start;
	}
	else
	{
		higher = &start; lower = &end;
	}

	if (percent >= 1.f)
		return *higher;
	else if (percent <= 0.f)
		return *lower;
	else
		return Lerp(start, end, percent);
}

float LerpToRange(float start_a, float start_b, float end_a, float end_b, float value)
{
	float percent = (value - start_a) / (start_b - start_a);
	return Lerp(end_a, end_b, percent);
}

float LerpToRangeClamped(float start_a, float start_b, float end_a, float end_b, float value)
{
	if (start_b > start_a)
	{
		if (value < start_a)
			return end_a;
		else if (value > start_b)
			return end_b;
	}
	else
	{
		if (value > start_a)
			return end_a;
		else if (value < start_b)
			return end_b;
	}

	return LerpToRange(start_a, start_b, end_a, end_b, value);
}

MyRectangle GetScreenLocation(MyRectangle rectangle, ANCHOR_POINT anchor)
{
	MyRectangle returnRectangle(rectangle);

	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::HCENTER)
	{
		returnRectangle.SetX(Manager::GetScreenWidth() / 2.f + rectangle.X() - rectangle.Width()/2.f);
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::RIGHT)
	{
		returnRectangle.SetX(Manager::GetScreenWidth() - rectangle.X() - rectangle.Width());
	}

	if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::VCENTER)
	{
		returnRectangle.SetY(Manager::GetScreenHeight()/ 2.f + rectangle.Y() - rectangle.Height() / 2.f);
	}
	else if ((unsigned char)anchor & (unsigned char)ANCHOR_POINT::BOTTOM)
	{
		returnRectangle.SetY(Manager::GetScreenHeight() - rectangle.Y() - rectangle.Height());
	}

	return returnRectangle;
}