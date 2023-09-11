#pragma once


#include <Maths.h>


using namespace irrklang;

LAPI typedef ISound LichSound;
LAPI typedef ISoundSource LichSoundSource;
LAPI typedef ISoundEngine LichSoundEngine;

namespace AudioAPI
{
	bool Destroy(LichSound* sound);

	LichSoundSource* AddSoundSource(LichSoundEngine* engine, const char* path);
	void RemoveSoundSource(LichSoundEngine* engine,LichSoundSource* soundSource);


	LichSound* PlaySource2D(LichSoundEngine* engine, LichSoundSource* source, bool loop = 0, bool startPaused = 0, bool track = 0, bool effects = 0);
	LichSound* PlaySound2D(LichSoundEngine* engine, const char* path, bool loop = 0, bool startPaused = 0, bool track = 0, bool stream = 1, bool effects = 0);

	LichSound* PlaySource3D(LichSoundEngine* engine, LichSoundSource* source, Vector3D pos, bool loop = 0, bool startPaused = 0, bool track = 0, bool effects = 0);
	LichSound* PlaySound3D(LichSoundEngine* engine, const char* path, Vector3D pos, bool loop = 0, bool startPaused = 0, bool track = 0, bool stream = 1, bool effects = 0);

	// SET GET
	// ----------------------------------------------
	void SetPosition(LichSound* sound, Vector3D pos);
	void SetMinSoundDistance(LichSound* sound, float distance);
	void SetMaxSoundDistance(LichSound* sound, float distance);
	void SetMinSoundSourceDistance(LichSoundSource* soundSource, float distance);
	void SetMaxSoundSourceDistance(LichSoundSource* soundSource, float distance);

	float GetMinSoundDistance(LichSound* sound);
	float GetMaxSoundDistance(LichSound* sound);
	float GetMinSoundSourceDistance(LichSoundSource* soundSource);
	float GetMaxSoundSourceDistance(LichSoundSource* soundSource);

	Vector3D GetPosition(LichSound* sound);
}