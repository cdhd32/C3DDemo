#pragma once

#include "fbxsdk.h"

class C3DModel;

class C3DModelLoader
{
friend C3DModel;

public:
	C3DModelLoader();
	virtual ~C3DModelLoader();

	C3DModel* LoadFBX(const std::string& fileName);
private:
};

