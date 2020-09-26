#pragma once

class MyRectangle;
enum class ANCHOR_POINT : unsigned char;

float Lerp(float start, float end, float percent);

float LerpClamped(float start, float end, float percent);

float LerpToRange(float start_a, float start_b, float end_a, float end_b, float value);

float LerpToRangeClamped(float start_a, float start_b, float end_a, float end_b, float value);

MyRectangle GetScreenLocation(MyRectangle rectangle, ANCHOR_POINT anchor);