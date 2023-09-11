#include <pch.h>
#include <Core/TimeManager.h>
#include <Resources/ResourceManager.h>
#include <Generated/ImageFade.rfks.h>



void ImageFade::Start()
{
	 m_pImageBackground =  gameObject->GetComponent<UI::Image>();
	 m_isLaunch = true;
	 m_fadeTimer = 0.0f;
}

void ImageFade::UpdateScript()
{
	if (m_isLaunch && Resources::ResourceManager::LoadFinish())
	{
		if(!m_isFadeOut) m_pImageBackground->opacity = m_fadeTimer /fadeTime;
		if(m_isFadeOut)m_pImageBackground->opacity = (1.0f - (m_fadeTimer /fadeTime));

		if (m_fadeTimer < fadeTime)
		{
			m_fadeTimer += (float)Core::TimeManager::GetDeltaTime();
		}
		else
		{
			m_isLaunch = false;
		}
	}
}

void ImageFade::ChangeFadeState()
{
	m_isLaunch = true;
	m_fadeTimer = 0.f;
	m_isFadeOut = !m_isFadeOut;
}


std::string ImageFade::SaveScript()
{
	std::string content = std::to_string(fadeTime) + ',';
	return content;
}

void ImageFade::LoadScript(std::string data)
{
	const int sizeData = 2;
	std::string dataCut[sizeData];
	size_t index = 0,baseIndex = 0;

	for (size_t i = 0; i < sizeData; i++)
	{
		index = data.find(',',baseIndex + 1);
		dataCut[i] = data.substr(baseIndex,index - baseIndex);
		baseIndex = index + 1;
	}

	fadeTime = std::stof(dataCut[1]);
}