#pragma once

#include "..\Facilities\General\Property.h"

/**
* @class Abstract class to describe the characteristics of an EU
* @brief This class is useful to hold information of the generated EU, without to have to instantiate it.
*/

class EUKind
{
public:

	/**
	 * @return The name of the belonging ElaborationUnit
	 */
	virtual const char* getName(void) const = 0;
	/**
	 * @return True if the ElaborationUnit is physical (the instance number is limited by HW/drives)
	 */
	virtual bool isPhysical(void) const = 0;

//############################
//####### Port Control #######
//############################

	/**
	 * @fn Return the number of input ports of the EU
	 */
	virtual int getInputPortNumber(void) const = 0;
	/**
	 * @fn Return the number of output ports of the EU
	 */
	virtual int getOutputPortNumber(void) const = 0;
	/**
	 * @fn Return the name of the n-th input port
	 */
	virtual const char* getNthInputPortName(int portIndex) const = 0;
	/**
	 * @fn Return the name of the n-th input port
	 */
	virtual const char* getNthOutputPortName(int portIndex) const = 0;
	/**
	 * @fn Return the kind of the nth input port
	 */

	static char* getMIDIPortKind() {
		return "MIDI";
	}

	static char* getAudioPortKind() {
		return "AUDIO";
	}

	/**
	 * @fn Return the kind of the nth input port
	 */
	virtual const char* getNthInputPortKind(int portIndex) const = 0;

	/**
	 * @fn Return the kind of the nth output port
	 */
	virtual const char* getNthOutputPortKind(int portIndex) const = 0;

	/**
	 * @fn Tell wether the port input need to be connected in order to get a valid algorithm
	 */
	virtual bool isInputPortConnectionMandatory(int portIndex) const = 0;

	/**
	 * @fn Tell wether the port output need to be connected in order to get a valid algorithm
	 */
	virtual bool isOutputPortConnectionMandatory(int portIndex) const = 0;

//##########################################
//####### ALGORITH CONSISTENCE CHECK #######
//##########################################

	/**
	 * @fn Tell if the Elaboration Unit is the final block of a musical chain
	 */
	virtual bool isFinalElaborationUnit() const = 0;

//##########################
//####### PROPERTIES #######
//##########################

	/**
	 * @return The number of available property
	 */
	virtual const int getPropertyNumber() const = 0;
	/**
	 * @return The property name of the nth property
	 */
	virtual const char* getNthPropertyName(int propId) const = 0;
	/**
	 * @return The direction of the property
	 */
	virtual e_PropertyDirection getNthPropertyDirection(int propId) const = 0;
	/**
	 * Return the type of the nth property
	 */
	virtual e_PropertyType getPropertyType(int propertyIndex) const = 0;
	/**
	 * Read the ranges of the property
	 */
	virtual int getPropertyRanges(int propertyIndex, char* lowRange, char* highRange, int bufferSize) const = 0;
	/**
	 * Retrieve the property interface
	 */
	virtual Property* getProperty(int propertyIndex) const = 0;

//##########################
//####### PROPERTIES #######
//##########################

};

