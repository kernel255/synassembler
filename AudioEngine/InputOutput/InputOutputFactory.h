
#pragma once

#include "gg.h"

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <string>
#include <vector>
#include "ElaborationUnitFactory.h"
#include "AbstractPhysicalElaborationUnitFactory.h"
#include ".\ElaborationUnits\MIDIInput.h"
#include ".\ElaborationUnits\PolyKeyboard.h"

#define DIRECTSOUND_INDEX		0
#define MIDIIN_INDEX			1
#define POLYKEYBOARD_INDEX		2
#define PCKEYBOARD_INDEX		3

//#define SEND_INITIALIZATION_MESSAGE 1
#define INITIALIZATION_MESSAGE			'A'

class ModuleServices;

BOOL CALLBACK DSEnumCallBack(LPGUID  lpGuid, LPCWSTR  lpcstrDescription, LPCWSTR  lpcstrModule, LPVOID  lpContext);

typedef HRESULT (__stdcall * td_pfn_DirectSoundEnumerate)(LPDSENUMCALLBACK  lpDSEnumCallback, LPVOID  lpContext);
typedef HRESULT (__stdcall * td_pfn_DirectSoundCreate)(LPCGUID  lpcGuidDevice, LPDIRECTSOUND8 * ppDS8, LPUNKNOWN  pUnkOuter);

/**
* @class Create input-output Win32 objects: DirectSound, MCI MIDI
*/

class InputOutputFactory : public AbstractPhysicalElaborationUnitFactory
{
friend BOOL CALLBACK DSEnumCallBack(LPGUID  lpGuid, LPCWSTR  lpcstrDescription, LPCWSTR  lpcstrModule, LPVOID  lpContext);
public:
	class DirectSoundOutputDescription
	{
	public:
		std::wstring		m_wstrBoardDescription;
		int					m_Ordinal;
		GUID				m_guid;
		~DirectSoundOutputDescription()
		{
			int pippo = 0;
			//delete m_wstrBoardDescription;
			//delete m_guid;
		}
	};
	class MIDIInDescription
	{
	public:
		unsigned int m_id;
		bool m_bAllocated;
	};
	InputOutputFactory(ModuleServices* pServices);
	//######### VIRTUAL ELABORATION UNITS #########
	// No virtual EU produced by this factory	
	virtual unsigned int getVirtualEUKindNumber(void);
	virtual const EUKind* getNthVirtualEUKind(int n);
	ElaborationUnit* createVirtualElaborationUnit(unsigned euIndex);
	//######### PHYSICAL ELABORATION UNITS #########
	virtual unsigned int getPhysicalEUKindNumber(void);
	virtual const EUKind* getNthPhysicalEUKind(int n);
	//virtual unsigned int getPhysicalEUInstanceNumber(int euIndex);
	ElaborationUnit* createPhysicalElaborationUnit(unsigned euIndex, unsigned instanceIndex);
	ElaborationUnit* createNamedPhysicalElaborationUnit(unsigned euIndex, std::wstring name);
	
	/**
	 * @fn 
	 * @param Index of the EU to retrieve info
	 * @return Info about the nth EU
	 */
	const EUKind* getNthEUKind(int n);
	/**
	 * @return Name of the factory
	 */
	const char *getName(void);


	/**
	 * @fn Deallocate a specific EU
	 * @param Pointer to the EU to be destroyed
	 */
	void destroyElaborationUnit(ElaborationUnit* pGEU);
	virtual ~InputOutputFactory();
	virtual const int getEUIndex(ElaborationUnit* pEU);
private:
	static const int LocalEUNumber;
	/**
	 * @fn Search for all the available physical EU instances and store them
	 */
	void enumDirectSoundDevices(void);
	void enumMidiInDevices(void);
	void enumPolyKeyboard(void);
	void enumPCKeyboard(void);
	ElaborationUnit* getDirectSoundOutput(int instanceIndex);
	ElaborationUnit* getMidiIn(int instanceIndex);
	ElaborationUnit* getPolyKeyboard(int instanceIndex);
	ElaborationUnit* getPCKeyboard(int instanceIndex);
	ElaborationUnit* getDirectSoundOutputByName(std::wstring name);
	ElaborationUnit* getMIDIInByName(wstring miniInName);

	/**
	* @var m_bDirectSoundAvailable true if the DirectSound installed, this not means that the device is available
	*/ 
	static bool m_bDirectSoundAvailable;
	static HMODULE m_hDSoundModule;
	static td_pfn_DirectSoundEnumerate m_td_pfn_DirectSoundEnumerate;
	static td_pfn_DirectSoundCreate m_td_pfn_DirectSoundCreate;
	static int m_NumDevices;
	static std::vector<DirectSoundOutputDescription> m_DirectSoundOutputDescriptions;
	static std::vector<MIDIInDescription> m_MIDIInDescriptions;
	static bool AddDevice(LPCWSTR description, GUID* pguid);
	static bool GetNextAvailableDirectSoundOutput(GUID& guid, LPDIRECTSOUND8& pDSound8);
private:
	bool pcKeyAllocated;
};