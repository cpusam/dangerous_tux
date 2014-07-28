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

#include <string>
#include <vector>

#include "../chora.hpp"
#include "../statemachine.hpp"

enum ESoundType
{
	UNDEF_SOUND=-1,
	CHUNK_SOUND,
	MUSIC_SOUND
};

enum ESoundState
{
	UNLOADED_SOUND, // som ainda não carregado
	INACTIVE_SOUND, // som carregado e pronto para tocar
	PAUSED_SOUND, // som pausado
	PLAYING_SOUND // tocando o som
};

class CSound: public CStateMachine
{
	protected:
		int type;
		int channel; // canal usado para tocar o som
		std::string id;
		std::string path;
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
};

class CSoundPlayer: public CStateMachine
{
	protected:
		static CSoundPlayer * singleton;
		std::vector <CSound> sound;

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

#endif
