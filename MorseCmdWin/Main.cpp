#include "morse.h"
#include "morsewav.h"

using namespace std;
/**
* Main Class
*/
int main(int argc, char* argv[])
{
	Morse m;
	int n;
	string action = "encode";
	double sps = 44100;
	if (argc != 1)
	{
		if (strcmp(argv[1], "es") == 0) action = "sound"; else
			if (strcmp(argv[1], "ew") == 0) action = "wav"; else
				if (strcmp(argv[1], "ewm") == 0) action = "wav_mono"; else
					if (strcmp(argv[1], "e") == 0) action = "encode"; else
						if (strcmp(argv[1], "d") == 0) action = "decode"; else
							if (strcmp(argv[1], "b") == 0) action = "binary"; else
								if (strcmp(argv[1], "he") == 0) action = "hexa"; else
									if (strcmp(argv[1], "hd") == 0) action = "hexadec"; else
										if (strcmp(argv[1], "hb") == 0) action = "hexabin"; else
											if (strcmp(argv[1], "hbd") == 0) action = "hexabindec";
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
		if (action == "encode") cout << m.morse_encode(str) << "\n"; else
			if (action == "binary") cout << m.morse_binary(str) << "\n"; else
				if (action == "decode") cout << m.morse_decode(str) << "\n"; else
					if (action == "hexa") cout << m.bin_morse_hexadecimal(str, 0) << "\n"; else
						if (action == "hexadec") cout << m.hexadecimal_bin_txt(str, 0) << "\n"; else
							if (action == "hexabin") cout << m.bin_morse_hexadecimal(str, 1) << "\n"; else
								if (action == "hexabindec") cout << m.hexadecimal_bin_txt(str, 1) << "\n"; else
									if (action == "sound" || action == "wav" || action == "wav_mono")
									{
										cout << "-wpm: " << m.words_per_minute << " (" << m.duration_milliseconds(m.words_per_minute) << " ms)\n";
										cout << "-hz: " << m.frequency_in_hertz << "Hz (tone)\n";
										str.resize(750);
										string morse = m.morse_encode(str);
										cout << morse << "\n";
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
											cout << "to change Tone(Hz) and WPM use cmd morse.exe -help or -h for info\n";
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
		string arg_in;
		cout << "MORSE (CMD line version: morse.exe -help or -h for info)\n";
		cout << "morse table: \nABC DEFGHIJKLMNOPQRSTUVWXYZ 12 34567 890 ! $ ' \" (), . _ - / : ; = ? @ \n";
		cout << "morse actions: \n0 [encode with sound], 1 [encode with stereo sound to wav file] \n2 [encode with mono sound to wav file]\n";
		cout << "3 [encode], 4 [binary encode], 5 [decode morse/binary].\n";
		cout << "6 [hexa encode], 7 [hexa decode].\n";
		cout << "8 [hexa bin encode], 9 [hexa bin decode].\n";
		cout << "choose action 0,1,2,3,4,5,6,7,8 or 9 and press [enter]:\n";
		getline(cin, arg_in);
		regex e("[0-9]");
		if (!regex_match(arg_in, e))
		{
			arg_in = "3";
			cout << "wrong input, action " << arg_in << " is active now" << "\n";
		}
		if (regex_match(arg_in, e))
		{
			if (arg_in == "0") action = "sound"; else
				if (arg_in == "1") action = "wav"; else
					if (arg_in == "2") action = "wav_mono"; else
						if (arg_in == "3") action = "encode"; else
							if (arg_in == "4") action = "binary"; else
								if (arg_in == "5") action = "decode"; else
									if (arg_in == "6") action = "hexa"; else
										if (arg_in == "7") action = "hexadec"; else
											if (arg_in == "8") action = "hexabin"; else
												if (arg_in == "9") action = "hexabindec";

			cout << "type or paste input and press [enter]\n";
			getline(cin, arg_in);
			if (action == "sound" || action == "wav" || action == "wav_mono")
			{
				arg_in.resize(750);
				string str = m.morse_encode(arg_in);
				cout << str << "\n";
				if (action == "wav")
				{
					MorseWav mw = MorseWav(str.c_str(), m.frequency_in_hertz, m.words_per_minute, m.samples_per_second, true, 2);
				}
				else if (action == "wav_mono")
				{
					MorseWav mw = MorseWav(str.c_str(), m.frequency_in_hertz, m.words_per_minute, m.samples_per_second, true, 1);
				}
				else
				{
					int size = str.size();
					printf("wave: %9.3lf Hz (-sps:%lg)\n", sps, sps);
					printf("tone: %9.3lf Hz (-tone:%lg)\n", m.frequency_in_hertz, m.frequency_in_hertz);
					printf("code: %9.3lf Hz (-wpm:%lg)\n", m.words_per_minute / 1.2, m.words_per_minute);
					cout << "to change Tone(Hz) and WPM use cmd morse.exe -help or -h for info\n";
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
			else
				if (action == "encode") cout << m.morse_encode(arg_in) << "\n"; else
					if (action == "binary") cout << m.morse_binary(arg_in) << "\n"; else
						if (action == "decode") cout << m.morse_decode(arg_in) << "\n"; else
							if (action == "hexa") cout << m.bin_morse_hexadecimal(arg_in, 0) << "\n"; else
								if (action == "hexadec") cout << m.hexadecimal_bin_txt(arg_in, 0) << "\n"; else
									if (action == "hexabin") cout << m.bin_morse_hexadecimal(arg_in, 1) << "\n"; else
										if (action == "hexabindec") cout << m.hexadecimal_bin_txt(arg_in, 1) << "\n";
		}
		cout << "Press any key to close program . . .";
		int c = getchar();
		return 0;
	}
}