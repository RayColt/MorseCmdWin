#include "help.h"
/**
* C++ Help Class
*/
using namespace std;

/**
* Constructor
*/
//Help::Help() {}

/**
* Get help text for morse usage
*
* @return string
*/
string Help::GetHelpTxt()
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