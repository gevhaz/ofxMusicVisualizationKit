#pragma once

#include "ofMain.h"

class ofxMusicPlayer {
	public:
		void load(string songName, float audioVolume);
		void keyPressed(int key);
		float getTime();

	private:
		ofSoundPlayer player;
};
