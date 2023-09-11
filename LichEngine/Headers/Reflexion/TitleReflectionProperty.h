#pragma once
#include <Refureku/Refureku.h>
#include <Refureku/Properties/PropertySettings.h>
#include <Generated/TitleReflectionProperty.rfkh.h>

class CLASS(rfk::PropertySettings(rfk::EEntityKind::Field, false, true))
    TitleReflectionProperty : public rfk::Property
{
    const char* title;

public:
    TitleReflectionProperty(const char* type) noexcept :
        title(type)
    {}

   const char* GetTitle() const { return title; }

    TitleReflectionProperty_GENERATED
};

class CLASS(rfk::PropertySettings(rfk::EEntityKind::Field, false, true))
    SameLineProperty : public rfk::Property
{

public:
    SameLineProperty() noexcept {}

    SameLineProperty_GENERATED
};


File_TitleReflectionProperty_GENERATED
