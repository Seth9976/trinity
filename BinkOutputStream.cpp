#include "StdAfx.h"

#if BINK_ENABLED

#include "BinkOutputStream.h"

#if defined(__RAD32__)
#define PTR4
#else
#define PTR4 __far
#endif


unsigned char g_tempChannel = 0;
unsigned int g_maxAudioSamples = 16384;
typedef unsigned int (__stdcall * SETAUDIOSTREAMCALL) (float* const data, unsigned int const dataSize, int const input);
SETAUDIOSTREAMCALL g_directSoundMethod = NULL;
U8* g_soundBuffer = NULL;

typedef struct BinkAudioBuf
{
	float* g_audioBuffer;
	float vol;
	float destVol;
	unsigned short channel;
} BinkAudioBuf;

std::map <BINKSND PTR4*, BinkAudioBuf> g_binkSoundToBuffer;

S32 RADLINK Ready( struct BINKSND PTR4* bs )
{	
	return 1;
}

// this function is only called if ready has returned a non-zero value
S32 RADLINK Lock( struct BINKSND PTR4* bs, U8 PTR4* PTR4* addr, U32 PTR4* len )
{
  *addr = g_soundBuffer;
  *len = g_maxAudioSamples*sizeof(S16); // 16-bit
  //printf("Lock\n");
  return 1;
}

// called when the buffer has been filled
S32  RADLINK Unlock (struct BINKSND PTR4* bs,U32 filled)
{
	S16* curPos = (S16*) g_soundBuffer;
	S16* endPos = curPos + ( filled/sizeof(S16) );
	float* curFloatPos = g_binkSoundToBuffer[bs].g_audioBuffer;
	float* endFloatPos = curFloatPos + g_maxAudioSamples;
	
	float vol = g_binkSoundToBuffer[bs].vol;
	float volRamp = ( g_binkSoundToBuffer[bs].destVol - g_binkSoundToBuffer[bs].vol ) / filled;

	unsigned int numFrames = 0;
	while ( curPos < endPos && curFloatPos < endFloatPos )
	{
		vol += volRamp;
		*curFloatPos++ = ( (float) *curPos++ ) * vol / 0x8000;
		numFrames++;
	}

	g_binkSoundToBuffer[bs].vol = g_binkSoundToBuffer[bs].destVol;

	(*g_directSoundMethod) (g_binkSoundToBuffer[bs].g_audioBuffer, numFrames, g_binkSoundToBuffer[bs].channel );
	return 1;
}

// set the volume 0=quiet, 32767=max, 65536=double volume (not always supported)
void RADLINK Volume ( struct BINKSND PTR4* bs, S32 volume )
{
	g_binkSoundToBuffer[bs].destVol = ( (float) volume ) / 0x8000;
}

// set the pan 0=left, 32767=middle, 65536=right
void RADLINK Pan( struct BINKSND PTR4* bs, S32 pan )
{
	// Nothing ... panning is 3D
}

// called to turn the sound on or off
S32  RADLINK SetOnOff( struct BINKSND PTR4* bs, S32 status )
{
	//printf("SetOnOff\n");
	// Do nothing (sound will always run)

  if ( ( status == 1 ) && ( bs->OnOff == 0 ) )
  {
      bs->OnOff = 1;
  }
  else if ( ( status == 0 ) && ( bs->OnOff == 1 ) )
  {
    bs->OnOff = 0;
  }

  return( bs->OnOff );
}

// pauses/resumes the playback
S32  RADLINK Pause( struct BINKSND PTR4* bs, S32 status )  // 1=pause, 0=resume, -1=resume and clear
{
	// Do nothing for now
  if ( status )
  {
	//printf("Pause\n");
    return 1;
  }

  //printf("Unpause\n");
  return 0;
}

void RADLINK SpeakerVolumes( struct BINKSND PTR4* bs, F32 * volumes, U32 total )
{
	//printf("SpeakerVolumes\n");
	// Do nothing for now (controlled in WWise currently)
}

void RADLINK Close( struct BINKSND PTR4* bs )
{
	CCP_FREE(g_binkSoundToBuffer[bs].g_audioBuffer);
	g_binkSoundToBuffer.erase(bs);
}

void SetBinkOutputChannel( int chan )
{
	g_tempChannel = chan;
}

S32 RADLINK Open( struct BINKSND PTR4* bs, U32 freq, S32 bits, S32 chans, U32 flags, HBINK bink )
{
  if ( g_tempChannel < 0 || g_tempChannel > 7 )
	  return 0;

  memset( bs, 0, sizeof( *bs ) );

  bs->SoundDroppedOut = 0;

  bs->freq  = freq;
  bs->bits  = bits;
  bs->chans = chans;

  //printf("Freq: %d, Bits: %d, Chans: %d\n", freq, bits, chans);
  //bink->limit_speakers = 2; // Stereo, let's try mono after this


  bs->OnOff     = 1;

  bs->Ready    = Ready;
  bs->Lock     = Lock;
  bs->Unlock   = Unlock;
  bs->Volume   = Volume;
  bs->Pan      = Pan;
  bs->Pause    = Pause;
  bs->SetOnOff = SetOnOff;
  bs->Close    = Close;
  bs->SpeakerVols  = SpeakerVolumes;


  bs->BestSizeIn16 = g_maxAudioSamples * bs->chans;
  bs->Latency = 0;

  BinkAudioBuf audioBuf;
  audioBuf.channel = g_tempChannel;

  audioBuf.g_audioBuffer = (float*)CCP_MALLOC( "g_audioBuffer", g_maxAudioSamples * sizeof(float));
  for (unsigned int i = 0; i < g_maxAudioSamples; i++) 
  {
    audioBuf.g_audioBuffer[i] = 0;
  }

  g_binkSoundToBuffer[bs] = audioBuf;
  g_binkSoundToBuffer[bs].vol = 1.0f;
  g_binkSoundToBuffer[bs].destVol = 1.0f;
  (*g_directSoundMethod) (g_binkSoundToBuffer[bs].g_audioBuffer, g_maxAudioSamples, g_binkSoundToBuffer[bs].channel);

  return (1);
}


BINKSNDOPEN RADEXPLINK BinkOpenCustomStream(UINTa param)
{
	if ( !param )
	{
		// Log error here, something fucked up
		return ( NULL );
	}
	
	g_directSoundMethod = (SETAUDIOSTREAMCALL) param;

	if ( !g_soundBuffer )	// Since this is used for all bink videos, how do we know when to release this?
		  g_soundBuffer = (U8*) CCP_MALLOC( "g_soundBuffer", sizeof(S16) * g_maxAudioSamples );

	return ( Open );
}

#endif
