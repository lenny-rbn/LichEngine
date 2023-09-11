#include <pch.h>
#include <UI/Font.h>
#include <Debugs/Log.h>
#include <utility>

#include <ft2build.h>
#include FT_FREETYPE_H  
#include <Generated/Font.rfks.h>

namespace UI
{

	Font::Font()
	{
		type = Resources::FONT;
	}

	Font::Font(const std::string& resourceName, const std::string& resourcePath)
	{
		name = resourceName;
		path = resourcePath;
		type = Resources::FONT;
	}

	Font::~Font()
	{

	}


	void Font::Load(const std::string& path)
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			LWARNING("ERROR::FREETYPE: Could not init FreeType Library");
			return;
		}

		FT_Face face;
		if (FT_New_Face(ft, path.c_str(), 0, &face))
		{
			LWARNING("ERROR::FREETYPE: Failed to load font");
			return;
		}

		FT_Set_Pixel_Sizes(face, 0, 64);
		WrapperRHI::RHI::SetPixelUnpackAlignment(1);

		int numberChar = 253;
		if (numberChar > face->num_glyphs)
			numberChar = face->num_glyphs;

		for (unsigned char c = 0; c < 253; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				LWARNING("ERROR::FREETYTPE: Failed to load Glyph");
				continue;
			}

			uint glyph = -1;

			WrapperRHI::RHI::CreateGlyph(glyph, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);

			Character character = { 
				face->glyph->bitmap.buffer,
				glyph,
				Vector2D((int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows),
				Vector2D((int)face->glyph->bitmap_left, (int)face->glyph->bitmap_top),
				static_cast<int>(face->glyph->advance.x)
			};
			m_characters.insert(std::pair<char, Character>(c, character));
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
		loaded = true;
		LINFO(path);
	}

	void Font::CreateCharacterTextures()
	{
		for (std::map<char, UI::Character>::iterator it = m_characters.begin(); it != m_characters.end(); it++)

		created = true;
	}

	std::string Font::Save()
	{
		std::string content = "8,";
		content += path + ',';
		content += name + ',';
		return content;
	}

	bool Font::IsCreated()
	{
		if (created)
		{
			return true;
		}
		else if (loaded)
		{
			CreateCharacterTextures();
			return true;
		}

		return false;
	}

	void Font::Unload()
	{
		for (std::map<char, UI::Character>::iterator it = m_characters.begin(); it != m_characters.end(); it++)
			WrapperRHI::RHI::DeleteTexture(it->second.textureID);
	}
}

