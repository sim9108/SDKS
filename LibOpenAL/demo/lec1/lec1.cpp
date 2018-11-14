// lec1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>

#include <AL/al.h>
#include <AL/alc.h>
#include <Al/alext.h>
#include <AL/alut.h>

static void
playFile(const char *fileName){	
	ALenum error;	

	/* Create an AL buffer from the given sound file. */
	ALuint buffer = alutCreateBufferFromFile(fileName);
	error = alutGetError();
	if (buffer == AL_NONE)
	{		
		std::cerr<< "Error loading file:"<<	alutGetErrorString(error)<<std::endl;
		alutExit();
		std::exit(EXIT_FAILURE);
	}

	/* Generate a single source, attach the buffer to it and start playing. */
	ALuint source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);
	
	auto _pan = -10000.0f;
	float pan = (float)_pan / 10000.0f;
	float pan2 = (float)sqrt(1 - pan*pan);

	float sourcePosAL[] = { pan,pan2, 0.0f };
	alSourcefv(source, AL_POSITION, sourcePosAL);
	alSourcePlay(source);

	/* Normally nothing should go wrong above, but one never knows... */
	error = alGetError();
	if (error != ALUT_ERROR_NO_ERROR)
	{
		std::cerr<<alGetString(error)<<std::endl;
		alutExit();
		std::exit(EXIT_FAILURE);
	}

	/* Check every 0.1 seconds if the sound is still playing. */
	ALint status;
	do
	{
		alutSleep(0.1f);
		alGetSourcei(source, AL_SOURCE_STATE, &status);
	} while (status == AL_PLAYING);
}

int
main(int argc, char **argv)
{
	/* Initialise ALUT and eat any ALUT-specific commandline flags. */
	if (!alutInit(&argc, argv))
	{
		ALenum error = alutGetError();
		std::cerr << alutGetErrorString(error) << std::endl;
		std::exit(EXIT_FAILURE);
	}

	playFile(argv[1]);

	if (!alutExit())
	{
		ALenum error = alutGetError();
		std::cerr << alutGetErrorString(error) << std::endl;
		std::exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}