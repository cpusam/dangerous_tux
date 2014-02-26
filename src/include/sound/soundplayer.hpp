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
		}
		
		~CSound (  )
		{
			if (chunk)
				Mix_FreeChunk(chunk);
			
			if (music)
				Mix_FreeMusic(music);
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
		
		void set_chunk ( string p )
		{
			Mix_Chunk * c = Mix_LoadWAV(p.c_str());
			if (c)
			{
				unsigned int f = p.find_last_of("/\\");
				path = p.substr(0, f);
				id = p.substr(f + 1);
				
				if (chunk)
					Mix_FreeChunk(chunk);
				
				if (music)
				{
					Mix_FreeMusic(music);
					music = 0;
				}
				
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
				
				if (music)
					Mix_FreeMusic(music);
				
				if (chunk)
				{
					Mix_FreeChunk(chunk);
					chunk = 0;
				}
				
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
		}
		
		static CSoundPlayer * instance (  )
		{
			if (!singleton)
				singleton = new CSoundPlayer();
			
			return singleton;
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
		
		bool play_sound ( string id, int channel=-1, int loops=0 )
		{
			bool ret = false;
			for (vector <CSound>::iterator it = sound.begin(); it != sound.end(); it++)
				if (id == it->get_id())
				{
					switch (it->get_type())
					{
						case CHUNK_SOUND:
							Mix_PlayChannel(channel, it->get_chunk(), loops);
							ret = true;
							break;
						
						case MUSIC_SOUND:
							Mix_PlayMusic(it->get_music(), loops);
							ret = true;
							break;
					}
					
					if (ret)
						return true;
				}

			return false;
		}
		
		int update (  )
		{
			switch (get_state())
			{
				case 0:
					break;
				
				case 1:
					break;
				
				default:
					set_state(0);
					break;
			}
			
			return get_state();
		}
};

CSoundPlayer * CSoundPlayer::singleton = 0;

#endif

