
#include <al.h>
#include <alc.h>
#include <iostream>

#include	<cstdio>
#include	<cstring>

#include <sndfile.hh>

static void list_audio_devices(const ALCchar *devices)
{
	const ALCchar *device = devices, *next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}

void checkerror(ALCenum &error)
{
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		printf("something wrong happened %d\n", error);
	}
}
int main()
{
	ALCenum error;
	ALboolean enumeration;

	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
	{
		printf("boooo we dont do enumeration\n");
	}
		
	else
	{
		// enumeration supported
		printf("yay we do enumeration\n");
	}
		
		
	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
	 
	//1. create handle for device	
	ALCdevice *device;
	device = alcOpenDevice(alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER));
	//2. create a context to interact with
	ALCcontext *context;
	context = alcCreateContext(device, nullptr);
	
	//3. initialize the context
	if (!alcMakeContextCurrent(context))
	{

		checkerror(error);
	}
	//4. create a buffer for sound
	ALuint buffer;
	alGenBuffers((ALuint)1, &buffer);
	checkerror(error);
	//5. read audio from file 

	const char * fname = "aliens_game_over2.wav";

	SF_INFO soundInfo;
	SNDFILE *sound;
	sound = sf_open(fname, SFM_READ, &soundInfo);
	unsigned int sampleCount = static_cast<std::size_t>(soundInfo.frames) * soundInfo.channels;
	if (!sound)
	{
		printf("no sound... done did it....");
		system("pause");
		return 0;
	}


	//6. Source generation
	ALuint source;
	alGenSources((ALuint)1, &source);

	alSourcef(source, AL_PITCH, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, AL_TRUE);
	//end Source generation


	//7. Load the audio stream to a buffer
	short *samples = new short[sampleCount + 1];

	//8. gen buffer for audio
	alBufferData(buffer, 
		soundInfo.format,
		&samples[0], 
		sampleCount,
		soundInfo.samplerate);

	checkerror(error);
	//bind source to buffer
	alSourcei(source, AL_BUFFER, buffer);

	checkerror(error);
	//play the source
	alSourcePlay(source);	
	checkerror(error);

	ALint source_state;
	alGetSourcei(source, AL_SOURCE_STATE, &source_state);

	while (source_state == AL_PLAYING) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
		printf("playing");
	}

	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	system("pause");

	return true;
}