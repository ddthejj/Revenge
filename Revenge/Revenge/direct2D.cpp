#pragma once
#include "Renderer.h"
#include <d2d1.h>
#include <wincodec.h>
#include <vector>
#include <algorithm>

#pragma comment(lib, "d2d1.lib")

ID2D1Factory* factory = NULL;
ID2D1HwndRenderTarget* renderTarget = NULL;

struct ObjectDraw
{
	enum ROTATIONS
	{
		NONE = 0,
		HORIZONTAL,
		VERTICAL,
		ROT_90,
		ROT_90_VERTICAL,
		ROT_180,
		ROT_270,
		ROT_270_VERTICAL
	};

	unsigned int textureID;
	D2D1_RECT_F destination, source;
	float opacity, layer;
	ROTATIONS rotation = NONE;

	ObjectDraw(unsigned int _textureID, D2D1_RECT_F _destination, float _opacity, D2D1_RECT_F _source, float _layer, int rot = 0)
	{
		textureID = _textureID; destination = _destination; opacity = _opacity; source = _source; layer = _layer; rotation = (ROTATIONS)rot;
	}
};

std::vector<ID2D1Bitmap*> bitmaps;
std::vector<PCWSTR> filenames;
std::vector<ObjectDraw*> draws;

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

HRESULT LoadBitmapFromFile(
	ID2D1RenderTarget *pRenderTarget,
	IWICImagingFactory *pIWICFactory,
	PCWSTR uri, // file name
	UINT destinationWidth,
	UINT destinationHeight,
	ID2D1Bitmap **ppBitmap
)
{
	IWICBitmapFrameDecode *Source = NULL;
	IWICBitmapDecoder *Decoder = NULL;
	IWICStream *Stream = NULL;
	IWICFormatConverter *Converter = NULL;
	IWICBitmapScaler *Scaler = NULL;

	// Create the decoder to reformat the WIC image into D2D filetypes
	// This automatically loads the image
	HRESULT res = pIWICFactory->CreateDecoderFromFilename(
		uri, 
		NULL, 
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, 
		&Decoder
	);

	// Decode the file
	if (SUCCEEDED(res))
	{
		res = Decoder->GetFrame(0, &Source);
	}

	// Creates the converter to convert the file to the right format
	if (SUCCEEDED(res))
	{
		res = pIWICFactory->CreateFormatConverter(&Converter);
	}


	// Scaling

	// If there is scaling
	if (SUCCEEDED(res) && (destinationWidth != 0 || destinationHeight != 0))
	{
		unsigned int originalWidth, originalHeight;
		// Get the width and height of the image
		res = Source->GetSize(&originalWidth, &originalHeight);
		if (destinationWidth == 0)
		{
			// Scale the width the same as the height
			float scalar = (float)(destinationHeight) / (float)(originalHeight);
			destinationWidth = (unsigned int)(scalar * (float)(originalWidth));
		}
		else if (destinationHeight == 0)
		{
			// Scale the height the same as the width
			float scalar = (float(destinationWidth) / (float)(originalWidth));
			destinationHeight = (unsigned int)(scalar * (float)(originalHeight));
		}

		// Create the scaler
		if (SUCCEEDED(res))
		{
			res = pIWICFactory->CreateBitmapScaler(&Scaler);
		}
		// Initialize the scaler
		if (SUCCEEDED(res))
		{
			res = Scaler->Initialize(
				Source, 
				destinationWidth,
				destinationHeight,
				WICBitmapInterpolationModeCubic
			);
		}
		// Initialize the converter with the scaler
		if (SUCCEEDED(res))
		{
			res = Converter->Initialize(
				Scaler, 
				GUID_WICPixelFormat32bppPBGRA, 
				WICBitmapDitherTypeNone,
				NULL, 
				0.f, 
				WICBitmapPaletteTypeMedianCut
			);
		}
	}
	// If there's no scaling
	else
	{
		// Initialize the converter with the source
		if (SUCCEEDED(res))
		{
			res = Converter->Initialize(
				Source,
				GUID_WICPixelFormat32bppPBGRA,
				WICBitmapDitherTypeNone,
				NULL,
				0.0f,
				WICBitmapPaletteTypeMedianCut
			);
		}
	}

	// Create the Direct2D bitmap from the WIC bitmap
	if (SUCCEEDED(res))
	{
		res = pRenderTarget->CreateBitmapFromWicBitmap(
			Converter,
			NULL,
			ppBitmap
		);
	}

	SafeRelease(&Decoder);
	SafeRelease(&Source);
	SafeRelease(&Stream);
	SafeRelease(&Converter);
	SafeRelease(&Scaler);

	return res;
}

Renderer::Renderer(HWND hwnd)
{
	HRESULT res;

	res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);

	D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		D2D1_ALPHA_MODE_PREMULTIPLIED
	);
	D2D1_RENDER_TARGET_PROPERTIES rtp =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(
				DXGI_FORMAT_UNKNOWN,
				D2D1_ALPHA_MODE_PREMULTIPLIED
			)
		);

	RECT rc;
	GetClientRect(hwnd, &rc);
	res = factory->CreateHwndRenderTarget(
		rtp,
		D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&renderTarget
	);
}

Renderer::~Renderer()
{
	//SafeRelease(&imageFactory);
	for (int i = 0; i < bitmaps.size(); i++)
	{
		SafeRelease(&bitmaps[i]);
	}
	bitmaps.clear();

	SafeRelease(&renderTarget);
	SafeRelease(&factory);
}

bool Renderer::Begin()
{
	renderTarget->BeginDraw();
	renderTarget->Clear(D2D1::ColorF(0, 0, 0, 0));//D2D1::ColorF::Black));

	return true;
}

bool Renderer::Draw(unsigned int textureID, 
	float x, float y, int width, int height, 
	float x_S, float y_S, int width_S, int height_S, 
	float opacity, float layer, int rot)
{
	if (textureID >= bitmaps.size())
		return false;

	draws.push_back(
		new ObjectDraw(
			textureID, 
			D2D1::RectF(x, y, x + width, y + height), 
			opacity, 
			D2D1::RectF(x_S, y_S, x_S + width_S, y_S + height_S), 
			layer,
			rot)
	);

	return true;
}

bool Renderer::End()
{
	std::sort(
		draws.begin(),
		draws.end(),
		[](const ObjectDraw* lhs, const ObjectDraw* rhs)
	{
		return lhs->layer < rhs->layer;
	}
	);


	for (int i = 0; i < draws.size(); i++)
	{
		ObjectDraw* object = draws[i];

		if (object->rotation == ObjectDraw::NONE)
		{
			renderTarget->DrawBitmap(
				bitmaps[object->textureID],
				object->destination,
				object->opacity,
				D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
				object->source
			);
		}
		else
		{
			HRESULT res;

			ID2D1Bitmap* originalBitmap = bitmaps[object->textureID];
			ID2D1Bitmap* rotatedBitmap = NULL;
			//(bitmaps[object->textureID]);

			// get the frame
			IWICImagingFactory* imagingfactory = NULL;
			IWICFormatConverter* converter = NULL;
			IWICBitmapFrameDecode* frame = NULL;
			IWICBitmapDecoder* decoder = NULL;
			IWICBitmapFlipRotator* rotator = NULL;

			res = CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&imagingfactory)
			);

			if (SUCCEEDED(res))
			{
				D2D1_SIZE_U origsize;
				origsize.width = (UINT32)originalBitmap->GetSize().width;
				origsize.height = (UINT32)originalBitmap->GetSize().height;

				D2D1_BITMAP_PROPERTIES origproperties;
				origproperties.pixelFormat = originalBitmap->GetPixelFormat();
				originalBitmap->GetDpi(&origproperties.dpiX, &origproperties.dpiY);

				renderTarget->CreateBitmap(origsize, origproperties, &originalBitmap);
			}

			if (SUCCEEDED(res))
				res = imagingfactory->CreateDecoderFromFilename(
					filenames[object->textureID],
					NULL,
					GENERIC_READ,
					WICDecodeMetadataCacheOnLoad,
					&decoder
				);

			if (SUCCEEDED(res))
				res = decoder->GetFrame(0, &frame);

			if (SUCCEEDED(res))
				res = imagingfactory->CreateFormatConverter(&converter);

			if (SUCCEEDED(res))
			{
				imagingfactory->CreateBitmapFlipRotator(&rotator);
			}

			if (SUCCEEDED(res))
			{
				switch (object->rotation)
				{
				case ObjectDraw::HORIZONTAL:
					res = rotator->Initialize(frame, WICBitmapTransformFlipHorizontal);
					break;
				case ObjectDraw::VERTICAL:
					res = rotator->Initialize(frame, WICBitmapTransformFlipVertical);
					break;
				case ObjectDraw::ROT_90:
					res = rotator->Initialize(frame, WICBitmapTransformRotate90);
					break;
				case ObjectDraw::ROT_90_VERTICAL:
					res = rotator->Initialize(frame, (WICBitmapTransformOptions)(WICBitmapTransformRotate90 | WICBitmapTransformFlipVertical));
					break;
				case ObjectDraw::ROT_180:
					res = rotator->Initialize(frame, WICBitmapTransformRotate180);
					break;
				case ObjectDraw::ROT_270:
					res = rotator->Initialize(frame, WICBitmapTransformRotate270);
					break;
				case ObjectDraw::ROT_270_VERTICAL:
					res = rotator->Initialize(frame, (WICBitmapTransformOptions)(WICBitmapTransformRotate270 | WICBitmapTransformFlipVertical));
					break;
				}
				if (SUCCEEDED(res))
					res = converter->Initialize(
						rotator,
						GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone,
						NULL,
						0.f,
						WICBitmapPaletteTypeMedianCut
					);

				res = renderTarget->CreateBitmapFromWicBitmap(
					converter,
					&rotatedBitmap
				);

				renderTarget->DrawBitmap(
					rotatedBitmap,
					object->destination,
					object->opacity,
					D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
					object->source
				);
			}

			SafeRelease(&rotatedBitmap);
			SafeRelease(&imagingfactory);
			SafeRelease(&converter);
			SafeRelease(&frame);
			SafeRelease(&decoder);
			SafeRelease(&rotator);
		}
	}

	for (int i = 0; i < draws.size(); i++)
		delete draws[i];

	draws.clear();

	HRESULT hr =
	renderTarget->EndDraw();


	if (SUCCEEDED(hr))
		return true;
	else
		return false;
}
	 

int Renderer::LoadContent(const wchar_t* filePath, float height, float width)
{
	IWICImagingFactory* imageFactory = NULL;
	//ID2D1Bitmap* bmp;
	unsigned int index = (int)(bitmaps.size());
	bitmaps.push_back(nullptr);
	filenames.push_back(filePath);

	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&imageFactory)
	);
	hr = LoadBitmapFromFile(renderTarget, imageFactory, filePath,
		(UINT)height, (UINT)width, &bitmaps.at(index));

	
	SafeRelease(&imageFactory);

	if (!SUCCEEDED(hr))
		return -1;

	//bitmaps.push_back(bmp);
	return index;
}

bool Renderer::Resize(HWND hWnd)
{
	if (!factory)
		return false;

	RECT rc;
	GetClientRect(hWnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

	renderTarget->Resize(size);

	InvalidateRect(hWnd, NULL, FALSE);

	return true;
}