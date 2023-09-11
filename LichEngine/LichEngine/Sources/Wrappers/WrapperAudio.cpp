#include <pch.h>
#include <Resources/WrapperAudio.h>

using namespace irrklang;

namespace AudioAPI
{
	LichSoundSource* AddSoundSource(LichSoundEngine* engine, const char* path)
	{
		return engine->addSoundSourceFromFile(path);
	}

	void RemoveSoundSource(LichSoundEngine* engine,LichSoundSource* soundSource)
	{
		engine->removeSoundSource(soundSource);
	}

	LichSound* PlaySound2D(LichSoundEngine* engine, const char* path, bool loop, bool startPaused, bool track, bool stream, bool effects)
	{
		if (stream)
			return engine->play2D(path, loop, startPaused, track, ESM_AUTO_DETECT, effects);
		else
			return engine->play2D(path, loop, startPaused, track, ESM_NO_STREAMING, effects);
	}

	LichSound* PlaySource2D(LichSoundEngine* engine, LichSoundSource* source, bool loop, bool startPaused, bool track, bool effects)
	{
		return engine->play2D(source, loop, startPaused, track, effects);
	}

	LichSound* PlaySound3D(LichSoundEngine* engine, const char* path, Vector3D pos, bool loop, bool startPaused, bool track, bool stream, bool effects)
	{
		irrklang::vec3df result{ pos.x, pos.y, pos.z };

		if (stream)
			return engine->play3D(path, result, loop, startPaused, track, ESM_AUTO_DETECT, effects);
		else
			return engine->play3D(path, result, loop, startPaused, track, ESM_NO_STREAMING, effects);
	}

	LichSound* PlaySource3D(LichSoundEngine* engine, LichSoundSource* source, Vector3D pos, bool loop, bool startPaused, bool track, bool effects)
	{
		irrklang::vec3df result{ pos.x, pos.y, pos.z };

		return engine->play3D(source, result, loop, startPaused, track, effects);
	}

	bool Destroy(LichSound* sound)
	{
		return sound->drop();
	}

	void SetPosition(LichSound* sound, Vector3D pos)
	{
		irrklang::vec3df result{ pos.x, pos.y, pos.z };
		sound->setPosition(result);
	}

	Vector3D GetPosition(LichSound* sound)
	{
		return Vector3D(sound->getPosition().X, sound->getPosition().Y, sound->getPosition().Z);
	}

	void SetMinSoundDistance(LichSound* sound, float distance)
	{
		sound->setMinDistance(distance);
	}

	void SetMaxSoundDistance(LichSound* sound, float distance)
	{
		sound->setMaxDistance(distance);
	}

	void SetMinSoundSourceDistance(LichSoundSource* soundSource, float distance)
	{
		soundSource->setDefaultMinDistance(distance);
	}

	void SetMaxSoundSourceDistance(LichSoundSource* soundSource, float distance)
	{
		soundSource->setDefaultMaxDistance(distance);
	}

	float GetMinSoundDistance(LichSound* sound)
	{
		return sound->getMinDistance();
	}

	float GetMaxSoundDistance(LichSound* sound)
	{
		return sound->getMinDistance();
	}

	float GetMinSoundSourceDistance(LichSoundSource* soundSource)
	{
		return soundSource->getDefaultMinDistance();
	}

	float GetMaxSoundSourceDistance(LichSoundSource* soundSource)
	{
		return soundSource->getDefaultMaxDistance();
	}
}