#include "ofxShaderObject.hpp"

void ofxShaderObject::setup(string shaderPath) {
	this->shaderPath = shaderPath;
	shader.load(shaderPath);
}

void ofxShaderObject::update(const vector<float>& vols, const vector<bool>& beats, const vector<float>& beat_fade,
		float centroid, float time) {
	this->vols = vols;
	this->beats = beats;
	centr = ofMap(centroid, 0, 3000, 0.0, 1.0);
	this->time = time;
	this->beat_fade = beat_fade;
}

void ofxShaderObject::draw() {
	ofPushMatrix();
	shader.begin();
		setTransforms();
		setUniforms();
		ofDrawRectangle(0,0,ofGetWidth(),ofGetHeight());
	shader.end();
	ofPopMatrix();
}

void ofxShaderObject::setUniforms() {
	shader.setUniform1f("centr", centr);
	shader.setUniform1f("bass", vols[0]);
	shader.setUniform1f("mids", vols[1]);
	shader.setUniform1f("high", vols[2]);
	shader.setUniform1f("high_beat_fade", beat_fade[2]);
	shader.setUniform1f("bass_beat_fade", beat_fade[0]);
	shader.setUniform1i("beat", (int)beats[0]);
	shader.setUniform1f("u_time", time/1000.0);
	shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
}

void ofxShaderObject::setTransforms() {
	//ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
}

void ofxShaderObject::keyPressed(int key) { 
	if (key = 'r') {
		shader.load(shaderPath); 
	}
}
