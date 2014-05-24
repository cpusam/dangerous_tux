/*
 Copyright (C) 2014 Samuel Leonardo

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#ifndef SOUNDPLAYER_HPP
#define SOUNDPLAYER_HPP

enum ESoundType
{
	UNDEF_SOUND=-1,
	CHUNK_SOUND,
	MUSIC_SOUND
};

enum ESoundState
{
	INACTIVE_SOUND,
	PAUSED_SOUND,
	PLAYING_SOUND
};

class CSound
{
	protected:
		int type;
		int state;
		int channel; // canal usado para tocar o som
		string id;
		string path;
		Mix_Chunk * chunk;
		Mix_Music * music;
	
	public:
		CSound (  )
		{
			type = UNDEF_SOUND;
			chunk = 0;
			music = 0;
			state = INACTIVE_SOUND;
			channel = -1;
		}
		
		int get_type (  )
		{
			return type;
		}
		
		int get_state (  )
		{
			return state;
		}
		
		void set_state ( int s )
		{
			state = s;
		}
		
		string get_id (  )
		{
			return id;
		}
		
		string get_path (  )
		{
			return path;
		}
		
		Mix_Chunk * get_chunk (  )
		{
			return chunk;
		}
		
		Mix_Music * get_music (  )
		{
			return music;
		}
		
		int get_channel (  )
		{
			return channel;
		}
		
		bool play_sound ( int ch, int loops )
		{
			bool ret = false;
			channel = ch;
			
			switch (type)
			{
				case CHUNK_SOUND:
					ch = Mix_PlayChannel(channel, chunk, loops);
					if (channel == -1)
						channel = ch;
					ret = true;
					break;
				
				case MUSIC_SOUND:
					ch = Mix_PlayMusic(music, loops);
					if (channel == -1)
						channel = ch;
					ret = true;
					break;
				default:
					break;
			}
			
			return ret;
		}
		
		void free_sound (  )
		{
			if (chunk)
				Mix_FreeChunk(chunk);
			
			if (music)
				Mix_FreeMusic(music);
			
			chunk = 0;
			music = 0;
		}
		
		void set_chunk ( string p )
		{
			Mix_Chunk * c = Mix_LoadWAV(p.c_str());
			if (c)
			{
				unsigned int f = p.find_last_of("/\\");
				path = p.substr(0, f);
				id = p.substr(f + 1);
				
				chunk = c;
				type = CHUNK_SOUND;
			}
			else
			{
				string t;
				t = "CSound: não conseguiu abrir efeito ";
				t.append(p);
				throw t.c_str();
			}
		}
		
		void set_music ( string p )
		{
			Mix_Music * m = Mix_LoadMUS(p.c_str());
			if (m)
			{
				unsigned int f = p.find_last_of("/\\");
				path = p.substr(0, f);
				id = p.substr(f + 1);
				
				music = m;
				type = MUSIC_SOUND;
			}
			else
			{
				string t;
				t = "CSound: não conseguiu abrir musica ";
				t.append(p);
				throw t.c_str();
			}
		}
};

class CSoundPlayer: public CStateMachine
{
	protected:
		static CSoundPlayer * singleton;
		vector <CSound> sound;

	protected:
		CSoundPlayer (  )
		{
			singleton = 0;
		}
	
	public:
		~CSoundPlayer (  )
		{
			if (singleton)
				delete singleton;
			
			free_sounds();
		}
		
		static CSoundPlayer * instance (  )
		{
			if (!singleton)
				singleton = new CSoundPlayer();
			
			return singleton;
		}
		
		void free_sounds (  )
		{
			for (vector <CSound>::iterator it = sound.begin(); it != sound.end(); it++)
				it->free_sound();
			
			sound.clear();
		}
		
		bool has_sound ( string id )
		{
			for (vector <CSound>::iterator it = sound.begin(); it != sound.end(); it++)
				if (id == it->get_id())
					return true;
			
			return false;
		}
		
		bool add_sound ( CSound s )
		{
			if (has_sound(s.get_id()))
				return false;
			
			sound.push_back(s);
			return true;
		}
		
		bool playing ( string id )
		{
			for (vector <CSound>::iterator it = sound.begin(); it != sound.end(); it++)
				if (it->get_id() == id && it->get_state() == PLAYING_SOUND)
					return true;
			
			return false;
		}
		
		bool pause_sound ( string id )
		{
			bool ret = false;
			
			for (vector <CSound>::iterator it = sound.begin(); it != sound.end(); it++)
				if (id == it->get_id())
				{
					switch (it->get_type())
					{
						case CHUNK_SOUND:
							Mix_Pause(it->get_channel());
							ret = true;
							break;
						case MUSIC_SOUND:
							Mix_PauseMusic();
							ret = true;
							break;
					}
					
					if (ret)
					{
						it->set_state(PAUSED_SOUND);
						break;
					}
				}
			
			return ret;
		}
		
		bool resume_sound ( string id )
		{
			bool ret = false;
			
			for (vector <CSound>::iterator it = sound.begin(); it != sound.end(); it++)
				if (id == it->get_id())
				{
					switch (it->get_type())
					{
						case CHUNK_SOUND:
							Mix_Resume(it->get_channel());
							ret = true;
							break;
						case MUSIC_SOUND:
							Mix_ResumeMusic();
							ret = true;
							break;
					}
					
					if (ret)
					{
						it->set_state(PLAYING_SOUND);
						break;
					}
				}
			
			return ret;
		}
		
		bool play_sound ( string id, int channel=-1, int loops=0 )
		{
			bool ret = false;
			for (vector <CSound>::iterator it = sound.begin(); it != sound.end(); it++)
				if (id == it->get_id())
				{
					if (it->get_state() == PAUSED_SOUND)
					{
						resume_sound(id);
						ret = true;
					}
					else
						ret = it->play_sound(channel, loops);
					it->set_state(PLAYING_SOUND);
					
					if (ret)
						return true;
				}

			return ret;
		}
		
		bool halt_sound ( string id )
		{
			bool ret = false;
			
			for (vector <CSound>::iterator it = sound.begin(); it != sound.end(); it++)
				if (id == it->get_id())
				{
					switch (it->get_type())
					{
						case CHUNK_SOUND:
							Mix_HaltChannel(it->get_channel());
							ret = true;
							break;
						case MUSIC_SOUND:
							Mix_HaltMusic();
							ret = true;
							break;
					}
					
					if (ret)
					{
						it->set_state(INACTIVE_SOUND);
						break;
					}
				}
				else if (id == "all")
				{
					
				}
			
			return ret;
		}

		int update (  )
		{
			for (vector <CSound>::iterator it = sound.begin(); it != sound.end(); it++)
				switch (it->get_type())
				{
					case CHUNK_SOUND:
						if (Mix_Playing(it->get_channel()))
							it->set_state(PLAYING_SOUND);
						else if (it->get_state() != PAUSED_SOUND)
							it->set_state(INACTIVE_SOUND);
						break;
					
					case MUSIC_SOUND:
						if (Mix_PlayingMusic())
							it->set_state(PLAYING_SOUND);
						else if (it->get_state() != PAUSED_SOUND)
							it->set_state(INACTIVE_SOUND);
						break;
					default:
						break;
				}
			
			return get_state();
		}
};

CSoundPlayer * CSoundPlayer::singleton = 0;

#endif
