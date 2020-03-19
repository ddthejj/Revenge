#pragma once

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
	if (start_b > start_a )
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