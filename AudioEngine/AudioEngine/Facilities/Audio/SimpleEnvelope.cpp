
#include <stdio.h>
#include "SimpleEnvelope.h"

SimpleEnvelope::SimpleEnvelope(ModuleServices* pService, double _updatePeriod)
{
	state = None;
	updatePeriod = _updatePeriod;
	currentTime = 0;
	attackTime = 60;
	decayTime = 60;
	releaseTime = 400;
	attackLevel = 1.0;
	sustainLevel = 0.6;
	currentLevel = 0.0;

	m_pModuleServices = pService;
}

double SimpleEnvelope::getmsec(int msec)
{
	double dmsec = (double) msec;
	return dmsec / 1000.0;
}

void SimpleEnvelope::start(ADSR adsr)
{
	attackTime = adsr.attackTime;
	decayTime = adsr.decayTime;
	releaseTime = adsr.releaseTime;
	attackLevel = adsr.attackLevel;
	sustainLevel = adsr.sustainLevel;
	currentLevel = 0.0;
	currentTime = 0;
	setState(Attack);
	currentRatio = getRatioByState(state);
}

EAG_SAMPLE_TYPE SimpleEnvelope::getRatioByState(State state)
{
	switch(state)
	{
	case Attack:
		return attackLevel / (getmsec(attackTime) / updatePeriod);
		break;
	case Decay:
		return (sustainLevel - attackLevel) / (getmsec(decayTime) / updatePeriod);
		break;
	case Release:
		return -sustainLevel / (getmsec(releaseTime) / updatePeriod);
		break;
	default:
		return 0.0;
	}
}

void SimpleEnvelope::setState(State _state)
{
	state = _state;
	char* strState = "Unknown";
	switch (state)
	{
		case None:
			strState = "None";
			break;
		case Attack:
			strState = "Attack";
			break;
		case Decay:
			strState = "Decay";
			break;
		case Sustain:
			strState = "Sustain";
			break;
		case Release:
			strState = "Release";
			break;
		case Completed:
			strState = "Completed";
			break;
	}
	m_pModuleServices->pLogger->writeLine("Env State: %s", strState);
}

EAG_SAMPLE_TYPE SimpleEnvelope::getUpdatedLevel(void)
{
	currentTime += updatePeriod;
	switch(state)
	{
	case None:
		break;
	case Attack:
		if(currentTime>getmsec(attackTime))
		{
			setState(Decay);
			currentRatio = getRatioByState(state);
			m_pModuleServices->pLogger->writeLine("ADSR-State:  Attack Complete");
		}
		break;
	case Decay:
		if(currentTime>getmsec(attackTime + decayTime))
		{
			setState(Sustain);
			currentRatio = 0.0;
			m_pModuleServices->pLogger->writeLine("ADSR-State:  Decay Complete");
		}
		break;
	case Sustain:
		break;
	case Release:
		if ((currentTime - envOffTime) > getmsec(releaseTime))
		{
			if (currentLevel < 0.0)
			{
				currentLevel = 0.0;
				setState(Completed);
			}
			
		}
		//m_pModuleServices->pLogger->writeLine("ADSR-State:  Release Complete");
		break;
	case Completed:
		break;
	default:
		break;
	}
	currentLevel += currentRatio;

	//m_pModuleServices->pLogger->writeLine("currentLevel=%f", currentLevel);

	return currentLevel;
}

void SimpleEnvelope::stop(void)
{
	setState(Release);
	currentRatio = getRatioByState(state);
	envOffTime = currentTime;
	m_pModuleServices->pLogger->writeLine("envOffTime=%f", envOffTime);
}

EAG_SAMPLE_TYPE SimpleEnvelope::getCurrentLevel(void)
{
	return currentLevel;
}

bool SimpleEnvelope::releaseCompleted(void)
{
	if(state==Completed)
		return true;
	else
		return false;
}