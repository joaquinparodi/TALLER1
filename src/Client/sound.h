#pragma once

#include <SDL2/SDL_mixer.h>
#include <vector>

enum struct SoundType : int {
	HIT_BLOCK,
	PICK_UP_COIN,
	PICK_UP_MUSHROOM,
	STOMP_ENEMY,
	LOSE_LIFE,
	END_LEVEL,
	MAX
};

class Sound {
public:
	Sound();
	~Sound();
	void play_sound(SoundType type);
	void mute();
private:
	Mix_Music* music;
	std::vector<Mix_Chunk*> sounds;
	bool m_b = false;
};