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
	/**
	* Constructor / Destructor
	*/
	MorseWav(const char* morsecode, double tone, double wpm, double samples_per_second, bool play, int modus);
	~MorseWav() = default;

private:
	/**
	* Write wav file
	*
	* @param filename
	* @param pcmData
	*/
	void WriteWav(
		const char* filename,
		const std::vector<int16_t>& pcmData
	);

	/**
	* Get binary morse code (dit/dah) for a given character.
	* Generate one quantum of silence or tone in PCM/WAV array.
	* sine wave: y(t) = amplitude * sin(2 * PI * frequency * time), time = s / sample_rate
	*
	* @param silence
	*/
	void Tones(int silence);

	/**
	* Define dit, dah, end of letter, end of word.
	*
	* The rules of 1/3/7 and 1/2/4(more suitable for common microphones, like webcams and phones):
	* Morse code is: tone for one unit (dit) or three units (dah)
	* followed by the sum of one unit of silence (always),
	* plus two units of silence (if end of letter, one space),
	* plus four units of silence (if also end of word).
	*/
	void Dit();
	void Dah();
	void Space();

	/**
	* Morse code tone generator
	*
	* @param code
	*/
	void MorseTones(const char* code);
};

