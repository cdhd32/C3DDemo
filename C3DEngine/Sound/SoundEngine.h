#pragma once
//fmod 라이브러리를 사용한 음원 재생 클래스

#include <map>
#include <string>
#include <fmod.h>
#include "SoundEngineDefine.h"

class SoundEngine
{
private:
	//static SoundEngine* m_instance;

	float m_bgmvolume;
	float m_sfxvolume;

	eSoundChannelPlayMode bgmPlayMode;
	FMOD_SYSTEM* m_FMODSystem;

	//재생할 사운드를 저장하는 map -> 파일명으로 사운드 리소스 읽고 저장하는 함수 추가
	//enum 값 key로 FMOD_SOUND* 를 찾아주는 map
	std::map<eSoundResourceBGM, FMOD_SOUND*> m_BGMsoundDict;
	std::map<eSoundResourceSFX, FMOD_SOUND*> m_SFXsoundDict;
	
	//테스트용 사운드 객체
	FMOD_SOUND* sound_test;

	//BGM용, 이펙트용 사운드 채널
	FMOD_CHANNEL* m_soundChannelBGM;
	FMOD_CHANNEL* m_soundChannelSFX;

	//사운드 채널별 현재 재생되고 있는 사운드


	//사운드, 채널 enum으로 만들기

private:
	SoundEngine();
	~SoundEngine();

	//내부에서 Load 할 때 사용하는 sound 파일 읽기 함수

	void CreateSoundObj(std::string fileLocation, FMOD_SOUND** sound, FMOD_MODE playmode);
	void InsertSoundDict(eSoundResourceSFX sfx, FMOD_SOUND* sound);
	void InsertSoundDict(eSoundResourceBGM bgm, FMOD_SOUND* sound);

	void PlayOnChannel(FMOD_CHANNEL** channel, FMOD_SOUND* sound);
	void SetChannelVolume(FMOD_CHANNEL* channel, float volume);

public:
	//싱글톤
	static SoundEngine& GetInstance()
	{
		static SoundEngine soundEngine;
		/*if (m_instance == nullptr)
		{
			m_instance = new SoundEngine();
		}*/
		return soundEngine;
	}

	void Initialize();
	void Finalize();

	//주기적인 사운드 시스템 업데이트 FMOD는 스트리밍 방식을 사용하기 때문
	void SoundUpdate();
	
	void PlayBGMSound(eSoundResourceBGM bgm);
	void PlaySFXSound(eSoundResourceSFX sfx);
	//외부에서 Load 할 때 사용하는 sound 파일 읽기 함수
	void LoadSoundFile(std::string fileLocation, eSoundResourceBGM bgm);
	void LoadSoundFile(std::string fileLocation, eSoundResourceSFX sfx);

	void StopBGM();
	void StopSFX();

	void SetBGMVolume(float volume);
	void SetSFXVolume(float volume);

	void BGMFadeIn(float volume);
	void BGMFadeOut(float volume);
};

