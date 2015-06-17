
#pragma once

#include "..\..\DSPTypes.h"
#include "..\..\ElaborationUnits\ModuleServices.h"
#include "ADSR.h"

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
	void start(ADSR adsr);
	/**
	 * @fn Updates the Envelope by moving ahead the time by an updatePeriod unit
	 */
	EAG_SAMPLE_TYPE getUpdatedLevel(void);
	void stop(void);
	EAG_SAMPLE_TYPE getCurrentLevel(void);
	bool releaseCompleted(void);
	
	/*
	void setAttackTime(int attack)	{	attackTime = attack;	}
	int getAttackTime()	{ return attackTime; }

	void setDecayTime(int decay) { decayTime = decay; }
	int getDecayTime() { return decayTime; }

	void setReleaseTime(int release) { releaseTime = release; }
	int getReleaseTime() { return releaseTime; }

	void setAttackLevel(EAG_SAMPLE_TYPE attack) { attackLevel = attack; }
	EAG_SAMPLE_TYPE* getAttackLevel() { return &attackLevel; }

	void setSustainLevel(EAG_SAMPLE_TYPE sustain) { sustainLevel = sustain; }
	EAG_SAMPLE_TYPE getSustainLevel() { return sustainLevel; }
	*/
private:
	double getmsec(int msec);
	ModuleServices* m_pModuleServices;
};