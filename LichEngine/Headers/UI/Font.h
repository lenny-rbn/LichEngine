#pragma once

#include <Define.h>
#include <maths.h>
#include <Resources/Resource.h>
#include <Core/RHI.h>
#include <map>
#include <Generated/Font.rfkh.h>


namespace UI NAMESPACE()
{
	
	struct LICH_ENGINE Character
	{
		unsigned char* data;
		uint textureID;  
		Vector2D size; 
		Vector2D bearing;
		uint advance;
	};

	class CLASS() LICH_ENGINE Font : public Resources::Resource
	{
	public:

		Font();
		Font(const std::string& resourceName, const std::string& resourcePath);
		~Font();

		void Load(const std::string& path) override;
		std::string Save() override;
		bool IsCreated() override;
		void Unload() override;

		inline Character GetCharacter(char c) { return m_characters[c]; }

	private:

		std::map<char, UI::Character> m_characters;

		void CreateCharacterTextures();
		UI_Font_GENERATED
	};
}
File_Font_GENERATED