
#pragma once

#include "MIDIChannelMessage.h"
#include "ElaborationUnitPort.h"
#include "DSPTypes.h"

/**
 * @class Atomic elaboration element. Nothing smaller than this.
 * This is a pure virtual class, any shared method will be implemented in AbstractElaborationUnit
 */


#define C_AddNewConnection			-1


class EUKind;

class ElaborationUnit {
protected:
	static const int C_PropertyGetSucceess = 0;
	static const int C_PropertyGetFailure = -1;

public:
	/**
	* @fn Function to retrieve an unique ElaborationUnit inside on Algorithm
	*/
	virtual int getId() = 0;
	virtual void setId(int id) = 0;
	/**
	* @fn Set the EU to which connect
	* @param pLocalPort Port of the EU where connect the other EU
	* @param pInputEU EU to connect to
	* @param pInputPort Port of the other EU where connect
	*/
	virtual bool setInputEU(ElaborationUnitPort* pLocalPort, ElaborationUnit* pInputEU, ElaborationUnitPort* pInputPort) = 0;
	/**
	* @fn Set the EU to which connect
	* @param pLocalPort Port of the EU where connect the other EU
	* @param pInputEU EU to connect to
	* @param pInputPort Port of the other EU where connect
	*/
	virtual bool setOutputEU(ElaborationUnitPort* pLocalPort, ElaborationUnit* pOutputEU, ElaborationUnitPort* pOutputPort) = 0;
	/**
	 * @fn Return the Nth input port
	 */
	virtual ElaborationUnitPort* getNthInputPort(int n) = 0;
	/**
	 * @fn Return the Nth output port
	 */
	virtual ElaborationUnitPort* getNthOutputPort(int n) = 0;
	/**
	* @fn Return the EU's input ports number
	*/
	virtual int getInputPortNumber(void) = 0;
	/**
	* @fn Return the EU's output ports number
	*/
	virtual int getOutputPortNumber(void) = 0;
	virtual ElaborationUnitPort* getInputPortByEU(ElaborationUnit* pEU, int& n) = 0;
	virtual ElaborationUnitPort* getOutputPortByEU(ElaborationUnit* pEU, int& n) = 0;
	/**
	* \fn Do the processing on a buffer of audio samples
	*/
	virtual void updateAudioSamples(EAG_SAMPLE_TYPE *pSamplesBuffer,int numsamples) = 0;
	virtual void receiveMIDIMessage(MIDIChannelMessage& midimsg) = 0;
	/**
	* \brief Allocate the resource of the EU, both physical and virtual.
	* \brief Calling this fun won't start the signal processing (play), but will make it possible
	*/
	virtual void allocate(void) = 0;
	/**
	* \brief Deallocate the resource of the EU, both physical and virtual
	* \brief Play won't be possible after this function call
	*/
	virtual void deallocate(void) = 0;
	/**
	* \brief Start the processing of the signal. This will effectively take place when Update will be called
	*/
	virtual void play(void) = 0;
	/**
	* \brief Pause the processing of the signal, all the resources, buffer etc will be maintained
	*/
	virtual void pause(void) = 0;
	/**
	* \brief Stop completely the Algorithm, all the resouce will be allocated.
	*/
	virtual void stop(void) = 0;
	/**
	* @fn virtual PhysicalElaborationUnit* GetPhisicalInterface(void)
	* @brief This function can give an idea to the connected EU of what samples memory will be necessary.
	*/
	virtual void setSamplesBufferMaximumSize(int size) = 0;
	/**
	 * @return The EUKind class than contains info about the ElaborationUnit
	 */
	virtual const EUKind* getKind() = 0;
	/**
	 * @fn Set the Nth property
	 */
	virtual int setProperty(int propertyIndex, const char* value) = 0;
	/**
	 * @fn Get the Nth property
	 */
	virtual int getProperty(int propertyIndex, char* value, int bufferSize) = 0;

	/**
	 * @fn Set the Nth double property
	 */
	virtual int setDProperty(int propertyIndex, double value) = 0;
	/**
	 * @fn Get the Nth double property
	 */
	virtual int getDProperty(int propertyIndex, double* value) = 0;
	/**
	* @fn Set the Nth integer property
	*/
	virtual int setIProperty(int propertyIndex, int value) = 0;
	/**
	* @fn Get the Nth integer property
	*/
	virtual int getIProperty(int propertyIndex, int* value) = 0;
	/**
	* @fn Set the Nth bool property
	*/
	virtual int setBProperty(int propertyIndex, bool value) = 0;
	/**
	* @fn Get the Nth bool property
	*/
	virtual int getBProperty(int propertyIndex, bool* value) = 0;

	/**
	* \brief Virtual destructor (necessary accordingly to Vincent Greco teaching)
	*/
	virtual ~ElaborationUnit() = 0;
};