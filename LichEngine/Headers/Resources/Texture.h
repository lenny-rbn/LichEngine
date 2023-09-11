#pragma once
#include <Core/RHI.h>
#include <Generated/Texture.rfkh.h>

namespace Resources NAMESPACE()
{
	class CLASS() LICH_ENGINE Texture : public Resource
	{
	public:
		Texture();
		Texture(const std::string& resourceName, const std::string& resourcePath);
		Texture(const std::string& resourcePath, const std::string& resourceName, const bool doGammaCorrection);
		~Texture();

		bool IsCreated() override;
		void Load(const std::string& path) override;
		std::string Save() override;
		void Unload() override;

		inline bool DoGammaCorrection()		{ return m_gammaCorrection; }
		inline int GetWidth()				{ return m_width; }
		inline int GetHeight()				{ return m_height; }
		inline int GetNrChannels()			{ return m_nrChannels; }
		inline uint& GetTexture()			{ return m_texture; }
		inline uint& GetSampler()			{ return m_sampler; }
		inline unsigned char* GetData()		{ return m_pData; }
		inline bool IsLoaded()				{ return loaded; }
		inline void SetGammaCorrection(bool gamma) { m_gammaCorrection = gamma; }


	private:
		bool m_gammaCorrection;

		unsigned char* m_pData;

		int m_width;
		int m_height;
		int m_nrChannels;

		uint m_texture;
		uint m_sampler;

		Resources_Texture_GENERATED
	};
}

File_Texture_GENERATED