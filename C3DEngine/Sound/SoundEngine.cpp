#include "SoundEngine.h"

//SoundEngine* SoundEngine::m_instance = nullptr;

SoundEngine::SoundEngine()
{

}

SoundEngine::~SoundEngine()
{

}

void SoundEngine::Initialize()
{
	FMOD_System_Create(&m_FMODSystem);
	FMOD_System_Init(m_FMODSystem, 32, FMOD_INIT_NORMAL, nullptr);
	bgmPlayMode = eSoundChannelPlayMode::NORMAL;
	LoadSoundFile("..\\Resources\\Sound\\Ingame_BGM.wav", eSoundResourceBGM::INGAME);
	LoadSoundFile("..\\Resources\\Sound\\Lobby.wav", eSoundResourceBGM::LOBBY);

	LoadSoundFile("..\\Resources\\Sound\\Cube_Slap.wav", eSoundResourceSFX::CUBETURN);
	LoadSoundFile("..\\Resources\\Sound\\StageClear.wav", eSoundResourceSFX::STAGECLEAR);
	LoadSoundFile("..\\Resources\\Sound\\Gameover.wav", eSoundResourceSFX::GAMEOVER);

}

void SoundEngine::Finalize()
{
	//Release map에 맞게 수정 해야함
	FMOD_Sound_Release(sound_test); // FMOD sound 객체 해제 -> map에 있는 모든 사운드 해제하는 걸로 변경
	FMOD_System_Close(m_FMODSystem); // FMOD system 객체 close
	FMOD_System_Release(m_FMODSystem); // FMOD system 객체 해제
}



void SoundEngine::CreateSoundObj(std::string fileLocation, FMOD_SOUND** sound, FMOD_MODE playmode)
{
	//define 상수 설명
	//#define FMOD_LOOP_OFF 0x00000001  /* For non looping sounds. (DEFAULT).  Overrides FMOD_LOOP_NORMAL / FMOD_LOOP_BIDI. */
	//#define FMOD_LOOP_NORMAL 0x00000002  /* For forward looping sounds. */
	FMOD_RESULT result = FMOD_System_CreateSound(m_FMODSystem, fileLocation.c_str(), playmode, 0, sound);
}

void SoundEngine::InsertSoundDict(eSoundResourceBGM bgm, FMOD_SOUND* sound)
{
	if (sound != nullptr)
	{
		m_BGMsoundDict.insert(std::make_pair(bgm, sound));
	}
}

void SoundEngine::InsertSoundDict(eSoundResourceSFX sfx, FMOD_SOUND* sound)
{
	if (sound != nullptr)
	{
		m_SFXsoundDict.insert(std::make_pair(sfx, sound));
	}
}

//파일 주소로 FMOD_SOUND를 생성하고 std::map 에서 bgm enum 값으로 찾을 수 있게 저장
void SoundEngine::LoadSoundFile(std::string fileLocation, eSoundResourceBGM bgm)
{
	FMOD_SOUND* sound = nullptr;

	//bgm은 루프 재생 FMOD_LOOP_NORMAL
	CreateSoundObj(fileLocation, &sound, FMOD_LOOP_NORMAL);
	InsertSoundDict(bgm, sound);
}

//파일 주소로 FMOD_SOUND를 생성하고 std::map 에서 sfx enum 값으로 찾을 수 있게 저장
void SoundEngine::LoadSoundFile(std::string fileLocation, eSoundResourceSFX sfx)
{
	FMOD_SOUND* sound = nullptr;

	//sfx는 한번만 재생 FMOD_LOOP_OFF
	CreateSoundObj(fileLocation, &sound, FMOD_LOOP_OFF);
	InsertSoundDict(sfx, sound);
}

//채널에 사운드 재생
void SoundEngine::PlayOnChannel(FMOD_CHANNEL** channel, FMOD_SOUND* sound)
{
	FMOD_System_PlaySound(m_FMODSystem, FMOD_CHANNEL_FREE, sound, 0, channel);
}

void SoundEngine::SetChannelVolume(FMOD_CHANNEL* channel, float volume)
{
	FMOD_Channel_SetVolume(channel, volume);
}

//외부에서 사운드 업데이트
void SoundEngine::SoundUpdate()
{
	FMOD_System_Update(m_FMODSystem);
}

//외부에서 bgm 재생
void SoundEngine::PlayBGMSound(eSoundResourceBGM bgm)
{
	FMOD_SOUND* sound = m_BGMsoundDict.find(bgm)->second;
	PlayOnChannel(&m_soundChannelBGM, sound);
}

//외부에서 sfx 재생
void SoundEngine::PlaySFXSound(eSoundResourceSFX sfx)
{
	FMOD_SOUND* sound = m_SFXsoundDict.find(sfx)->second;
	PlayOnChannel(&m_soundChannelSFX, sound);
}

//BGM 채널 사운드 정지
void SoundEngine::StopBGM()
{
	FMOD_Channel_Stop(m_soundChannelBGM);
}

//SFX 채널 사운드 정지
void SoundEngine::StopSFX()
{
	FMOD_Channel_Stop(m_soundChannelSFX);
}

//BGM 채널 볼륨 조절 0.0f ~ 1.0f
void SoundEngine::SetBGMVolume(float volume)
{
	m_bgmvolume = volume;
	FMOD_Channel_SetVolume(m_soundChannelBGM, m_bgmvolume);
}

//SFX 채널 볼륨 조절 0.0f ~ 1.0f
void SoundEngine::SetSFXVolume(float volume)
{
	m_sfxvolume = volume;
	FMOD_Channel_SetVolume(m_soundChannelSFX, m_sfxvolume);
}

//void SoundEngine::SetBGMPlayMode(float maxVolume)
//{
//	/// 페이드 인 페이드 아웃 구혀뉴ㅠ
//}
