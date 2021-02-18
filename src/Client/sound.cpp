#include "sound.h"
#include <iostream>


Sound::Sound() {
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048);

	//Load the music
	music = Mix_LoadMUS("assets/sounds/principal_level1.wav");

	sounds.reserve((int)SoundType::MAX);

	//Load the sound effects
	sounds[(int)SoundType::HIT_BLOCK] = Mix_LoadWAV("assets/sounds/impacto_sorpresa.wav");
	sounds[(int)SoundType::PICK_UP_COIN] = Mix_LoadWAV("assets/sounds/moneda.wav");
	sounds[(int)SoundType::PICK_UP_MUSHROOM] = Mix_LoadWAV("assets/sounds/hongo.wav");
	sounds[(int)SoundType::STOMP_ENEMY] = Mix_LoadWAV("assets/sounds/destruccion_enemigo.wav");
	sounds[(int)SoundType::LOSE_LIFE] = Mix_LoadWAV("assets/sounds/pierde_vida.wav");
	sounds[(int)SoundType::END_LEVEL] = Mix_LoadWAV("assets/sounds/final_nivel.wav");

	Mix_PlayMusic(music, -1);
}

Sound::~Sound() {

	Mix_PauseMusic();

	//Free the sound effects
	for (Mix_Chunk* sound : sounds) {
		Mix_FreeChunk(sound);
	}

	//Free the music
	Mix_FreeMusic(music);

	//Quit SDL_mixer
	Mix_CloseAudio();
}

void Sound::play_sound(SoundType type) {
	if (sounds[(int)type] != NULL && !m_b) {
		Mix_PlayChannel(-1, sounds[(int)type], 0) == -1;
	}
}

void Sound::mute() {
	if (m_b) {
		Mix_ResumeMusic();
	} else {
		Mix_PauseMusic();
	}
	m_b = !m_b;
}
