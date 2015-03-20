
#pragma once

/**
 * @class Holds all the settings of the Engine (e.g. sampling frequency, etc...)
 */
struct EngineSettings {
public:
	EngineSettings(double _samplingFrequence = 48000.0, int _bitResolution = 16, int _numChannels = 1) {
		samplingFrequence = _samplingFrequence;
		bitResolution = _bitResolution;
		numChannels = _numChannels;
	};
	EngineSettings(EngineSettings& src)
	{
		samplingFrequence = src.samplingFrequence;
		bitResolution = src.bitResolution;
		numChannels = src.numChannels;
	}
	double samplingFrequence;
	int bitResolution;
	int numChannels;
};