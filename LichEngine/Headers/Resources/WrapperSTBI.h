#pragma once
#include <Define.h>


namespace Resources
{
	void FreeTextureData(void* data);

	unsigned char* LoadTextureData(const char* filename, int* width, int* height, int* nbChannel, int req_comp);
	unsigned char* LoadTextureDataFromMemory(const unsigned char* data, int len, int* width, int* height, int* nbChannel, int req_comp);
	float* LoadTextureHDRData(const char* filename, int* width, int* height, int* nbChannel, int req_comp);
}