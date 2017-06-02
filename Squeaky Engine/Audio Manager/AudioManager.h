#pragma once
#ifdef WIN32
#include <windows.h>
#include <conio.h>
inline void sleepSomeTime() { Sleep(100); }
#endif
#include <ik\irrKlang.h>
#include <iostream>
#include <string>
#pragma comment(lib, "irrKlang.lib")
using namespace irrklang;
using namespace std;
class AudioManager
{
public:
	
	AudioManager();
	void Play2DSound(char *path,bool loop);
	void Play3DSound(char * path, vec3df position, bool loop);
	void End();
private:
	ISound* audioClip;
	ISoundEngine* audioSource;
};