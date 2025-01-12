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

	// Fill table data
	data.resize(20); 
	for (int i = 0; i < data.size(); ++i) {
		data[i] = { i + 1, "Name " + std::to_string(i + 1), i * 10.0f, i * 1.0e5, i * 100, "Description " + std::to_string(i + 1) };

		ofLogNotice() << data[i].id << "," << data[i].name << "," << data[i].value << "," << data[i].largeValue << "," << data[i].anotherInt << "," << data[i].description;
	}

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

	std::string scriptPath = "scripts/calculate_formulas.sh";
	//scriptPath = apothecaryRoot.string() + scriptPath;
	ofLogNotice() << scriptPath;
	std::replace(scriptPath.begin(), scriptPath.end(), '\\', '/');
	ofLogNotice() << scriptPath;
	//std::string scriptPath = "./calculate_formulas.sh";
	std::string calculateFormulasResult = runBashCommand(scriptPath);
	ofLogNotice() << calculateFormulasResult;

	 // GLFWwindow * window = static_cast<ofAppGLFWWindow *>(ofGetWindowPtr())->getGLFWWindow();

	 //// Set the user pointer to pass the ofApp instance
	 //glfwSetWindowUserPointer(window, this);

	 //// Set the focus callback
	 //glfwSetWindowFocusCallback(window, windowFocusCallback);

}

//--------------------------------------------------------------
void ofApp::draw() {
	if (isPaused) {
		return;
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
	ofSetColor(ofColor(7, 7, 7, 240));
	ofDrawRectangle(1, 1, screenWidth-3, screenHeight-3);

	ui.Begin();
	{
		/* Put windows here */

		if (ui.BeginWindow(bGui)) // Handles windows management. No need to use RAW ImGui window/frame.
		{
			/* Put RAW Dear ImGui or custom ofxSurfingImGui widgets here. */

			ui.AddAutoResizeToggle();
			ui.AddSpacing();

			// No need to handle the window as we are using ofxSurfingImGui
			ShowExampleTable();

			// Another RAW Dear ImGui 
			ShowAngledHeaders();

			ui.EndWindow();
		}

		// RAW Dear ImGui with his own window/frame management
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
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == ' ') {
		bGui = !bGui;
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

// Function to display a table with data
//--------------------------------------------------------------
void ofApp::ShowExampleTable() {
	//// Begin the ImGui frame/window
	//ImGui::Begin("Example Table");

	// Create a table with 10 columns
	if (ImGui::BeginTable("MyTable", 10)) {
		// Setup the table columns
		ImGui::TableSetupColumn("ID");
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Value");
		ImGui::TableSetupColumn("Large Value");
		ImGui::TableSetupColumn("Another Int");
		ImGui::TableSetupColumn("Description");
		// Add more columns as needed
		for (int col = 6; col < 10; ++col) {
			ImGui::TableSetupColumn(("Column " + std::to_string(col)).c_str());
		}

		// Display the headers
		ImGui::TableHeadersRow();

		// Display the data rows
		for (const auto& entry : data) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn(); ImGui::Text("%d", entry.id);
			ImGui::TableNextColumn(); ImGui::Text("%s", entry.name.c_str());
			ImGui::TableNextColumn(); ImGui::Text("%.1f", entry.value);
			ImGui::TableNextColumn(); ImGui::Text("%.2f", entry.largeValue);
			ImGui::TableNextColumn(); ImGui::Text("%d", entry.anotherInt);
			ImGui::TableNextColumn(); ImGui::Text("%s", entry.description.c_str());
			// Add more data columns if necessary
			for (int col = 6; col < 10; ++col) {
				ImGui::TableNextColumn(); ImGui::Text("Data %d", col);
			}
		}

		// End the table
		ImGui::EndTable();
	}

	//// End the ImGui frame/window
	//ImGui::End();
}

// Another function to display a table with data
//--------------------------------------------------------------
void ofApp::ShowAngledHeaders() {
	// Using those as a base value to create width/height that are factor of the size of our font
	const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
	const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

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
}

void ofApp::mouseMoved(int x, int y ){
	ofLogVerbose("ofApp") << "mouseMoved: x:" << x << " y:" << y;
	
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
