#include "ofxSoundAnalyzer.hpp"

void ofxSoundAnalyzer::setup(float volume, float smoothing, const vector<int>& lims) {
	adjustments.assign(lims.size(), 1.0);
	setup(volume, smoothing, lims, adjustments);
}

void ofxSoundAnalyzer::setup(float volume, float smoothing, const vector<int>& lims, const vector<float>& adjustments) { 
	audioVolume = volume;
	this->smoothing = smoothing;
	this->lims = lims;
	this->adjustments = adjustments;
	fftBands = FFTBANDS;
	previousSpectrum.fill(0.0);

	int nRanges = lims.size();
	meanVolumes.resize(nRanges);
	setupBeatDetection(nRanges);
	beatFade.resize(nRanges);
}

void ofxSoundAnalyzer::setupBeatDetection(int nRanges) {
	beatDetections.resize(nRanges);
	decayRates.assign(nRanges, 0.09);
	decayRates[1] = 0.03;
	minimumThresholds.assign(nRanges, 0.15);
	thresholds.assign(nRanges, 0.15); //should be same as min thresholds
}

void ofxSoundAnalyzer::analyze() {
	ofSoundUpdate();
	spectrum = ofSoundGetSpectrum(fftBands);

	// Smoothing
	for (int i = 0; i < fftBands; ++i) {
		spectrum[i] = ofLerp(spectrum[i], previousSpectrum[i], smoothing);
		previousSpectrum[i] = spectrum[i];
	}

	// Get means (which might be very small) and normalize (needs to be set)
	int lastLim = 0;
	for (size_t i = 0; i < lims.size(); ++i) {
		meanVolumes[i] = getMeanValue(spectrum, lastLim, lims[i]) / audioVolume;
		meanVolumes[i] = ofMap(meanVolumes[i], 0, adjustments[i], 0.0, 1.0, true);
		if (beatDetections[i]) {
			beatFade[i] = ofLerp(beatFade[i], 1.0, 0.3);
		} else {
			beatFade[i] = ofLerp(beatFade[i], 0.0, 0.02);
		}
		lastLim = lims[i];
	}
	
	// Centroid
	centroid = calculateCentroid(spectrum);

	// Onset detection
	detectBeats();
}

float ofxSoundAnalyzer::getMeanValue(float * vals, int startIndex, int endIndex) {
	float meanValue = 0;
	for (int i = startIndex; i < endIndex; ++i) {
		meanValue += vals[i];
	}
	return meanValue /= (endIndex - startIndex);
}

void ofxSoundAnalyzer::detectBeats() {
	// Onset detection for all the ranges being analyzed
	for (size_t i = 0; i < beatDetections.size(); ++i) {
		thresholds[i] = ofLerp(thresholds[i], minimumThresholds[i], decayRates[i]);
		if (meanVolumes[i] > 1.2 * thresholds[i]) {
			thresholds[i] = meanVolumes[0];
			beatDetections[i] = true;
		} else if (meanVolumes[i] > thresholds[i]) {
		// if ( meanVolumes[i] > thresholds[i] && meanVolumes[i] > 0.5) {
			thresholds[i] = meanVolumes[i];
			beatDetections[i] = true;
		} else {
			beatDetections[i] = false;
		}
	}
}

int ofxSoundAnalyzer::calculateCentroid(float* vals) {
	float cumulativeSum = 0;
	float centroidNormalization = 0;
	float nyquistFrequency = 22050; //assuming the sample rate is 44100
	for (int i = 0; i < fftBands; ++i) {
		cumulativeSum += i * vals[i];
		centroidNormalization += vals[i];
	}
	int indexOfMeanFrequency = 0;
	if (centroidNormalization) {
		indexOfMeanFrequency = (int)(cumulativeSum / centroidNormalization);
	}
	float centroidFrequency = indexOfMeanFrequency * (nyquistFrequency / fftBands);
	return centroidFrequency > 10000 ? 1 : centroidFrequency; //otherwise gigantic when volume is low 
}
