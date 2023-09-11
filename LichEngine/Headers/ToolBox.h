#pragma once
#include <vector>
#include <fstream>

#include <Resources/Shape.h>
#include <Refureku/Refureku.h>
#include <Generated/ToolBox.rfkh.h> 


namespace Utils
{
	template<typename T >
	bool IsInVector(std::vector<T> vec, T& element)
	{
		if (std::find(vec.begin(), vec.end(), element) != vec.end())
			return true;

		return false;
	}

	template<typename T >
	bool EraseVectorElement(std::vector<T>& vec, T& element)
	{
		if (IsInVector<T>(vec, element))
		{
			typename std::vector<T>::iterator value;
			value = std::find(vec.begin(), vec.end(), element);
			vec.erase(value);
			return true;
		}

		return false;
	}

	bool ReadFile(const std::string& path, std::string& data);
	void WriteFile(const std::string& name, const std::string& data);
	LAPI std::string remove_extension(const std::string& filename);
	LAPI std::string GetNameFromPath(const std::string& path);
	LAPI char* remove_extensionChr(char* filename);
};

LAPI bool TestField(const rfk::Field&, void*);

class CLASS() LICH_ENGINE SubShapeContainer : public std::vector<Resources::SubModel*>
{
public:
	rfk::Vector<const rfk::Field*> GetIndexField(int index);
	Resources::SubModel* GetDataIndexPointer(int index);

	SubShapeContainer_GENERATED
};

template<class T>
class CLASS() VectorContainer : public std::vector<T*>
{

	bool m_classicType = false;
public:
	METHOD() const rfk::Archetype* GetClassArchetype()
	{
		return	rfk::getType<T>().getArchetype();
	}

	METHOD() bool IsFundamentalData()
	{
		const rfk::Archetype* arc = rfk::getType<T>().getArchetype();
		const rfk::Class* classInstance = rfk::classCast(arc);
		std::string name = arc->getName();
		if (arc->getKind() == rfk::EEntityKind::FundamentalArchetype) return true;
		if (name == "Vector3D" || name == "Vector4D")
		{
			m_classicType = true;
			return true;
		}
		if (classInstance->isSubclassOf(Resources::Resource::staticGetArchetype())) return true;

		return false;
	}

	METHOD() bool IsPointer()
	{
		return rfk::getType<T>().isPointer();
	}

	METHOD() void* GetDataIndexPointer(int index)
	{
		if (m_classicType)
		{
			T* instance = ((*this)[index]);
			return (void*)instance;
		}
		else
		{
			T** instance = &((*this)[index]);
			return (void*)instance;
		}
	}

	METHOD() int GetSize()
	{
		return (int)this->size();
	}

	METHOD() void AddObject()
	{
		if (m_classicType)
		{
			T* value = new T();
			this->push_back(value);
		}
		else
		{
			this->push_back(nullptr);
		}

	}
	METHOD() void RemoveObject()
	{
		this->pop_back();
	}
	VectorContainer_GENERATED
};

File_ToolBox_GENERATED