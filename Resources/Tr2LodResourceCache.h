////////////////////////////////////////////////////////////////////////////////
//
// Created:		September 2018
// Copyright:	CCP 2018
//

#pragma once
#ifndef Tr2LodResourceCache_h
#define Tr2LodResourceCache_h

#include "Tr2LodResource.h"

template <typename T>
class Tr2LodResourceCache
{
public:
	Tr2LodResourceCache() {};
	~Tr2LodResourceCache() {};

	void SetResource( Tr2LodResourcePtr resource )
	{
		m_resource = resource;
		m_resourceCache = nullptr;
	}

	T* GetResource() const
	{
		if (m_resource)
		{
			IBlueResource *data = m_resource->GetResource();
			if (data != static_cast<void*>(m_resourceCache))
			{
				// Not undefined behaviour as long as Tr2LodResourceCache is not created as a const object
				const_cast<Tr2LodResourceCache*>(this)->m_resourceCache = dynamic_cast<T*>(data);
			}

			return m_resourceCache;
		}

		return nullptr;
	}
private:
	T* m_resourceCache;
	Tr2LodResourcePtr m_resource;
};

#endif