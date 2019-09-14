#pragma once
#include "defines.h"
#include "Renderer.h"
#include "Manager.h"
#include <d2d1.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <vector>
#include <algorithm>
#include <string>
#include <stringapiset.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite.lib")

// safely release directX objects
template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = nullptr;
	}
}

#pragma region Structs

// elements required by directX
struct Renderer::Impl_Elements
{
	ID2D1Factory* factory = nullptr;					// factory for creating other objects
	ID2D1HwndRenderTarget* renderTarget = nullptr;		// the surface to draw onto
	IDWriteFactory5* writeFactory = nullptr;			// factory for creating other write objects
	IDWriteTextFormat* textFormat = nullptr;			// format for how text will be drawn
	ID2D1SolidColorBrush* textBrush = nullptr;			// the brush to draw text with

	std::vector<ID2D1Bitmap*> bitmaps;					// list of loaded textures
	std::vector<PCWSTR> filenames;						// list of loaded textures' file names
	std::vector<Renderer::ToDraw*> draws;				// list of objects to draw

	Impl_Elements(HWND hwnd);
	~Impl_Elements() { Clear(); }
	void Clear();
	bool Begin();
	bool End();

	void Draw(const ObjectDraw* object);
	void Draw(const TextDraw* text);
};

struct Renderer::ToDraw
{
	float layer = 0;
	virtual ~ToDraw()
	{

	}
	virtual void Draw(Impl_Elements* elements) = 0;
};

struct Renderer::ObjectDraw : public Renderer::ToDraw
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
	float opacity;
	ROTATIONS rotation = NONE;

	ObjectDraw(unsigned int _textureID, D2D1_RECT_F _destination, float _opacity, D2D1_RECT_F _source, float _layer, int rot = 0)
	{
		textureID = _textureID; destination = _destination; opacity = _opacity; source = _source; layer = _layer; rotation = (ROTATIONS)0;//rot;
	}
	virtual ~ObjectDraw();
	void Draw(Impl_Elements* elements);
};

struct Renderer::TextDraw : public Renderer::ToDraw
{
	std::string* text;
	D2D1_RECT_F destination;

	TextDraw(std::string* _text, D2D1_RECT_F _destination, float _layer) { text = _text; destination = _destination; layer = _layer; }
	virtual ~TextDraw();
	void Draw(Impl_Elements* elements);
};

#pragma endregion

HRESULT LoadBitmapFromFile(
	ID2D1RenderTarget *pRenderTarget,
	IWICImagingFactory *pIWICFactory,
	PCWSTR uri, // file name
	UINT destinationWidth,
	UINT destinationHeight,
	ID2D1Bitmap **ppBitmap
)
{
	IWICBitmapFrameDecode *Source = nullptr;
	IWICBitmapDecoder *Decoder = nullptr;
	IWICStream *Stream = nullptr;
	IWICFormatConverter *Converter = nullptr;
	IWICBitmapScaler *Scaler = nullptr;

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
	elements = new Impl_Elements(hwnd);
}

Renderer::~Renderer()
{
	//SafeRelease(&imageFactory);
	delete elements;
}

bool Renderer::Begin()
{
	return elements->Begin();
}

bool Renderer::Draw(unsigned int textureID,
	float x, float y, int width, int height,
	float x_S, float y_S, int width_S, int height_S,
	float opacity, float layer, int rot)
{
	if (textureID >= elements->bitmaps.size())
		return false;

	elements->draws.push_back(
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

bool Renderer::Write(const char* text, float x, float y, float width, float height, float layer)
{
	elements->draws.push_back(
		new TextDraw(new std::string(text),
			D2D1::RectF(x, y, x + width, y + height),
			layer
		)
	);

	return true;
}

bool Renderer::End()
{
	return elements->End();
}


int Renderer::LoadContent(const wchar_t* filePath, float height, float width)
{
	IWICImagingFactory* imageFactory = NULL;
	//ID2D1Bitmap* bmp;
	unsigned int index = (int)(elements->bitmaps.size());
	elements->bitmaps.push_back(nullptr);
	elements->filenames.push_back(filePath);

	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&imageFactory)
	);
	hr = LoadBitmapFromFile(elements->renderTarget, imageFactory, filePath,
		(UINT)width, (UINT)height, &elements->bitmaps.at(index));


	SafeRelease(&imageFactory);

	if (!SUCCEEDED(hr))
		return -1;

	//bitmaps.push_back(bmp);
	return index;
}

bool Renderer::Resize(HWND hWnd)
{
	if (!elements->factory)
		return false;

	RECT rc;
	GetClientRect(hWnd, &rc);

	D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

	elements->renderTarget->Resize(size);

	InvalidateRect(hWnd, NULL, FALSE);

	return true;
}

Point<float> Renderer::MeasureString(std::string text, float screenWidth, float screenHeight)
{
	IDWriteTextLayout* layout;

	size_t ret, size = strlen(text.c_str() + 1);
	wchar_t* wtext = new wchar_t[size];
	mbstowcs_s(&ret, wtext, size, text.c_str(), size - 1);

	HRESULT hr = elements->writeFactory->CreateTextLayout(
		wtext,
		(UINT32)strlen(text.c_str()),
		elements->textFormat,
		screenWidth,
		screenHeight,
		&layout
	);

	delete[] wtext;
	DWRITE_TEXT_METRICS metrics;
	layout->GetMetrics(&metrics);

	return Point<float>(metrics.width, metrics.height);
}

Renderer::Impl_Elements::Impl_Elements(HWND hwnd)
{
	HRESULT res;

	// create the d2d factory
	res = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);

	// create the d2d pixel format
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

	res = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory5),
		reinterpret_cast<IUnknown * *>(&writeFactory)
	);

	IDWriteFontSetBuilder1* fontSetBuilder;
	res = writeFactory->CreateFontSetBuilder(&fontSetBuilder);
	IDWriteFontFile* fontFile;
	res = writeFactory->CreateFontFileReference(
		L"../Assets/Fonts/APPLE_KID.TTF",
		nullptr,
		&fontFile
	);
	if (fontFile == 0)
		return;
	res = fontSetBuilder->AddFontFile(fontFile);
	IDWriteFontSet* fontSet;
	fontSetBuilder->CreateFontSet(&fontSet);
	IDWriteFontCollection1* fontCollection;
	writeFactory->CreateFontCollectionFromFontSet(fontSet, &fontCollection);


	res = writeFactory->CreateTextFormat(
		L"Apple Kid",
		fontCollection,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20.f,
		L"en-us",
		&textFormat
	);

	textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&textBrush
	);

	SafeRelease(&fontCollection);
	SafeRelease(&fontSet);
	SafeRelease(&fontFile);
	SafeRelease(&fontSetBuilder);
}

bool Renderer::Impl_Elements::Begin()
{
	renderTarget->BeginDraw();
	renderTarget->Clear(D2D1::ColorF(0, 0, 0, 0));//D2D1::ColorF::Black));
	return true;
}

bool Renderer::Impl_Elements::End()
{
	std::sort(
		draws.begin(),
		draws.end(),
		[](const ToDraw* lhs, const ToDraw* rhs)
	{
		return lhs->layer < rhs->layer;
	}
	);


	for (int i = 0; i < draws.size(); i++)
	{
		draws[i]->Draw(this);
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

void Renderer::Impl_Elements::Clear()
{
	for (int i = 0; i < bitmaps.size(); i++)
	{
		SafeRelease(&bitmaps[i]);
	}
	for (int i = 0; i < draws.size(); i++)
	{
		delete draws[i];
	}
	draws.clear();
	bitmaps.clear();
	filenames.clear();

	RemoveFontResource(L"APPLE_KID.TTF");
	SafeRelease(&textBrush);
	SafeRelease(&writeFactory);
	SafeRelease(&textFormat);
	SafeRelease(&renderTarget);
	SafeRelease(&factory);
}

void Renderer::Impl_Elements::Draw(const ObjectDraw* object)
{

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
		ID2D1Bitmap* rotatedBitmap = nullptr;
		//(bitmaps[object->textureID]);

		// get the frame
		IWICImagingFactory* imagingfactory = nullptr;
		IWICFormatConverter* converter = nullptr;
		IWICBitmapFrameDecode* frame = nullptr;
		IWICBitmapDecoder* decoder = nullptr;
		IWICBitmapFlipRotator* rotator = nullptr;

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
			D2D1_RECT_F newSource = D2D1::RectF(0, 0, 0, 0);
			D2D1_RECT_F source = object->source;
			if (!originalBitmap)
				return;
			float width = (float)originalBitmap->GetSize().width;
			float height = (float)originalBitmap->GetSize().height;
			float sourceHeight = (source.bottom - source.top);
			float sourceWidth = (source.right - source.left);

			// x′= xcosθ − ysinθ
			// y′ = ycosθ + xsinθ
			switch (object->rotation)
			{
			case ObjectDraw::HORIZONTAL:
				res = rotator->Initialize(frame, WICBitmapTransformFlipHorizontal);
				newSource.left = width - source.left - sourceWidth;
				newSource.right = newSource.left + sourceWidth;
				newSource.top = source.top;
				newSource.bottom = source.bottom;
				break;
			case ObjectDraw::VERTICAL:
				res = rotator->Initialize(frame, WICBitmapTransformFlipVertical);
				newSource.left = source.left;
				newSource.right = source.right;
				newSource.top = height - source.top - sourceHeight;
				newSource.bottom = newSource.bottom + sourceHeight;
				break;
			case ObjectDraw::ROT_90:
				res = rotator->Initialize(frame, WICBitmapTransformRotate90);
				newSource.left = height - source.bottom;
				newSource.right = height - source.top;
				newSource.top = source.left;
				newSource.bottom = source.right;
				break;
			case ObjectDraw::ROT_90_VERTICAL:
				res = rotator->Initialize(frame, (WICBitmapTransformOptions)(WICBitmapTransformRotate90 | WICBitmapTransformFlipVertical));
				newSource.left = source.left;
				newSource.right = source.right;
				newSource.top = height - source.top - (source.bottom - source.top);
				newSource.bottom = newSource.bottom + (source.bottom - source.top);
				source = D2D1::RectF(newSource.left, newSource.top, newSource.right, newSource.bottom);
				newSource.left = height - source.bottom;
				newSource.right = height - source.top;
				newSource.top = source.left;
				newSource.bottom = source.right;
				break;
			case ObjectDraw::ROT_180:
				res = rotator->Initialize(frame, WICBitmapTransformRotate180);
				newSource.left = width - source.left - (source.right - source.left);
				newSource.right = newSource.left + (source.right - source.left);
				newSource.top = height - source.top - (source.bottom - source.top);
				newSource.bottom = newSource.bottom + (source.bottom - source.top);
				break;
			case ObjectDraw::ROT_270:
				res = rotator->Initialize(frame, WICBitmapTransformRotate270);
				newSource.left = source.top;
				newSource.right = source.bottom;
				newSource.top = width - source.right;
				newSource.bottom = width - source.left;
				break;
			case ObjectDraw::ROT_270_VERTICAL:
				res = rotator->Initialize(frame, (WICBitmapTransformOptions)(WICBitmapTransformRotate270 | WICBitmapTransformFlipVertical));
				newSource.left = source.left;
				newSource.right = source.right;
				newSource.top = height - source.top - (source.bottom - source.top);
				newSource.bottom = newSource.bottom + (source.bottom - source.top);
				source = D2D1::RectF(newSource.left, newSource.top, newSource.right, newSource.bottom);
				newSource.left = source.top;
				newSource.right = source.bottom;
				newSource.top = width - source.right;
				newSource.bottom = width - source.left;
				break;
			}

			newSource.left = (float)round(newSource.left);
			newSource.right = (float)round(newSource.right);
			newSource.top = (float)round(newSource.top);
			newSource.bottom = (float)round(newSource.bottom);

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

			if (!rotatedBitmap)
				return;
			renderTarget->DrawBitmap(
				rotatedBitmap,
				object->destination,
				object->opacity,
				D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
				newSource
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

void Renderer::Impl_Elements::Draw(const TextDraw* text)
{
	size_t ret, size = strlen(text->text->c_str()) + 1;
	wchar_t* wtext = new wchar_t[size];

	mbstowcs_s(&ret, wtext, size, text->text->c_str(), size - 1);


	renderTarget->DrawTextW(
		wtext,
		(UINT32)wcslen(wtext),
		textFormat,
		text->destination,
		textBrush
	);

	delete[] wtext;

}

Renderer::TextDraw::~TextDraw()
{
	delete text;
}

void Renderer::TextDraw::Draw(Impl_Elements* elements)
{
	elements->Draw(this);
}

Renderer::ObjectDraw::~ObjectDraw()
{

}

void Renderer::ObjectDraw::Draw(Impl_Elements* elements)
{
	elements->Draw(this);
}