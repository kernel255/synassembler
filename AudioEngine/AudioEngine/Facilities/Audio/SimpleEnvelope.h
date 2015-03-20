
#pragma once

#include "..\..\DSPTypes.h"
#include "..\..\ElaborationUnits\ModuleServices.h"

class SimpleEnvelope
{
private:
	/**
	 * @var Attack time in ms
	 */
	int attackTime;
	/**
	 * @var Decay time in ms
	 */
	int decayTime;
	/**
	 * @var Release time in ms
	 */
	int releaseTime;
	EAG_SAMPLE_TYPE	attackLevel;
	EAG_SAMPLE_TYPE sustainLevel;
	EAG_SAMPLE_TYPE currentLevel;
	EAG_SAMPLE_TYPE currentRatio;
	enum State {
		None,
		Attack,
		Decay,
		Sustain,
		Release,
		Completed
	};
	State state;
	EAG_SAMPLE_TYPE updatePeriod;
	EAG_SAMPLE_TYPE currentTime;
	EAG_SAMPLE_TYPE envOffTime;
	EAG_SAMPLE_TYPE getRatioByState(State state);
	void setState(State state);
public:
	/**
	 * @param updatePeriod Time in secons when the update function will be called periodically
	 */
	SimpleEnvelope(ModuleServices* pService, double updatePeriod);
	void start(void);
	/**
	 * @fn Updates the Envelope by moving ahead the time by an updatePeriod unit
	 */
	EAG_SAMPLE_TYPE getUpdatedLevel(void);
	void stop(void);
	EAG_SAMPLE_TYPE getCurrentLevel(void);
	bool releaseCompleted(void);
private:
	double getmsec(int msec);
	ModuleServices* m_pModuleServices;
};