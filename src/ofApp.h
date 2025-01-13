#pragma once

#include "ofMain.h"

#include "ofxSurfingImGui.h"

#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
#include <cstdlib>
#include <cstdio>
#include <array>
#include <filesystem>
namespace fs = std::filesystem;
#include <GLFW/glfw3.h>

class ofApp : public ofBaseApp {
public:
	void setup();
	void draw();
	void update() override;
	void keyPressed(int key);
	void mouseMoved(int x, int y) override;

	void introUpdate();
	void introDraw();
	void backgroundUpdate();
	void backgroundDraw();
	void faderDraw();


	glm::vec3 make_point(float R, float r, float u, float v, float scale = 1.0);

	void windowFocusGained();
	void windowFocusLost();

	ofxSurfingGui ui;
	ofParameter<bool> bGui { "ofApp", true };

	void ShowExampleTable();
	struct Data {
		int id;
		std::string name;
		float value;
		double largeValue;
		int anotherInt;
		std::string description;
	};
	std::vector<Data> data;

	void ShowAngledHeaders();

	std::string runBashCommand(const std::string & command);

	bool bFullscreen;
	bool bAllowPassThrough = true;
	bool bForcePassThrough = false;
	bool bDrawGuides = true;
	bool bLogVerbose = false;
	bool bCursorEnabled = true;
	bool isPaused = false;

	fs::path exeRoot;
	fs::path apothecaryRoot;

	enum APP_STATE { INTRO, BACKGROUND, COMPILING };
	APP_STATE state;

	float timerIntro = 0.0f;
	float timerIntroMAX = 10.0f;	

	ofEasyCam cam;
	ofMesh face, line;

	std::string baseText = "Apothecary Sage";
	std::string ellipsis = "";
	std::vector<std::string> ellipsisStates = { "", ".", "..", "...", "....." };
	int ellipsisIndex = 0;
	float ellipsisTimer = 0.0f;
	float ellipsisInterval = 0.5f; // Time between ellipsis updates

	ofTrueTypeFont customFont;
};
