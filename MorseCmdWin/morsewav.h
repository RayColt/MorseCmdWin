#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#define _USE_MATH_DEFINES // Required for MSVC/Windows

#include <cmath>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <direct.h>
#include <errno.h>
#define NOMINMAX
#include <windows.h>

class MorseWav
{

public:
	MorseWav(const char* morsecode, double tone, double wpm, double samples_per_second, bool play, int modus);
	~MorseWav() = default;

	// (Optional) Simple WAV-header + data writer for 16-bit PCM.
	static void WriteWav(
		const char* filename,
		const std::vector<int16_t>& pcmData
	);

	// Tone and silence generator
	static void Tones(int silence);
	static void Dit();
	static void Dah();
	static void Space();

	// Morse code tone generator
	static void MorseTones(const char* code);
};

