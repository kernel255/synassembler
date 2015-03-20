#pragma once

#include "..\MarbleDLL\ModuleServices.h"
#include "..\MarbleDLL\AbstractModuleInfo.h"

class BasicEUModuleService : public ModuleServices
{
public:
	AbstractModuleInfo* GetModuleInfo(void) { return m_pModuleInfo; }
	void SetModuleInfo(AbstractModuleInfo* pModInfo) { m_pModuleInfo = pModInfo; }
	virtual ~BasicEUModuleService() { delete m_pModuleInfo; }
private:
	AbstractModuleInfo* m_pModuleInfo;
};