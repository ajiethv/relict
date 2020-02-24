#include "Window.h"

Window::Window(const std::string title, int windowWidth, int windowHeight, bool resizable)
{
	//Window is now open
	m_open = true;

	//Create the window
	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	//change to 20% zoom
	//clear = 0000, black = F444, red = FF10
	Uint16 pixels[54 * 54] = {
		0x0000,0x0000,0x0000,0x0000,0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0x0000,0x0000,0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0xF444,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xF444,0xF444,0xF444,0xF444,0xF444,0xFF10,0xFF10,0xFF10,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0x0000,0x0000,
		0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xFF10,0xFF10,0xFF10,0xFF10,0xFF10,0x0000,0x0000,0x0000,0x0000,
	};
	SDL_Surface* icon = SDL_CreateRGBSurfaceFrom(pixels, 54, 54, 16, 54 * 2, 0x0F00, 0x00F0, 0x000F, 0xF000);
	SDL_SetWindowIcon(m_window, icon);

	//If window is NULL
		//Give failed window creation error message
	//Else
		//Set up the GL context
	if (m_window == NULL)
	{
		std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
	}
	else
	{
		//Create GL context
		m_context = SDL_GL_CreateContext(m_window);
		SetWindowResizable(resizable);

		//Check to make sure the GL context was created properly
		if (m_context == NULL)
		{
			std::cerr << "Failed to create GL context!" << std::endl;

			Close();
		}
	}
}

Window::~Window()
{
	//If context isn't NUll
	if (m_context != NULL)
	{
		//delete context
		SDL_GL_DeleteContext(m_context);
		//Set context to NULL
		m_context = NULL;
	}

	//If window isn't nullptr
	if (m_window != nullptr)
	{
		//Delete window
		SDL_DestroyWindow(m_window);
		//Set window equal to nullptr
		m_window = nullptr;
	}
}

void Window::Clear(float r, float g, float b, float a)
{
	//Set the clear color
	glClearColor(r, g, b, a);
	//Clear the color buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Clear(vec4 color)
{
	//Set the clear color
	glClearColor(color.x, color.y, color.z, color.w);
	//Clear the color buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Flip()
{
	//Swaps the buffers (using double buffer system)
	SDL_GL_SwapWindow(m_window);
}

void Window::SetWindowName(std::string name)
{
	SDL_SetWindowTitle(m_window, name.c_str());
}

void Window::SetWindowResizable(bool resizable)
{
	//Sets the window resizable
	SDL_SetWindowResizable(m_window, SDL_bool(resizable));
}

void Window::SetFullscreen(Uint32 flags)
{
	//Sets the window fullscreen flag
	//SDL_WINDOW_FULLSCREEN
	SDL_SetWindowFullscreen(m_window, m_fullscreen);
	m_fullscreen = flags;
}

Uint32 Window::GetFullscreen() const
{
	//Return window fullscreen flag
	return m_fullscreen;
}

void Window::Close()
{
	//Set window open to false
	m_open = false;
}
