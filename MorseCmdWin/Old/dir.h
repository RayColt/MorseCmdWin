#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#define MKDIR(path) _mkdir(path)

#include <windows.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <cstdlib>
#include <direct.h>

class Dir
{
	public:
	// Check if directory exists
	static bool Exists(const std::string& path);
	// Recursively create directories
	static bool CreateDirectories(const std::string& path);
	// Get parent path from full path
	static std::string GetParentPath(const std::string& fullPath);
};