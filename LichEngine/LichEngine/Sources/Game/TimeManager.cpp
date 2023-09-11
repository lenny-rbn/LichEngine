#include <pch.h>
#include <Core/TimeManager.h>

using namespace Core;

double TimeManager::m_deltaTime = 0.0;
double TimeManager::m_fixedDeltaTime = 0.0;
double TimeManager::m_timeGeneral = 0.0;

TimeManager::TimeManager()
{
	m_deltaTime = 0.f;
	m_fixedDeltaTime = 0.0;
	m_timeGeneral = 0.0;
	m_prevFrameTime = WindowAPI::GetTimer();
}

TimeManager::~TimeManager()
{
}

void Core::TimeManager::Update()
{
	m_timeGeneral = WindowAPI::GetTimer();
	m_deltaTime = m_timeGeneral - m_prevFrameTime;
	m_prevFrameTime = m_timeGeneral;
}

double Core::TimeManager::GetDeltaTime()
{
	return m_deltaTime;
}

/// <summary>
/// Fixed Time is not implemented 
/// </summary>
/// <returns></returns>
double Core::TimeManager::GetFixedUpdate()
{
	return m_fixedDeltaTime;
}

double Core::TimeManager::GetGeneralTime()
{
	return m_timeGeneral;
}