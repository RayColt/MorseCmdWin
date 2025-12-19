#include "windows.h"
#include "morse.h"
#include "morsewav.h"
#include "menu.h"
#include <iostream>
/**
* C++ Morse CMD for Windows
* a Morse console and command line app
*/
using namespace std;

const int MAX_TXT_INPUT = 6000; // max chars for morse encoding/decoding
const int MAX_MORSE_INPUT = 2000; // max chars for morse encoding/decoding
const int MAX_SOUND_INPUT = 750; // max chars for sound generation

/**
* Create Safe morse settings
*/
static void MakeMorseSafe(Morse &morse)
{
	if (morse.samples_per_second < 8000.0) morse.samples_per_second = 8000.0;
	if (morse.samples_per_second > 48000) morse.samples_per_second = 48000.0;
	if (morse.frequency_in_hertz < 20.0) morse.frequency_in_hertz = 20.0;
	if (morse.frequency_in_hertz > 8000.0) morse.frequency_in_hertz = 8000.0;
	if (morse.words_per_minute < 0.0) morse.words_per_minute = 0.0;
	if (morse.words_per_minute > 50.0) morse.words_per_minute = 50.0;
}

/**
* Set action from menu
*/
string action = "";
void SetAction(string a)
{
	action = a;
}

/**
* Main Class
*/
int main(int argc, char* argv[])
{
	Morse m;
	int n;
	double sps = 44100;

	if (argc != 1)
	{
		if (strcmp(argv[1], "es") == 0) { action = "sound"; }
		else if (strcmp(argv[1], "ew") == 0) { action = "wav"; }
		else if (strcmp(argv[1], "ewm") == 0) { action = "wav_mono"; } 
		else if (strcmp(argv[1], "e") == 0) { action = "encode"; }
		else if (strcmp(argv[1], "d") == 0) { action = "decode"; }
		else if (strcmp(argv[1], "b") == 0) { action = "binary"; }
		else if (strcmp(argv[1], "he") == 0) { action = "hex"; }
		else if (strcmp(argv[1], "hd") == 0) { action = "hexdec"; }
		else if (strcmp(argv[1], "hb") == 0) { action = "hexbin"; }
		else if (strcmp(argv[1], "hbd") == 0) { action = "hexbindec"; }
		// check options
		n = m.get_options(argc, argv);
		argc -= n;
		argv += n;
		// generate morse code
		string arg_in;
		// choose max allowed chars based on requested action
		int max_chars = MAX_TXT_INPUT;
		if (action == "decode") max_chars = MAX_MORSE_INPUT;
		else if (action == "sound" || action == "wav" || action == "wav_mono") max_chars = MAX_SOUND_INPUT;	

		// collect arguments but never exceed max_chars
		while (argc > 2 && static_cast<int>(arg_in.size()) < max_chars)
		{
			string part = m.arg_string(argv[2]);
			int remaining = max_chars - static_cast<int>(arg_in.size());
			if (remaining <= 0) break; // nothing more allowed
			if (static_cast<int>(part.size()) > remaining) 
			{
				part = part.substr(0, remaining);
				cerr << "Maximum input size reached (" << max_chars << " characters).\n";
			}
			arg_in += part;
			argc -= 1;
			argv += 1;
		}

		if (action == "encode") { cout << m.morse_encode(arg_in) << "\n"; }
		else if (action == "binary") { cout << m.morse_binary(arg_in) << "\n"; }
		else if (action == "decode") { cout << m.morse_decode(arg_in) << "\n"; }
		else if (action == "hex") { cout << m.bin_morse_hexdecimal(arg_in, 0) << "\n"; }
		else if (action == "hexdec") { cout << m.hexdecimal_bin_txt(arg_in, 0) << "\n"; }
		else if (action == "hexbin") { cout << m.bin_morse_hexdecimal(arg_in, 1) << "\n"; }
		else if (action == "hexbindec") { cout << m.hexdecimal_bin_txt(arg_in, 1) << "\n"; }
		else if (action == "sound" || action == "wav" || action == "wav_mono")
		{
			string morse = m.morse_encode(arg_in);
			cout << morse << "\n";
			MakeMorseSafe(m);

			if (action == "wav")
			{
				MorseWav mw = MorseWav(morse.c_str(), m.frequency_in_hertz, m.words_per_minute, m.samples_per_second, true, 2);
			}
			else if (action == "wav_mono")
			{
				MorseWav mw = MorseWav(morse.c_str(), m.frequency_in_hertz, m.words_per_minute, m.samples_per_second, true, 1);
			}
			else
			{
				int size = (int)morse.size();
				printf("wave: %9.3lf Hz (-sps:%lg)\n", sps, sps);
				printf("tone: %9.3lf Hz (-tone:%lg)\n", m.frequency_in_hertz, m.frequency_in_hertz);
				printf("code: %9.3lf Hz (-wpm:%lg)\n", m.words_per_minute / 1.2, m.words_per_minute);
				cout << "to be able to change sound settings, choose sound to wav file\n";
				for (size_t i = 0; i < size; ++i)
				{
					char c = morse.at(i);
					string s(1, c);
					if (s == ".") Beep((DWORD)m.frequency_in_hertz, (DWORD)(1 * m.duration_milliseconds(m.words_per_minute)));
					if (s == "-") Beep((DWORD)m.frequency_in_hertz, (DWORD)(3 * m.duration_milliseconds(m.words_per_minute)));
					if (s == " ") Beep(0, (DWORD)(3.5 * m.duration_milliseconds(m.words_per_minute)));
				}
			}
		}
	}
	else
	{
		// clear console
		system("cls");
		int choice = 0;
		std::vector<MenuItem> morse_menu =
		{ {"Encode [.-]", []() {  SetAction("encode");  }},
		{"Encode Binary [01]", []() { SetAction("binary"); }},
		{"Decode Morse / Binary Morse", []() { SetAction("decode"); }},
		{"Encode Hex [2D2E]", []() { SetAction("hex"); }},
		{"Decode Hex", []() { SetAction("hexdec"); }},
		{"Encode Binary Hex [3130]", []() { SetAction("hexbin"); }},
		{"Decode Binary Hex", []() { SetAction("hexbindec"); }},
		{"Encode Morse, sound only", []() { SetAction("sound"); }},
		{"Encode Morse, stereo sound to wav file", []() { SetAction("wav"); }},
		{"Encode Morse, mono sound to wav file", []() { SetAction("wav_mono"); }}
		};
		string arg_in;
		Menu menu(morse_menu);
		menu.Run();
		cout << "Type Morse/Txt and press [enter] (empty line to finish):\n";
		// choose max allowed chars based on requested action
		int max_chars =  MAX_TXT_INPUT;
		if (action == "decode") max_chars = MAX_MORSE_INPUT;
		else if (action == "sound" || action == "wav" || action == "wav_mono") max_chars = MAX_SOUND_INPUT;
		while (true)
		{
			string line;
			cout << flush;
			getline(cin, line);
			if(static_cast<int>(line.size()) > max_chars)
			{
				cerr << "Maximum input size reached (" << max_chars << " characters), re-enter:\n";
				continue;
			}
			arg_in = line;
			break;
		}
		if (action == "encode") cout << m.morse_encode(arg_in) << "\n";
		if (action == "binary") cout << m.morse_binary(arg_in) << "\n";
		if (action == "decode") cout << m.morse_decode(arg_in) << "\n";
		if (action == "hex") cout << m.bin_morse_hexdecimal(arg_in, 0) << "\n";
		if (action == "hexdec") cout << m.hexdecimal_bin_txt(arg_in, 0) << "\n";
		if (action == "hexbin") cout << m.bin_morse_hexdecimal(arg_in, 1) << "\n";
		if (action == "hexbindec") cout << m.hexdecimal_bin_txt(arg_in, 1) << "\n";
		if (action == "sound" || action == "wav" || action == "wav_mono")
		{
			string morse = m.morse_encode(arg_in);
			cout << morse << "\n";

			if (action == "wav" || action == "wav_mono")
			{
				cout << "Enter Samples Per Second(like 44100):\n";
				while (true)
				{
					string line;
					cout << flush;
					getline(cin, line);
					if (static_cast<int>(line.size()) > 5)
					{
						cerr << "Maximum input size reached (" << 5 << " characters), re-enter:\n";
						continue;
					}
					arg_in = line;
					break;
				}
				m.samples_per_second = atof(arg_in.c_str());

				cout << "Enter Tone Frequency(like 880):\n";
				while (true)
				{
					string line;
					cout << flush;
					getline(cin, line);
					if (static_cast<int>(line.size()) > 4)
					{
						cerr << "Maximum input size reached (" << 4 << " characters), re-enter:\n";
						continue;
					}
					arg_in = line;
					break;
				}
				m.frequency_in_hertz = atof(arg_in.c_str());
				
				cout << "Enter Words Per Minute(WPM):\n";
				while (true)
				{
					string line;
					cout << flush;
					getline(cin, line);
					if (static_cast<int>(line.size()) > 2)
					{
						cerr << "Maximum input size reached (" << 2 << " characters), re-enter:\n";
						continue;
					}
					arg_in = line;
					break;
				}
				m.words_per_minute = atof(arg_in.c_str());

				MakeMorseSafe(m);
				if (action == "wav")
				{
					MorseWav mw = MorseWav(morse.c_str(), m.frequency_in_hertz, m.words_per_minute, m.samples_per_second, true, 2);
				}
				else if (action == "wav_mono")
				{
					MorseWav mw = MorseWav(morse.c_str(), m.frequency_in_hertz, m.words_per_minute, m.samples_per_second, true, 1);
				}
				else
				{
					int size = (int)morse.size();
					printf("wave: %9.3lf Hz (-sps:%lg)\n", sps, sps);
					printf("tone: %9.3lf Hz (-tone:%lg)\n", m.frequency_in_hertz, m.frequency_in_hertz);
					printf("code: %9.3lf Hz (-wpm:%lg)\n", m.words_per_minute / 1.2, m.words_per_minute);
					cout << "to be able to change sound settings, choose sound to wav file\n";
					for (size_t i = 0; i < size; ++i)
					{
						char c = morse.at(i);
						string s(1, c);
						if (s == ".") Beep((DWORD)m.frequency_in_hertz, (DWORD)(1 * m.duration_milliseconds(m.words_per_minute)));
						if (s == "-") Beep((DWORD)m.frequency_in_hertz, (DWORD)(3 * m.duration_milliseconds(m.words_per_minute)));
						if (s == " ") Beep(0, (DWORD)(3.5 * m.duration_milliseconds(m.words_per_minute)));
					}
				}
			}
		}
		cout << "Press any key to close program . . .";
		int c = getchar();
		return 0;
	}
}