#include "morsewav.h"
#include <shellapi.h>
#pragma comment(lib, "Shell32.lib")

using namespace std;

/**
* member variables
*/
#define EPW 50      // elements per word (definition)
const char* MorseCode; // string array with morse
int Debug;      // debug mode
int Play;       // play WAV file
int NumChannels;   // stereo or mono modus
const char* Path;    // output filename
double Tone;    // tone frequency (Hz)
double Wpm;     // words per minute
double Eps;     // elements per second (frequency of basic morse element)
double Bit;     // duration of basic morse element,cell,quantum (seconds)
double Sps;     // samples per second (WAV file, sound card)
double amplitude = 0.8; // 80% of max volume
vector<int16_t> pcm; // array with pcm data
const string dir = "C:\\Users\\User\\Desktop\\wav-files-morseII\\"; // output directory

MorseWav::MorseWav(const char* morsecode, double tone, double wpm, double samples_per_second, bool play, int modus)
{
    string filename = "morse_";
    filename += to_string(time(NULL));
    filename += ".wav";
    string fp = dir + filename;
	Path = fp.c_str();
    MorseCode = morsecode;
    NumChannels = modus;
    Wpm = wpm;
    Tone = tone;
    Sps = samples_per_second;
    // Note 60 seconds = 1 minute and 50 elements = 1 morse word.
    Eps = Wpm / 1.2;    // elements per second (frequency of morse coding)
    Bit = 1.2 / Wpm;    // seconds per element (period of morse coding)
    printf("wave: %9.3lf Hz (-sps:%lg)\n", Sps, Sps);
    printf("tone: %9.3lf Hz (-tone:%lg)\n", Tone, Tone);
    printf("code: %9.3lf Hz (-wpm:%lg)\n", Eps, Wpm);

    MorseWav::MorseTones(MorseCode);
    MorseWav::WriteWav(filename.c_str(), pcm);

    printf(" written to %s \n", Path); 

    if (play)
    {
        /*
        string str = Path;
        str += " /play /close ";
        str += Path;
        const char* c = str.c_str();
        printf("** %s\n", c);
        system(c);*/
        ShellExecuteA(NULL, "open", Path, NULL, NULL, SW_SHOWNORMAL);
    }
}
void MorseWav::Tones(int silence)
{
    double seconds = Bit;   // keep seconds explicit
	size_t numsamples = static_cast<size_t>(std::round(seconds * Sps)); // number of samples to generate
    const double twoPiF = 2 * M_PI * Tone;
    const double twoPi = 2.0 * M_PI;
    constexpr int maxInt16 = std::numeric_limits<int16_t>::max(); // maximum for signed 16‑bit PCM

    pcm.reserve(pcm.size() + numsamples);
    if (NumChannels == 2)
    {
        for (size_t i = 0; i < numsamples; ++i)
        {
			double t = static_cast<double>(i) / Sps; // time in seconds
            double sampleL = std::sin(silence * twoPiF * t) * amplitude;
            double sampleR = std::sin(silence * twoPiF * t) * amplitude;

            int16_t intSampleL = static_cast<int16_t>(clamp(sampleL, -1.0, 1.0) * maxInt16);
            int16_t intSampleR = static_cast<int16_t>(clamp(sampleR, -1.0, 1.0) * maxInt16);

            pcm.push_back(intSampleL);
            pcm.push_back(intSampleR);
        }
    }
    else
    {
        for (size_t i = 0; i < numsamples; ++i)
        {
            double t = static_cast<double>(i) / Sps;
            double sampleL = std::sin(silence * twoPiF * t) * amplitude;
            int16_t intSampleL = static_cast<int16_t>(clamp(sampleL, -1.0, 1.0) * maxInt16);
            pcm.push_back(intSampleL);
        }
    }
}

/**
* Define dit, dah, end of letter, end of word.
*
* The rules of 1/3/7 and 1/2/4(more suitable for common microphones, like webcams and phones):
* Morse code is: tone for one unit (dit) or three units (dah)
* followed by the sum of one unit of silence (always),
* plus two units of silence (if end of letter, one space),
* plus four units of silence (if also end of word).
*/
void MorseWav::Dit() { Tones(1); Tones(0); }
void MorseWav::Dah() { Tones(1); Tones(1); Tones(1); Tones(0); }
void MorseWav::Space() { Tones(0); Tones(0); }

/**
* Create Tones from morse code.
*
* @param code char array with morse code ('.' = dit, '-' = dah, ' ' = space)
*/
void MorseWav::MorseTones(const char* code)
{
    char c;
    while ((c = *code++) != '\0')
    {
        //printf("%c", c);
        if (c == '.') Dit();
        if (c == '-') Dah();
        if (c == ' ') Space();
    }
}

// (Optional) Simple WAV-header + data writer for 16-bit stereo PCM.
void MorseWav::WriteWav(const char* filename, const std::vector<int16_t>& pcmData)
{
    // Try to create the directory
    if (_mkdir(dir.c_str()) == 0)
    {
        cerr << "Directory created successfully.\n";
    }
    else
    {
        if (errno == EEXIST)
        {
            cerr << "Directory already exists.\n";
        }
        else
        {
            cerr << "Error creating directory\n";
            exit(1);
        }
    }
    // Open file for binary writing
    ofstream out((dir + filename), std::ios::binary);
    if (!out.is_open())
    {
        cerr << "Failed to open file: " << dir + filename << '\n';
        // optionally inspect errno: std::perror("open");
        exit(1);
    }
    int byteRate = Sps * NumChannels * sizeof(int16_t);
    int blockAlign = NumChannels * sizeof(int16_t);
    int dataSize = static_cast<int>(pcmData.size() * sizeof(int16_t));
    int modus = NumChannels;

    // RIFF header
    out.write("RIFF", 4);
    uint32_t chunkSize = 36 + dataSize;
    out.write(reinterpret_cast<const char*>(&chunkSize), 4);
    out.write("WAVE", 4);

    // fmt subchunk
    out.write("fmt ", 4);
    uint32_t subChunk1Size = 16;
    out.write(reinterpret_cast<const char*>(&subChunk1Size), 4);
    uint16_t audioFormat = 1; // PCM
    out.write(reinterpret_cast<const char*>(&audioFormat), 2);
    out.write(reinterpret_cast<const char*>(&modus), 2);
    out.write(reinterpret_cast<const char*>(&Sps), 4);
    out.write(reinterpret_cast<const char*>(&byteRate), 4);
    out.write(reinterpret_cast<const char*>(&blockAlign), 2);
    uint16_t bitsPerSample = 16;
    out.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
    const char* sizebytes = "0";
    out.write(reinterpret_cast<const char*>(&sizebytes), 2);

    // data subchunk
    out.write("data", 4);
    out.write(reinterpret_cast<const char*>(&dataSize), 4);
    out.write(reinterpret_cast<const char*>(pcmData.data()), dataSize);

    out.flush();
    out.close();
}
