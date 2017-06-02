#include "AudioManager.h"

AudioManager::AudioManager()
{
	audioSource = createIrrKlangDevice();
	if (!audioSource)
	{
		cout << "Error Initializing audio Source" << endl;
	}
}

void AudioManager::End()
{
	audioSource->drop();
}

void AudioManager::Play2DSound(char *path, bool loop)
{
	audioSource->play2D(path,loop);
}

void AudioManager::Play3DSound(char * path, vec3df position, bool loop)
{
	audioClip = audioSource->play3D(path,
		vec3df(0, 0, 0), loop, false, true);
}
