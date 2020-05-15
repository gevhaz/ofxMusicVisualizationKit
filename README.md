# Description

This plugin provides three classes to simplify music visualization with 
shaders:

* **ofxSoundAnalyzer** – Provides functions that return basic 
  information such as volumes for ranges of bins from the FFT of an 
  ofSoundPlayer.

* **ofxMusicPlayer** – creates an ofSoundPlayer object and provides 
  keyboard shortcuts for playback.

* **ofxShaderObject** – helps setup, update and draw with an ofShader 
  object. Meant for use with ofxSoundAnalyzer to transfer results of FFT 
  analysis to uniforms for the shader.

This addon is mainly meant for my personal use but if anyone comes 
across it and want to use it, feel free to open an issue if you 
encounter any problems.

I provide explanations for how to use the classes that come with the 
plugin in the "Usage" section but the easiest way to get started is 
probably to have a look at [an 
example](https://github.com/gevhaz/32_delete-forever). It's really 
short, which is the whole purpose of making this plugin.

# Compatibility

Tested on OpenFrameworks version 0.11.0 for Linux.

# Dependencies

None other than a basic openFrameworks installation.

# Usage

1.  Add ofxMusicVisualizationKit as an addon (for example by editing 
    `addons.make`).

2.  Include the line:

    ```
    #include "ofxMusicVisualizationKit.hpp"
    ```

    in `ofApp.h`.

## How to use ofxSoundAnalyzer

1. Create a ofxSoundAnalyzer object in your `ofApp.h`

2. Call the objects setup() function in ofApp::setup() and provide the 
   following parameters:

   * Audiovolume (0.0–1.0)
   
   * Smoothing (0.0–1.0)
   
   * A vector of what bins in the spectrum you want the breakpoints to 
     be at. By default, a ShaderObject object can send the volume for 
     three ranges to the shader, although you can modify 
     `ofxShaderObject::setUniforms()` to send more if you want. To make 
     the bass range bin 1–3, the mids range 4–20 and highs range 21-90, 
     you would send a vector such as the following:

     ```
     vector<int> lims { 4,20,90 };
     ```

     The number of bins available is hardcoded to 512.


   * (optional) a vector of adjustments to the volumes. The mean volumes 
     of higher bin ranges from the FFT spectrum tends to be low so you 
     will either want to assume that they are low when writing the 
     shader or adjust them to be around to 0.0–1.0 range. Mean volumes 
     sent to the shader are capped at 1.0 so it's okay if you increase 
     the volume to more than 1.0. The adjustments are basically the 
     value that the max volume for the range should be, so if the volume 
     for the 21-90 range from above never goes above 0.234, you'd set 
     the third value of the vector to 0.234. An example vector to send 
     as the fourth parameter would be:

     ```
     vector<float> adjs { 0.524587, 0.113462, 0.0529223 };
     ```

3.  Call `analyze()` in `ofApp::update()`. E.g. if you named the 
    ofSoundAnalyzer object analyzer:

    ```
    analyzer.analyze();
    ```

4.  Also in `ofApp::update()`: use the results of the analysis. You will 
    probably want to just send them to an ofxShaderObject. That's the 
    use case they're meant for. The results can be gotten by calling the 
    functions:

    * getVols() – the most useful one, providing the mean volumes over 
      ranges of bins.

    * getBeats() – very primitive beat detection. Returns a vector of 
      booleans which are true when a beat is detected for a range. The 
      ranges in questions are the same ones we set up in step 2. Beat 
      detection is super hard and so this function has a very large 
      number of false positives.

    * getBeatFades() – A value between 0.0 and 1.0 that increases when 
      there is a beat for a range and decreases when there isn't. In 
      other words, it's a more continuous way of representing a beat.

    * getCentroid() – returns the centroid frequency.

## How to use ofxMusicPlayer

1.  Create an object in `ofApp.h`. Let's call it `musicPlayer`.

2.  In `ofApp::setup()`, call `musicPlayer.setup(songpath, 
    audiovolume)`.

3.  In `ofApp::keyPressed(int key)`, transmit the keycode to the 
    ofxMusicPlayer object's `keyPressed()` function: 
    `musicPlayer.keyPressed(key)`.

You can also get the current time in the playback of a song with 
getTime().

The keyboard shortcuts for controlling the music is:

*   <kbd>Space</kbd>: Play/pause
*   <kbd>→</kbd>: Jump 15 seconds forward
*   <kbd>←</kbd>: Jump 15 seconds back
*   <kbd>s</kbd>: Stop playback
*   <kbd>f</kbd>: Switch your app to fullscreen

## How to use ofxShaderObject

1.  Create an ofxShaderObject, let's call it `shader`.

2.  In `ofApp::setup()`, provide the path of the shader. The current 
    method is based in the one-parameter call to ofShader's [load 
    function](https://openframeworks.cc/documentation/gl/ofShader/#show_load) 
    so you will need to use the same name for the fragment and vertex 
    shader. An example call: `shader.load("shaders/shader")`.

3.  In `ofApp:update()` you will need to call the ofxShaderObject's 
    update function with the values that will be sent as uniforms to the 
    shader. The values to be sent are: sound volumes, beats, continuous 
    beat detections, centroid, and playback position. This is meant to 
    be used with ofxSoundAnalyzer and ofxMusicPlayer, so the simplest 
    way to do it, with on ofxSoundAnalyzer object called analyzer and an 
    ofxMusicPlayer object called musicPlayer, is:

    ```
	shader.update(analyzer.getVols(), 
                  analyzer.getBeats(), 
                  analyzer.getBeatFades(), 
                  analyzer.getCentroid(), 
                  musicPlayer.getTime());
    ```

For reloading the shader during the development with the <kbd>R</kbd> 
key, call `shader.keyPressed(key)` in `ofApp::keyPressed(int key)`.

# Licence

All code is written by me and licenced under GPLv3.
