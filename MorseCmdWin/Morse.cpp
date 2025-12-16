#include "morse.h"
#include "morsewav.h"

using namespace std;

/**
* C++ Morse Class
*
* @author Ray Colt <ray_colt@pentagon.mil>
* @copyright Copyright (c) 1975, 2026 Ray Colt
* @license MIT License
*
* Derived from ARPANET Pentagon's morse.
*
* You can damage your hearing or your speakers if you play tones at extreme volumes!
* This program will not allow to play morse < 37 Hz and > 8,000 Hz.
*
* Usage program, see: ./morse -help or -h
*
* The Math: sine wave: y(t) = amplitude * sin(2 * PI * frequency * time), time = s / sample_rate
**/

/**
* Constructor
*/
Morse::Morse()
	: error_in("INPUT-ERROR"),
	frequency_in_hertz(880.0),
	words_per_minute(16.0),
	max_frequency_in_hertz(8000.0),
	min_frequency_in_hertz(37.0),
	samples_per_second(44100.0)
{
	fill_morse_maps();
}

/**
* Fill Multimaps with morse tables
*/
multimap <string, string> morse_map;
multimap <string, string> morse_map_reversed;
void Morse::fill_morse_maps()
{
	morse_map.insert(pair <string, string>(" ", ""));        // SPACE (0b1)
	morse_map.insert(pair <string, string>("!", "101011"));  // -.-.--
	morse_map.insert(pair <string, string>("$", "0001001")); // ...-..-
	morse_map.insert(pair <string, string>("\"", "010010")); // .-..-.

	morse_map.insert(pair <string, string>("'", "011110"));  // .----.
	morse_map.insert(pair <string, string>("(", "10110"));   // -.--.
	morse_map.insert(pair <string, string>(")", "101101"));  // -.--.-

	morse_map.insert(pair <string, string>(",", "110011"));  // --..--
	morse_map.insert(pair <string, string>("-", "100001"));  // -....-
	morse_map.insert(pair <string, string>(".", "010101"));  // .-.-.-
	morse_map.insert(pair <string, string>("/", "10010"));   // -..-.

	morse_map.insert(pair <string, string>("0", "11111"));   // -----
	morse_map.insert(pair <string, string>("1", "01111"));   // .----
	morse_map.insert(pair <string, string>("2", "00111"));   // ..---
	morse_map.insert(pair <string, string>("3", "00011"));   // ...--
	morse_map.insert(pair <string, string>("4", "00001"));   // ....-
	morse_map.insert(pair <string, string>("5", "00000"));   // .....
	morse_map.insert(pair <string, string>("6", "10000"));   // -....
	morse_map.insert(pair <string, string>("7", "11000"));   // --...
	morse_map.insert(pair <string, string>("8", "11100"));   // ---..
	morse_map.insert(pair <string, string>("9", "11110"));   // ----.

	morse_map.insert(pair <string, string>(":", "111000"));  // ---...
	morse_map.insert(pair <string, string>(";", "101010"));  // -.-.-.
	morse_map.insert(pair <string, string>("=", "10001"));   // -...-
	morse_map.insert(pair <string, string>("?", "001100"));  // ..--..
	morse_map.insert(pair <string, string>("@", "011010"));  // .--.-.

	morse_map.insert(pair <string, string>("A", "01"));      // .-
	morse_map.insert(pair <string, string>("B", "1000"));    // -...
	morse_map.insert(pair <string, string>("C", "1010"));    // -.-.
	morse_map.insert(pair <string, string>("D", "100"));     // -..
	morse_map.insert(pair <string, string>("E", "0"));       // .
	morse_map.insert(pair <string, string>("F", "0010"));    // ..-.
	morse_map.insert(pair <string, string>("G", "110"));     // --.
	morse_map.insert(pair <string, string>("H", "0000"));    // ....
	morse_map.insert(pair <string, string>("I", "00"));      // ..
	morse_map.insert(pair <string, string>("J", "0111"));    // .---
	morse_map.insert(pair <string, string>("K", "101"));     // -.-
	morse_map.insert(pair <string, string>("L", "0100"));    // .-..
	morse_map.insert(pair <string, string>("M", "11"));      // --
	morse_map.insert(pair <string, string>("N", "10"));      // -.
	morse_map.insert(pair <string, string>("O", "111"));     // ---
	morse_map.insert(pair <string, string>("P", "0110"));    // .--.
	morse_map.insert(pair <string, string>("Q", "1101"));    // --.-
	morse_map.insert(pair <string, string>("R", "010"));     // .-.
	morse_map.insert(pair <string, string>("S", "000"));     // ...
	morse_map.insert(pair <string, string>("T", "1"));       // -
	morse_map.insert(pair <string, string>("U", "001"));     // ..-
	morse_map.insert(pair <string, string>("V", "0001"));    // ...-
	morse_map.insert(pair <string, string>("W", "011"));     // .--
	morse_map.insert(pair <string, string>("X", "1001"));    // -..-
	morse_map.insert(pair <string, string>("Y", "1011"));    // -.--
	morse_map.insert(pair <string, string>("Z", "1100"));    // --..
	morse_map.insert(pair <string, string>("_", "001101"));  // ..--.-
	morse_map.insert(pair <string, string>("ERR", "00000000")); // ........
	// invert morse_map
	for (const auto& it : morse_map)
	{
		morse_map_reversed.insert(make_pair(it.second, it.first));
	}
}

/**
* Get binary morse code (dit/dah) for a given character
*
* @param character
* @return string
*/
string Morse::getBinChar(string character)
{
	return morse_map.find(character)->second;
}

/**
* Get morse code (dit/dah) for a given character
*
* @param character
* @return string
*/
string Morse::getMorse(string character)
{
	return strtr(morse_map.find(character)->second, "01", ".-");
}

/**
* Get character for given morse code
*
* @param morse
* @return string
*/
string Morse::getCharacter(string morse)
{
	return morse_map_reversed.find(strtr(morse, ".-", "01"))->second;
}

/**
* Get binary morse code for given string
*
* @param str
* @return string
*/
string Morse::morse_binary(string str)
{
	string line = "";
	str = fix_input(str);
	str = regex_replace(str, regex("\\s{2,}"), " ");
	str = regex_replace(str, regex("[\t]+"), " ");
	for (size_t i = 0; i < str.length(); i++)
	{
		string chr = str.substr(i, 1);
		line += getBinChar(stringToUpper(chr));
		line += " ";
	}
	return trim(line);
}

/**
* Get morse code for given string
*
* @param str
* @return string
*/
string Morse::morse_encode(string str)
{
	string line = "";
	str = fix_input(str);
	str = regex_replace(str, regex("\\s{2,}"), " ");
	str = regex_replace(str, regex("[\t]+"), " ");
	for (size_t i = 0; i < str.length(); i++)
	{
		string chr = str.substr(i, 1);
		line += getMorse(stringToUpper(chr));
		line += " ";
	}
	return trim(line);
}

/**
* Get character string for given morse code
*
* @param str
* @return string
*/
string Morse::morse_decode(string str)
{
	string line = "";
	str = regex_replace(str, regex("[\t]+"), " ");
	if (regex_match(str, regex("[10\\s\\.\\-]+")))
	{
		vector<string> morsecodes = explode(str, ' ');
		for (auto morse : morsecodes)
		{
			if (morse.empty())
				line += " ";
			if (morse.size() < 9)
			{
				line += getCharacter(morse);
			}
		}
		return regex_replace(line, regex("\\s{2,}"), " ");
	}
	else
	{
		return error_in;
	}
}

/**
* (\  /)
* ( .  .)
* Get hexadecimal morse code for given string
*
* @param str
* @param modus
* @return string
*/
string Morse::bin_morse_hexdecimal(string str, int modus)
{
	string str1, str2;
	const char* a[] = { "2E ", "2D ", "30 ", "31 " };
	if (modus == 0) { str1 = a[0]; str2 = a[1]; };
	if (modus == 1) { str1 = a[2]; str2 = a[3]; };
	string line = morse_binary(str);
	line = regex_replace(line, regex("  "), "A");
	line = regex_replace(line, regex(" "), "K");
	line = regex_replace(line, regex("0"), str1);
	line = regex_replace(line, regex("1"), str2);
	line = regex_replace(line, regex("A"), "20 20 ");
	line = regex_replace(line, regex("K"), "20 ");
	return trim(line);
}

/**
* (\  /)
* ( .  .)
* Get txt for given hexadecimal morse code
*
* @param str
* @param modus
* @return string
*/
string Morse::hexdecimal_bin_txt(string str, int modus)
{
	string str1, str2;
	if (regex_match(str, regex("[20|30|31|2D|2E|\\s]+")))
	{
		const char* a[] = { "2E", "2D", "30", "31" };
		if (modus == 0) { str1 = a[0]; str2 = a[1]; };
		if (modus == 1) { str1 = a[2]; str2 = a[3]; };
		string line = remove_whitespaces(str);
		line = regex_replace(line, regex("2020"), "  ");
		line = regex_replace(line, regex("20"), " ");
		line = regex_replace(line, regex(str1), "0");
		line = regex_replace(line, regex(str2), "1");
		string s = morse_decode(trim(line));
		return s;
	}
	else
	{
		return error_in;
	}
}

/**
* A function that converts a string to uppercase letters
*
* @param str
* @return string
*/
string Morse::stringToUpper(string str)
{
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

/**
* Convert a string array into a string <br>
*
* @param vstr
* @return string
*/
string Morse::stringArrToString(vector<string> vstr)
{
	string scr = "";
	if (!vstr.empty())
	{
		for (size_t i = 0; i < vstr.size(); i++)
		{
			scr += vstr[i];
		}
	}
	return scr;
}

/**
* Similar to strtr in php, characters in 'from' will be <br>
* replaced by characters in 'to' in the same <br>
* order character by character.
*
* @param str
* @param from
* @param to
* @return string
*/
string Morse::strtr(string str, string from, string to)
{
	vector<string> out;
	for (size_t i = 0, len = str.length(); i < len; i++)
	{
		char c = str.at(i);
		int p = from.find(c);
		if (p >= 0)
		{
			char t = to.at(p);
			out.push_back(string(1, t));
		}
	}
	return !out.empty() ? stringArrToString(out) : str;
}

/**
* trimp automatically strips space at the start and end of a given string <br>
*
* @param str
* @return string
*/
string Morse::trim(const string& str)
{
	size_t first = str.find_first_not_of(' ');
	if (string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

/**
* Similar to explode in php, a split function on a delimiter <br>
*
* @param s
* @param c
* @return vector
*/
const vector<string> Morse::explode(const string& s, const char& c)
{
	string buff;
	vector<string> vstr;
	for (auto str : s)
	{
		if (str != c)
		{
			buff += str;
		}
		else
		{
			vstr.push_back(buff);
			buff = "";
		}
	}
	if (buff != "")
		vstr.push_back(buff);
	return vstr;
}

/**
* Fix input with whitespace to reduce errors
* info: regex specialChars{ R"([-[\]{}()*+?.,\^$|#\s])" };
*
* @param str
* @return string
*/
string Morse::fix_input(string str)
{
	string ret = "";
	regex e("[^a-zA-Z0-9!'\"@/_=\\s\\$\\(\\)\\,\\.\\:\\;\\?\\-]+");
	sregex_token_iterator iter(str.begin(), str.end(), e, -1), end;
	vector<string> vec(iter, end);
	for (auto a : vec)
	{
		ret += a + " ";
	}
	return trim(ret);
}

/**
* Remove all whitespaces for given string
*
* @param str
* @return string
*/
string Morse::remove_whitespaces(string str)
{
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
	return str;
}

/**
* Calculate words per second to the duration in milliseconds
*
* Dit: 1 unit
* Dah: 3 units
* Intra-character space: 1 unit
* Inter-character space: 3 Farnsworth-units
* Word space: longer than 7 Farnsworth-units
* Standard word Paris is 50 units, elements. (with one extra word space)
*
* @param wpm - words per minute
* @return double
*/
double Morse::duration_milliseconds(double wpm)
{
	double ms = 0.0;
	if (!wpm <= 0.0)
	{
		//50 elements per word
		double wps = wpm / 60.0; //words per second
		double eps = 50 * wps; //elements per second
		ms = 1000.0 / eps; //milliseconds per element
		return ms;
	}
	else return ms;
}

/**
* Reaf cmd line user arguments
*
* @param argc
* @param argv[]
* @return int
*/
int Morse::get_options(int argc, char* argv[])
{
	int args = 0;
	bool ok = false;
	if (strncmp(argv[1], "e", 1) == 0 || strncmp(argv[1], "b", 1) == 0 || strncmp(argv[1], "d", 1) == 0 ||
		strncmp(argv[1], "he", 2) == 0 || strncmp(argv[1], "hd", 2) == 0 || strncmp(argv[1], "hb", 2) == 0 ||
		strncmp(argv[1], "hbd", 3) == 0)
	{
		ok = true;
	}
	if (strncmp(argv[1], "-help", 5) == 0 || strncmp(argv[1], "-h", 2) == 0)
	{
		cout << GetHelpTxt();
		ok = true;
	}
	else if (ok)
	{
		while (argc > 1)
		{
			if (strncmp(argv[2], "-hz:", 4) == 0)
			{
				frequency_in_hertz = atof(&argv[2][4]);
				if (frequency_in_hertz > max_frequency_in_hertz) frequency_in_hertz = max_frequency_in_hertz;
				if (frequency_in_hertz < min_frequency_in_hertz) frequency_in_hertz = min_frequency_in_hertz;
			}
			else if (strncmp(argv[2], "-wpm:", 5) == 0)
			{
				words_per_minute = atof(&argv[2][5]);
			}
			else if (strncmp(argv[2], "-sps:", 5) == 0)
			{
				samples_per_second = atof(&argv[2][5]);
			}
			else
			{
				break;
			}
			argc -= 1;
			argv += 1;
			args += 1;
		}
	}
	else
	{
		fprintf(stderr, "option error %s, see morse -help for info\n", argv[2]);
		exit(1);
	}
	return args;
}

/**
* Generate string from arguments
* @param arg
*
* @return string
*/
string Morse::arg_string(char* arg)
{
	char c; string str;
	while ((c = *arg++) != '\0')
	{
		str += c;
	}
	str += " ";
	return str;
}

/**
* Get help text for morse usage
*
* @return string
*/
string Morse::GetHelpTxt()
{
	string str = "";
	str += "################# MORSE HELP SCREEN #################\n";
	str += "Morse Dictionary, chars(url save) and spaces, lower case will be made upper case:\n";
	str += "ABC DEFGHIJKLMNOPQRSTUVWXYZ 12 34567 890 !$ ' \" (), . _ - / : ; = ? @ \n\n";
	str += "Usage console app version:\n morse.exe or morse\n\n";
	str += "Usage cmd line version:\n morse.exe [modus] 'morse or txt'\n\n";
	str += "Select Modus for encoding or decoding:\n";
	str += "-h, -help    : Opens help screen\n";
	str += "e, d	     : [. - space] Morse Normal\n";
	str += "b, d         : [0 1 space] Binary Morse\n";
	str += "he, hd       : [2E 2D 20] HeX Morse\n";
	str += "hb, hbd      : [30 31 20] HeX Binary\n\n";
	str += "Select modus for encoding to sound :\n";
	str += "ew  : [Morse to Wav] Windows Wav Stereo - with local sound file\n";
	str += "ewm : [Morse to Wav] Windows Wav Mono - with local sound file\n";
	str += "es  : [Morse to Windows beep] Windows Speaker Beep - no sps\n\n";
	str += "Example: ./morse.exe d \"... ---  ...  ---\"\n";
	str += "(only with decoding, option d, double quotes are necessary to preserve double spaces who create words)\n\n";
	str += "Sound settings:\n";
	str += "Tone(Hz), tone frequency in Herz, allowed between 20 Hz - 8000 Hz\n";
	str += "WPM, words per minute, allowed between 0 wpm - 50 wpm\n";
	str += "SPS, samples per second, allowed between 8000 Hz - 48000 Hz\n";
	str += "For creating sound files there is a maximum of 750 chars, bigger text might lead to a long term 'not responding'.\n\n";
	str += "For inspiration have look at music notes their frequencies.\n";
	str += "Example: ./morse.exe ew -wpm:20 -hz:1050 -sps:22050 paris paris paris\n\n";
	return str;
}