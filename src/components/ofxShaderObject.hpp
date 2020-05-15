#pragma once

#include "ofMain.h"

class ofxShaderObject {
	public:
		void setup(string shaderPath);
		void update(const vector<float>& vols, const vector<bool>& beats, const vector<float>& beat_fade, float
				centroid, float time);
		void draw();
		void setUniforms();
		void setTransforms();
		void keyPressed(int key);

	private:
		ofShader shader;
		string shaderPath;
		float centr;
		vector<float> vols;
		vector<bool> beats;
		vector<float> beat_fade;
		float time;
};
