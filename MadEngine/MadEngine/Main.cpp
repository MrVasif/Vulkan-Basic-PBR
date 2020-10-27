#include  <iostream>
#include "WindowHandler.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
const int WINDOW_WIDTH = 1800;
const int WINDOW_HEIGHT = 1000;

WindowHandler windowHandler(WINDOW_WIDTH, WINDOW_HEIGHT);


int main() {	
	windowHandler.InitializeWindow();
	windowHandler.MainLoop();
}

