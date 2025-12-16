#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#include <windows.h>
#include <conio.h>
#include <vector>
#include <iostream>
#include <functional>
#include <string>

struct MenuItem
{
	std::string label;
	std::function<void()> action;
};

class Menu
{
	std::vector<MenuItem> items;
	int selected = 0;
	HANDLE hIn, hOut;
	DWORD prevMode;

public:
	Menu(const std::vector<MenuItem>& options);
	~Menu();
	void HideCursor();
	void ShowCursor();
	void GotoXY(int x, int y);
	void Eraselines(int count);

	// Clear a few lines below the menu (where actions print) so previous longer output	
	void ClearOutputArea(int lines);

	void Draw();

	// Wait for a console key-down event and return (consumes function keys properly)
	void WaitForKeyDown();

	void Run();
};
