#include "morse.h"
#include "morsewav.h"
#include <menu.h>
/**
* C++ Morse CMD for Windows
* a Morse console and command line app
*/
using namespace std;

string action = "";

/**
* Set action from menu
*/
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
		string str;
		while (argc > 2)
		{
			str += m.arg_string(argv[2]);
			argc -= 1;
			argv += 1;
		}
		if (action == "encode") { cout << str << "\n" << m.morse_encode(str) << "\n"; }
		else if (action == "binary") { cout << str << "\n" << m.morse_binary(str) << "\n"; }
		else if (action == "decode") { cout << str << "\n" << m.morse_decode(str) << "\n"; }
		else if (action == "hex") { cout << str << "\n" << m.bin_morse_hexdecimal(str, 0) << "\n"; }
		else if (action == "hexdec") { cout << str << "\n" << m.hexdecimal_bin_txt(str, 0) << "\n"; }
		else if (action == "hexbin") { cout << str << "\n" << m.bin_morse_hexdecimal(str, 1) << "\n"; }
		else if (action == "hexbindec") { cout << str << "\n" << m.hexdecimal_bin_txt(str, 1) << "\n"; }
		else if (action == "sound" || action == "wav" || action == "wav_mono")
		{
			//cout << "-wpm: " << m.words_per_minute << " (" << m.duration_milliseconds(m.words_per_minute) << " ms)\n";
			//cout << "-hz: " << m.frequency_in_hertz << "Hz (tone)\n";
			str.resize(750);
			string morse = m.morse_encode(str);
			cout << morse << "\n";

			if (m.samples_per_second < 8000.0) m.samples_per_second = 8000.0;
			if (m.samples_per_second > 48000) m.samples_per_second = 48000.0;
			if (m.frequency_in_hertz < 20.0) m.frequency_in_hertz = 20.0;
			if (m.frequency_in_hertz > 8000.0) m.frequency_in_hertz = 8000.0;
			if (m.words_per_minute < 0.0) m.words_per_minute = 0.0;
			if (m.words_per_minute > 50.0) m.words_per_minute = 50.0;

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
				int size = morse.size();
				printf("wave: %9.3lf Hz (-sps:%lg)\n", sps, sps);
				printf("tone: %9.3lf Hz (-tone:%lg)\n", m.frequency_in_hertz, m.frequency_in_hertz);
				printf("code: %9.3lf Hz (-wpm:%lg)\n", m.words_per_minute / 1.2, m.words_per_minute);
				cout << "to be able to change sound settings, choose sound to wav file\n";
				for (size_t i = 0; i < size; ++i)
				{
					char c = morse.at(i);
					string s(1, c);
					if (s == ".") Beep(m.frequency_in_hertz, 1 * m.duration_milliseconds(m.words_per_minute));
					if (s == "-") Beep(m.frequency_in_hertz, 3 * m.duration_milliseconds(m.words_per_minute));
					if (s == " ") Beep(0, 3.5 * m.duration_milliseconds(m.words_per_minute));
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
		cout << "Type Morse/Txt and press [enter]:\n";
		getline(cin, arg_in);
		if (action == "encode") cout << m.morse_encode(arg_in) << "\n";
		if (action == "binary") cout << m.morse_binary(arg_in) << "\n";
		if (action == "decode") cout << m.morse_decode(arg_in) << "\n";
		if (action == "hex") cout << m.bin_morse_hexdecimal(arg_in, 0) << "\n";
		if (action == "hexdec") cout << m.hexdecimal_bin_txt(arg_in, 0) << "\n";
		if (action == "hexbin") cout << m.bin_morse_hexdecimal(arg_in, 1) << "\n";
		if (action == "hexbindec") cout << m.hexdecimal_bin_txt(arg_in, 1) << "\n";
		if (action == "sound" || action == "wav" || action == "wav_mono")
		{
			arg_in.resize(750);
			string str = m.morse_encode(arg_in);
			cout << str << "\n";
			if (action == "wav")
			{
				cout << "Enter Samples Per Second(like 44100):\n";
				getline(cin, arg_in);
				m.samples_per_second = atof(arg_in.c_str());

				cout << "Enter Tone Frequency(like 880):\n";
				getline(cin, arg_in);
				m.frequency_in_hertz = atof(arg_in.c_str());
				
				cout << "Enter Words Per Minute(WPM):\n";
				getline(cin, arg_in);
				m.words_per_minute = atof(arg_in.c_str());

				MorseWav mw = MorseWav(str.c_str(), m.frequency_in_hertz, m.words_per_minute, m.samples_per_second, true, 2);
			}
			else if (action == "wav_mono")
			{
				cout << "Enter Samples Per Second(like 44100):\n";
				getline(cin, arg_in);
				m.samples_per_second = atof(arg_in.c_str());
				if (m.samples_per_second < 8000.0) m.samples_per_second = 8000.0;
				if (m.samples_per_second > 48000) m.samples_per_second = 48000.0;

				cout << "Enter Tone Frequency(like 880):\n";
				getline(cin, arg_in);
				m.frequency_in_hertz = atof(arg_in.c_str());
				if (m.frequency_in_hertz < 20.0) m.frequency_in_hertz = 20.0;
				if (m.frequency_in_hertz > 8000.0) m.frequency_in_hertz = 8000.0;

				cout << "Enter Words Per Minute(WPM):\n";
				getline(cin, arg_in);
				m.words_per_minute = atof(arg_in.c_str());
				if (m.words_per_minute < 0.0) m.words_per_minute = 0.0;
				if (m.words_per_minute > 50.0) m.words_per_minute = 50.0;

				MorseWav mw = MorseWav(str.c_str(), m.frequency_in_hertz, m.words_per_minute, m.samples_per_second, true, 1);
			}
			else
			{
				int size = str.size();
				printf("wave: %9.3lf Hz (-sps:%lg)\n", sps, sps);
				printf("tone: %9.3lf Hz (-tone:%lg)\n", m.frequency_in_hertz, m.frequency_in_hertz);
				printf("code: %9.3lf Hz (-wpm:%lg)\n", m.words_per_minute / 1.2, m.words_per_minute);
				cout << "to be able to change sound settings, choose sound to wav file\n";
				for (size_t i = 0; i < size; ++i)
				{
					char c = str.at(i);
					string s(1, c);
					if (s == ".") Beep(m.frequency_in_hertz, 1 * m.duration_milliseconds(m.words_per_minute));
					if (s == "-") Beep(m.frequency_in_hertz, 3 * m.duration_milliseconds(m.words_per_minute));
					if (s == " ") Beep(0, 3.5 * m.duration_milliseconds(m.words_per_minute));
				}
			}
		}
		cout << "Press any key to close program . . .";
		int c = getchar();
		return 0;
	}
}