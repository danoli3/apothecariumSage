#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofGLFWWindowSettings settings;
	settings.transparent = true;
	settings.mousePassThrough = false;
	settings.floating = true;
	settings.decorated = true;
	settings.windowMode = OF_WINDOW;
	settings.setGLVersion(4, 1);
	settings.setSize(1700, 900);
	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();
}
