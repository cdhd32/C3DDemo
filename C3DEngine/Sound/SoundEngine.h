#pragma once
//fmod ���̺귯���� ����� ���� ��� Ŭ����

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

	//����� ���带 �����ϴ� map -> ���ϸ����� ���� ���ҽ� �а� �����ϴ� �Լ� �߰�
	//enum �� key�� FMOD_SOUND* �� ã���ִ� map
	std::map<eSoundResourceBGM, FMOD_SOUND*> m_BGMsoundDict;
	std::map<eSoundResourceSFX, FMOD_SOUND*> m_SFXsoundDict;
	
	//�׽�Ʈ�� ���� ��ü
	FMOD_SOUND* sound_test;

	//BGM��, ����Ʈ�� ���� ä��
	FMOD_CHANNEL* m_soundChannelBGM;
	FMOD_CHANNEL* m_soundChannelSFX;

	//���� ä�κ� ���� ����ǰ� �ִ� ����


	//����, ä�� enum���� �����

private:
	SoundEngine();
	~SoundEngine();

	//���ο��� Load �� �� ����ϴ� sound ���� �б� �Լ�

	void CreateSoundObj(std::string fileLocation, FMOD_SOUND** sound, FMOD_MODE playmode);
	void InsertSoundDict(eSoundResourceSFX sfx, FMOD_SOUND* sound);
	void InsertSoundDict(eSoundResourceBGM bgm, FMOD_SOUND* sound);

	void PlayOnChannel(FMOD_CHANNEL** channel, FMOD_SOUND* sound);
	void SetChannelVolume(FMOD_CHANNEL* channel, float volume);

public:
	//�̱���
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

	//�ֱ����� ���� �ý��� ������Ʈ FMOD�� ��Ʈ���� ����� ����ϱ� ����
	void SoundUpdate();
	
	void PlayBGMSound(eSoundResourceBGM bgm);
	void PlaySFXSound(eSoundResourceSFX sfx);
	//�ܺο��� Load �� �� ����ϴ� sound ���� �б� �Լ�
	void LoadSoundFile(std::string fileLocation, eSoundResourceBGM bgm);
	void LoadSoundFile(std::string fileLocation, eSoundResourceSFX sfx);

	void StopBGM();
	void StopSFX();

	void SetBGMVolume(float volume);
	void SetSFXVolume(float volume);

	void BGMFadeIn(float volume);
	void BGMFadeOut(float volume);
};

