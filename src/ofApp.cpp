#include "ofApp.h"
#include <cstdio>

void windowFocusCallback(GLFWwindow * window, int focused) {
	ofApp * app = static_cast<ofApp *>(glfwGetWindowUserPointer(window));
	if (app) {
		if (focused) {
			app->windowFocusGained();
		} else {
			app->windowFocusLost();
		}
	}
}

void ofApp::exit() {
	if (customData) {
		delete customData;
		customData = NULL;
	}
}

//--------------------------------------------------------------
void ofApp::setup()
{
	if (bLogVerbose) {
		ofSetLogLevel(OF_LOG_VERBOSE);
	}
	int screenW = ofGetScreenWidth();
	int screenH = ofGetScreenHeight();
	ofSetWindowPosition(screenW / 2 - 1700 / 2, screenH / 2 - 900 / 2);

	ofSetWindowTitle("apothecary Sage");
	ofSetFrameRate(60);
	uint64_t stepNanos = ofGetFixedStepForFps(60);
	ofSetTimeModeFixedRate(stepNanos);
	ofShowCursor();

	ofSetupScreen();
	bFullscreen = 0;
	ofBackground(0, 0, 0, 0);

	ofSetLineWidth(1);
	//ofEnableDepthTest();

	state = INTRO;

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	customFont.load("fonts/OCR.ttf", 48);

//	ofxSurfingHelpers::setMonitorsLayout(0, true, true);
	ui.setup();
	string path = "fonts/OCR.ttf";
	float size = 14;
	ui.setupFontForDefaultStyles(ofToDataPath(path), size);
	e.setPathGlobal(ui.getPath());
	e.setUiPtr(&ui);
	e.setup();
	
//	GLFWwindow* window = ofGetWindowPtr()->getGLFWWindow();
//	float pixelDensityX, pixelDensityY;
//   glfwGetWindowContentScale(window, &pixelDensityX, &pixelDensityY);
//   float pixelDensity = pixelDensityX;  // Use X scale for simplicity

   // Scale ImGui UI
//   ImGui::GetIO().FontGlobalScale = 2.0;

   // Load Retina-scaled font
//   ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/Roboto-Regular.ttf", 16.0f * pixelDensity);

   // Optional: Adjust framebuffer scaling
//   ImGui::GetIO().DisplayFramebufferScale = ImVec2(2, 2);
	
	queryYmlFiles();
	

	customData = new dataCustom();
	customData->color = ofColor(7,7,7,240);
	customData->lastCommand = "hello";
	customData->frameNum = -1;

	 try {
		 ofLogNotice() << "Current Path: " << fs::current_path();
		
		fs::path projectPath = fs::weakly_canonical(fs::current_path());
		fs::path projectNamePath = projectPath.filename();
		apothecaryRoot = projectNamePath.string();

		ofFile exeFile(projectPath);
		
		exeRoot = exeFile.getEnclosingDirectory(); // Get directory of the executable
		apothecaryRoot = ofFile(exeRoot.string() + "../").getAbsolutePath(); // Move two directories up
		ofLogNotice() << "Weakly Canonical Path: " << projectPath;
		ofLogNotice() << "Project Name Path: " << projectNamePath;
		ofLogNotice() << "Apothecary Root (after moving up): " << apothecaryRoot;

	 } catch (const std::exception & ex) {
		 ofLogError() << "Error: " << ex.what();
		 apothecaryRoot = "";
	 }
	
	std::string lsResult = runBashCommand("ls -l");
	ofLogNotice() << lsResult;

	//std::string magic_up = "./magic_up.sh";
	//ofLogNotice() << magic_up;	
	//std::string magicUpResult = runBashCommand(magic_up + " pwd");
	//ofLogNotice() << magicUpResult;

	//std::string command_one = "." + magicUpResult + "/scripts/calculate_formulas.sh";
	//command_one.erase(std::remove(command_one.begin(), command_one.end(), '\n'), command_one.end());
	//ofLogNotice() << command_one;
	//std::string calculateFormulasResult = runBashCommand(command_one);
	//ofLogNotice() << calculateFormulasResult;



	std::string magicUpResult = runBashCommand("pwd");
	ofLogNotice() << magicUpResult;
	magicUpResult = runBashCommand("ls -a");
	ofLogNotice() << magicUpResult;

	std::string magic_up = "./apothecary";
	std::string command_one = magic_up + "/scripts/calculate_formulas.sh";
	command_one.erase(std::remove(command_one.begin(), command_one.end(), '\n'), command_one.end());
	ofLogNotice() << command_one;
	std::string calculateFormulasResult = runBashCommand(command_one);
	ofLogNotice() << calculateFormulasResult;
	
	std::string calculateFormulasExport = runBashCommand("export TARGET=vs;export BUNLDE=1;");
	ofLogNotice() << calculateFormulasExport;
	std::string calculateFormulasExportResult = runBashCommand(command_one);
	ofLogNotice() << calculateFormulasExportResult;

	std::string command_two = "TARGET=linux ARCH=64 BUNLDE=1 ./" + magic_up + "/scripts/calculate_formulas.sh";
	ofLogNotice() << command_two;
	std::string calculateFormulasVSResult = runBashCommand(command_two);
	ofLogNotice() << calculateFormulasVSResult;

	 // GLFWwindow * window = static_cast<ofAppGLFWWindow *>(ofGetWindowPtr())->getGLFWWindow();

	 //// Set the user pointer to pass the ofApp instance
	 //glfwSetWindowUserPointer(window, this);

	 //// Set the focus callback
	 //glfwSetWindowFocusCallback(window, windowFocusCallback);

	/*data = new dataCustom();
	data->color = ofColor::fuchsia;
	data->lastCommand = "hello";
	data->frameNum = -1;*/

}

//--------------------------------------------------------------
void ofApp::draw() {
	if (isPaused) {
		return;
	}

	if (timerIntro <= timerIntroMAX) {
		faderDraw();
	}

	int screenWidth = ofGetWidth();
	int screenHeight = ofGetHeight();
	if (bDrawGuides) { // Draw edge of transparent window for clarity
		
		//ofSetColor(ofColor(0, 174, 0, 196));
		ofSetColor(ofColor(255, 102, 0, 196));
		int padding = 7;
		ofDrawRectangle(0, 0, screenWidth, padding);
		ofDrawRectangle(0, screenHeight - padding, screenWidth, padding);
		ofDrawRectangle(0, padding, padding, screenHeight - 2 * padding);
		ofDrawRectangle(screenWidth - padding, padding, padding, screenHeight - 2 * padding);

	}
	ofSetColor(ofColor(7, 7, 7, 220));
	ofDrawRectangle(1, 1, screenWidth-3, screenHeight-3);

	if (state == INTRO) {
		introDraw();
		return;
	} else if (state == BACKGROUND) {
		backgroundDraw();
	} else if (state == COMPILING) {
		//backgroundDraw();
	}

//	ofClear(customData->color);
	//update a var of the data
	customData->frameNum = ofGetFrameNum();

	string s;

	ui.Begin();
	{
//		static bool bTheme = false;
//		static bool b = 0;
//		if (bTheme) {
//			bTheme = 0;
//			if (!b) {
//				b = 1;
//				ofxImGuiSurfing::ImGui_ThemeMoebiusSurfingBlue();
//			}
//		}
		

		if (ui.BeginWindow(bGui)) 
		{
			ui.Add(e.bGui, OFX_IM_TOGGLE_ROUNDED);
		
			ShowAngledHeaders();
			
			static ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable;
				const int columnsCount = 3;  // Columns: Filename, State, Buttons
				const int rowsCount = ymlFiles.size();

				if (ImGui::BeginTable("workflow_table", columnsCount, tableFlags)) {
					// Set up columns
					ImGui::TableSetupColumn("Filename", ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
					ImGui::TableSetupColumn("State", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableHeadersRow();

					// Populate rows
					for (int row = 0; row < rowsCount; row++) {
						const std::string& fileName = ymlFiles[row];
						bool& isEnabled = workflowStates[fileName];

						ImGui::PushID(row);
						ImGui::TableNextRow();

						// Column 0: Filename
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("%s", fileName.c_str());

						// Column 1: State
						ImGui::TableSetColumnIndex(1);
						ImGui::TextColored(isEnabled ? ImVec4(0, 1, 0, 1) : ImVec4(1, 0, 0, 1), isEnabled ? "Enabled" : "Disabled");

						// Column 2: Actions
						ImGui::TableSetColumnIndex(2);

						// Enable Button
						if (!isEnabled && ImGui::Button("Enable")) {
							// Run a command to enable the workflow
							std::string enableCommand = "sed -i 's/DISABLE_WORKFLOW: \"true\"/DISABLE_WORKFLOW: \"false\"/' ./apothecary/.github/workflows/" + fileName + ".yml";
							runBashCommand(enableCommand.c_str());
							addToLog(fileName + " enableed");
							isEnabled = true;
						}
						ImGui::SameLine();

						// Disable Button
						if (isEnabled && ImGui::Button("Disable")) {
							// Run a command to disable the workflow
							std::string disableCommand = "sed -i 's/DISABLE_WORKFLOW: \"false\"/DISABLE_WORKFLOW: \"true\"/' ./apothecary/.github/workflows/" + fileName + ".yml";
							runBashCommand(disableCommand.c_str());
							addToLog(fileName + " disabled");
							isEnabled = false;
						}

						ImGui::PopID();
					}

					ImGui::EndTable();
				}

			ui.EndWindow();
		}

		if (ui.BeginWindow(bGuiConsole)) {			

			ui.AddLabelBig("Apothecary Command Center");
			ui.AddSpacingSeparated();
			ui.AddLabel("frameNum:\n" + ofToString(customData->frameNum));
			ui.AddLabel("lastCommand:\n" + ofToString(customData->lastCommand));
			ui.AddSpacingSeparated();
			if (ui.AddButton("HELP")) {
				s = "[ HELP ]";
				addToLog(s);

				c.help_();
				customData->lastCommand = "added help";
			}
			if (ui.AddButton("CLEAR")) {
				s = "[ CLEAR ]";
				addToLog(s);

				c.clear_();
				customData->lastCommand = "added clear";
			}
			ui.AddSpacingSeparated();
			ui.AddSpacingSeparated();
			ui.Add(c._terminalSizeLimit);
			ui.Add(c._maxHistoryLines);
			ui.AddSpacingSeparated();
			
#if disabled

			// Define platform and architecture options
			std::vector<string> platforms = { "osx", "linux", "vs", "msys2", "ios", "tvos", "android", "emscripten" };
			std::vector<string> architectures = { "32", "64", "arm", "arm64", "x86", "x86_64" };
			ofParameter<int> selectedPlatform = 0; // Default to "osx"
			ofParameter<int> selectedArchitecture = 0; // Default to "32"

			// Dropdowns for platform and architecture
			ui.AddLabel("Platform");
			if (ui.AddCombo(selectedPlatform, platforms)) {
				ofLogNotice() << "Selected platform: " << platforms[selectedPlatform];
			}
			ui.AddLabel("Architecture");
			if (ui.AddCombo(selectedArchitecture, architectures)) {
				ofLogNotice() << "Selected architecture: " << architectures[selectedArchitecture];
			}

			// Core Commands Section
			ui.AddLabel("Core Commands");
			if (ui.AddButton("Update Core", OFX_IM_BUTTON)) {
				std::string command = "./apothecary/apothecary -t" + std::string(platforms[selectedPlatform]) + " -a" + std::string(architectures[selectedArchitecture]) + " update core";
				ofLogNotice() << "Executing command: " << command;
				system(command.c_str());
			}

			if (ui.AddButton("Clean Core", OFX_IM_BUTTON)) {
				std::string command = "./apothecary/apothecary -t" + std::string(platforms[selectedPlatform]) + " -a" + std::string(architectures[selectedArchitecture]) + " clean core";
				ofLogNotice() << "Executing command: " << command;
				system(command.c_str());
			}

			// Addons Commands Section
			ui.AddLabel("Addons Commands");
			if (ui.AddButton("Update Addons", OFX_IM_BUTTON)) {
				std::string command = "./apothecary/apothecary -t" + std::string(platforms[selectedPlatform]) + " -a" + std::string(architectures[selectedArchitecture]) + " update addons";
				ofLogNotice() << "Executing command: " << command;
				system(command.c_str());
			}

			if (ui.AddButton("Clean Addons", OFX_IM_BUTTON)) {
				std::string command = "./apothecary/apothecary -t" + std::string(platforms[selectedPlatform]) + " -a" + std::string(architectures[selectedArchitecture]) + " clean addons";
				ofLogNotice() << "Executing command: " << command;
				system(command.c_str());
			}

			ui.AddLabelBig("Post Commands");
			ui.AddLabel("setColor");
			if (ui.AddButton("red", OFX_IM_BUTTON, 2, true)) {
				s = "setColor red";
				c.addLineCommnand(s, customData);
			}
			if (ui.AddButton("blue", OFX_IM_BUTTON, 2)) {
				s = "setColor blue";
				c.addLineCommnand(s, customData);
			}
			if (ui.AddButton("colorRandom", OFX_IM_BUTTON)) {
				s = "colorRandom";
				c.addLineCommnand(s, customData);
			}
#endif
			ui.EndWindow();
		}
		c.show(customData);
		// Editor
		

		e.draw();
		// Demo
		ImGui::ShowDemoWindow();

		
	}
	ui.End();
}

void ofApp::update() {
	if (isPaused) {
		return;
	}
	if (bFullscreen && bCursorEnabled == true) {
		ofHideCursor();
		bCursorEnabled = false;
	} else if (bFullscreen == false && bCursorEnabled == false) {
		ofShowCursor();
		bCursorEnabled = true;
	}

	int posx = ofGetWindowPositionX();
	int posy = ofGetWindowPositionY();

	ellipsisTimer += ofGetLastFrameTime();
	if (ellipsisTimer > ellipsisInterval) {
		ellipsisTimer = 0.0f;
		ellipsisIndex = (ellipsisIndex + 1) % ellipsisStates.size();
		ellipsis = ellipsisStates[ellipsisIndex];
	}

	if (state == INTRO) {
		introUpdate();
		return;
	} else if (state == BACKGROUND) {
		backgroundUpdate();
	} else if (state == COMPILING) {
		//backgroundUpdate();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (ui.isMouseOverInputText()) return;
	if (ui.isOverGui()) return; // could be commented if preferred


	if (key == ' ') {

	} else if (key == 'f' || key == 'F') {
		bFullscreen = !bFullscreen;
		if (!bFullscreen) {
			ofSetWindowShape(1280, 720);
			ofSetFullscreen(false);
			int screenW = ofGetScreenWidth();
			int screenH = ofGetScreenHeight();
			ofSetWindowPosition(screenW / 2 - 1280 / 2, screenH / 2 - 720 / 2);
		} else if (bFullscreen == 1) {
			ofSetFullscreen(true);
		}
		ofLogNotice("ofApp") << "bFullscreen:" << bFullscreen;

	} else if (key == 'g' || key == 'G') {
		bDrawGuides = !bDrawGuides;
		ofLogNotice("ofApp") << "bDrawGuides:" << bDrawGuides;

	} else if (key == 'h' || key == 'H') {
		bAllowPassThrough = !bAllowPassThrough;
		bForcePassThrough = !bForcePassThrough;
		ofSetWindowMousePassThrough(bAllowPassThrough);
		ofLogNotice("ofApp") << "ofSetWindowMousePassThrough:" << bAllowPassThrough << " Force:" << bForcePassThrough;

	} else if (key == 'l' || key == 'L') {
		bLogVerbose = !bLogVerbose;
		if (bLogVerbose)
			ofSetLogLevel(OF_LOG_VERBOSE);
		else
			ofSetLogLevel(OF_LOG_NOTICE);
		ofLogNotice("ofApp") << "bLogVerbose:" << bLogVerbose;
	}
}

// Another function to display a table with data
//--------------------------------------------------------------
void ofApp::ShowAngledHeaders() {
	// Using those as a base value to create width/height that are factor of the size of our font
	const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
	const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

	if (ImGui::TreeNode("Apothecary Build Matrix")) {
		const char * column_names[] = { "", "arm", "arm64", "x86", "x86_64", "jetson", "armv7l", "armv6l", "aarch64", "arm64ec" };
		const int columns_count = IM_ARRAYSIZE(column_names);
		const char * platform_names[] = { "Windows", "Linux", "MacOS", "iOS", "Android" };
		const char * platform_types[] = { "vs", "linux", "osx", "ios", "android" };
		const int rows_count = IM_ARRAYSIZE(platform_types);

		static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_RowBg;

		if (ImGui::BeginTable("apothecary_build_table", columns_count, table_flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * (rows_count + 1)))) {
			ImGui::TableSetupColumn(column_names[0], ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
			for (int n = 1; n < columns_count; n++) {
				ImGui::TableSetupColumn(column_names[n], ImGuiTableColumnFlags_WidthFixed);
			}
			ImGui::TableHeadersRow();
			for (int row = 0; row < rows_count; row++) {
				ImGui::PushID(row);
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("%s", platform_types[row]);
				for (int column = 1; column < columns_count; column++) {
					if (ImGui::TableSetColumnIndex(column)) {
						ImGui::PushID(column);
						std::string button_label = "Build##" + std::to_string(row) + "_" + std::to_string(column);
						if (ImGui::Button(button_label.c_str())) {
							std::string command = "./apothecary/apothecary -t" + std::string(platform_types[row]) + " -a" + std::string(column_names[column]) + " update core";
							ofLogNotice() << "Executing command: " << command;
							runBashCommand(command.c_str());
						}
						ImGui::PopID();
					}
				}
				ImGui::PopID();
			}
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
#if disabled
	if (ImGui::TreeNode("Angled headers"))
	{
		const char* column_names[] = { "Track", "cabasa", "ride", "smash", "tom-hi", "tom-mid", "tom-low", "hihat-o", "hihat-c", "snare-s", "snare-c", "clap", "rim", "kick" };
		const int columns_count = IM_ARRAYSIZE(column_names);
		const int rows_count = 12;

		static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_Hideable | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_HighlightHoveredColumn;
		static ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_AngledHeader | ImGuiTableColumnFlags_WidthFixed;
		static bool bools[columns_count * rows_count] = {}; // Dummy storage selection storage
		static int frozen_cols = 1;
		static int frozen_rows = 2;
		if (ImGui::TreeNode("Settings"))
		{
		ImGui::CheckboxFlags("_ScrollX", &table_flags, ImGuiTableFlags_ScrollX);
		ImGui::CheckboxFlags("_ScrollY", &table_flags, ImGuiTableFlags_ScrollY);
		ImGui::CheckboxFlags("_Resizable", &table_flags, ImGuiTableFlags_Resizable);
		ImGui::CheckboxFlags("_NoBordersInBody", &table_flags, ImGuiTableFlags_NoBordersInBody);
		ImGui::CheckboxFlags("_HighlightHoveredColumn", &table_flags, ImGuiTableFlags_HighlightHoveredColumn);
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
		ImGui::SliderInt("Frozen columns", &frozen_cols, 0, 2);
		ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
		ImGui::SliderInt("Frozen rows", &frozen_rows, 0, 2);
		ImGui::CheckboxFlags("Disable header contributing to column width", &column_flags, ImGuiTableColumnFlags_NoHeaderWidth);
		ImGui::TreePop();
		}

		if (ImGui::TreeNode("Style settings"))
		{
			ImGui::SameLine();
			//HelpMarker("Giving access to some ImGuiStyle value in this demo for convenience.");
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
			ImGui::SliderAngle("style.TableAngledHeadersAngle", &ImGui::GetStyle().TableAngledHeadersAngle, -50.0f, +50.0f);
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
			ImGui::SliderFloat2("style.TableAngledHeadersTextAlign", (float*)&ImGui::GetStyle().TableAngledHeadersTextAlign, 0.0f, 1.0f, "%.2f");
			ImGui::TreePop();
		}

		if (ImGui::BeginTable("table_angled_headers", columns_count, table_flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 12)))
		{
			ImGui::TableSetupColumn(column_names[0], ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder);
			for (int n = 1; n < columns_count; n++)
				ImGui::TableSetupColumn(column_names[n], column_flags);
			ImGui::TableSetupScrollFreeze(frozen_cols, frozen_rows);

			ImGui::TableAngledHeadersRow(); // Draw angled headers for all columns with the ImGuiTableColumnFlags_AngledHeader flag.
			ImGui::TableHeadersRow();       // Draw remaining headers and allow access to context-menu and other functions.
			for (int row = 0; row < rows_count; row++)
			{
				ImGui::PushID(row);
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Track %d", row);
				for (int column = 1; column < columns_count; column++)
					if (ImGui::TableSetColumnIndex(column))
					{
						ImGui::PushID(column);
						ImGui::Checkbox("", &bools[row * columns_count + column]);
						ImGui::PopID();
					}
				ImGui::PopID();
			}
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
#endif
}

void ofApp::mouseMoved(int x, int y ){

	/*float distance = ofDist(x, y, ballPositionX, ballPositionY);

	if(distance <= 15 || y < 300) {
		if(bForcePassThrough == true && distance <= 15){
			bForcePassThrough = false;
		}
		if(bAllowPassThrough && (bForcePassThrough == false || distance <= 15)) {
			bAllowPassThrough = false;
			ofLogNotice("ofApp") << "ofSetWindowMousePassThrough:" << bAllowPassThrough;
			ofSetWindowMousePassThrough(bAllowPassThrough);
		}
	} else {
		if(!bAllowPassThrough) {
			bAllowPassThrough = true;
			ofLogNotice("ofApp") << "ofSetWindowMousePassThrough:" << bAllowPassThrough;
			ofSetWindowMousePassThrough(bAllowPassThrough);
		}
	}*/
}

std::string ofApp::runBashCommand(const std::string & command) {
	try {

		std::string tmpFile = "command_output.txt";

#ifdef _WIN32
		// On Windows, use bash -c to execute Bash commands
		std::string fullCommand = "bash -c \"" + command + " > " + tmpFile + "\"";
#else
		// On Linux/macOS, execute the command directly
		std::string fullCommand = command + " > " + tmpFile;
#endif
		std::cout << "Running Command: " << fullCommand << std::endl;

		// Execute the command and redirect output to a temporary file
		int retCode = std::system(fullCommand.c_str());
		if (retCode != 0) {
			throw std::runtime_error("Command failed with return code: " + std::to_string(retCode));
		}

		// Read the output from the temporary file
		std::ifstream file(tmpFile);
		if (!file) {
			throw std::runtime_error("Failed to open temporary output file");
		}

		std::string result((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());
		file.close();

		// Clean up the temporary file
		std::remove(tmpFile.c_str());
		return result;
	}
	catch (const std::exception & ex) {
		std::ostringstream errorStream;
		errorStream << "Error: " << ex.what();
		return errorStream.str();
	}
}



void ofApp::windowFocusGained() {
	isPaused = false; 
	ofLogNotice() << "Window focus gained. Resuming app.";
}

void ofApp::windowFocusLost() {
	isPaused = true;
	ofLogNotice() << "Window focus lost. Pausing app.";
}


void ofApp::introUpdate() {

	timerIntro += ofGetLastFrameTime();
	if (timerIntro >= timerIntroMAX) {
		state = BACKGROUND;
		ofLogNotice() << "INTRO to BACKGROUND";
		timerIntro = 0.0f;
	}

	return;

	// https: //junkiyoshi.com/openframeworks20241206/
	// author junkiyoshi
	this->face.clear();
	this->line.clear();

	float phi_deg_step = 8;
	float theta_deg_step = 20;

	float R = 230;
	float threshold_1 = 0.55;
	float threshold_2 = 0.65;
	float r = R * 0.3;

	int target_deg_start = ofGetFrameNum() * 1.44;

	for (int target_deg = target_deg_start; target_deg <= target_deg_start + 180; target_deg += 180) {

		auto target = glm::vec3(R * cos(target_deg * DEG_TO_RAD), R * sin(target_deg * DEG_TO_RAD), 0);

		for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

			for (float theta_deg = 0; theta_deg < 360; theta_deg += theta_deg_step) {

				auto location = this->make_point(R, r, theta_deg, phi_deg);
				auto distance = glm::distance(target, location);

				if (distance > 250) {
					continue;
				}

				auto power = distance < 150 ? 1 : ofMap(distance, 150, 250, 1, 0);

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(this->make_point(R, r, theta_deg - theta_deg_step * 0.49 * power - 2, phi_deg - phi_deg_step * 0.49 * power - 2));
				vertices.push_back(this->make_point(R, r, theta_deg + theta_deg_step * 0.49 * power - 2, phi_deg - phi_deg_step * 0.49 * power - 2));
				vertices.push_back(this->make_point(R, r, theta_deg - theta_deg_step * 0.49 * power - 2, phi_deg + phi_deg_step * 0.49 * power - 2));
				vertices.push_back(this->make_point(R, r, theta_deg + theta_deg_step * 0.49 * power - 2, phi_deg + phi_deg_step * 0.49 * power - 2));

				this->face.addVertices(vertices);
				this->line.addVertices(vertices);

				this->face.addIndex(index + 0);
				this->face.addIndex(index + 1);
				this->face.addIndex(index + 3);
				this->face.addIndex(index + 0);
				this->face.addIndex(index + 3);
				this->face.addIndex(index + 2);

				this->line.addIndex(index + 0);
				this->line.addIndex(index + 1);
				this->line.addIndex(index + 0);
				this->line.addIndex(index + 2);
				this->line.addIndex(index + 1);
				this->line.addIndex(index + 3);
				this->line.addIndex(index + 2);
				this->line.addIndex(index + 3);
			}
		}
	}
}
void ofApp::introDraw() {

	std::string displayText = baseText;
	ofRectangle textBounds = customFont.getStringBoundingBox(displayText, 0, 0);
	ofRectangle textBoundsWhole = customFont.getStringBoundingBox(displayText + " " + ellipsis, 0, 0);

	float centerX = ofGetWidth() / 2.0f;
	float centerY = ofGetHeight() / 2.0f;
	/*ofSetColor(0);
	ofDrawRectangle(centerX - textBounds.getWidth() / 2 - 20,
		centerY - textBounds.getHeight() / 2 - 20,
		textBounds.getWidth() + 147,
		textBounds.getHeight() + 147);*/

	ofSetColor(0, 200, 255);
	float shadowOffset = 3.0f; 
	customFont.drawString(displayText + " " + ellipsis,
		centerX - textBounds.getWidth() / 2 + shadowOffset,
		centerY + textBounds.getHeight() / 2 + shadowOffset);

	ofSetColor(100, 255, 255);
	customFont.drawString(displayText + " " + ellipsis,
		centerX - textBounds.getWidth() / 2,
		centerY + textBounds.getHeight() / 2);

	return;
	// art 
	this->cam.begin();
	ofRotateX(90);

	ofSetColor(255, 128, 255);
	this->line.draw();

	ofSetColor(0);
	this->face.draw();

	this->cam.end();
}
void ofApp::backgroundUpdate() {
	timerIntro += ofGetLastFrameTime();
	if (timerIntro >= timerIntroMAX) {
		/*state = BACKGROUND;
		ofLogNotice() << "INTRO to BACKGROUND";
		timerIntro = 0.0f;*/
	}
}
void ofApp::backgroundDraw() {

}

glm::vec3 ofApp::make_point(float R, float r, float u, float v, float scale) {
	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u) * scale) * cos(v);
	auto y = (R + r * cos(u) * scale) * sin(v);
	auto z = r * sin(u) * scale;

	return glm::vec3(x, y, z);
}

void ofApp::faderDraw() {
		float midPoint = timerIntroMAX / 2.0f;
		float alpha = 0.0f;
		if (timerIntro <= midPoint) {
			alpha = ofLerp(0.0f, 255, timerIntro / midPoint);
		} else if (timerIntro <= timerIntroMAX) {
			alpha = ofLerp(255, 0.0f, (timerIntro - midPoint) / midPoint);
		}
		//ofLogNotice() << "alpha: " << alpha;
		if (alpha > 1) {
			ofSetColor(71, 0, 40, alpha);
			ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		}
}

void ofApp::addToLog(std::string s) {
	//return;
	std::stringstream ss;
	ss << s;
	std::cout << ss.str() << endl;
	ofLog() << ss.str();
}


void ofApp::queryYmlFiles() {
	ymlFiles.clear();
	workflowStates.clear();

	try {
		ofDirectory dir("../apothecary/.github/workflows/");
		dir.allowExt("yml"); // Filter for .yml files
		dir.listDir();       // List the files

		for (auto& file : dir.getFiles()) {
			std::string filePath = file.getAbsolutePath();
			ofLogNotice() << filePath;
			ymlFiles.push_back(filePath);

			// Check if DISABLE_WORKFLOW is set to "false"
			std::string checkCommand = "grep 'DISABLE_WORKFLOW: \"false\"' " + filePath + " > /dev/null 2>&1";
			int result = system(checkCommand.c_str());
			workflowStates[filePath] = (result == 0); // True if "false", otherwise false
		}
	} catch (const std::exception & ex) {
		ofLogError() << "Error: " << ex.what();
		apothecaryRoot = "";
	}
}
