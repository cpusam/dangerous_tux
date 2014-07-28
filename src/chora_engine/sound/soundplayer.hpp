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

<<<<<<< HEAD
#include <string>
#include <vector>

#include "../chora.hpp"
#include "../statemachine.hpp"

=======
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
enum ESoundType
{
	UNDEF_SOUND=-1,
	CHUNK_SOUND,
	MUSIC_SOUND
};

enum ESoundState
{
	UNLOADED_SOUND, // som ainda n�o carregado
	INACTIVE_SOUND, // som carregado e pronto para tocar
	PAUSED_SOUND, // som pausado
	PLAYING_SOUND // tocando o som
};

class CSound: public CStateMachine
{
	protected:
		int type;
		int channel; // canal usado para tocar o som
<<<<<<< HEAD
		std::string id;
		std::string path;
=======
		string id;
		string path;
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
		Mix_Chunk * chunk;
		Mix_Music * music;
	
		virtual int update (  )
		{
			return get_state();
		}
	
	public:
		CSound (  )
		{
			type = UNDEF_SOUND;
			chunk = 0;
			music = 0;
			set_state(UNLOADED_SOUND);
			channel = -1;
		}
		
<<<<<<< HEAD
		int get_type (  );
		
		std::string get_id (  );
		
		std::string get_path (  );
		
		Mix_Chunk * get_chunk (  );
		
		Mix_Music * get_music (  );
		
		int get_channel (  );
		
		bool play ( int ch, int loops );
		
		bool pause (  );
		
		bool resume (  );
		
		void free (  );
		
		void set_chunk ( std::string p );
		
		void set_music ( std::string p );
=======
		int get_type (  )
		{
			return type;
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
		
		bool play ( int ch, int loops )
		{
			bool ret = false;
					
			switch (type)
			{
				case CHUNK_SOUND:
					ch = Mix_PlayChannel(channel, chunk, loops);
					if (channel == -1)
					{
						channel = ch;
						ret = true;
					}
					break;
				
				case MUSIC_SOUND:
					ch = Mix_PlayMusic(music, loops);
					if (channel == -1)
					{
						channel = ch;
						ret = true;
					}
					break;
					
				case UNDEF_SOUND:
					if (chunk || music)
						set_state(INACTIVE_SOUND);
					else
						set_state(UNLOADED_SOUND);
					return false;
				
				default:
					break;
			}
			
			if (ret)
				set_state(PLAYING_SOUND);
			else
				set_state(PAUSED_SOUND);
			
			return ret;
		}
		
		bool pause (  )
		{
			bool ret = false;
			
			switch (type)
			{
				case CHUNK_SOUND:
					if (channel > -1)
					{
						Mix_Pause(channel);
						ret = true;
					}
					break;
				
				case MUSIC_SOUND:
					Mix_PauseMusic();
					ret = true;
					break;
				
				case UNDEF_SOUND:
					if (chunk || music)
						set_state(INACTIVE_SOUND);
					else
						set_state(UNLOADED_SOUND);
					return false;
				
				default:
					break;
			}
			
			if (ret)
				set_state(PAUSED_SOUND);
			else
				set_state(PLAYING_SOUND);
			
			return ret;
		}
		
		bool resume (  )
		{
			bool ret = false;
			
			switch (type)
			{
				case CHUNK_SOUND:
					if (channel > -1)
					{
						Mix_Resume(channel);
						ret = true;
					}
					break;
				
				case MUSIC_SOUND:
					
					break;
				
				case UNDEF_SOUND:
					if (chunk || music)
						set_state(INACTIVE_SOUND);
					else
						set_state(UNLOADED_SOUND);
					return false;
			}
		}
		
		void free (  )
		{
			if (chunk)
				Mix_FreeChunk(chunk);
			
			if (music)
				Mix_FreeMusic(music);
			
			chunk = 0;
			music = 0;
			
			set_state(UNLOADED_SOUND);
			type = UNDEF_SOUND;
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
				t = "CSound: n�o conseguiu abrir efeito ";
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
				t = "CSound: n�o conseguiu abrir musica ";
				t.append(p);
				throw t.c_str();
			}
		}
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
};

class CSoundPlayer: public CStateMachine
{
	protected:
		static CSoundPlayer * singleton;
<<<<<<< HEAD
		std::vector <CSound> sound;
=======
		vector <CSound> sound;
>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813

	protected:
		CSoundPlayer (  )
		{
			singleton = 0;
		}
	
	public:
		~CSoundPlayer (  )
		{
			free_sounds();
			
			if (singleton)
				delete singleton;
		}
		
		static CSoundPlayer * instance (  )
		{
			if (!singleton)
				singleton = new CSoundPlayer();
			
			return singleton;
		}
		
<<<<<<< HEAD
		void free_sounds (  );
		
		bool has_sound ( std::string id );
		
		bool add_sound ( CSound s );
		
		bool playing ( std::string id );
		
		bool pause_sound ( std::string id );
		
		bool resume_sound ( std::string id );
		
		bool play_sound ( std::string id, int channel=-1, int loops=0 );
		
		bool halt_sound ( std::string id );

		int update (  );
};

=======
		void free_sounds (  )
		{
			for (vector <CSound>::iterator it = sound.begin(); it != sound.end(); it++)
				it->free();
			
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
					it->pause();
			
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
						it->resume();
						ret = true;
					}
					else
						ret = it->play(channel, loops);
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

>>>>>>> 1fba5f672f27675ef61fc15b644b461379515813
#endif
