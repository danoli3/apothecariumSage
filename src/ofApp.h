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

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <future>
#include <algorithm>
#include <mutex>

#include "SurfingThemeEditor.h"
#include "SurfingConsole.h"



class ofApp : public ofBaseApp {
public:
	void setup() override;
	void exit() override;
	void draw() override;
	void update() override;
	void keyPressed(int key) override;
	void mouseMoved(int x, int y) override;

	void introUpdate();
	void introDraw();
	void backgroundUpdate();
	void backgroundDraw();
	void faderDraw();

	void runBashCommandAsync(const std::string & command, std::function<void(const std::string &)> callback);


	glm::vec3 make_point(float R, float r, float u, float v, float scale = 1.0);

	void windowFocusGained();
	void windowFocusLost();

	ofxSurfingGui ui;
	ofParameter<bool> bGui { "apothecariumSage", true };
	ofParameter<bool> bGuiUIConfig { "uiConfiguration", true };


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

	string text = "";
	float v = 0;

	SurfingThemeEditor e;

	ofParameter<bool> bGuiConsole { "Console", true };

	// A custom data struct for testing.
	dataCustom * customData = NULL;

	surfingConsole c = { 1 };

	void addToLog(std::string s);
	void queryYmlFiles();
	// workflow files
	std::vector<std::string> ymlFiles;
	std::map<std::string, bool> workflowStates;
	
	ofColor defaultBackground;

	std::atomic<bool> isRunning { false };
	std::mutex logMutex;
	std::vector<std::string> commandLog;


	// Shared mutex for thread-safe updates
	std::mutex stateMutex;




};
