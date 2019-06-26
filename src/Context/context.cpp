#include "context.hpp"
#include <iostream>
#include "config.h"
#include "colors.hpp"
#include <fstream>

/************************************ Vars ************************************/
GGView* Context::currentView;
GLFWwindow* Context::window;
Renderer* Context::renderer;
int Context::window_width = default_window_width;
int Context::window_height = default_window_height;
GGView* viewToBeDeleted = NULL;
bool Context::textInput = false;
glm::mat4 Context::projection = glm::mat4(1.0f);

/********************************* Functions **********************************/
void Context::invalidate(){
	Context::renderer->clear_window();
	
	if(currentView)
		Context::currentView->draw();
	
	Context::renderer->render();
}
	
void Context::onResize(GLFWwindow* window, int width, int height){
	Context::window_width = width;
	Context::window_height = height;
	Context::renderer->resize_window(width, height);
	Context::projection = glm::ortho(0.0f, (float) Context::window_width, (float) Context::window_height, 0.0f);
	
	//Notify view
	GGResizeEvent* evt = new GGResizeEvent(width, height);
	Context::currentView->onResize(evt);
	delete evt;
}

void Context::onMouseMove(GLFWwindow* window, double xpos, double ypos){
	//Notify view
	GGMouseMoveEvent* evt = new GGMouseMoveEvent(xpos, ypos);
	Context::currentView->onMouseMove(evt);
	delete evt;
}

void Context::onClick(GLFWwindow* window, int button, int action, int mods){
	//Get Mouse coordinates
	double x, y;
    glfwGetCursorPos(window, &x, &y);
	//Notify view
	GGClickEvent* evt = new GGClickEvent(button, action, mods, x, y);
	
	if(action == GLFW_RELEASE){
		Context::currentView->onMouseUp(evt);
	} else if(action == GLFW_PRESS){
		Context::currentView->onMouseDown(evt);
	}
	
	delete evt;
}

void Context::charInput(GLFWwindow* window, unsigned int codepoint){
	
	GGKeyboardEvent* evt = new GGKeyboardEvent(codepoint, GG_EVENT_REGULAR, 0);
	
	Context::currentView->onKeyDown(evt);
	Context::currentView->onKeyUp(evt);
	
	delete evt;
}

void Context::keyInput(GLFWwindow* window, int key, int scancode, int action, int mods){
	
	const char* keyVal;
	
	keyVal = glfwGetKeyName(key, 0);
	
	if(keyVal && mods == 0 && textInput){ //if not null the other callback will fire
		return;
	}
	
	GGKeyboardEvent* evt = new GGKeyboardEvent(key, GG_EVENT_SPECIAL, mods);
	
	if(action == GLFW_PRESS){
		Context::currentView->onKeyDown(evt);
	} else if (action == GLFW_RELEASE) {
		Context::currentView->onKeyUp(evt);
	}/* else if(action == GLFW_REPEAT){
		Context::currentView->onKeyDown(evt);
		Context::currentView->onKeyUp(evt);
	}*/
	
	delete evt;
}

void Context::setView(GGView* view){
	Context::currentView = view;
}

void Context::init(std::string win_title){
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
	}

	Context::window = glfwCreateWindow(
		Context::window_width,
		Context::window_height,
		win_title.c_str(),
		NULL,
		NULL
	);
	
	if (!Context::window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(Context::window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) {
		fprintf(stderr, "ERROR: could not start GLEW\n");
		glfwTerminate();
	}

	// Set OpenGL options
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// get version info
	const GLubyte* rendererString = glGetString(GL_RENDERER);// get renderer string
	const GLubyte* version = glGetString(GL_VERSION);// version as a string
	printf("Renderer: %s\n", rendererString);
	printf("OpenGL version supported %s\n", version);

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	/*glEnable(GL_DEPTH_TEST);// enable depth-testing
	   glDepthFunc(GL_LESS);// depth-testing interprets a smaller value as "closer"
	 */
		
	//keyboard callback
	glfwSetKeyCallback(Context::window, &Context::keyInput);
	glfwSetCharCallback(window, &Context::charInput);
	Context::textInput = true;
	//resize callback
	glfwSetFramebufferSizeCallback(Context::window, &Context::onResize);
	//click callback
	glfwSetMouseButtonCallback(Context::window, &Context::onClick);
	//mouse move callback
	glfwSetCursorPosCallback(Context::window, &Context::onMouseMove);
	
	std::string* font_location = new std::string(std::string(PROJECT_PATH) + std::string("/fonts/DejaVuSansMono.ttf"));
	Context::renderer = new Renderer(Context::window, Context::window_width, Context::window_height, font_location);
	
    renderer->set_background_color(WHITE);
	
	Context::invalidate();
	
	Context::projection = glm::ortho(0.0f, (float) Context::window_width, (float) Context::window_height, 0.0f);
}

void Context::switchView(GGView *view){
	viewToBeDeleted = Context::currentView;
	Context::currentView = view;
	Context::invalidate();
}

void Context::logic(){
	Context::currentView->viewLogic();
}


void Context::mainLoop(){
	while(!glfwWindowShouldClose(Context::window)) {
	    //_update_fps_counter(window);
	    glfwPollEvents();
	    
	    if(viewToBeDeleted != NULL){
	    	delete viewToBeDeleted;
	    	viewToBeDeleted = NULL;
	    }
	    Context::logic();
		
		Context::invalidate();
	}
	Context::freeResources();
}

void Context::setCursorToNormal(){
	Context::renderer->set_cursor(GLFW_CURSOR_NORMAL);
}

void Context::setCursorToResize(){
	renderer->set_cursor(GLFW_HRESIZE_CURSOR);
}

void Context::setCursorToClick(){
	renderer->set_cursor(GLFW_HAND_CURSOR);
}

std::string Context::getClipboardContent(){
	return glfwGetClipboardString(NULL);
}

bool Context::copyFile(const char* SRC, const char* DEST){
	std::ifstream src(SRC, std::ios::binary);
	std::ofstream dest(DEST, std::ios::binary);
	dest << src.rdbuf();
	src.close();
	dest.close();
	return src && dest;
}

void Context::writeToFile(const char* SRC, const char* DEST){
	std::ofstream dest(DEST, std::ios::binary);
	dest << SRC;
	dest.close();
}

std::string Context::readFromFile(const char *SRC){
	std::ifstream src(SRC, std::ios::binary);
	std::string content(
		(std::istreambuf_iterator<char>(src)),
		(std::istreambuf_iterator<char>())
	);
	src.close();
	
	return content;
}

void Context::freeResources(){
	delete Context::currentView;
	delete Context::renderer;
	
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLint Context::getTexureProgram(){
	return Context::renderer->getTexureProgram();
}

void Context::unsubscribeTextInput(){	
	glfwSetCharCallback(window, NULL);
	Context::textInput = false;
}
void Context::subscribeTextInput(){	
	glfwSetCharCallback(window, &Context::charInput);
	Context::textInput = true;
}

glm::vec2 Context::getCursorPos(){
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	
	return glm::vec2(xpos, ypos);
}
