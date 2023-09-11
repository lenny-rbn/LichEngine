#include <pch.h>
#include <Resources/WrapperSTBI.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


namespace Resources
{
	unsigned char* LoadTextureData(const char* filename, int* width, int* height, int* nbChannel, int req_comp)
	{
		return stbi_load(filename, width, height, nbChannel, req_comp);
	}

	unsigned char* LoadTextureDataFromMemory(const unsigned char* data, int len, int* width, int* height, int* nbChannel, int req_comp)
	{
		return stbi_load_from_memory(data, len, width, height, nbChannel, req_comp);
	}

	float* LoadTextureHDRData(const char* filename, int* width, int* height, int* nbChannel, int req_comp)
	{
		return stbi_loadf(filename, width, height, nbChannel, req_comp);
	}

	void FreeTextureData(void* data)
	{
		stbi_image_free(data);
	}
}