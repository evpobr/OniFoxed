// ======================================================================
// BFW_SS2_Platform_Win32.c
// ======================================================================

// ======================================================================
// includes
// ======================================================================

#include "bfw_math.h"

#include "BFW.h"
#include "BFW_SoundSystem2.h"
#include "BFW_SS2_Private.h"
#include "BFW_SS2_Platform.h"
#include "BFW_SS2_IMA.h"
#include "BFW_WindowManager.h"

#include "BFW_Console.h"

#define INITGUID
#include <dsound.h>
#include <mmreg.h>
#include <msacm.h>

// ======================================================================
// defines
// ======================================================================
#define SScMaxSoundChannels				16

#define SScVolume_Scale					3

#define SScVolume_Max					DSBVOLUME_MAX
#define SScVolume_Min					(DSBVOLUME_MIN / SScVolume_Scale)
#define SScPan_Left						(DSBPAN_LEFT / SScVolume_Scale)
#define SScPan_Center					DSBPAN_CENTER
#define SScPan_Right					(DSBPAN_RIGHT / SScVolume_Scale)

// at 2.816k bytes per packet to decompress
#define SScNumPacketsToDecompress		500

#define SScZeroSound					(0.050f)

// ======================================================================
// globals
// ======================================================================
static LPDIRECTSOUND			SSgDirectSound;
static LPDIRECTSOUNDBUFFER		SSgPrimaryBuffer;
static LPDIRECTSOUND3DLISTENER	SSgDS3DListener;
//static LPDIRECTSOUND3DBUFFER	SSgReverbBuffer;
//static LPKSPROPERTYSET			SSgReverbPropertySet;
static UUtUns32					SSgNumChannels;

static DSCAPS					SSgDSCaps;

static D3DVALUE					SSgDistanceFactor;
static D3DVALUE					SSgRolloffFactor;

static HANDLE					SSgEventThread;
static UUtUns32					SSgEventThreadID;

static UUtMemory_Array			*SSgSoundChannelList;
static UUtBool					SSgUpdate_Run;

#if defined(SScMSADPCMTimer) && (SScMSADPCMTimer == 1)
static UUtUns32					t1;
static UUtUns32					t2;

static UUtUns32					SSgMSADPCMTimer_Max = UUcMinUns32;
static UUtUns32					SSgMSADPCMTimer_Min = UUcMaxUns32;

static UUtUns32					SSgMSADPCMTimer_NumDecompress_Frame;
static UUtUns32					SSgMSADPCMTimer_NumDecompress_Total;
static UUtUns32					SSgMSADPCMTimer_DecompressTime_Frame;
static UUtUns32					SSgMSADPCMTimer_DecompressTime_Total;
static UUtUns32					decomp_count;
static UUtUns32					decomp_time;

#endif

// ======================================================================
// prototypes
// ======================================================================
static char*
SSiP_DS_GetErrorMsg(
	HRESULT					inResult);

// ======================================================================
// functions
// ======================================================================
// ----------------------------------------------------------------------
static UUtError
SSiSoundChannelList_Add(
	SStSoundChannel				*inSoundChannel)
{
	UUtError					error;
	UUtUns32					index;
	SStSoundChannel				**sound_channel_array;
	
	error = UUrMemory_Array_GetNewElement(SSgSoundChannelList, &index, NULL);
	UUmError_ReturnOnError(error);
	
	sound_channel_array = (SStSoundChannel**)UUrMemory_Array_GetMemory(SSgSoundChannelList);
	UUmAssert(sound_channel_array);
	
	sound_channel_array[index] = inSoundChannel;
	
	return UUcError_None;
}

// ----------------------------------------------------------------------
static void
SSiSoundChannelList_Delete(
	SStSoundChannel				*inSoundChannel)
{
	SStSoundChannel				**sound_channel_array;
	UUtUns32					num_elements;
	UUtUns32					i;
	
	sound_channel_array = (SStSoundChannel**)UUrMemory_Array_GetMemory(SSgSoundChannelList);
	num_elements = UUrMemory_Array_GetUsedElems(SSgSoundChannelList);
	for (i = 0; i < num_elements; i++)
	{
		if (sound_channel_array[i] == inSoundChannel)
		{
//			SSrDeleteGuard(&inSoundChannel->guard);
			UUrMemory_Array_DeleteElement(SSgSoundChannelList, i);
			break;
		}
	}
}

// ----------------------------------------------------------------------
static UUtError
SSiSoundChannelList_Initialize(
	void)
{
	SSgSoundChannelList = 
		UUrMemory_Array_New(
			sizeof(SStSoundChannel*),
			5,
			0,
			SScMaxSoundChannels);
	UUmError_ReturnOnNull(SSgSoundChannelList);
	
	return UUcError_None;
}

// ----------------------------------------------------------------------
static void
SSiSoundChannelList_Terminate(
	void)
{
	if (SSgSoundChannelList != NULL) {
		UUrMemory_Array_Delete(SSgSoundChannelList);
		SSgSoundChannelList = NULL;
	}
}

// ======================================================================
#if 0
#pragma mark -
#endif
// ======================================================================
// ----------------------------------------------------------------------
static UUtBool
SSiPlatform_SoundChannel_MSADPCM_Initialize(
	SStSoundChannel				*inSoundChannel)
{
	MMRESULT					result;
	UUtBool						return_val;
	WAVEFORMATEX				*wave_format_ex = (WAVEFORMATEX *) (SSrSound_IsStereo(inSoundChannel->sound_data) ? &SSgWaveFormat_Stereo : &SSgWaveFormat_Mono);

#if defined(SScMSADPCMTimer) && (SScMSADPCMTimer == 1)
	t1 = timeGetTime();
#endif
	
	// determine the best format for the destination
	UUrMemory_Clear(&inSoundChannel->pd.DstHeader, sizeof(WAVEFORMATEX));
	inSoundChannel->pd.DstHeader.wFormatTag = WAVE_FORMAT_PCM;

	
	// get the format for the PCM header
	result =
		acmFormatSuggest(
			NULL,
			wave_format_ex,
			&inSoundChannel->pd.DstHeader,
			sizeof(WAVEFORMATEX),
			ACM_FORMATSUGGESTF_WFORMATTAG);
	if (result != 0)
	{
		return_val = UUcFalse;
		goto end;
	}
	
	if (inSoundChannel->pd.acm != NULL)
	{
		acmStreamClose(inSoundChannel->pd.acm, 0);
		inSoundChannel->pd.acm = NULL;
	}
	
	// open the stream
	result =
		acmStreamOpen(
			&inSoundChannel->pd.acm,
			NULL,
			wave_format_ex,
			&inSoundChannel->pd.DstHeader,
			NULL,
			0,
			0,
			ACM_STREAMOPENF_NONREALTIME);
	if (result != 0)
	{
		return_val = UUcFalse;
		goto end;
	}
	if (inSoundChannel->pd.acm == NULL)
	{
		return_val = UUcFalse;
		goto end;
	}

	// get the read size
	result =
		acmStreamSize(
			inSoundChannel->pd.acm,
			inSoundChannel->pd.decompressed_data_length,
			&inSoundChannel->pd.DefaultReadSize,
			ACM_STREAMSIZEF_DESTINATION);
	if (result != 0)
	{
		return_val = UUcFalse;
		goto end;
	}
	
#if defined(SScMSADPCMTimer) && (SScMSADPCMTimer == 1)
	t2 = timeGetTime();
	
	SSgMSADPCMTimer_Max = UUmMax(SSgMSADPCMTimer_Max, (t2 - t1));
	SSgMSADPCMTimer_Min = UUmMin(SSgMSADPCMTimer_Min, (t2 - t1));
	
	decomp_time += (t2 - t1);
#endif
	
	return_val = UUcTrue;
	
end:
	return return_val;
}

// ----------------------------------------------------------------------
static void
SSiPlatform_SoundChannel_MSADPCM_Terminate(
	SStSoundChannel				*inSoundChannel)
{
	HRESULT						result;
	
#if defined(SScMSADPCMTimer) && (SScMSADPCMTimer == 1)
	t1 = timeGetTime();;
#endif

	result =
		acmStreamClose(
			inSoundChannel->pd.acm,
			0);
	inSoundChannel->pd.acm = NULL;

#if defined(SScMSADPCMTimer) && (SScMSADPCMTimer == 1)
	t2 = timeGetTime();
	
	SSgMSADPCMTimer_Max = UUmMax(SSgMSADPCMTimer_Max, (t2 - t1));
	SSgMSADPCMTimer_Min = UUmMin(SSgMSADPCMTimer_Min, (t2 - t1));
	
	decomp_time += (t2 - t1);
#endif
}

// ======================================================================
#if 0
#pragma mark -
#endif
// ======================================================================
// ----------------------------------------------------------------------
static UUtBool
SSiPlatform_SoundChannel_MSADPCM_DecompressData(
	SStSoundChannel				*inSoundChannel)
{
	HRESULT						result;
	UUtUns8						*src;
	UUtUns32					src_size;
	UUtUns32					write_size;
	UUtBool						return_val;
	
#if defined(SScMSADPCMTimer) && (SScMSADPCMTimer == 1)
	t1 = timeGetTime();
#endif

	if (inSoundChannel->pd.acm == NULL)
	{
		return_val = UUcFalse;
		goto end;
	}

	if (inSoundChannel->pd.num_packets_decompressed >= inSoundChannel->sound_data->num_bytes)
	{
		return_val = UUcFalse;
		goto end;
	}
	
	src =
		((UUtUns8*)inSoundChannel->sound_data->data) +
		inSoundChannel->pd.num_packets_decompressed;
		
	src_size = 
		UUmMin(
			inSoundChannel->pd.DefaultReadSize,
			(inSoundChannel->sound_data->num_bytes - inSoundChannel->pd.num_packets_decompressed));
		
	// determine the write size
	result =
		acmStreamSize(
			inSoundChannel->pd.acm,
			src_size,
			&write_size,
			ACM_STREAMSIZEF_SOURCE);
	if (result != 0)
	{
		return_val = UUcFalse;
		goto end;
	}
	
	UUmAssert(write_size <= inSoundChannel->pd.decompressed_data_length);
	
	// prepare the stream
	ZeroMemory(&inSoundChannel->pd.stream, sizeof(ACMSTREAMHEADER));
	inSoundChannel->pd.stream.cbStruct = sizeof(ACMSTREAMHEADER);
	inSoundChannel->pd.stream.pbSrc = src;
	inSoundChannel->pd.stream.cbSrcLength = src_size;
	inSoundChannel->pd.stream.pbDst = inSoundChannel->pd.decompressed_data;
	inSoundChannel->pd.stream.cbDstLength = write_size;
	
	result =
		acmStreamPrepareHeader(
			inSoundChannel->pd.acm,
			&inSoundChannel->pd.stream,
			0);
	if (result != 0)
	{
		return_val = UUcFalse;
		goto end;
	}
		
	// decompress sound data into the buffer
	result =
		acmStreamConvert(
			inSoundChannel->pd.acm,
			&inSoundChannel->pd.stream,
			0);
	if (result != 0)
	{
		return_val = UUcFalse;
		goto end;
	}
	
	#if defined(DEBUGGING) && (DEBUGGING != 0)
	if (inSoundChannel->pd.stream.cbDstLengthUsed == 0)
	{
		UUrPrintWarning(
			"Error: no data was actually decompressed from %s",
			SSrSoundData_GetName(inSoundChannel->sound_data));
	}
	#endif

	inSoundChannel->pd.decompressed_packets_length = inSoundChannel->pd.stream.cbDstLengthUsed;
	inSoundChannel->pd.num_packets_decompressed += inSoundChannel->pd.stream.cbSrcLengthUsed;
	inSoundChannel->pd.bytes_read = 0;
	
	result =
		acmStreamUnprepareHeader(
			inSoundChannel->pd.acm,
			&inSoundChannel->pd.stream,
			0);
	

#if defined(SScMSADPCMTimer) && (SScMSADPCMTimer == 1)
	t2 = timeGetTime();
	
	SSgMSADPCMTimer_Max = UUmMax(SSgMSADPCMTimer_Max, (t2 - t1));
	SSgMSADPCMTimer_Min = UUmMin(SSgMSADPCMTimer_Min, (t2 - t1));
	
	decomp_time += (t2 - t1);
	decomp_count++;
#endif
	
	return_val = UUcTrue;
	
end:
	return return_val;
}

// ----------------------------------------------------------------------
static UUtBool
SSiPlatform_SoundChannel_IMA_DecompressData(
	SStSoundChannel				*inSoundChannel)
{
	UUtUns32					number_of_packets_decompressed;
	UUtUns32					num_channels = SSrSound_IsStereo(inSoundChannel->sound_data) ? 2 : 1;
	WAVEFORMATEX				*wave_format_ex = (WAVEFORMATEX *) (SSrSound_IsStereo(inSoundChannel->sound_data) ? &SSgWaveFormat_Stereo : &SSgWaveFormat_Mono);
	
	if (inSoundChannel->pd.num_packets_decompressed >= wave_format_ex->nBlockAlign)
	{
		return UUcFalse;
	}
	
	// decompress sound data into the buffer
	number_of_packets_decompressed =
		SSrIMA_DecompressSoundData(
			inSoundChannel->sound_data,
			inSoundChannel->pd.decompressed_data,
			SScNumPacketsToDecompress,
			inSoundChannel->pd.num_packets_decompressed);
	
	inSoundChannel->pd.decompressed_packets_length =
		number_of_packets_decompressed *
		sizeof(UUtUns16) *
		SScIMA_SamplesPerPacket *
		num_channels;
	inSoundChannel->pd.num_packets_decompressed += number_of_packets_decompressed;
	inSoundChannel->pd.bytes_read = 0;

	return UUcTrue;
}

// ----------------------------------------------------------------------
static void
SSiPlatform_SoundChannel_Terminate(
	SStSoundChannel				*inSoundChannel,
	char						*inErrorString)
{
	if (inErrorString)
	{
		UUrError_Report(UUcError_Generic, inErrorString);
	}
	
	if (inSoundChannel->pd.decompressed_data != NULL)
	{
		UUrMemory_Block_Delete(inSoundChannel->pd.decompressed_data);
		inSoundChannel->pd.decompressed_data = NULL;
	}
	
	if (inSoundChannel->pd.soundBuffer)
	{
		Mix_FreeChunk(inSoundChannel->pd.soundBuffer);
		inSoundChannel->pd.soundBuffer = NULL;
	}
	
	SSiSoundChannelList_Delete(inSoundChannel);
}

// ----------------------------------------------------------------------
#if 0
static UUtBool
SSiPlatform_UpdateSoundBuffer_IMA(
	SStSoundChannel				*inSoundChannel)
{
	HRESULT						result;
	UUtUns32					section;
	UUtUns32					write_size;
	void						*audioData;
	UUtUns32					audioDataSize;
	void						*audioData2;
	UUtUns32					audioDataSize2;
	UUtUns8						*src;
	UUtUns8						*dest;
	UUtInt32					bytes_remaining;
	UUtBool						data_decompressed;
	
	// determine which half of the buffer to fill
	section = inSoundChannel->pd.section;
	
	// lock the buffer
	result = 
		IDirectSoundBuffer_Lock(
			inSoundChannel->pd.soundBuffer,
			inSoundChannel->pd.buffer_pos[section],
			inSoundChannel->pd.section_size,
			&audioData,
			&audioDataSize,
			&audioData2,
			&audioDataSize2,
			0);
	if (result != DS_OK) { return UUcFalse; }
	
	bytes_remaining = 0;
	write_size = 0;
	dest = (UUtUns8*)audioData;
	
	do
	{
		if ((inSoundChannel->pd.bytes_read == 0) || 
			(inSoundChannel->pd.bytes_read == inSoundChannel->pd.decompressed_packets_length))
		{
			// decompress some more sound data
			data_decompressed = SSiPlatform_SoundChannel_IMA_DecompressData(inSoundChannel);
			if (data_decompressed == UUcFalse)
			{
				bytes_remaining = (inSoundChannel->pd.section_size - inSoundChannel->pd.bytes_written);
				break;
			}
		}
		
		// determine how many bytes to write
		write_size =
			UUmMin(
				(inSoundChannel->pd.section_size - inSoundChannel->pd.bytes_written),
				(inSoundChannel->pd.decompressed_packets_length - inSoundChannel->pd.bytes_read));
		if (write_size == 0) { break; }
		
		// copy data from the decompressed_data into the buffer
		src = inSoundChannel->pd.decompressed_data + inSoundChannel->pd.bytes_read;
		dest = ((UUtUns8*)audioData) + inSoundChannel->pd.bytes_written;
		UUrMemory_MoveFast(src, dest, write_size);
		
		// increment the number of sound bytes read and the number of buffer bytes written
		inSoundChannel->pd.bytes_read += write_size;
		inSoundChannel->pd.bytes_written += write_size;
		
		bytes_remaining = (inSoundChannel->pd.section_size - inSoundChannel->pd.bytes_written);
	}
	while (bytes_remaining > 0);

	if ((bytes_remaining > 0) && (SSiSoundChannel_IsLooping(inSoundChannel) == UUcFalse))
	{
		dest += write_size;
		UUrMemory_Set8(
			dest, 
			((inSoundChannel->sound_data->f.wBitsPerSample == 8) ? 0x80 : 0),
			bytes_remaining);
		
		if (SSiSoundChannel_IsAmbient(inSoundChannel) == UUcFalse)
		{
			bytes_remaining = 0;
		}
		
		inSoundChannel->pd.stop = UUcTrue;
		inSoundChannel->pd.stop_section = section;
		inSoundChannel->pd.stop_pos =
			inSoundChannel->pd.bytes_written +
			(section * inSoundChannel->pd.section_size);
	}
	
	// unlock the buffer
	result = 
		IDirectSoundBuffer_Unlock(
			inSoundChannel->pd.soundBuffer,
			audioData,
			audioDataSize,
			audioData2,
			audioDataSize2);
	if (result != DS_OK) { return UUcFalse; }
	
	// if this is a looping sound, fill in the remaining bytes with another permutation
	if (bytes_remaining > 0)
	{
		if (SSiSoundChannel_IsLooping(inSoundChannel) == UUcTrue)
		{
			SSrGroup_Play(inSoundChannel->group, inSoundChannel, "sound channel", NULL);
		}
		else if (SSiSoundChannel_IsAmbient(inSoundChannel) == UUcTrue)
		{
			// update the sound channel
			SSiSoundChannel_SetUpdating(inSoundChannel, UUcTrue);
			SSiPlayingAmbient_UpdateSoundChannel(inSoundChannel);
			SSiSoundChannel_SetUpdating(inSoundChannel, UUcFalse);
		}
	}
	
	return UUcTrue;
}
#endif

// ----------------------------------------------------------------------
#if 0
static UUtBool
SSiPlatform_UpdateSoundBuffer_Uncompressed(
	SStSoundChannel				*inSoundChannel)
{
	HRESULT						result;
	UUtUns32					section;
	UUtUns32					write_size;
	void						*audioData;
	UUtUns32					audioDataSize;
	void						*audioData2;
	UUtUns32					audioDataSize2;
	UUtUns8						*src;
	UUtUns8						*dest;
	UUtInt32					bytes_remaining;
	
	// determine which half of the buffer to fill
	section = inSoundChannel->pd.section;
	
	// determine how many bytes to write
	write_size =
		UUmMin(
			(inSoundChannel->pd.section_size - inSoundChannel->pd.bytes_written),
			(inSoundChannel->sound_data->num_bytes - inSoundChannel->pd.bytes_read));
	if (write_size == 0)
	{
		inSoundChannel->pd.stop = UUcTrue;
		return UUcFalse;
	}
	
	// set the source pointer
	src = ((UUtUns8*)inSoundChannel->sound_data->data) + inSoundChannel->pd.bytes_read;

	// lock the buffer
	result = 
		IDirectSoundBuffer_Lock(
			inSoundChannel->pd.soundBuffer,
			inSoundChannel->pd.buffer_pos[section],
			inSoundChannel->pd.section_size,
			&audioData,
			&audioDataSize,
			&audioData2,
			&audioDataSize2,
			0);
	if (result != DS_OK) { return UUcFalse; }
	
	// copy data from the sound data into the buffer
	dest = ((UUtUns8*)audioData) + inSoundChannel->pd.bytes_written;
	UUrMemory_MoveFast(src, dest, write_size);
	
	// increment the number of sound bytes read and the number of buffer bytes written
	inSoundChannel->pd.bytes_read += write_size;
	inSoundChannel->pd.bytes_written += write_size;
	
	// fill the remaining bytes with silence if this is not a looping channel
	bytes_remaining = (inSoundChannel->pd.section_size - inSoundChannel->pd.bytes_written);
	if ((bytes_remaining > 0) && (SSiSoundChannel_IsLooping(inSoundChannel) == UUcFalse))
	{
		dest += write_size;
		UUrMemory_Set8(
			dest,
			((inSoundChannel->sound_data->f.wBitsPerSample == 8) ? 0x80 : 0),
			bytes_remaining);
		
		if (SSiSoundChannel_IsAmbient(inSoundChannel) == UUcFalse)
		{
			bytes_remaining = 0;
		}
		
		inSoundChannel->pd.stop = UUcTrue;
		inSoundChannel->pd.stop_section = section;
		inSoundChannel->pd.stop_pos =
			inSoundChannel->pd.bytes_written +
			(section * inSoundChannel->pd.section_size);
	}
	
	// unlock the buffer
	result = 
		IDirectSoundBuffer_Unlock(
			inSoundChannel->pd.soundBuffer,
			audioData,
			audioDataSize,
			audioData2,
			audioDataSize2);
	if (result != DS_OK) { return UUcFalse; }
	
	// if this is a looping sound, fill in the remaining bytes with another permutation
	if (bytes_remaining > 0)
	{
		if (SSiSoundChannel_IsLooping(inSoundChannel) == UUcTrue)
		{
			SSrGroup_Play(inSoundChannel->group, inSoundChannel, "sound channel", NULL);
		}
		else
		{
			// if the channel isn't updated then make it stop
			SSiSoundChannel_SetUpdating(inSoundChannel, UUcTrue);
			SSiPlayingAmbient_UpdateSoundChannel(inSoundChannel);
			SSiSoundChannel_SetUpdating(inSoundChannel, UUcFalse);
		}
	}
	
	return UUcTrue;
}
#endif

// ----------------------------------------------------------------------
static UUtBool
SSiPlatform_UpdateSoundBuffer(
	SStSoundChannel				*inSoundChannel)
{
	return UUcTrue;
}

// ----------------------------------------------------------------------
static UUcInline UUtUns32
SSiPlatform_GetPlaySection(
	SStSoundChannel					*inSoundChannel,
	UUtUns32						inPlayPos)
{
	UUtUns32						section;
	
	section = inPlayPos / inSoundChannel->pd.section_size;
	
	return section;
}

// ----------------------------------------------------------------------
static void
SSiPlatform_SilenceSection(
	SStSoundChannel					*inSoundChannel,
	UUtUns32						inSection)
{
	return;
}

// ======================================================================
#if 0
#pragma mark -
#endif
// ======================================================================
// ----------------------------------------------------------------------
void
SS2rPlatform_GetDebugNeeds(
	UUtUns32					*outNumLines)
{
	*outNumLines = 3;
}

// ----------------------------------------------------------------------
void
SS2rPlatform_ShowDebugInfo_Overall(
	IMtPoint2D					*inDest)
{
#if defined(SScMSADPCMTimer) && (SScMSADPCMTimer == 1)
	char						string[128];
	float						avg;
	
	sprintf(
		string,
		"max %d min %d",
		SSgMSADPCMTimer_Max,
		SSgMSADPCMTimer_Min);
	
	DCrText_DrawText(string, NULL, inDest);
	inDest->y += DCrText_GetLineHeight();
	
	if (SSgMSADPCMTimer_NumDecompress_Frame != 0)
	{
		avg = (float)SSgMSADPCMTimer_DecompressTime_Frame / (float)SSgMSADPCMTimer_NumDecompress_Frame;
	}
	else
	{
		avg = 0.0f;
	}
	sprintf(
		string,
		"per frame: time = %d,  number = %d,  average = %5.5f",
		SSgMSADPCMTimer_DecompressTime_Frame,
		SSgMSADPCMTimer_NumDecompress_Frame,
		avg);
	
	DCrText_DrawText(string, NULL, inDest);
	inDest->y += DCrText_GetLineHeight();
	
	if (SSgMSADPCMTimer_NumDecompress_Total != 0)
	{
		avg = (float)SSgMSADPCMTimer_DecompressTime_Total / (float)SSgMSADPCMTimer_NumDecompress_Total;
	}
	else
	{
		avg = 0.0f;
	}
	sprintf(
		string,
		"total: time = %d,  number = %d,  average = %5.5f",
		SSgMSADPCMTimer_DecompressTime_Total,
		SSgMSADPCMTimer_NumDecompress_Total,
		avg);
	
	DCrText_DrawText(string, NULL, inDest);
	inDest->y += DCrText_GetLineHeight();

#else
	
	return;
	
#endif
}

// ----------------------------------------------------------------------
void
SS2rPlatform_PerformanceStartFrame(
	void)
{
#if defined(SScMSADPCMTimer) && (SScMSADPCMTimer == 1)
	decomp_count = 0;
	decomp_time = 0;
#endif
}

// ----------------------------------------------------------------------
void
SS2rPlatform_PerformanceEndFrame(
	void)
{
#if defined(SScMSADPCMTimer) && (SScMSADPCMTimer == 1)
	t1 = timeGetTime();
	
	SSgMSADPCMTimer_NumDecompress_Frame = decomp_count;
	SSgMSADPCMTimer_NumDecompress_Total += decomp_count;
	SSgMSADPCMTimer_DecompressTime_Frame = decomp_time;
	SSgMSADPCMTimer_DecompressTime_Total += decomp_time;
#endif
}

// ======================================================================
#if 0
#pragma mark -
#endif
// ======================================================================
// ----------------------------------------------------------------------
void
SS2rPlatform_SoundChannel_Initialize(
	SStSoundChannel				*inSoundChannel)
{
	WAVEFORMATEX				wave_format;
	DSBUFFERDESC				buffer_desc;
	UUtUns32					i;
	
	// initialize the sound channel
	inSoundChannel->pd.soundBuffer = NULL;
	inSoundChannel->pd.buffer_size = 0;
	inSoundChannel->pd.section_size = 0;
	inSoundChannel->pd.section = 0;
	inSoundChannel->pd.bytes_read = 0;
	inSoundChannel->pd.bytes_written = 0;
	inSoundChannel->pd.stop = UUcTrue;
	inSoundChannel->pd.decompressed_data = NULL;
	inSoundChannel->pd.decompressed_data_length = 0;
	inSoundChannel->pd.num_packets_decompressed = 0;
	inSoundChannel->pd.decompressed_packets_length = 0;
	inSoundChannel->pd.acm = NULL;
	
	for (i = 0; i < SScNotifiesPerChannel; i++)
	{
		inSoundChannel->pd.buffer_pos[i] = 0;
	}

	// create a sound buffer
	UUrMemory_Clear(&wave_format, sizeof(WAVEFORMATEX));
	wave_format.wFormatTag		= WAVE_FORMAT_PCM;
	wave_format.nChannels		= (inSoundChannel->flags & SScSoundChannelFlag_Mono) ? 1 : 2;
	wave_format.nSamplesPerSec	= 22050;
	wave_format.wBitsPerSample	= 16;
	wave_format.nBlockAlign		= wave_format.wBitsPerSample * wave_format.nChannels / 8;
	wave_format.nAvgBytesPerSec	= wave_format.nSamplesPerSec * wave_format.nBlockAlign;
	wave_format.cbSize			= 0;
	
	// set up the buffer description
	UUrMemory_Clear(&buffer_desc, sizeof(DSBUFFERDESC));
	buffer_desc.dwSize			= sizeof(DSBUFFERDESC);
	buffer_desc.dwFlags			=
		DSBCAPS_GETCURRENTPOSITION2 |
		DSBCAPS_CTRLPOSITIONNOTIFY |
		DSBCAPS_STATIC |
		DSBCAPS_CTRLVOLUME |
		DSBCAPS_CTRLPAN |
		DSBCAPS_CTRLFREQUENCY;
	buffer_desc.dwBufferBytes	= wave_format.nAvgBytesPerSec * 3;
	buffer_desc.lpwfxFormat		= &wave_format;
	
	// save the buffer size
	inSoundChannel->pd.buffer_size = buffer_desc.dwBufferBytes;
	inSoundChannel->pd.section_size = buffer_desc.dwBufferBytes / SScNotifiesPerChannel;
	
	// add the channel to the list
	
	// set the buffer_positions for the update to watch for
	for (i = 0; i < SScNotifiesPerChannel; i++)
	{
		inSoundChannel->pd.buffer_pos[i] = (inSoundChannel->pd.section_size * i);
	}
	
	// add the sound channel to the sound channel update list
	SSiSoundChannelList_Add(inSoundChannel);
	
	// allocate memory for SScNumPacketsToDecompress packet worth of decompressed data
	inSoundChannel->pd.decompressed_data_length =
		(SScIMA_SamplesPerPacket *
		sizeof(UUtUns16) *
		SScNumPacketsToDecompress *
		wave_format.nChannels);
	inSoundChannel->pd.decompressed_data = 
		UUrMemory_Block_New(inSoundChannel->pd.decompressed_data_length);
	inSoundChannel->pd.decompressed_packets_length = 0;
	inSoundChannel->pd.num_packets_decompressed = 0;
}

// ----------------------------------------------------------------------
void
SS2rPlatform_SoundChannel_Pause(
	SStSoundChannel				*inSoundChannel)
{
	UUmAssert(inSoundChannel);
	
	// make sure the channel is playing
	if (SSiSoundChannel_IsPlaying(inSoundChannel) == UUcFalse) { return; }
	
	// stop the channel buffer
	Mix_Pause(inSoundChannel->pd.soundChannel);
	
	// set the status field
	SSiSoundChannel_SetPaused(inSoundChannel, UUcTrue);
}

// ----------------------------------------------------------------------
void
SS2rPlatform_SoundChannel_Play(
	SStSoundChannel				*inSoundChannel)
{
	int						result;
	
	UUmAssert(inSoundChannel);
	UUmAssert(inSoundChannel->pd.soundBuffer);
	
	// if the channel is already playing then don't do anything
	if (SSiSoundChannel_IsPlaying(inSoundChannel) == UUcTrue) { return; }
	
	// play the sound
	result = Mix_PlayChannel(inSoundChannel->pd.soundChannel, inSoundChannel->pd.soundBuffer, -1);
	if (result == -1)
	{
		UUrError_Report(UUcError_Generic, Mix_GetError());
	}
	
	// set the status field
	SSiSoundChannel_SetPlaying(inSoundChannel, UUcTrue);
}

// ----------------------------------------------------------------------
void
SS2rPlatform_SoundChannel_Resume(
	SStSoundChannel				*inSoundChannel)
{
	int						result;
	
	UUmAssert(inSoundChannel);
	
	// make sure the channel is playing and paused
	if ((SSiSoundChannel_IsPlaying(inSoundChannel) == UUcFalse) ||
		(SSiSoundChannel_IsPaused(inSoundChannel) == UUcFalse))
	{
		return;
	}
	
	// stop the channel buffer
	result = Mix_PlayChannel(-1, inSoundChannel->pd.soundBuffer, -1);
	if (result == -1)
	{
		UUrError_Report(UUcError_Generic, Mix_GetError());
	}
	
	// set the status field
	SSiSoundChannel_SetPaused(inSoundChannel, UUcFalse);
}

// ----------------------------------------------------------------------
UUtBool
SS2rPlatform_SoundChannel_SetSoundData(
	SStSoundChannel				*inSoundChannel,
	SStSoundData				*inSoundData)
{
	UUtBool						result = UUcTrue;
	
	UUmAssert(inSoundChannel);
	UUmAssert(inSoundChannel->pd.soundBuffer);
	UUmAssert(inSoundData);	
		
//	SSrWaitGuard(&inSoundChannel->guard);

	// set the sound data and number of bytes read
// 	inSoundChannel->sound_data = inSoundData;
// 	inSoundChannel->pd.bytes_read = 0;

// 	inSoundChannel->pd.stop = UUcFalse;
// 	inSoundChannel->pd.can_stop = UUcFalse;
// 	inSoundChannel->pd.stop_pos = 0;
		
// 	// decompress data from the sound data into the buffer
// 	if ((inSoundChannel->sound_data->flags & SScSoundDataFlag_Compressed) != 0)
// 	{
// 		inSoundChannel->pd.num_packets_decompressed = 0;
		
// 		if (inSoundChannel->pd.acm != NULL)
// 		{
// 			SSiPlatform_SoundChannel_MSADPCM_Terminate(inSoundChannel);
// 		}
		
// 		result = SSiPlatform_SoundChannel_MSADPCM_Initialize(inSoundChannel);
// 		if (result == UUcFalse)
// 		{
// //			SSrReleaseGuard(&inSoundChannel->guard);
// 			SSrReleaseGuard(SSgGuardAll);
// 			return result;
// 		}
// 	}
	
// 	// set the section if the sound is not playing
// 	if ((SSiSoundChannel_IsPlaying(inSoundChannel) == UUcFalse) &&
// 		(SSiSoundChannel_IsUpdating(inSoundChannel) == UUcFalse))
// 	{
// 		// set the section to 0 so that section zero gets filled
// 		inSoundChannel->pd.section = 0;
		
// 		// set the position
// 		IDirectSoundBuffer_SetCurrentPosition(inSoundChannel->pd.soundBuffer, 0);
// 	}
	
// 	// fill in the section
// 	result = SSiPlatform_UpdateSoundBuffer(inSoundChannel);
	
// //	SSrReleaseGuard(&inSoundChannel->guard);

	return result;
}

// ----------------------------------------------------------------------
void
SS2rPlatform_SoundChannel_SetPan(
	SStSoundChannel				*inSoundChannel,
	UUtUns32					inPanFlags,
	float						inPan)
{
	UUtInt32					pan;
	UUtUns8						left = 0, right = 0;
	
	// calculate the pan
	switch (inPanFlags)
	{
		case SScPanFlag_Left:
			if (inPan > SScZeroSound)
			{
				pan = SScPan_Left - (UUtInt32)((float)SScPan_Left * inPan);
			}
			else
			{
				pan = DSBPAN_LEFT;
			}
		break;
		
		case SScPanFlag_Right:
			if (inPan > SScZeroSound)
			{
				pan = SScPan_Right - (UUtInt32)((float)SScPan_Right * inPan);
			}
			else
			{
				pan = DSBPAN_RIGHT;
			}
		break;
		
		case SScPanFlag_None:
		default:
			pan = DSBPAN_CENTER;
		break;
	}
	
	// set the pan
	Mix_SetPanning(inSoundChannel->pd.soundChannel, 255, 255);
}

// ----------------------------------------------------------------------
void
SS2rPlatform_SoundChannel_SetPitch(
	SStSoundChannel				*inSoundChannel,
	float						inPitch)
{
	UUtInt32					frequency;
	
	// calculate the frequency
	frequency = (UUtInt32)(22050.0f * inPitch);
	
	// set the frequency
	
	// TODO
}

// ----------------------------------------------------------------------
void
SS2rPlatform_SoundChannel_SetVolume(
	SStSoundChannel				*inSoundChannel,
	float						inVolume)
{
	UUtInt32					volume;
	
	// calculate the volume
	if (inVolume > SScZeroSound)
	{
		volume = (UUtInt32)fabs((float)SScVolume_Min * inVolume) + SScVolume_Min;
	}
	else
	{
		volume = DSBVOLUME_MIN;
	}
	
	// set the volume
	Mix_VolumeChunk(inSoundChannel->pd.soundBuffer, volume);
}

// ----------------------------------------------------------------------
void
SS2rPlatform_SoundChannel_Silence(
	SStSoundChannel				*inSoundChannel)
{
	return;
}

// ----------------------------------------------------------------------
void
SS2rPlatform_SoundChannel_Stop(
	SStSoundChannel				*inSoundChannel)
{
	// set the status field
	SSiSoundChannel_SetPlaying(inSoundChannel, UUcFalse);
	
	// clear some of the platform data
	inSoundChannel->pd.bytes_read = 0;
	inSoundChannel->pd.bytes_written = 0;
	inSoundChannel->pd.section = 0;
	
	// stop the buffer
	Mix_Pause(inSoundChannel->pd.soundChannel);
}

// ----------------------------------------------------------------------
void
SS2rPlatform_SoundChannel_Terminate(
	SStSoundChannel				*inSoundChannel)
{
	SSiPlatform_SoundChannel_Terminate(inSoundChannel, NULL);
}

// ======================================================================
#if 0
#pragma mark -
#endif
// ======================================================================
// ----------------------------------------------------------------------
LPDIRECTSOUND
SS2rPlatform_GetDirectSound(
	void)
{
	return SSgDirectSound;
}

// ----------------------------------------------------------------------
UUtError
SS2rPlatform_Initialize(
	UUtWindow					inWindow,			/* only used by Win32 */
	UUtUns32					*outNumChannels,
	UUtBool						inUseSound)
{
	WAVEFORMATEX				wave_format;
	UUtError					error;
	
	*outNumChannels = 0;
	
	if (!inUseSound) {
		return UUcError_None;
	}

	UUrStartupMessage("Mix_OpenAudioDevice");
	if (Mix_OpenAudioDevice(22050, AUDIO_S16SYS, 2, 2048, NULL, 0) < 0) {
		UUrStartupMessage("Mix_OpenAudioDevice() failed : %s", Mix_GetError());
		return UUcError_Generic;
	}
	
	SSgNumChannels = SScMaxSoundChannels;
	
	Mix_AllocateChannels(SSgNumChannels);
	
    
	// set up the wave format structure
	UUrMemory_Clear(&wave_format, sizeof(WAVEFORMATEX));
	wave_format.wFormatTag		= WAVE_FORMAT_PCM;
	wave_format.nChannels		= 2;
	wave_format.nSamplesPerSec	= 22050;
	wave_format.wBitsPerSample	= 16;
	wave_format.nBlockAlign		= 
		wave_format.wBitsPerSample * wave_format.nChannels / 8;
	wave_format.nAvgBytesPerSec	=
		wave_format.nSamplesPerSec * wave_format.nBlockAlign;
	wave_format.cbSize			= 0;
	
	// ------------------------------
	// Intitialize the buffer update
	// ------------------------------
	// initialize the sound channel list
	error = SSiSoundChannelList_Initialize();
	if (error != UUcError_None)
	{
		goto cleanup;
	}
		
	*outNumChannels = SSgNumChannels;
	
	return UUcError_None;

cleanup:
	SS2rPlatform_Terminate();
	
	return UUcError_Generic;
}

// ----------------------------------------------------------------------
UUtError
SS2rPlatform_InitializeThread(
	void)
{
	return UUcError_None;
}

// ----------------------------------------------------------------------
void
SS2rPlatform_TerminateThread(
	void)
{
	return;
}

// ----------------------------------------------------------------------
void
SS2rPlatform_Terminate(
	void)
{
	
	SS2rPlatform_TerminateThread();
	
	SSiSoundChannelList_Terminate();
	
	Mix_CloseAudio();
	
	SSgNumChannels = 0;
}

// ======================================================================
#if 0
#pragma mark -
#endif
// ======================================================================
// ----------------------------------------------------------------------
void
SSrDeleteGuard(
	SStGuard					*inGuard)
{
	DeleteCriticalSection(inGuard);
	
	UUrMemory_Block_Delete(inGuard);
}

// ----------------------------------------------------------------------
void
SSrCreateGuard(
	SStGuard					**inGuard)
{
	SStGuard					*guard;
	
	guard = UUrMemory_Block_New(sizeof(CRITICAL_SECTION));
	InitializeCriticalSection(guard);
	
	*inGuard = guard;
}

// ----------------------------------------------------------------------
void
SSrReleaseGuard(
	SStGuard					*inGuard)
{
	LeaveCriticalSection(inGuard);
}

// ----------------------------------------------------------------------
void
SSrWaitGuard(
	SStGuard					*inGuard)
{
	EnterCriticalSection(inGuard);
}

// ======================================================================
#if 0
#pragma mark -
#endif
// ======================================================================
// ----------------------------------------------------------------------
static char*
SSiP_DS_GetErrorMsg(
	HRESULT				inResult)
{
	char				*errorMsg;
	
	switch (inResult)
	{
		case DSERR_ALLOCATED:
			errorMsg = "The call failed because resources (such as a priority level) were already being used by another caller.";
			break;
			
		case DSERR_CONTROLUNAVAIL:
			errorMsg = "The control (vol,pan,etc.) requested by the caller is not available.";
			break;
			
		case DSERR_INVALIDPARAM:
			errorMsg = "An invalid parameter was passed to the returning function.";
			break;
			
		case DSERR_INVALIDCALL:
			errorMsg = "This call is not valid for the current state of this object.";
			break;
			
		case DSERR_GENERIC:
			errorMsg = "An undetermined error occured inside the DirectSound subsystem.";
			break;
			
		case DSERR_PRIOLEVELNEEDED:
			errorMsg = "The caller does not have the priority level required for the function to succeed.";
			break;
			
		case DSERR_OUTOFMEMORY:
			errorMsg = "Not enough free memory is available to complete the operation.";
			break;
			
		case DSERR_BADFORMAT:
			errorMsg = "The specified WAVE format is not supported.";
			break;
			
		case DSERR_UNSUPPORTED:
			errorMsg = "The function called is not supported at this time.";
			break;
			
		case DSERR_NODRIVER:
			errorMsg = "No sound driver is available for use.";
			break;
			
		case DSERR_ALREADYINITIALIZED:
			errorMsg = "This object is already initialized.";
			break;
			
		case DSERR_NOAGGREGATION:
			errorMsg = "This object does not support aggregation.";
			break;
			
		case DSERR_BUFFERLOST:
			errorMsg = "The buffer memory has been lost, and must be restored.";
			break;
			
		case DSERR_OTHERAPPHASPRIO:
			errorMsg = "Another app has a higher priority level, preventing this call from succeeding.";
			break;
			
		case DSERR_UNINITIALIZED:
			errorMsg = "This object has not been initialized.";
			break;
			
		case DSERR_NOINTERFACE:
			errorMsg = "The requested COM interface is not available.";
			break;
			
		default:
			errorMsg = "Unknown error.";
			break;
	}
	
	return errorMsg;
}

