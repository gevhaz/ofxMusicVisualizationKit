#pragma once

#define FFTBANDS 512
#include "ofMain.h"

class ofxSoundAnalyzer {

	public:
		void setup(float volume, float smoothing, const vector<int>& lims);
		void setup(float volume, float smoothing, const vector<int>& lims, const vector<float>& adjustments);
		void analyze();

		vector<float> getVols() {return meanVolumes; }
		vector<bool> getBeats() { return beatDetections; }
		vector<float> getBeatFades() {return beatFade; }
		float getCentroid() { return centroid; }

	private:
		float getMeanValue(float * vals, int startIndex, int endIndex);
		void setupBeatDetection(int nRanges);
		void detectBeats();
		int calculateCentroid(float* vals);

		int fftBands;
		float * spectrum;

		// Smoothing
		std::array<float, FFTBANDS> previousSpectrum;
		float smoothing;
		float audioVolume;

		// Decides which bin the freqency range should end by
		vector<int> lims;

		// Adjusting for low peak volume (different for each range)
		vector<float> adjustments;

		// Sound analysis results (can be got with the get- functions)
		vector<float> meanVolumes;
		vector<bool> beatDetections;
		vector<float> beatFade;
		float centroid;

		// onset detection (settings can be adjusted for the different ranges)
		vector<float> thresholds;
		vector<float> minimumThresholds;
		vector<float> decayRates;
};
