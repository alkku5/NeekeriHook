#pragma once
#include "SDK.h"

class ICollideable
{
public:
	virtual void			    GetEntityHandle() = 0;
	virtual const Vector&       GetMins() const = 0;
	virtual const Vector&       GetMaxs() const = 0;
};