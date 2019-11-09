#include <alc.h>
#include <al.h>
#include <time.h>
#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib,"OpenAL32.lib")

static ALuint buffers[AUDIO_WAVEFORM_PULSE_MAX];

//static float freqStart= DEFAULT_FREQ;

typedef struct {
	ALuint sid;
	int waveform;
	unsigned int length;
	unsigned int start;
	float decay;
	float startGain;
	float gain;
	float sweep;
	float freqStart;
	float freq;
	float freqEnd;
}CHANNEL;

CHANNEL channels[AUDIO_CHANNEL_MAX];

int audioInit() {
	ALCdevice* device = alcOpenDevice(NULL);// const ALCchar* devicename );
	if (device == NULL)
		return 1;
	ALCcontext* context = alcCreateContext(
		device,//ALCdevice *device,
		NULL); //const ALCint* attrlist );
	if (context == NULL)
		return 1;

	alcMakeContextCurrent(context); //ALCcontext *context


	alGenBuffers(
		AUDIO_WAVEFORM_PULSE_MAX,//ALsizei n,
		buffers);//ALuint* buffers );

	unsigned char pulse[][8] = {
		{0xff,0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0xff,0xff,0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0xff,0xff,0xff, 0xff, 0x00, 0x00, 0x00, 0x00},
		{0xff,0xff,0xff, 0xff, 0xff0, 0xff, 0x00, 0x00} };

	for (int i = AUDIO_WAVEFORM_PULSE_12_5; i <= AUDIO_WAVEFORM_PULSE_MAX; i++) {
		ALsizei size = sizeof pulse[0];
		alBufferData(
			buffers[i], AL_FORMAT_MONO8,	//ALuint bid, ALenum format, 
			pulse[i],//const ALvoid* data,
			size, size);//ALsizei size, ALsizei freq );
	}
	unsigned char triangle[] = {
		0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff };

	alBufferData(
		buffers[AUDIO_WAVEFORM_TRIANGLE], AL_FORMAT_MONO8,	//ALuint bid, ALenum format, 
		triangle,//const ALvoid* data,
		sizeof triangle, sizeof triangle);//ALsizei size, ALsizei freq );
	{
		const int len = 32767;
		int shiftReg = 1 << 14;
		unsigned char noise[len];
		for (int i = 0; i < len; i++) {
			int result = (shiftReg ^ (shiftReg >> 1)) & 1;
			shiftReg >>= 1;
			shiftReg |= result << 14;
			noise[i] = 0xff * result;
		}
		alBufferData(
			buffers[AUDIO_WAVEFORM_NOISE_LONG], AL_FORMAT_MONO8,	//ALuint bid, ALenum format, 
			noise,//const ALvoid* data,
			sizeof noise, 1);
	}
	{
		const int len = 93;
		int shiftReg = 1 << 14;
		unsigned char noise[len];
		for (int i = 0; i < len; i++) {
			int result = (shiftReg ^ (shiftReg >> 6)) & 1;
			shiftReg >>= 1;
			shiftReg |= result << 14;
			noise[i] = 0xff * result;
		}
		alBufferData(
			buffers[AUDIO_WAVEFORM_NOISE_SHORT], AL_FORMAT_MONO8,	//ALuint bid, ALenum format, 
			noise,//const ALvoid* data,
			sizeof noise, 1);
	}

	for (int i = 0; i < AUDIO_CHANNEL_MAX; i++) {
		audioGain(i,AUDIO_DEFAULT_GAIN);  //‰Šú‰»
		
		alGenSources(
			1,//ALsizei n, 
			&channels[i].sid);//ALuint * sources

		alSourcei(
			channels[i].sid, //ALuint sid, 
			AL_LOOPING,      //ALenum param, 
			AL_TRUE);        //ALint value
	}

	audioWaveform(AUDIO_CHANNEL_PULSE0, AUDIO_WAVEFORM_PULSE_12_5);
	audioWaveform(AUDIO_CHANNEL_PULSE1, AUDIO_WAVEFORM_PULSE_12_5);
	audioWaveform(AUDIO_CHANNEL_TRIANGLE, AUDIO_WAVEFORM_TRIANGLE);
	audioWaveform(AUDIO_CHANNEL_NOISE, AUDIO_WAVEFORM_NOISE_LONG);
	
	audioFreq(AUDIO_CHANNEL_PULSE0, AUDIO_DEFAULT_FREQ);
	audioFreq(AUDIO_CHANNEL_PULSE1, AUDIO_DEFAULT_FREQ);
	audioFreq(AUDIO_CHANNEL_TRIANGLE, AUDIO_DEFAULT_FREQ);
	audioFreq(AUDIO_CHANNEL_NOISE, audioIndexToFreq(8));
		
		return 0;
}
void audioWaveform(int _channel, int _waveform) {
	channels[_channel].waveform = _waveform;
	alSourcei(
		channels[_channel].sid,//ALuint sid, 
		AL_BUFFER,//ALenum param, 
		buffers[channels[_channel].waveform]);//ALint value
}
void audioGain(int _channel, float _gain) {
	channels[_channel].gain = channels[_channel].startGain = _gain;
	alSourcef(
		channels[_channel].sid,
		AL_GAIN, 
		channels[_channel].gain );

}
void audioLength(int _channel, unsigned int mills) {
	channels[_channel].length = mills;

}
void audioDecay(int _channel, float _decay) {
	channels[_channel].decay = _decay;

}
void audioSweep(int _channel, float _sweep, float _freqEnd ) {
	channels[_channel].sweep = _sweep;
	channels[_channel].freqEnd = _freqEnd;
}

void audioFreq(int _channel, float _freq) {
	channels[_channel].freqStart = _freq;
	alSourcef(
		channels[_channel].sid,//ALuint sid,
		AL_PITCH,//ALenum param, 
		channels[_channel].freq);	//ALfloat value 
}
float audioIndexToFreq(int _index) {
	int divisorTable[] = { 4,8,16,32,64,96,128,160,
		202,254,380,508,762,1016,2034,4068 };
	return 1789772.5f / divisorTable[_index];
}
void audioPlay(int _channel) {
	channels[_channel].gain = channels[_channel].startGain;
	alSourcef(
		channels[_channel].sid,     //ALuint sid,
		AL_GAIN,                    //ALenum param, 
		channels[_channel].gain);	//ALfloat value 

	channels[_channel].freq = channels[_channel].freqStart;
	alSourcef(
		channels[_channel].sid,   //ALuint sid,
		AL_PITCH,                 //ALenum param, 
		channels[_channel].freq );
	
	alSourcePlay(channels[_channel].sid);
	channels[_channel].start = clock();
}
void audioStop(int _channel) {
	alSourceStop(channels[_channel].sid);
}
void audioUpdate() {
	for (int i = 0; i < AUDIO_CHANNEL_MAX; i++) {
		if ((channels[i].length > 0) && (clock() - channels[i].start) >= channels[i].length)
			audioStop(i);

		if ((channels[i].decay != 0) && (channels[i].decay < 1)) {
			/*gain *= decay;
			if (gain <= 1.f / 256) {
				gain = 0;
			}*/
			alSourcef(
				channels[i].sid, 
				AL_GAIN, 
				channels[i].gain *= channels[i].decay);
		}
		if (channels[i].sweep != 0) {
			channels[i].freq *= channels[i].sweep;
			if (channels[i].freqEnd != 0) {
				if (
					((channels[i].sweep > 1) && (channels[i].freq >= channels[i].freqEnd))
					|| ((channels[i].sweep < 1) && (channels[i].freq <= channels[i].freqEnd))
					)
					audioStop(i);

			}
			alSourcef(
				channels[i].sid,//ALuint sid,
				AL_PITCH,//ALenum param, 
				channels[i].freq);	//ALfloat value 
		}
	}
		ALenum error = alGetError();
		if (error != AL_NO_ERROR) {
			printf("%s\n",
				alGetString(error));
		}
}