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

/**
* C++ MorseWav.h file
*/	

typedef struct PCM16_stereo_s
{
	int16_t left;
	int16_t right;
} PCM16_stereo_t;

typedef struct PCM16_mono_s
{
	int16_t speaker;
} PCM16_mono_t;

class MorseWav
{
public:
	PCM16_stereo_t* allocate_PCM16_stereo_buffer(int32_t size);
	PCM16_stereo_t* reallocate_PCM16_stereo_buffer(PCM16_stereo_t* buffer, int32_t size);
	PCM16_mono_t* allocate_PCM16_mono_buffer(int32_t size);
	PCM16_mono_t* reallocate_PCM16_mono_buffer(PCM16_mono_t* buffer, int32_t size);

	/**
	* Constructor
	*/
	MorseWav(const char* morsecode, double tone, double wpm, double samples_per_second, bool play, int modus);

private:
	/**
	* Get binary morse code (dit/dah) for a given character.
	* Generate one quantum of silence or tone in PCM/WAV array.
	* sine wave: y(t) = amplitude * sin(2 * PI * frequency * time), time = s / sample_rate
	*
	* @param on_off
	*/
	void tone(int on_off);

	/**
	* Define dit, dah, end of letter, end of word.
	*
	* The rules of 1/3/7 and 1/2/4(more suitable for common microphones, like webcams and phones):
	* Morse code is: tone for one unit (dit) or three units (dah)
	* followed by the sum of one unit of silence (always),
	* plus two units of silence (if end of letter, one space),
	* plus four units of silence (if also end of word).
	*/
	void dit();
	void dah();
	void space();
	
	/**
	* Create Tones from morse code.
	*
	* @param code
	*/
	void morse_tone(const char* code);

	/**
	* Calculate poor ratio.
	*
	* @param a
	* @param b
	* @return int
	*/
	int ratio_poor(double a, double b);

	/**
	* Check ratios between Sps, Tone and Eps
	*/
	void check_ratios();

	/**
	* Show details about morse/wav parameters
	*/
	void show_details();

	/**
	* Write wav file
	*
	* @param path
	* @param data
	* @param count
	*/
	void wav_write(const char* path, PCM16_mono_t* buffer_mono_pcm, PCM16_stereo_t* buffer_pcm, long count);
};