#include "AudioPlayer.h"
#include "defines.h"
#include <vector>


struct Impl_Elements
{
	IDirectSound8* m_DirectSound = nullptr;
	IDirectSoundBuffer* m_primaryBuffer = nullptr;

	std::vector<IDirectSoundBuffer8*> m_secondaryBuffers;

	bool LoadWAV_Internal(const char* filepath, IDirectSoundBuffer8** soundBuffer);
	bool UnloadWAV_Internal(IDirectSoundBuffer8** soundBuffer);
	bool UnloadAllWAVs_Internal();
	bool PlayWAV_Internal(IDirectSoundBuffer8** soundBuffer);
	bool IsWAVLoaded(int index) { return index < m_secondaryBuffers.size() ? m_secondaryBuffers[index] : false; }
	int WAVsLoaded() { return (int)m_secondaryBuffers.size(); }
};

struct WaveHeaderType
{
	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subChunkId[4];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char dataChunkId[4];
	unsigned long dataSize;
};


AudioPlayer::AudioPlayer()
{
	elements = new Impl_Elements();
}

AudioPlayer::~AudioPlayer()
{
	delete elements;
}

bool AudioPlayer::Init(HWND hWnd)
{
	DSBUFFERDESC bd;
	WAVEFORMATEX waveFormat;

	// initialize the direct sound interface pointer
	HRESULT result = DirectSoundCreate8(NULL, &elements->m_DirectSound, NULL);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Directsound Error: DirectSoundCreate8 Failed to initialize");
		return false;
	}

	// set the cooperative level to priority 
	result = elements->m_DirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Directsound Error: SetCooperativeLevel Failed");
		return false;
	}

	// Setup the primary buffer description.
	bd.dwSize = sizeof(DSBUFFERDESC);
	bd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bd.dwBufferBytes = 0;
	bd.dwReserved = 0;
	bd.lpwfxFormat = NULL;
	bd.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = elements->m_DirectSound->CreateSoundBuffer(&bd, &elements->m_primaryBuffer, NULL);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Directsound Error: CreateSoundBuffer Failed");
		return false;
	}

	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified.
	result = elements->m_primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Directsound Error: SetFormat Failed");
		return false;
	}

	return true;
}

void AudioPlayer::Clean()
{
	// clear all loaded secondary buffer pointers.
	elements->UnloadAllWAVs_Internal();

	// Release the primary sound buffer pointer.
	if (elements->m_primaryBuffer)
	{
		elements->m_primaryBuffer->Release();
		elements->m_primaryBuffer = 0;
	}

	// Release the direct sound interface pointer.
	if (elements->m_DirectSound)
	{
		elements->m_DirectSound->Release();
		elements->m_DirectSound = 0;
	}


	SafeDelete(elements);
	return;
}


int AudioPlayer::LoadWAV(const char* filepath)
{
	elements->m_secondaryBuffers.push_back(nullptr);
	elements->LoadWAV_Internal(filepath, &elements->m_secondaryBuffers[elements->m_secondaryBuffers.size() - 1]);
	return elements->WAVsLoaded() - 1;
}

bool AudioPlayer::UnloadWAV(int index)
{
	return elements->UnloadWAV_Internal(&elements->m_secondaryBuffers[index]);
}

bool AudioPlayer::PlayWAV(int index)
{
	return elements->PlayWAV_Internal(&elements->m_secondaryBuffers[index]);
}


bool Impl_Elements::LoadWAV_Internal(const char* filepath, IDirectSoundBuffer8** soundBuffer)
{
#pragma region Declarations

	int error;
	FILE* filePtr = nullptr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

#pragma endregion


	// Open the wave file in binary.
	error = fopen_s(&filePtr, filepath, "rb");
	if (error != 0)
	{
		OutputDebugStringW(L"Directsound Error: failed to open file");
		return false;
	}

	// Read in the wave file header.
	if (filePtr == 0)
		return false;
	count = (unsigned int)fread(&waveFileHeader, (size_t)sizeof(waveFileHeader), (size_t)1, filePtr);
	if (count != 1)
	{
		OutputDebugStringW(L"Directsound Error: wav file header size wrong");
		return false;
	}

	// Check that the chunk ID is the RIFF format.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		OutputDebugStringW(L"Directsound Error: wav file chunk id is not in riff format");
		return false;
	}

	// Check that the file format is the WAVE format.
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		OutputDebugStringW(L"Directsound Error: wav file is not in wave format");
		return false;
	}

	// Check that the sub chunk ID is the fmt format.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		OutputDebugStringW(L"Directsound Error: wav file sub chunk id is not in fmt format.");
		return false;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		OutputDebugStringW(L"Directsound Error: wave file audio format is not WAVE_FORMAT_PCM.");
		return false;
	}

	// Check that the wave file was recorded in stereo format.
	if (waveFileHeader.numChannels != 2)
	{
		OutputDebugStringW(L"Directsound Error: wav file not in stereo format.");
		//return false;
	}

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if (waveFileHeader.sampleRate != 44100)
	{
		OutputDebugStringW(L"Directsound Error: wav file not at 44.1 KHz sample rate.");
		return false;
	}

	// Ensure that the wave file was recorded in 16 bit format.
	if (waveFileHeader.bitsPerSample != 16)
	{
		OutputDebugStringW(L"Directsound Error: wav file not in 16 bits.");
		return false;
	}

	// Check for the data chunk header.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		OutputDebugStringW(L"Directsound Error: failed to find wave file data chunk header.");
		return false;
	}

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Directsound Error: failed to create sound buffer.");
		return false;
	}

	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*soundBuffer);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Directsound Error: failed to query interface.");
		return false;
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		OutputDebugStringW(L"Directsound Error: invalid wav file header size.");
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	count = (unsigned int)fread(waveData, (size_t)1, (size_t)waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		OutputDebugStringW(L"Directsound Error: failed to read wav data.");
		return false;
	}

	// Close the file once done reading.
	error = fclose(filePtr);
	if (error != 0)
	{
		OutputDebugStringW(L"Directsound Error: failed to close file.");
		return false;
	}

	// Lock the secondary buffer to write wave data into it.
	result = (*soundBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Directsound Error: failed to lock sound buffer.");
		return false;
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	result = (*soundBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Directsound Error: failed to unlock sound buffer.");
		return false;
	}

	// Release the wave data since it was copied into the secondary buffer.
	delete[] waveData;
	waveData = 0;

	return true;
}

bool Impl_Elements::UnloadWAV_Internal(IDirectSoundBuffer8** soundBuffer)
{
	// Release the secondary sound buffer.
	if (*soundBuffer)
	{
		(*soundBuffer)->Release();
		*soundBuffer = 0;
	}

	return true;
}

bool Impl_Elements::UnloadAllWAVs_Internal()
{
	// Release all secondary buffers
	for (int i = 0; i < m_secondaryBuffers.size(); i++)
	{
		UnloadWAV_Internal(&m_secondaryBuffers[i]);
	}
	m_secondaryBuffers.clear();
	return true;
}

bool Impl_Elements::PlayWAV_Internal(IDirectSoundBuffer8** soundBuffer)
{
	HRESULT result;


	// Set position at the beginning of the sound buffer.
	result = (*soundBuffer)->SetCurrentPosition(0);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Directsound Error: failed to set buffer position.");
		return false;
	}

	// Set volume of the buffer to 100%.
	result = (*soundBuffer)->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Directsound Error: failed to set buffer volume.");
		return false;
	}

	// Play the contents of the secondary sound buffer.
	result = (*soundBuffer)->Play(0, 0, 0);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Directsound Error: failed to play wav.");
		return false;
	}

	return true;
}