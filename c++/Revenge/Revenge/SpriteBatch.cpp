#include "SpriteBatch.h"
#include "Texture.h"
#include "MyRectangle.h"

SpriteBatch::SpriteBatch()
{
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;
}

SpriteBatch::~SpriteBatch()
{
	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();
	if (brush) brush->Release();
}

bool SpriteBatch::Init(HWND windowHandle)
{
	HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (res != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect);

	res = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),
		D2D1::HwndRenderTargetProperties(windowHandle, D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget
	);

	if (res != S_OK) return false;

	res = renderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f), &brush);

	return true;
}


void SpriteBatch::ClearScreen(float r, float g, float b)
{
	renderTarget->Clear(D2D1::ColorF(r, g, b));
}

void SpriteBatch::Draw(Texture* texture, MyRectangle rectangle, MyRectangle sourceRectangle, float transparency)
{
	ID2D1Bitmap* bmp = texture->Bitmap();

	renderTarget->DrawBitmap(
		bmp,
		rectangle.d2d1RECT(),
		transparency,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1::RectF(0.0f, 0.0f, bmp->GetSize().width, bmp->GetSize().height)
		//sourceRectangle.d2d1RECT()
	);


}