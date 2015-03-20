#pragma once

//#include "..\MarbleDLL\AbstractResourceLocator.h"

class BasicEUResourceLocator : public AbstractResourceLocator
{
	void* GetBitmapHandle(unsigned id);
};