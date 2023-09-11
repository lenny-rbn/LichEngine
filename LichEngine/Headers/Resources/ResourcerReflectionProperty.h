#pragma once
#include <string>
#include <Resources/Resource.h>
#include <Refureku/Refureku.h>
#include <Refureku/Properties/PropertySettings.h>
#include <Generated/ResourcerReflectionProperty.rfkh.h>



class CLASS(rfk::PropertySettings(rfk::EEntityKind::Field,false,true))
    ResourceTypeProperty : public rfk::Property
{
    Resources::ResourceType typeResources;

public:
    ResourceTypeProperty(Resources::ResourceType type) noexcept :
        typeResources(type)
    {}

    Resources::ResourceType GetType() const { return typeResources; }

    ResourceTypeProperty_GENERATED
};
File_ResourcerReflectionProperty_GENERATED
