#pragma once
#include <Refureku/Refureku.h>
#include <Refureku/Properties/PropertySettings.h>
#include <Generated/VectorReflexionProperty.rfkh.h>

class CLASS(rfk::PropertySettings(rfk::EEntityKind::Field, false, true))
	RangeProperty : public rfk::Property
{
	float m_min, m_max;

public:
	RangeProperty(float min , float max) noexcept :
		m_min(min), m_max(max)
	{}

	float GetMin() const { return m_min; }
	float GetMax() const { return m_max; }

	RangeProperty_GENERATED
};
File_VectorReflexionProperty_GENERATED