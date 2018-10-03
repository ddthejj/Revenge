#include "MyRectangle.h"
#include <d2d1.h>

MyRectangle::~MyRectangle()
{
}
D2D_RECT_F MyRectangle::d2d1RECT()
{
	return D2D1::RectF(Left(), Top(), Right(), Bottom());
}