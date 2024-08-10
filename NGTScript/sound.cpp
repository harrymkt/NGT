//NGTAUDIO
#define NOMINMAX
#include "MemoryStream.h"
#define CURL_STATICLIB
#include "curl/curl.h"
#include "ngt.h"
#include "scriptarray/scriptarray.h"
#include "sound.h"
#include <numeric>
#include <thread>
using namespace std;
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <stdint.h> /* Required for uint32_t which is used by STEAMAUDIO_VERSION. That dependency needs to be removed from Steam Audio - use IPLuint32 or "unsigned int" instead! */
#include<algorithm>
#include "phonon.h" /* Steam Audio */
#include "pack.h"
#define FORMAT      ma_format_f32   /* Must be floating point. */
#define CHANNELS    2               /* Must be stereo for this example. */
#define SAMPLE_RATE 44100
#include "fx/freeverb.h"
#define VERBLIB_IMPLEMENTATION

#include "fx/verblib.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*
	The reverb node has one input and one output.
	*/
	typedef struct
	{
		ma_node_config nodeConfig;
		ma_uint32 channels;         /* The number of channels of the source, which will be the same as the output. Must be 1 or 2. */
		ma_uint32 sampleRate;
		float roomSize;
		float damping;
		float width;
		float wetVolume;
		float dryVolume;
		float mode;
	} ma_reverb_node_config;

	MA_API ma_reverb_node_config ma_reverb_node_config_init(ma_uint32 channels, ma_uint32 sampleRate);


	typedef struct
	{
		ma_node_base baseNode;
		verblib reverb;
	} ma_reverb_node;

	MA_API ma_result ma_reverb_node_init(ma_node_graph* pNodeGraph, const ma_reverb_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_reverb_node* pReverbNode);
	MA_API void ma_reverb_node_uninit(ma_reverb_node* pReverbNode, const ma_allocation_callbacks* pAllocationCallbacks);

#ifdef __cplusplus
}
#endif
MA_API ma_reverb_node_config ma_reverb_node_config_init(ma_uint32 channels, ma_uint32 sampleRate, float dry, float wet, float room_size)
{
	ma_reverb_node_config config;

	MA_ZERO_OBJECT(&config);
	config.nodeConfig = ma_node_config_init();  /* Input and output channels will be set in ma_reverb_node_init(). */
	config.channels = channels;
	config.channels = channels;
	config.sampleRate = sampleRate;
	config.roomSize = room_size;
	config.damping = verblib_initialdamp;
	config.width = verblib_initialwidth;
	config.wetVolume = wet;
	config.dryVolume = dry;
	config.mode = verblib_initialmode;

	return config;
}


static void ma_reverb_node_process_pcm_frames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut)
{
	ma_reverb_node* pReverbNode = (ma_reverb_node*)pNode;

	(void)pFrameCountIn;

	verblib_process(&pReverbNode->reverb, ppFramesIn[0], ppFramesOut[0], *pFrameCountOut);
}

static ma_node_vtable g_ma_reverb_node_vtable =
{
	ma_reverb_node_process_pcm_frames,
	NULL,
	1,  /* 1 input channel. */
	1,  /* 1 output channel. */
MA_NODE_FLAG_CONTINUOUS_PROCESSING
};

MA_API ma_result ma_reverb_node_init(ma_node_graph* pNodeGraph, const ma_reverb_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_reverb_node* pReverbNode)
{
	ma_result result;
	ma_node_config baseConfig;

	if (pReverbNode == NULL) {
		return MA_INVALID_ARGS;
	}

	MA_ZERO_OBJECT(pReverbNode);

	if (pConfig == NULL) {
		return MA_INVALID_ARGS;
	}

	if (verblib_initialize(&pReverbNode->reverb, (unsigned long)pConfig->sampleRate, (unsigned int)pConfig->channels) == 0) {
		return MA_INVALID_ARGS;
	}

	baseConfig = pConfig->nodeConfig;
	baseConfig.vtable = &g_ma_reverb_node_vtable;
	baseConfig.pInputChannels = &pConfig->channels;
	baseConfig.pOutputChannels = &pConfig->channels;

	result = ma_node_init(pNodeGraph, &baseConfig, pAllocationCallbacks, &pReverbNode->baseNode);
	if (result != MA_SUCCESS) {
		return result;
	}

	return MA_SUCCESS;
}

MA_API void ma_reverb_node_uninit(ma_reverb_node* pReverbNode, const ma_allocation_callbacks* pAllocationCallbacks)
{
	/* The base node is always uninitialized first. */
	ma_node_uninit(pReverbNode, pAllocationCallbacks);
}


/* Include ma_vocoder_node.h after miniaudio.h */
#define VOCLIB_IMPLEMENTATION
#include "fx/voclib.h"

#ifdef __cplusplus
extern "C" {
#endif


	/*
	The vocoder node has two inputs and one output. Inputs:

		Input Bus 0: The source/carrier stream.
		Input Bus 1: The excite/modulator stream.

	The source (input bus 0) and output must have the same channel count, and is restricted to 1 or 2.
	The excite (input bus 1) is restricted to 1 channel.
	*/
	typedef struct
	{
		ma_node_config nodeConfig;
		ma_uint32 channels;         /* The number of channels of the source, which will be the same as the output. Must be 1 or 2. The excite bus must always have one channel. */
		ma_uint32 sampleRate;
		ma_uint32 bands;            /* Defaults to 16. */
		ma_uint32 filtersPerBand;   /* Defaults to 6. */
	} ma_vocoder_node_config;

	MA_API ma_vocoder_node_config ma_vocoder_node_config_init(ma_uint32 channels, ma_uint32 sampleRate);


	typedef struct
	{
		ma_node_base baseNode;
		voclib_instance voclib;
	} ma_vocoder_node;

	MA_API ma_result ma_vocoder_node_init(ma_node_graph* pNodeGraph, const ma_vocoder_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_vocoder_node* pVocoderNode);
	MA_API void ma_vocoder_node_uninit(ma_vocoder_node* pVocoderNode, const ma_allocation_callbacks* pAllocationCallbacks);

#ifdef __cplusplus
}
#endif


MA_API ma_vocoder_node_config ma_vocoder_node_config_init(ma_uint32 channels, ma_uint32 sampleRate)
{
	ma_vocoder_node_config config;

	MA_ZERO_OBJECT(&config);
	config.nodeConfig = ma_node_config_init();   /* Input and output channels will be set in ma_vocoder_node_init(). */
	config.channels = channels;
	config.sampleRate = sampleRate;
	config.bands = 16;
	config.filtersPerBand = 6;

	return config;
}


static void ma_vocoder_node_process_pcm_frames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut)
{
	ma_vocoder_node* pVocoderNode = (ma_vocoder_node*)pNode;

	(void)pFrameCountIn;

	voclib_process(&pVocoderNode->voclib, ppFramesIn[0], ppFramesIn[1], ppFramesOut[0], *pFrameCountOut);
}

static ma_node_vtable g_ma_vocoder_node_vtable =
{
	ma_vocoder_node_process_pcm_frames,
	NULL,
	2,  /* 2 input channels. */
	1,  /* 1 output channel. */
	0
};

MA_API ma_result ma_vocoder_node_init(ma_node_graph* pNodeGraph, const ma_vocoder_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_vocoder_node* pVocoderNode)
{
	ma_result result;
	ma_node_config baseConfig;
	ma_uint32 inputChannels[2];
	ma_uint32 outputChannels[1];

	if (pVocoderNode == NULL) {
		return MA_INVALID_ARGS;
	}

	MA_ZERO_OBJECT(pVocoderNode);

	if (pConfig == NULL) {
		return MA_INVALID_ARGS;
	}

	if (voclib_initialize(&pVocoderNode->voclib, (unsigned char)pConfig->bands, (unsigned char)pConfig->filtersPerBand, (unsigned int)pConfig->sampleRate, (unsigned char)pConfig->channels) == 0) {
		return MA_INVALID_ARGS;
	}

	inputChannels[0] = pConfig->channels;   /* Source/carrier. */
	inputChannels[1] = 1;                   /* Excite/modulator. Must always be single channel. */
	outputChannels[0] = pConfig->channels;   /* Output channels is always the same as the source/carrier. */

	baseConfig = pConfig->nodeConfig;
	baseConfig.vtable = &g_ma_vocoder_node_vtable;
	baseConfig.pInputChannels = inputChannels;
	baseConfig.pOutputChannels = outputChannels;

	result = ma_node_init(pNodeGraph, &baseConfig, pAllocationCallbacks, &pVocoderNode->baseNode);
	if (result != MA_SUCCESS) {
		return result;
	}

	return MA_SUCCESS;
}

MA_API void ma_vocoder_node_uninit(ma_vocoder_node* pVocoderNode, const ma_allocation_callbacks* pAllocationCallbacks)
{
	/* The base node must always be initialized first. */
	ma_node_uninit(pVocoderNode, pAllocationCallbacks);
}
#ifdef __cplusplus
extern "C" {
#endif

	/*
	The trim node has one input and one output.
	*/
	typedef struct
	{
		ma_node_config nodeConfig;
		ma_uint32 channels;
		float threshold;
	} ma_ltrim_node_config;

	MA_API ma_ltrim_node_config ma_ltrim_node_config_init(ma_uint32 channels, float threshold);


	typedef struct
	{
		ma_node_base baseNode;
		float threshold;
		ma_bool32 foundStart;
	} ma_ltrim_node;

	MA_API ma_result ma_ltrim_node_init(ma_node_graph* pNodeGraph, const ma_ltrim_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_ltrim_node* pTrimNode);
	MA_API void ma_ltrim_node_uninit(ma_ltrim_node* pTrimNode, const ma_allocation_callbacks* pAllocationCallbacks);

#ifdef __cplusplus
}
#endif
MA_API ma_ltrim_node_config ma_ltrim_node_config_init(ma_uint32 channels, float threshold)
{
	ma_ltrim_node_config config;

	MA_ZERO_OBJECT(&config);
	config.nodeConfig = ma_node_config_init();  /* Input and output channels will be set in ma_ltrim_node_init(). */
	config.channels = channels;
	config.threshold = threshold;

	return config;
}


static void ma_ltrim_node_process_pcm_frames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut)
{
	ma_ltrim_node* pTrimNode = (ma_ltrim_node*)pNode;
	ma_uint32 framesProcessedIn = 0;
	ma_uint32 framesProcessedOut = 0;
	ma_uint32 channelCount = ma_node_get_input_channels(pNode, 0);

	/*
	If we haven't yet found the start, skip over every input sample until we find a frame outside
	of the threshold.
	*/
	if (pTrimNode->foundStart == MA_FALSE) {
		while (framesProcessedIn < *pFrameCountIn) {
			ma_uint32 iChannel = 0;
			for (iChannel = 0; iChannel < channelCount; iChannel += 1) {
				float sample = ppFramesIn[0][framesProcessedIn * channelCount + iChannel];
				if (sample < -pTrimNode->threshold || sample > pTrimNode->threshold) {
					pTrimNode->foundStart = MA_TRUE;
					break;
				}
			}

			if (pTrimNode->foundStart) {
				break;  /* The start has been found. Get out of this loop and finish off processing. */
			}
			else {
				framesProcessedIn += 1;
			}
		}
	}

	/* If there's anything left, just copy it over. */
	framesProcessedOut = ma_min(*pFrameCountOut, *pFrameCountIn - framesProcessedIn);
	ma_copy_pcm_frames(ppFramesOut[0], &ppFramesIn[0][framesProcessedIn], framesProcessedOut, ma_format_f32, channelCount);

	framesProcessedIn += framesProcessedOut;

	/* We always "process" every input frame, but we may only done a partial output. */
	*pFrameCountIn = framesProcessedIn;
	*pFrameCountOut = framesProcessedOut;
}

static ma_node_vtable g_ma_ltrim_node_vtable =
{
	ma_ltrim_node_process_pcm_frames,
	NULL,
	1,  /* 1 input channel. */
	1,  /* 1 output channel. */
	MA_NODE_FLAG_DIFFERENT_PROCESSING_RATES
};

MA_API ma_result ma_ltrim_node_init(ma_node_graph* pNodeGraph, const ma_ltrim_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_ltrim_node* pTrimNode)
{
	ma_result result;
	ma_node_config baseConfig;

	if (pTrimNode == NULL) {
		return MA_INVALID_ARGS;
	}

	MA_ZERO_OBJECT(pTrimNode);

	if (pConfig == NULL) {
		return MA_INVALID_ARGS;
	}

	pTrimNode->threshold = pConfig->threshold;
	pTrimNode->foundStart = MA_FALSE;

	baseConfig = pConfig->nodeConfig;
	baseConfig.vtable = &g_ma_ltrim_node_vtable;
	baseConfig.pInputChannels = &pConfig->channels;
	baseConfig.pOutputChannels = &pConfig->channels;

	result = ma_node_init(pNodeGraph, &baseConfig, pAllocationCallbacks, &pTrimNode->baseNode);
	if (result != MA_SUCCESS) {
		return result;
	}

	return MA_SUCCESS;
}

MA_API void ma_ltrim_node_uninit(ma_ltrim_node* pTrimNode, const ma_allocation_callbacks* pAllocationCallbacks)
{
	/* The base node is always uninitialized first. */
	ma_node_uninit(pTrimNode, pAllocationCallbacks);
}
#ifdef __cplusplus
extern "C" {
#endif

	typedef struct
	{
		ma_node_config nodeConfig;
		ma_uint32 channels;
	} ma_channel_combiner_node_config;

	MA_API ma_channel_combiner_node_config ma_channel_combiner_node_config_init(ma_uint32 channels);


	typedef struct
	{
		ma_node_base baseNode;
	} ma_channel_combiner_node;

	MA_API ma_result ma_channel_combiner_node_init(ma_node_graph* pNodeGraph, const ma_channel_combiner_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_channel_combiner_node* pSeparatorNode);
	MA_API void ma_channel_combiner_node_uninit(ma_channel_combiner_node* pSeparatorNode, const ma_allocation_callbacks* pAllocationCallbacks);


#ifdef __cplusplus
}
#endif
MA_API ma_channel_combiner_node_config ma_channel_combiner_node_config_init(ma_uint32 channels)
{
	ma_channel_combiner_node_config config;

	MA_ZERO_OBJECT(&config);
	config.nodeConfig = ma_node_config_init();  /* Input and output channels will be set in ma_channel_combiner_node_init(). */
	config.channels = channels;

	return config;
}


static void ma_channel_combiner_node_process_pcm_frames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut)
{
	ma_channel_combiner_node* pCombinerNode = (ma_channel_combiner_node*)pNode;

	(void)pFrameCountIn;

	ma_interleave_pcm_frames(ma_format_f32, ma_node_get_output_channels(pCombinerNode, 0), *pFrameCountOut, (const void**)ppFramesIn, (void*)ppFramesOut[0]);
}

static ma_node_vtable g_ma_channel_combiner_node_vtable =
{
	ma_channel_combiner_node_process_pcm_frames,
	NULL,
	MA_NODE_BUS_COUNT_UNKNOWN,  /* Input bus count is determined by the channel count and is unknown until the node instance is initialized. */
	1,  /* 1 output bus. */
	0   /* Default flags. */
};

MA_API ma_result ma_channel_combiner_node_init(ma_node_graph* pNodeGraph, const ma_channel_combiner_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_channel_combiner_node* pCombinerNode)
{
	ma_result result;
	ma_node_config baseConfig;
	ma_uint32 inputChannels[MA_MAX_NODE_BUS_COUNT];
	ma_uint32 outputChannels[1];
	ma_uint32 iChannel;

	if (pCombinerNode == NULL) {
		return MA_INVALID_ARGS;
	}

	MA_ZERO_OBJECT(pCombinerNode);

	if (pConfig == NULL) {
		return MA_INVALID_ARGS;
	}

	/* All input channels are mono. */
	for (iChannel = 0; iChannel < pConfig->channels; iChannel += 1) {
		inputChannels[iChannel] = 1;
	}

	outputChannels[0] = pConfig->channels;

	baseConfig = pConfig->nodeConfig;
	baseConfig.vtable = &g_ma_channel_combiner_node_vtable;
	baseConfig.inputBusCount = pConfig->channels; /* The vtable has an unknown channel count, so must specify it here. */
	baseConfig.pInputChannels = inputChannels;
	baseConfig.pOutputChannels = outputChannels;

	result = ma_node_init(pNodeGraph, &baseConfig, pAllocationCallbacks, &pCombinerNode->baseNode);
	if (result != MA_SUCCESS) {
		return result;
	}

	return MA_SUCCESS;
}

MA_API void ma_channel_combiner_node_uninit(ma_channel_combiner_node* pCombinerNode, const ma_allocation_callbacks* pAllocationCallbacks)
{
	/* The base node is always uninitialized first. */
	ma_node_uninit(pCombinerNode, pAllocationCallbacks);
}
#ifdef __cplusplus
extern "C" {
#endif

	typedef struct
	{
		ma_node_config nodeConfig;
		ma_uint32 channels;
	} ma_channel_separator_node_config;

	MA_API ma_channel_separator_node_config ma_channel_separator_node_config_init(ma_uint32 channels);


	typedef struct
	{
		ma_node_base baseNode;
	} ma_channel_separator_node;

	MA_API ma_result ma_channel_separator_node_init(ma_node_graph* pNodeGraph, const ma_channel_separator_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_channel_separator_node* pSeparatorNode);
	MA_API void ma_channel_separator_node_uninit(ma_channel_separator_node* pSeparatorNode, const ma_allocation_callbacks* pAllocationCallbacks);

#ifdef __cplusplus
}
#endif
MA_API ma_channel_separator_node_config ma_channel_separator_node_config_init(ma_uint32 channels)
{
	ma_channel_separator_node_config config;

	MA_ZERO_OBJECT(&config);
	config.nodeConfig = ma_node_config_init();  /* Input and output channels will be set in ma_channel_separator_node_init(). */
	config.channels = channels;

	return config;
}


static void ma_channel_separator_node_process_pcm_frames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut)
{
	ma_channel_separator_node* pSplitterNode = (ma_channel_separator_node*)pNode;

	(void)pFrameCountIn;

	ma_deinterleave_pcm_frames(ma_format_f32, ma_node_get_input_channels(pSplitterNode, 0), *pFrameCountOut, (const void*)ppFramesIn[0], (void**)ppFramesOut);
}

static ma_node_vtable g_ma_channel_separator_node_vtable =
{
	ma_channel_separator_node_process_pcm_frames,
	NULL,
	1,  /* 1 input bus. */
	MA_NODE_BUS_COUNT_UNKNOWN,  /* Output bus count is determined by the channel count and is unknown until the node instance is initialized. */
	0   /* Default flags. */
};

MA_API ma_result ma_channel_separator_node_init(ma_node_graph* pNodeGraph, const ma_channel_separator_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_channel_separator_node* pSeparatorNode)
{
	ma_result result;
	ma_node_config baseConfig;
	ma_uint32 inputChannels[1];
	ma_uint32 outputChannels[MA_MAX_NODE_BUS_COUNT];
	ma_uint32 iChannel;

	if (pSeparatorNode == NULL) {
		return MA_INVALID_ARGS;
	}

	MA_ZERO_OBJECT(pSeparatorNode);

	if (pConfig == NULL) {
		return MA_INVALID_ARGS;
	}

	if (pConfig->channels > MA_MAX_NODE_BUS_COUNT) {
		return MA_INVALID_ARGS; /* Channel count cannot exceed the maximum number of buses. */
	}

	inputChannels[0] = pConfig->channels;

	/* All output channels are mono. */
	for (iChannel = 0; iChannel < pConfig->channels; iChannel += 1) {
		outputChannels[iChannel] = 1;
	}

	baseConfig = pConfig->nodeConfig;
	baseConfig.vtable = &g_ma_channel_separator_node_vtable;
	baseConfig.outputBusCount = pConfig->channels; /* The vtable has an unknown channel count, so must specify it here. */
	baseConfig.pInputChannels = inputChannels;
	baseConfig.pOutputChannels = outputChannels;

	result = ma_node_init(pNodeGraph, &baseConfig, pAllocationCallbacks, &pSeparatorNode->baseNode);
	if (result != MA_SUCCESS) {
		return result;
	}

	return MA_SUCCESS;
}

MA_API void ma_channel_separator_node_uninit(ma_channel_separator_node* pSeparatorNode, const ma_allocation_callbacks* pAllocationCallbacks)
{
	/* The base node is always uninitialized first. */
	ma_node_uninit(pSeparatorNode, pAllocationCallbacks);
}

IPLAudioSettings iplAudioSettings;
IPLContextSettings iplContextSettings;
IPLContext iplContext;
IPLHRTFSettings iplHRTFSettings;
IPLHRTF iplHRTF;
bool g_SoundInitialized = false;
ma_engine sound_default_mixer;
ma_device sound_mixer_device;
ma_engine_config engineConfig;
asUINT period_size = 256;
static void sound_mixer_device_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	ma_engine_read_pcm_frames(&sound_default_mixer, pOutput, frameCount, nullptr);
	(void)pInput;
}
struct AudioDevice {
	std::string name;
	ma_device_id id;
};
static std::vector<AudioDevice> GetOutputAudioDevices()
{
	std::vector<AudioDevice> audioDevices;
	ma_result result;
	ma_context context;
	ma_device_info* pPlaybackDeviceInfos;
	ma_uint32 playbackDeviceCount;
	ma_uint32 iPlaybackDevice;

	if (ma_context_init(NULL, 0, NULL, &context) != MA_SUCCESS) {
		return audioDevices;;
	}

	result = ma_context_get_devices(&context, &pPlaybackDeviceInfos, &playbackDeviceCount, nullptr, nullptr);
	if (result != MA_SUCCESS) {
		return audioDevices;
	}
	for (iPlaybackDevice = 0; iPlaybackDevice < playbackDeviceCount; ++iPlaybackDevice) {
		const char* name = pPlaybackDeviceInfos[iPlaybackDevice].name;
		std::string name_str(name);
		AudioDevice ad;
		ad.id = pPlaybackDeviceInfos[iPlaybackDevice].id;
		ad.name = name;
		audioDevices.push_back(ad);
	}

	ma_context_uninit(&context);
	return audioDevices;
}
std::vector<AudioDevice> devs;
CScriptArray* get_output_audio_devices() {
	if (!g_SoundInitialized)soundsystem_init();
	asIScriptContext* ctx = asGetActiveContext();
	asIScriptEngine* engine = ctx->GetEngine();
	asITypeInfo* arrayType = engine->GetTypeInfoById(engine->GetTypeIdByDecl("array<string>"));
	CScriptArray* array = CScriptArray::Create(arrayType, (asUINT)0);
	devs = GetOutputAudioDevices();
	if (devs.size() == 0)return array;
	array->Reserve(devs.size());
	for (asUINT i = 0; i < devs.size(); i++) {
		array->InsertLast(&devs[i].name);
	}
	return array;
}
void mixer_start();
void mixer_stop();
bool set_output_audio_device(asUINT id) {
	if (!g_SoundInitialized)soundsystem_init();
	if (devs.size() == 0)devs = GetOutputAudioDevices();
	mixer_stop();// Need to reset and uninitialize audio_device
	ma_device_uninit(&sound_mixer_device);
	ma_device_config devConfig = ma_device_config_init(ma_device_type_playback);;
	devConfig.playback.pDeviceID = &devs[id].id;
	devConfig.periodSizeInFrames = period_size;
	devConfig.playback.channels = CHANNELS;
	devConfig.sampleRate = SAMPLE_RATE;
	devConfig.noClip = MA_TRUE;
	devConfig.dataCallback = sound_mixer_device_callback;
	if (ma_device_init(nullptr, &devConfig, &sound_mixer_device) != MA_SUCCESS)return false;
	mixer_start();
	return true;
}
static ma_result ma_result_from_IPLerror(IPLerror error)
{
	switch (error)
	{
	case IPL_STATUS_SUCCESS:      return MA_SUCCESS;
	case IPL_STATUS_OUTOFMEMORY:  return MA_OUT_OF_MEMORY;
	case IPL_STATUS_INITIALIZATION:
	case IPL_STATUS_FAILURE:
	default: return MA_ERROR;
	}
}


typedef struct
{
	ma_node_config nodeConfig;
	ma_uint32 channelsIn;
	IPLAudioSettings iplAudioSettings;
	IPLContext iplContext;
	IPLHRTF iplHRTF;   /* There is one HRTF object to many binaural effect objects. */
} ma_steamaudio_binaural_node_config;

MA_API ma_steamaudio_binaural_node_config ma_steamaudio_binaural_node_config_init(ma_uint32 channelsIn, IPLAudioSettings iplAudioSettings, IPLContext iplContext, IPLHRTF iplHRTF);


typedef struct
{
	ma_node_base baseNode;
	IPLAudioSettings iplAudioSettings;
	IPLContext iplContext;
	IPLHRTF iplHRTF;
	IPLBinauralEffect iplEffect;
	IPLDirectEffect effect;
	ma_vec3f direction;
	float* ppBuffersIn[2];      /* Each buffer is an offset of _pHeap. */
	float* ppBuffersOut[2];     /* Each buffer is an offset of _pHeap. */
	void* _pHeap;
	ma_sound handle_;
} ma_steamaudio_binaural_node;

MA_API ma_result ma_steamaudio_binaural_node_init(ma_node_graph* pNodeGraph, const ma_steamaudio_binaural_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_steamaudio_binaural_node* pBinauralNode);
MA_API void ma_steamaudio_binaural_node_uninit(ma_steamaudio_binaural_node* pBinauralNode, const ma_allocation_callbacks* pAllocationCallbacks);
MA_API ma_result ma_steamaudio_binaural_node_set_direction(ma_steamaudio_binaural_node* pBinauralNode, float x, float y, float z);


MA_API ma_steamaudio_binaural_node_config ma_steamaudio_binaural_node_config_init(ma_uint32 channelsIn, IPLAudioSettings iplAudioSettings, IPLContext iplContext, IPLHRTF iplHRTF)
{
	ma_steamaudio_binaural_node_config config;

	MA_ZERO_OBJECT(&config);
	config.nodeConfig = ma_node_config_init();
	config.channelsIn = channelsIn;
	config.iplAudioSettings = iplAudioSettings;
	config.iplContext = iplContext;
	config.iplHRTF = iplHRTF;

	return config;
}
IPLSimulationOutputs outputs{};

static void ma_steamaudio_binaural_node_process_pcm_frames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut)
{
	ma_steamaudio_binaural_node* pBinauralNode = (ma_steamaudio_binaural_node*)pNode;
	IPLBinauralEffectParams binauralParams;
	IPLDirectEffectParams params;
	params.flags = IPLDirectEffectFlags(IPL_DIRECTEFFECTFLAGS_APPLYDISTANCEATTENUATION | IPL_DIRECTEFFECTFLAGS_APPLYAIRABSORPTION | IPL_DIRECTEFFECTFLAGS_APPLYOCCLUSION);
	IPLAudioBuffer inputBufferDesc;
	IPLAudioBuffer outputBufferDesc;
	ma_uint32 totalFramesToProcess = *pFrameCountOut;
	ma_uint32 totalFramesProcessed = 0;
	binauralParams.direction.x = pBinauralNode->direction.x;
	binauralParams.direction.y = pBinauralNode->direction.z;
	binauralParams.direction.z = pBinauralNode->direction.y;
	binauralParams.interpolation = IPL_HRTFINTERPOLATION_NEAREST;
	ma_vec3f listener = ma_engine_listener_get_position(&sound_default_mixer, ma_sound_get_listener_index(&pBinauralNode->handle_));
	float distance = sqrt((listener.x + binauralParams.direction.x) * (listener.x + binauralParams.direction.x) +
		(listener.y + binauralParams.direction.y) * (listener.y + binauralParams.direction.y) +
		(listener.z - binauralParams.direction.z) * (listener.z - binauralParams.direction.z));
	float maxDistance = 2.0f;

	float normalizedDistance = distance / maxDistance;
	binauralParams.spatialBlend = min(0.0f + normalizedDistance, 1.0f);
	if (binauralParams.spatialBlend > 1.0f)
		binauralParams.spatialBlend = 1.0f;
	binauralParams.hrtf = pBinauralNode->iplHRTF;
	binauralParams.peakDelays = NULL;
	inputBufferDesc.numChannels = (IPLint32)ma_node_get_input_channels(pNode, 0);

	/* We'll run this in a loop just in case our deinterleaved buffers are too small. */
	outputBufferDesc.numSamples = pBinauralNode->iplAudioSettings.frameSize;
	outputBufferDesc.numChannels = 2;
	outputBufferDesc.data = pBinauralNode->ppBuffersOut;

	while (totalFramesProcessed < totalFramesToProcess) {
		ma_uint32 framesToProcessThisIteration = totalFramesToProcess - totalFramesProcessed;
		if (framesToProcessThisIteration > (ma_uint32)pBinauralNode->iplAudioSettings.frameSize) {
			framesToProcessThisIteration = (ma_uint32)pBinauralNode->iplAudioSettings.frameSize;
		}

		if (inputBufferDesc.numChannels == 1) {
			/* Fast path. No need for deinterleaving since it's a mono stream. */
			pBinauralNode->ppBuffersIn[0] = (float*)ma_offset_pcm_frames_const_ptr_f32(ppFramesIn[0], totalFramesProcessed, 1);
		}
		else {
			/* Slow path. Need to deinterleave the input data. */
			ma_deinterleave_pcm_frames(ma_format_f32, inputBufferDesc.numChannels, framesToProcessThisIteration, ma_offset_pcm_frames_const_ptr_f32(ppFramesIn[0], totalFramesProcessed, inputBufferDesc.numChannels), (void**)pBinauralNode->ppBuffersIn);
		}

		inputBufferDesc.data = pBinauralNode->ppBuffersIn;
		inputBufferDesc.numSamples = (IPLint32)framesToProcessThisIteration;

		/* Apply the effect. */
		iplBinauralEffectApply(pBinauralNode->iplEffect, &binauralParams, &inputBufferDesc, &outputBufferDesc);
		//iplDirectEffectApply(pBinauralNode->effect, &params, &inputBufferDesc, &outputBufferDesc);

		/* Interleave straight into the output buffer. */
		ma_interleave_pcm_frames(ma_format_f32, 2, framesToProcessThisIteration, (const void**)pBinauralNode->ppBuffersOut, ma_offset_pcm_frames_ptr_f32(ppFramesOut[0], totalFramesProcessed, 2));

		/* Advance. */
		totalFramesProcessed += framesToProcessThisIteration;
	}

	(void)pFrameCountIn;    /* Unused. */
}

static ma_node_vtable g_ma_steamaudio_binaural_node_vtable =
{
	ma_steamaudio_binaural_node_process_pcm_frames,
	NULL,
	1,  /* 1 input channel. */
1    ,  /* 1 output channel. */
	0
};
IPLCoordinateSpace3 listenerCoordinates; // the world-space position and orientation of the listener
IPLCoordinateSpace3 sourceCoordinates; // the world-space position and orientation of the source;

MA_API ma_result ma_steamaudio_binaural_node_init(ma_node_graph* pNodeGraph, const ma_steamaudio_binaural_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_steamaudio_binaural_node* pBinauralNode)
{
	ma_result result;
	ma_node_config baseConfig;
	ma_uint32 channelsIn;
	ma_uint32 channelsOut;
	IPLBinauralEffectSettings iplBinauralEffectSettings;
	IPLDirectEffectSettings effectSettings{};
	size_t heapSizeInBytes;

	if (pBinauralNode == NULL) {
		return MA_INVALID_ARGS;
	}

	MA_ZERO_OBJECT(pBinauralNode);

	if (pConfig == NULL || pConfig->iplAudioSettings.frameSize == 0 || pConfig->iplContext == NULL || pConfig->iplHRTF == NULL) {
		return MA_INVALID_ARGS;
	}

	/* Steam Audio only supports mono and stereo input. */
	if (pConfig->channelsIn < 1 || pConfig->channelsIn > 2) {
		return MA_INVALID_ARGS;
	}

	channelsIn = pConfig->channelsIn;
	channelsOut = 2;    /* Always stereo output. */

	baseConfig = ma_node_config_init();
	baseConfig.vtable = &g_ma_steamaudio_binaural_node_vtable;
	baseConfig.pInputChannels = &channelsIn;
	baseConfig.pOutputChannels = &channelsOut;
	result = ma_node_init(pNodeGraph, &baseConfig, pAllocationCallbacks, &pBinauralNode->baseNode);
	if (result != MA_SUCCESS) {
		return result;
	}

	pBinauralNode->iplAudioSettings = pConfig->iplAudioSettings;
	pBinauralNode->iplContext = pConfig->iplContext;
	pBinauralNode->iplHRTF = pConfig->iplHRTF;

	MA_ZERO_OBJECT(&iplBinauralEffectSettings);
	iplBinauralEffectSettings.hrtf = pBinauralNode->iplHRTF;

	result = ma_result_from_IPLerror(iplBinauralEffectCreate(pBinauralNode->iplContext, &pBinauralNode->iplAudioSettings, &iplBinauralEffectSettings, &pBinauralNode->iplEffect));
	if (result != MA_SUCCESS) {
		ma_node_uninit(&pBinauralNode->baseNode, pAllocationCallbacks);
		return result;
	}
	MA_ZERO_OBJECT(&effectSettings);
	effectSettings.numChannels = 1; // input and output buffers will have 1 channel
	result = ma_result_from_IPLerror(iplDirectEffectCreate(pBinauralNode->iplContext, &pBinauralNode->iplAudioSettings, &effectSettings, &pBinauralNode->effect));
	if (result != MA_SUCCESS) {
		ma_node_uninit(&pBinauralNode->baseNode, pAllocationCallbacks);
		return result;
	}

	IPLSimulationSettings simulationSettings{};
	simulationSettings.flags = IPL_SIMULATIONFLAGS_DIRECT; // this enables occlusion/transmission simulation
	simulationSettings.sceneType = IPL_SCENETYPE_DEFAULT;

	IPLSimulator simulator = nullptr;
	iplSimulatorCreate(pBinauralNode->iplContext, &simulationSettings, &simulator);
	IPLSourceSettings sourceSettings;
	sourceSettings.flags = IPL_SIMULATIONFLAGS_DIRECT; // this enables occlusion/transmission simulator 
	IPLSource source = nullptr;
	iplSourceCreate(simulator, &sourceSettings, &source);
	iplSourceAdd(source, simulator);
	iplSimulatorCommit(simulator);

	IPLSimulationInputs inputs;
	inputs.flags = IPL_SIMULATIONFLAGS_DIRECT;
	inputs.directFlags = static_cast<IPLDirectSimulationFlags>(IPL_DIRECTSIMULATIONFLAGS_OCCLUSION | IPL_DIRECTSIMULATIONFLAGS_TRANSMISSION);
	inputs.source = sourceCoordinates;
	inputs.occlusionType = IPL_OCCLUSIONTYPE_RAYCAST;

	iplSourceSetInputs(source, IPL_SIMULATIONFLAGS_DIRECT, &inputs);
	IPLSimulationSharedInputs sharedInputs{};
	sharedInputs.listener = listenerCoordinates;

	iplSimulatorSetSharedInputs(simulator, IPL_SIMULATIONFLAGS_DIRECT, &sharedInputs);
	iplSourceGetOutputs(source, IPL_SIMULATIONFLAGS_DIRECT, &outputs);

	IPLDirectEffectParams params = outputs.direct; // this can be passed to a direct effect
	heapSizeInBytes = 0;

	/*
	Unfortunately Steam Audio uses deinterleaved buffers for everything so we'll need to use some
	intermediary buffers. We'll allocate one big buffer on the heap and then use offsets. We'll
	use the frame size from the IPLAudioSettings structure as a basis for the size of the buffer.
	*/
	heapSizeInBytes += sizeof(float) * channelsOut * pBinauralNode->iplAudioSettings.frameSize; /* Output buffer. */
	heapSizeInBytes += sizeof(float) * channelsIn * pBinauralNode->iplAudioSettings.frameSize; /* Input buffer. */

	pBinauralNode->_pHeap = ma_malloc(heapSizeInBytes, pAllocationCallbacks);
	if (pBinauralNode->_pHeap == NULL) {
		iplBinauralEffectRelease(&pBinauralNode->iplEffect);
		ma_node_uninit(&pBinauralNode->baseNode, pAllocationCallbacks);
		return MA_OUT_OF_MEMORY;
	}

	pBinauralNode->ppBuffersOut[0] = (float*)pBinauralNode->_pHeap;
	pBinauralNode->ppBuffersOut[1] = (float*)ma_offset_ptr(pBinauralNode->_pHeap, sizeof(float) * pBinauralNode->iplAudioSettings.frameSize);

	{
		ma_uint32 iChannelIn;
		for (iChannelIn = 0; iChannelIn < channelsIn; iChannelIn += 1) {
			pBinauralNode->ppBuffersIn[iChannelIn] = (float*)ma_offset_ptr(pBinauralNode->_pHeap, sizeof(float) * pBinauralNode->iplAudioSettings.frameSize * (channelsOut + iChannelIn));
		}
	}

	return MA_SUCCESS;
}

MA_API void ma_steamaudio_binaural_node_uninit(ma_steamaudio_binaural_node* pBinauralNode, const ma_allocation_callbacks* pAllocationCallbacks)
{
	if (pBinauralNode == NULL) {
		return;
	}

	/* The base node is always uninitialized first. */
	ma_node_uninit(&pBinauralNode->baseNode, pAllocationCallbacks);

	/*
	The Steam Audio objects are deleted after the base node. This ensures the base node is removed from the graph
	first to ensure these objects aren't getting used by the audio thread.
	*/
	iplBinauralEffectRelease(&pBinauralNode->iplEffect);
	ma_free(pBinauralNode->_pHeap, pAllocationCallbacks);
}

MA_API ma_result ma_steamaudio_binaural_node_set_direction(ma_steamaudio_binaural_node* pBinauralNode, float x, float y, float z)
{
	if (pBinauralNode == NULL) {
		return MA_INVALID_ARGS;
	}

	pBinauralNode->direction.x = x;
	pBinauralNode->direction.y = -y;
	pBinauralNode->direction.z = z;

	return MA_SUCCESS;
}
IPLScene scene;
IPLSimulator simulator;
IPLSimulationSharedInputs simulator_inputs;
void soundsystem_init() {
	if (g_SoundInitialized == true)return;
	engineConfig = ma_engine_config_init();
	engineConfig.noDevice = MA_TRUE;
	engineConfig.channels = CHANNELS;
	engineConfig.sampleRate = SAMPLE_RATE;
	ma_device_config devConfig = ma_device_config_init(ma_device_type_playback);;
	devConfig.noClip = MA_TRUE;
	devConfig.periodSizeInFrames = period_size;
	devConfig.playback.channels = CHANNELS;
	devConfig.sampleRate = SAMPLE_RATE;
	devConfig.dataCallback = sound_mixer_device_callback;
	if (ma_device_init(nullptr, &devConfig, &sound_mixer_device) != MA_SUCCESS)return;
	mixer_start();
	ma_engine_init(&engineConfig, &sound_default_mixer);
	MA_ZERO_OBJECT(&iplAudioSettings);
	iplAudioSettings.samplingRate = SAMPLE_RATE;

	iplAudioSettings.frameSize = devConfig.periodSizeInFrames;


	/* IPLContext */
	MA_ZERO_OBJECT(&iplContextSettings);
	iplContextSettings.version = STEAMAUDIO_VERSION;
	//    iplContextSettings.flags = IPL_CONTEXTFLAGS_VALIDATION;
	ma_result_from_IPLerror(iplContextCreate(&iplContextSettings, &iplContext));
	/* IPLHRTF */
	MA_ZERO_OBJECT(&iplHRTFSettings);
	iplHRTFSettings.type = IPL_HRTFTYPE_DEFAULT;
	iplHRTFSettings.volume = 1.0f;
	ma_result_from_IPLerror(iplHRTFCreate(iplContext, &iplAudioSettings, &iplHRTFSettings, &iplHRTF));
	IPLSimulationSettings simulation_settings{};
	simulation_settings.flags = IPLSimulationFlags(IPL_SIMULATIONFLAGS_DIRECT | IPL_SIMULATIONFLAGS_REFLECTIONS);
	simulation_settings.sceneType = IPL_SCENETYPE_DEFAULT;
	simulation_settings.reflectionType = IPL_REFLECTIONEFFECTTYPE_CONVOLUTION;
	simulation_settings.maxNumRays = 2048;
	simulation_settings.numDiffuseSamples = 128;
	simulation_settings.maxDuration = 2.0f;
	simulation_settings.maxOrder = 2;
	simulation_settings.maxNumSources = 64;
	simulation_settings.numThreads = 16;
	simulation_settings.samplingRate = engineConfig.sampleRate;
	simulation_settings.frameSize = engineConfig.periodSizeInFrames;
	iplSimulatorCreate(iplContext, &simulation_settings, &simulator);
	IPLSceneSettings scene_settings{};
	scene_settings.type = IPL_SCENETYPE_DEFAULT;
	iplSceneCreate(iplContext, &scene_settings, &scene);
	simulator_inputs.numRays = 2048;
	simulator_inputs.numBounces = 32;
	simulator_inputs.duration = 2.0f;
	simulator_inputs.order = 2;
	simulator_inputs.irradianceMinDistance = 1.0f;
	iplSimulatorSetScene(simulator, scene);
	iplSimulatorCommit(simulator);
}
void soundsystem_free() {
	if (g_SoundInitialized == false)return;
	ma_device_uninit(&sound_mixer_device);
	ma_engine_uninit(&sound_default_mixer);
	iplHRTFRelease(&iplHRTF);
	iplContextRelease(&iplContext);
	g_SoundInitialized = false;
}
void mixer_reinit();
void set_audio_period_size(asUINT size) {
	period_size = size;
	if (g_SoundInitialized)mixer_reinit();
}
string sound_path;
pack* sound_pack = nullptr;
void set_sound_storage(const string& path) {
	sound_path = path;
	sound_pack = nullptr;
}
string get_sound_storage() {
	return sound_path;
}
void set_sound_pack(pack* p) {
	if (p == nullptr)return;
	sound_pack = p;
	sound_path = "";
}
pack* get_sound_pack() {
	return sound_pack;
}
void set_master_volume(float volume) {
	if (volume > 0 or volume < -100)return;
	ma_engine_set_gain_db(&sound_default_mixer, volume);
}
float get_master_volume() {
	return ma_engine_get_gain_db(&sound_default_mixer);

}
void mixer_start() {
	ma_device_start(&sound_mixer_device);
}
void mixer_stop() {
	ma_device_stop(&sound_mixer_device);
}
bool mixer_play_sound(const string& filename) {
	if (!g_SoundInitialized) {
		soundsystem_init();
		g_SoundInitialized = true;
	}
	string result;
	if (sound_path != "") {
		result = sound_path + "/" + filename.c_str();
	}
	else {
		result = filename;
	}

	ma_result r;
	r = ma_engine_play_sound(&sound_default_mixer, result.c_str(), nullptr);
	if (r != MA_SUCCESS)
		return  false;
	return true;
}
void mixer_reinit() {
	soundsystem_free();
	soundsystem_init();
}
bool sound_global_hrtf = false;
struct MemoryStruct {
	char* memory;
	size_t size;
};

static size_t write_memory_callback(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)userp;

	char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL) {
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

vector<float> load_audio_from_url(const char* url) {
	CURL* curl_handle;
	CURLcode res;
	struct MemoryStruct chunk;

	chunk.memory = (char*)malloc(1);
	chunk.size = 0;

	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();

	curl_easy_setopt(curl_handle, CURLOPT_URL, url);

	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_memory_callback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);

	res = curl_easy_perform(curl_handle);

	if (res != CURLE_OK) {
		free(chunk.memory);
		return vector<float>();
	}

	vector<float> audioData(chunk.size / sizeof(float));
	memcpy(audioData.data(), chunk.memory, chunk.size);

	free(chunk.memory);
	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();

	return audioData;
}
typedef struct
{
	ma_node_config nodeConfig;
	ma_uint32 channels;         /* The number of channels of the source, which will be the same as the output. Must be 1 or 2. */
	ma_uint32 sampleRate;
	float playbackSpeed;       /* Playback speed multiplier. */
} ma_playback_speed_node_config;
typedef struct
{
	ma_node_base baseNode;
	ma_playback_speed_node_config conf;
} ma_playback_speed_node;


MA_API ma_playback_speed_node_config ma_playback_speed_node_config_init(ma_uint32 channels, ma_uint32 sampleRate)
{
	ma_playback_speed_node_config config;

	MA_ZERO_OBJECT(&config);
	config.nodeConfig = ma_node_config_init();  /* Input and output channels will be set in ma_reverb_node_init(). */
	config.channels = channels;
	config.sampleRate = sampleRate;

	return config;
}

static void ma_playback_speed_node_process_pcm_frames(ma_node* pNode, const float** ppFramesIn, ma_uint32* pFrameCountIn, float** ppFramesOut, ma_uint32* pFrameCountOut)
{
	ma_playback_speed_node* pPlaybackSpeedNode = (ma_playback_speed_node*)pNode;

	float playbackSpeed = pPlaybackSpeedNode->conf.playbackSpeed;

	for (ma_uint32 i = 0; i < *pFrameCountOut; i++) {
		float t = (float)i / playbackSpeed;
		int index = (int)t;
		float fraction = t - index;

		for (ma_uint32 j = 0; j < pPlaybackSpeedNode->conf.channels; j++) {
			int inputIndex1 = index * pPlaybackSpeedNode->conf.channels + j;
			int inputIndex2 = (index + 1) * pPlaybackSpeedNode->conf.channels + j;
			int outputIndex = i * pPlaybackSpeedNode->conf.channels + j;

			*ppFramesOut[outputIndex] = *ppFramesIn[inputIndex1] + fraction * (ppFramesIn[inputIndex2] - ppFramesIn[inputIndex1]);
		}
	}

}

static ma_node_vtable g_ma_playback_speed_node_vtable =
{
	ma_playback_speed_node_process_pcm_frames,
	NULL,
	1,  /* 1 input channel. */
	1,  /* 1 output channel. */
	MA_NODE_FLAG_CONTINUOUS_PROCESSING };
MA_API ma_result ma_playback_speed_node_init(ma_node_graph* pNodeGraph, const ma_playback_speed_node_config* pConfig, const ma_allocation_callbacks* pAllocationCallbacks, ma_playback_speed_node* pPlaybackSpeedNode)
{
	ma_result result;
	ma_node_config baseConfig;

	if (pPlaybackSpeedNode == NULL) {
		return MA_INVALID_ARGS;
	}

	MA_ZERO_OBJECT(pPlaybackSpeedNode);

	if (pConfig == NULL) {
		return MA_INVALID_ARGS;
	}

	baseConfig = pConfig->nodeConfig;
	baseConfig.vtable = &g_ma_playback_speed_node_vtable;
	baseConfig.pInputChannels = &pConfig->channels;
	baseConfig.pOutputChannels = &pConfig->channels;

	result = ma_node_init(pNodeGraph, &baseConfig, pAllocationCallbacks, &pPlaybackSpeedNode->baseNode);
	if (result != MA_SUCCESS) {
		return result;
	}

	return MA_SUCCESS;
}

MA_API void ma_playback_speed_node_uninit(ma_playback_speed_node* pPlaybackSpeedNode, const ma_allocation_callbacks* pAllocationCallbacks)
{
	/* The base node is always uninitialized first. */
	ma_node_uninit(pPlaybackSpeedNode, pAllocationCallbacks);
}




class  mixer {
public:
	ma_engine engine;
};
void at_stop(ma_node* fx, ma_sound* hndl) {
	//    ma_node_detach_output_bus(fx, 0);
}
class MINIAUDIO_IMPLEMENTATION sound {
public:
	bool is_3d_;
	bool playing = false, paused = false, active = false;
	ma_sound* handle_ = nullptr;
	ma_decoder decoder;
	bool decoderInitialized = false;
	ma_steamaudio_binaural_node g_binauralNode;   /* The echo effect is achieved using a delay node. */
	ma_reverb_node      g_reverbNode;        /* The reverb node. */
	ma_reverb_node_config reverbNodeConfig;
	bool reverb = false;
	ma_vocoder_node     g_vocoderNode;   /* The vocoder node. */
	ma_vocoder_node_config vocoderNodeConfig;
	bool vocoder = false;
	ma_delay_node       g_delayNode;         /* The delay node. */
	ma_delay_node_config delayNodeConfig;
	bool delayf = false;
	ma_ltrim_node       g_trimNode;        /* The trim node. */
	ma_ltrim_node_config trimNodeConfig;
	bool ltrim = false;
	ma_channel_separator_node g_separatorNode;   /* The separator node. */
	bool separator = false;
	ma_channel_combiner_node  g_combinerNode;    /* The combiner node. */
	ma_channel_separator_node_config separatorNodeConfig;
	ma_channel_combiner_node_config combinerNodeConfig;
	bool combiner = false;
	ma_hpf_node highpass;
	ma_hpf_node_config highpassConfig;
	bool hp = false;
	ma_lpf_node lowpass;
	ma_lpf_node_config lowpassConfig;
	bool lp = false;
	ma_notch_node notch;
	ma_notch_node_config notchConfig;
	bool notchf = false;
	ma_playback_speed_node g_playback_speed_node;
	ma_playback_speed_node_config g_speed_config;
	bool speeding = false;
	ma_steamaudio_binaural_node_config binauralNodeConfig;
	bool sound_hrtf = false;
	ma_node* current_fx = nullptr;
	mutable int ref = 0;
	string file;
	ngtvector position;
	sound(const string& filename = "", bool set3d = false) {
		ref = 1;
		if (!g_SoundInitialized) {
			soundsystem_init();
			g_SoundInitialized = true;
		}
		if (filename != "")
			this->load(filename, set3d);
		current_fx = ma_engine_get_endpoint(&sound_default_mixer);
	}
	~sound() {
		if (active)this->close();
	}
	void add() {
		ref += 1;
	}
	void release() {
		if (--ref < 1) {
			delete this;
		}
	}
	bool load(const string& filename, bool set3d) {
		string result;
		if (sound_path != "") {
			result = sound_path + "/" + filename.c_str();
		}
		else {
			result = filename;
		}
		if (active)this->close();
		if (sound_pack != nullptr and sound_pack->active()) {
			string file = sound_pack->get_file(filename);
			size_t size = sound_pack->get_file_size(filename);
			return this->load_from_memory(file, size, set3d);
		}
		handle_ = new ma_sound;
		ma_result loading_result = ma_sound_init_from_file(&sound_default_mixer, result.c_str(), 0, NULL, NULL, handle_);
		if (loading_result != MA_SUCCESS) {
			active = false;
			return false;
		}
		active = true;
		file = result;
		if (sound_global_hrtf)
			this->set_hrtf(true);
		ma_sound_set_rolloff(handle_, 2);
		ma_sound_set_end_callback(handle_, at_stop, current_fx);
		return true;
	}
	bool load_from_memory(string data, size_t stream_size, bool set3d) {
		if (active)this->close();
		handle_ = new ma_sound;
		ma_result r = ma_decoder_init_memory(data.c_str(), stream_size, NULL, &decoder);
		if (r != MA_SUCCESS)return false;
		ma_result loading_result = ma_sound_init_from_data_source(&sound_default_mixer, &decoder, 0, 0, handle_);
		if (loading_result != MA_SUCCESS) {
			active = false;
			return false;
		}
		decoderInitialized = true;
		active = true;
		if (sound_global_hrtf)
			this->set_hrtf(true);
		ma_sound_set_end_callback(handle_, at_stop, current_fx);

		return active;
	}
	bool stream(const string& filename, bool set3d) {
		string result;
		if (sound_path != "") {
			result = sound_path + "/" + filename.c_str();
		}
		else {
			result = filename;
		}
		handle_ = new ma_sound;
		ma_result loading_result = ma_sound_init_from_file(&sound_default_mixer, result.c_str(), MA_SOUND_FLAG_NO_SPATIALIZATION | MA_SOUND_FLAG_STREAM, NULL, NULL, handle_);
		if (loading_result != MA_SUCCESS) {
			active = false;
			return false;
		}
		active = true;
		file = result;
		if (sound_global_hrtf)
			this->set_hrtf(true);
		ma_sound_set_end_callback(handle_, at_stop, current_fx);

		return true;
	}
	bool load_url(const string& url, bool set3d) {
		handle_ = new ma_sound;
		ma_sound_config c;

		ma_decoder decoder;
		vector<float> audio = load_audio_from_url(url.c_str());
		ma_decoder_init_memory(audio.data(), audio.size(), NULL, &decoder);
		c = ma_sound_config_init();
		c.pDataSource = &decoder;

		if (!set3d)
			c.flags |= MA_SOUND_FLAG_NO_SPATIALIZATION;
		ma_sound_init_ex(&sound_default_mixer, &c, handle_);
		active = true;
		if (sound_global_hrtf)
			this->set_hrtf(true);
		ma_sound_set_end_callback(handle_, at_stop, current_fx);

		return active;
	}
	string push_memory() {
		if (!active)return"";
		char* source = (char*)ma_sound_get_data_source(handle_);
		return string(source);
	}
	string get_file_path() {
		return this->file;
	}
	void set_faid_time(float volume_beg, float volume_end, float time) {
		ma_sound_set_fade_in_milliseconds(handle_, volume_beg / 100, volume_end / 100, static_cast<ma_uint64>(time));
	}
	bool play() {
		if (!active)return false;
		ma_sound_set_looping(handle_, false);
		ma_sound_start(handle_);
		return true;
	}
	bool play_looped() {
		if (!active)return false;

		ma_sound_set_looping(handle_, true);
		ma_sound_start(handle_);
		return true;
	}
	bool pause() {
		if (!active)return false;
		ma_sound_stop(handle_);
		return true;
	}
	bool play_wait() {
		this->play();
		while (true) {
			wait(1);
			bool ac = sound::is_playing();
			if (ac == false) {
				break;
			}
		}
		return true;
	}

	bool stop() {
		if (!active)return false;
		ma_sound_stop(handle_);
		ma_sound_seek_to_pcm_frame(handle_, 0);
	}
	bool close() {
		if (!active)return false;
		if (reverb) {
			ma_reverb_node_uninit(&g_reverbNode, NULL);
			reverb = false;
		}
		if (vocoder) {

			ma_vocoder_node_uninit(&g_vocoderNode, NULL);
			vocoder = false;
		}
		if (delayf) {
			ma_delay_node_uninit(&g_delayNode, NULL);
			delayf = false;
		}
		if (ltrim) {
			ma_ltrim_node_uninit(&g_trimNode, NULL);
			ltrim = false;
		}
		if (combiner) {
			ma_channel_combiner_node_uninit(&g_combinerNode, NULL);
			combiner = false;
		}
		if (separator) {
			ma_channel_separator_node_uninit(&g_separatorNode, NULL);
			separator = false;
		}
		if (hp) {
			ma_hpf_node_uninit(&highpass, NULL);
			hp = false;

		}
		if (lp) {
			ma_lpf_node_uninit(&lowpass, NULL);
			lp = false;

		}
		if (notchf) {

			ma_notch_node_uninit(&notch, NULL);
			notchf = false;
		}
		if (sound_hrtf == true) {

			ma_steamaudio_binaural_node_uninit(&g_binauralNode, NULL);
			sound_hrtf = false;


		}
		current_fx = nullptr;
		if (decoderInitialized) {
			ma_decoder_uninit(&decoder);
			decoderInitialized = false;
		}
		ma_sound_uninit(handle_);
		delete handle_;
		file = "";
		handle_ = nullptr;
		active = false;
		return true;
	}
	void set_fx(const string& fx) {
		if (!active)return;
		if (fx == "reverb") {
			reverbNodeConfig = ma_reverb_node_config_init(engineConfig.channels, engineConfig.sampleRate, 100, 100, 100);
			if (reverb) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);
				ma_reverb_node_uninit(&g_reverbNode, NULL);
				reverb = false;
			}
			ma_reverb_node_init(ma_engine_get_node_graph(&sound_default_mixer), &reverbNodeConfig, NULL, &g_reverbNode);
			ma_node_attach_output_bus(&g_reverbNode, 0, current_fx, 0);
			ma_node_attach_output_bus(handle_, 0, &g_reverbNode, 0);
			current_fx = &g_reverbNode;
			reverb = true;
		}
		if (fx == "vocoder") {
			vocoderNodeConfig = ma_vocoder_node_config_init(engineConfig.channels, engineConfig.sampleRate);
			if (vocoder) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);

				ma_vocoder_node_uninit(&g_vocoderNode, NULL);
				vocoder = false;
			}
			ma_vocoder_node_init(ma_engine_get_node_graph(&sound_default_mixer), &vocoderNodeConfig, NULL, &g_vocoderNode);
			ma_node_attach_output_bus(&g_vocoderNode, 0, current_fx, 0);
			ma_node_attach_output_bus(handle_, 0, &g_vocoderNode, 0);
			current_fx = &g_vocoderNode;
			vocoder = true;
		}
		if (fx == "delay") {
			delayNodeConfig = ma_delay_node_config_init(engineConfig.channels, engineConfig.sampleRate, (100 * engineConfig.sampleRate) / 1000, 0.5f);
			if (delayf) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);
				ma_delay_node_uninit(&g_delayNode, NULL);
				delayf = false;
			}
			ma_delay_node_init(ma_engine_get_node_graph(&sound_default_mixer), &delayNodeConfig, NULL, &g_delayNode);
			ma_node_attach_output_bus(&g_delayNode, 0, current_fx, 0);
			ma_node_attach_output_bus(handle_, 0, &g_delayNode, 0);
			current_fx = &g_delayNode;
			delayf = true;
		}
		if (fx == "ltrim") {
			trimNodeConfig = ma_ltrim_node_config_init(engineConfig.channels, 0);
			trimNodeConfig.threshold = 0.3;
			if (ltrim) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);
				ma_ltrim_node_uninit(&g_trimNode, NULL);
				ltrim = false;
			}
			ma_ltrim_node_init(ma_engine_get_node_graph(&sound_default_mixer), &trimNodeConfig, NULL, &g_trimNode);
			ma_node_attach_output_bus(&g_trimNode, 0, current_fx, 0);
			ma_node_attach_output_bus(handle_, 0, &g_trimNode, 0);
			current_fx = &g_trimNode;

			ltrim = true;
		}
		if (fx == "channelsplit") {
			combinerNodeConfig = ma_channel_combiner_node_config_init(engineConfig.channels);
			if (combiner) {
				ma_channel_combiner_node_uninit(&g_combinerNode, NULL);
				combiner = false;
			}
			ma_channel_combiner_node_init(ma_engine_get_node_graph(&sound_default_mixer), &combinerNodeConfig, NULL, &g_combinerNode);
			ma_node_attach_output_bus(&g_combinerNode, 0, ma_engine_get_endpoint(&sound_default_mixer), 0);
			combiner = true;
			separatorNodeConfig = ma_channel_separator_node_config_init(engineConfig.channels);
			if (separator) {
				ma_channel_separator_node_uninit(&g_separatorNode, NULL);
				separator = false;
			}
			ma_channel_separator_node_init(ma_engine_get_node_graph(&sound_default_mixer), &separatorNodeConfig, NULL, &g_separatorNode);
			MA_ASSERT(ma_node_get_output_bus_count(&g_separatorNode) == ma_node_get_input_bus_count(&g_combinerNode));
			for (ma_uint32 iChannel = 0; iChannel < ma_node_get_output_bus_count(&g_separatorNode); iChannel += 1) {
				ma_node_attach_output_bus(&g_separatorNode, iChannel, &g_combinerNode, iChannel);
			}

			ma_node_attach_output_bus(handle_, 0, &g_separatorNode, 0);
			separator = true;
		}
		if (fx == "highpass") {
			highpassConfig = ma_hpf_node_config_init(engineConfig.channels, engineConfig.sampleRate, 600, -10);
			if (hp) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);

				ma_hpf_node_uninit(&highpass, NULL);
				hp = false;
			}
			ma_hpf_node_init(ma_engine_get_node_graph(&sound_default_mixer), &highpassConfig, NULL, &highpass);
			ma_node_attach_output_bus(&highpass, 0, current_fx, 0);
			ma_node_attach_output_bus(handle_, 0, &highpass, 0);
			current_fx = &highpass;

			hp = true;
		}
		if (fx == "lowpass") {
			lowpassConfig = ma_lpf_node_config_init(engineConfig.channels, engineConfig.sampleRate, 600, -10);
			if (lp) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);

				ma_lpf_node_uninit(&lowpass, NULL);
				lp = false;
			}
			ma_lpf_node_init(ma_engine_get_node_graph(&sound_default_mixer), &lowpassConfig, NULL, &lowpass);
			ma_node_attach_output_bus(&lowpass, 0, current_fx, 0);
			ma_node_attach_output_bus(handle_, 0, &lowpass, 0);
			current_fx = &lowpass;

			lp = true;
		}
		if (fx == "notch") {
			notchConfig = ma_notch_node_config_init(engineConfig.channels, engineConfig.sampleRate, 0, 300);
			if (notchf) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);

				ma_notch_node_uninit(&notch, NULL);
				notchf = false;
			}
			ma_notch_node_init(ma_engine_get_node_graph(&sound_default_mixer), &notchConfig, NULL, &notch);
			ma_node_attach_output_bus(&notch, 0, current_fx, 0);
			ma_node_attach_output_bus(handle_, 0, &notch, 0);
			current_fx = &notch;

			notchf = true;
		}
	}
	void delete_fx(const string& fx) {
		if (!active)return;
		if (fx == "reverb") {
			if (reverb) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);
				ma_reverb_node_uninit(&g_reverbNode, NULL);
				reverb = false;
			}
			ma_node_attach_output_bus(handle_, 0, current_fx, 0);
		}
		if (fx == "vocoder") {
			if (vocoder) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);

				ma_vocoder_node_uninit(&g_vocoderNode, NULL);
				vocoder = false;
			}
			ma_node_attach_output_bus(handle_, 0, current_fx, 0);
		}
		if (fx == "delay") {
			if (delayf) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);
				ma_delay_node_uninit(&g_delayNode, NULL);
				delayf = false;
			}
			ma_node_attach_output_bus(handle_, 0, current_fx, 0);
		}
		if (fx == "ltrim") {
			if (ltrim) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);
				ma_ltrim_node_uninit(&g_trimNode, NULL);
				ltrim = false;
			}
			ma_node_attach_output_bus(handle_, 0, current_fx, 0);
		}
		if (fx == "channelsplit") {
			combinerNodeConfig = ma_channel_combiner_node_config_init(engineConfig.channels);
			if (combiner) {
				ma_channel_combiner_node_uninit(&g_combinerNode, NULL);
				combiner = false;
			}
			if (separator) {
				ma_channel_separator_node_uninit(&g_separatorNode, NULL);
				separator = false;
			}
		}
		if (fx == "highpass") {
			if (hp) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);

				ma_hpf_node_uninit(&highpass, NULL);
				hp = false;
			}
			ma_node_attach_output_bus(handle_, 0, current_fx, 0);

		}
		if (fx == "lowpass") {
			if (lp) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);

				ma_lpf_node_uninit(&lowpass, NULL);
				lp = false;
			}
			ma_node_attach_output_bus(handle_, 0, current_fx, 0);

		}
		if (fx == "notch") {
			if (notchf) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);

				ma_notch_node_uninit(&notch, NULL);
				notchf = false;
			}
			ma_node_attach_output_bus(handle_, 0, current_fx, 0);

		}
	}

	void set_reverb_parameters(float dry, float wet, float room_size, float damping, float mode) {
		if (!active)return;
		verblib_set_dry(&g_reverbNode.reverb, dry);
		verblib_set_wet(&g_reverbNode.reverb, wet);
		verblib_set_room_size(&g_reverbNode.reverb, room_size);
		verblib_set_damping(&g_reverbNode.reverb, damping);
		verblib_set_mode(&g_reverbNode.reverb, mode);
	}
	void set_delay_parameters(float dry, float wet, float dcay) {
		if (!active)return;
		ma_delay_node_set_dry(&g_delayNode, dry);
		ma_delay_node_set_wet(&g_delayNode, wet);
		ma_delay_node_set_decay(&g_delayNode, dcay);
	}
	void set_position(float listener_x, float listener_y, float listener_z, float source_x, float source_y, float source_z, double theta, float pan_step, float volume_step, float behind_pitch_decrease, float start_pan, float start_volume, float start_pitch) {
		if (!active) return;
		float delta_x = 0;
		float delta_y = 0;
		float delta_z = 0;
		float final_pan = start_pan;
		float final_volume = start_volume;
		float final_pitch = start_pitch;
		float rotational_source_x = source_x;
		float rotational_source_y = source_y;
		// First, we calculate the x and y based on the theta the listener is facing. 
		if (theta > 0.0)
		{
			rotational_source_x = (cos(theta) * (source_x - listener_x)) - (sin(theta) * (source_y - listener_y)) + listener_x;
			rotational_source_y = (sin(theta) * (source_x - listener_x)) + (cos(theta) * (source_y - listener_y)) + listener_y;
			source_x = rotational_source_x;
			source_y = rotational_source_y;
		}
		// Next, we calculate the delta between the listener and the source.
		if (source_x < listener_x)
		{
			delta_x = listener_x - source_x;
			final_pan -= (delta_x * pan_step);
			final_volume -= (delta_x * volume_step);
		}
		if (source_x > listener_x)
		{
			delta_x = source_x - listener_x;
			final_pan += (delta_x * pan_step);
			final_volume -= (delta_x * volume_step);
		}
		if (source_y < listener_y)
		{
			final_pitch -= abs(behind_pitch_decrease);
			delta_y = listener_y - source_y;
			final_volume -= (delta_y * volume_step);
		}
		if (source_y > listener_y)
		{
			delta_y = source_y - listener_y;
			final_volume -= (delta_y * volume_step);
		}
		if (source_z < listener_z)
		{
			final_pitch -= abs(behind_pitch_decrease);
			delta_z = listener_z - source_z;
			final_volume -= (delta_z * volume_step);
		}
		if (source_z > listener_z)
		{
			delta_z = source_z - listener_z;
			final_volume -= (delta_z * volume_step);
		}
		// Then we check if the calculated values are out of range, and fix them if that's the case.
		if (final_pan < -100)
		{
			final_pan = -100;
		}
		if (final_pan > 100)
		{
			final_pan = 100;
		}
		if (final_volume < -100)
		{
			final_volume = -100;
		}
		// Now we set the properties on the sound, provided that they are not already correct.
		ma_steamaudio_binaural_node_set_direction(&g_binauralNode, source_x - listener_x, source_y - listener_y, source_z - listener_z);
		if (this->get_pan() != final_pan)
			this->set_pan(final_pan);
		if (this->get_volume() != final_volume)
			this->set_volume(final_volume);
		if (this->get_pitch() != final_pitch)
			this->set_pitch(final_pitch);
	}
	void set_position(const ngtvector* listener = nullptr, const ngtvector* source = nullptr) {
		if (!active)return;

	}
	void set_hrtf(bool hrtf) {
		if (!active)return;
		if (hrtf) {

			binauralNodeConfig = ma_steamaudio_binaural_node_config_init(CHANNELS, iplAudioSettings, iplContext, iplHRTF);
			if (sound_hrtf == true) {
				current_fx = ma_engine_get_endpoint(&sound_default_mixer);

				ma_steamaudio_binaural_node_uninit(&g_binauralNode, NULL);
				sound_hrtf = false;
			}
			g_binauralNode.handle_ = *this->handle_;
			ma_steamaudio_binaural_node_init(ma_engine_get_node_graph(&sound_default_mixer), &binauralNodeConfig, NULL, &g_binauralNode);
			/* Connect the output of the delay node to the input of the endpoint. */
			ma_node_attach_output_bus(&g_binauralNode, 0, current_fx, 0);
			ma_node_attach_output_bus(handle_, 0, &g_binauralNode, 0);
			current_fx = &g_binauralNode;
			ma_sound_set_directional_attenuation_factor(handle_, 0);
			sound_hrtf = true;
		}
		else {
			if (sound_hrtf == true) {

				ma_steamaudio_binaural_node_uninit(&g_binauralNode, NULL);
				sound_hrtf = false;
				ma_sound_set_directional_attenuation_factor(handle_, 1);

				current_fx = ma_engine_get_endpoint(&sound_default_mixer);
				ma_node_attach_output_bus(handle_, 0, current_fx, 0);

			}

		}
	}
	bool get_hrtf() {
		return sound_hrtf;
	}
	void set_volume_step(float volume_step) {
		if (!active)return;
		ma_sound_set_rolloff(handle_, volume_step);
	}
	void set_pan_step(float pan_step) {
		if (!active)return;
		ma_sound_set_directional_attenuation_factor(handle_, pan_step);
	}
	void set_pitch_step(float pitch_step) {
		if (!active)return;
		ma_sound_set_doppler_factor(handle_, pitch_step);
	}
	bool seek(float new_position) {
		if (!active)return false;
		if (new_position > this->get_length())
			return false;
		ma_sound_seek_to_pcm_frame(handle_, static_cast<ma_uint64>(new_position * 100));
		return true;
	}
	void set_looping(bool looping) {
		if (!active)return;
		ma_sound_set_looping(handle_, looping);
	}
	bool get_looping()const {
		if (!active)return false;
		return ma_sound_is_looping(handle_);
	}
	float get_pan() const {
		if (!active)return -17435;

		float pan = 0;
		pan = ma_sound_get_pan(handle_);
		return pan * 100;
	}

	void set_pan(float pan) {
		if (!active)return;
		ma_sound_set_pan(handle_, pan / 100);
	}

	float get_volume() const {
		if (!active)return -17435;

		float volume = 0;

		volume = ma_sound_get_volume(handle_);
		return ma_volume_linear_to_db(volume);
	}
	void set_volume(float volume) {
		if (!active)return;
		if (volume > 0 or volume < -100)return;
		ma_sound_set_volume(handle_, ma_volume_db_to_linear(volume));
	}
	float get_pitch() const {
		if (!active)return -17435;
		float pitch = 0;
		pitch = ma_sound_get_pitch(handle_);
		return pitch * 100;
	}

	void set_pitch(float pitch) {
		if (!active)return;
		ma_sound_set_pitch(handle_, pitch / 100);
	}
	void set_speed(float speed) {
		if (!speeding) {
			g_speed_config = ma_playback_speed_node_config_init(engineConfig.channels, engineConfig.sampleRate);
			ma_playback_speed_node_init(ma_engine_get_node_graph(&sound_default_mixer), &g_speed_config, NULL, &g_playback_speed_node);
			ma_node_attach_output_bus(&g_playback_speed_node, 0, current_fx, 0);
			ma_node_attach_output_bus(handle_, 0, &g_playback_speed_node, 0);
			current_fx = &g_playback_speed_node;
			speeding = true;
		}
		g_speed_config.playbackSpeed = speed / 100;
	}
	float get_speed()const {
		return g_speed_config.playbackSpeed * 100;
	}
	bool is_active() const {
		return active || handle_ != nullptr;
	}

	bool is_playing() const {
		if (!active)return false;
		return ma_sound_is_playing(handle_);
	}

	bool is_paused() const {
		if (!active)return false;
	}

	float get_position() {
		if (!active)return -17435;
		float position = 0;
		position = ma_sound_get_time_in_milliseconds(handle_);
		return position;
	}

	float get_length() {
		if (!active)return-17435;
		ma_uint64 length = 0;
		ma_sound_get_length_in_pcm_frames(handle_, &length);
		return static_cast<float>(length / 100);
	}

	void set_length(float length = 0.0) {
		if (!active)return;
		if (length > this->get_length())            return;
		ma_sound_set_stop_time_in_pcm_frames(handle_, static_cast<ma_uint64>(length * 100));
	}
	float get_sample_rate() const {
		float rate = 0;
		return rate;
	}
};
void set_sound_global_hrtf(bool hrtf) {
	sound_global_hrtf = hrtf;
}
bool get_sound_global_hrtf() {
	return sound_global_hrtf;
}
ma_result ma_encoder_write_callback(ma_encoder* encoder, const void* buffer, size_t bytesToWrite, size_t* pBytesWritten) {
	if (encoder == nullptr) {
		return MA_INVALID_ARGS;
	}

	MemoryStream* stream = reinterpret_cast<MemoryStream*>(encoder->pUserData);
	if (buffer == nullptr || stream == nullptr)return MA_INVALID_ARGS;
	stream->write((const char*)buffer, bytesToWrite);
	return MA_SUCCESS;
}



ma_result ma_encoder_seek_callback(ma_encoder* pEncoder, ma_int64 offset, ma_seek_origin origin) {
	MemoryStream* stream = reinterpret_cast<MemoryStream*>(pEncoder->pUserData);
	seek_origin so;
	switch (origin) {
	case ma_seek_origin_start: {
		so = seek_origin_start;
		break;
	}
	case ma_seek_origin_current: {
		so = seek_origin_current;
		break;
	}
	case ma_seek_origin_end: {
		so = seek_origin_end;
		break;
	}
	default:return MA_ERROR;
	}
	stream->seek(so, offset);
	return MA_SUCCESS;
}

class MINIAUDIO_IMPLEMENTATION audio_encoder {
public:
	MemoryStream stream;
	ma_encoder_config encoderConfig;
	ma_encoder encoder;

	audio_encoder() : stream(0) {
		encoderConfig = ma_encoder_config_init(ma_encoding_format_wav, ma_format_s16, 2, 44100);
		ma_encoder_init(ma_encoder_write_callback, ma_encoder_seek_callback, &stream, &encoderConfig, &encoder);
	}
	~audio_encoder() {
		ma_encoder_uninit(&encoder);
	}
	void encode(const std::string& audio_data) {
		ma_encoder_write_pcm_frames(&encoder, audio_data.c_str(), audio_data.size(), nullptr);
	}
	std::string get_data() {
		stream.seek(0);
		size_t dataSize = stream.size();
		std::vector<char> buffer(dataSize);
		stream.read(buffer.data(), dataSize);
		return std::string(buffer.data(), dataSize);
	}


};

void audio_recorder_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	audio_encoder* encoder = static_cast<audio_encoder*>(pDevice->pUserData);
	ma_encoder_write_pcm_frames(&encoder->encoder, pInput, frameCount, nullptr);

	(void)pOutput;
}

class MINIAUDIO_IMPLEMENTATION audio_recorder {
public:
	audio_encoder encoder;
	ma_device_config deviceConfig;
	ma_device recording_device;

	void start() {
		deviceConfig = ma_device_config_init(ma_device_type_capture);
		deviceConfig.capture.format = ma_format_s16;
		deviceConfig.capture.channels = 2;
		deviceConfig.sampleRate = 44100;
		deviceConfig.dataCallback = audio_recorder_callback;
		deviceConfig.pUserData = &encoder;

		ma_device_init(nullptr, &deviceConfig, &recording_device);
		ma_device_start(&recording_device);
	}

	void stop() {
		ma_encoder_uninit(&encoder.encoder);
		ma_device_uninit(&recording_device);
	}

	std::string get_data() {
		return encoder.get_data();
	}
};


ma_result audio_stream_callback(ma_decoder* pDecoder, void* buffer, size_t bytesToRead, size_t* pBytesRead) {
	string temp((char*)buffer, bytesToRead);
	temp.resize(bytesToRead);
	string* data = (string*)pDecoder->pUserData;
	*data = temp;
	return MA_SUCCESS;
}
ma_result audio_stream_seek_callback(ma_decoder* pDecoder, ma_int64 byteOffset, ma_seek_origin origin) {
	return MA_SUCCESS;
}

class audio_stream {
public:
	ma_device_config deviceConfig;
	ma_device device;
	ma_decoder decoder;
	audio_stream() {
		deviceConfig = ma_device_config_init(ma_device_type_playback);
		deviceConfig.playback.format = ma_format_f32;
		deviceConfig.playback.channels = 2;
		deviceConfig.sampleRate = 44100;
		//		deviceConfig.dataCallback = audio_recorder_callback;
		deviceConfig.pUserData = &decoder;

		//		ma_device_init(NULL, &deviceConfig, &recording_device);

	}
};


sound* fsound(const string& filename, bool set3d) { return new sound(filename, set3d); }
mixer* fmixer() { return new mixer; }
audio_recorder* faudio_recorder() { return new audio_recorder; }
audio_encoder* faudio_encoder() { return new audio_encoder; }

void register_sound(asIScriptEngine* engine) {
	engine->RegisterFuncdef("void sound_end_callback(const ?&in=null)");
	engine->RegisterGlobalFunction("void set_audio_period_size(uint)property", asFUNCTION(set_audio_period_size), asCALL_CDECL);
	engine->RegisterGlobalFunction("void set_sound_storage(const string &in)property", asFUNCTION(set_sound_storage), asCALL_CDECL);

	engine->RegisterGlobalFunction("string get_sound_storage()property", asFUNCTION(get_sound_storage), asCALL_CDECL);
	engine->RegisterGlobalFunction("void set_sound_pack(pack@)property", asFUNCTION(set_sound_pack), asCALL_CDECL);

	engine->RegisterGlobalFunction("pack@ get_sound_pack()property", asFUNCTION(get_sound_pack), asCALL_CDECL);
	engine->RegisterGlobalFunction("void set_master_volume(float)property", asFUNCTION(set_master_volume), asCALL_CDECL);
	engine->RegisterGlobalFunction("float get_master_volume()property", asFUNCTION(get_master_volume), asCALL_CDECL);
	engine->RegisterGlobalFunction("void mixer_start()", asFUNCTION(mixer_start), asCALL_CDECL);
	engine->RegisterGlobalFunction("void mixer_stop()", asFUNCTION(mixer_stop), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool mixer_play_sound(const string &in)", asFUNCTION(mixer_play_sound), asCALL_CDECL);
	engine->RegisterGlobalFunction("void mixer_reinit()", asFUNCTION(mixer_reinit), asCALL_CDECL);
	engine->RegisterGlobalFunction("array<string>@ get_output_audio_devices()", asFUNCTION(get_output_audio_devices), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool set_output_audio_device(uint)", asFUNCTION(set_output_audio_device), asCALL_CDECL);
	engine->RegisterObjectType("mixer", sizeof(mixer), asOBJ_REF | asOBJ_NOCOUNT);
	engine->RegisterObjectBehaviour("mixer", asBEHAVE_FACTORY, "mixer @m()", asFUNCTION(fmixer), asCALL_CDECL);
	engine->RegisterObjectType("sound", sizeof(sound), asOBJ_REF);
	engine->RegisterObjectBehaviour("sound", asBEHAVE_FACTORY, "sound@ s(const string &in=\"\", bool=false)", asFUNCTION(fsound), asCALL_CDECL);
	engine->RegisterObjectBehaviour("sound", asBEHAVE_ADDREF, "void f()", asMETHOD(sound, add), asCALL_THISCALL);
	engine->RegisterObjectBehaviour("sound", asBEHAVE_RELEASE, "void f()", asMETHOD(sound, release), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool load(const string &in, bool=false, sound_end_callback@=null)const", asMETHOD(sound, load), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool load_from_memory(string, size_t=0, bool=false)const", asMETHOD(sound, load_from_memory), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool stream(const string &in, bool=false)const", asMETHOD(sound, stream), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool load_url(const string &in, bool=false)const", asMETHOD(sound, load_url), asCALL_THISCALL);

	engine->RegisterObjectMethod("sound", "string push_memory()const", asMETHOD(sound, push_memory), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "string get_file_path()const property", asMETHOD(sound, get_file_path), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "void set_faid_time(float, float, float)const", asMETHOD(sound, set_faid_time), asCALL_THISCALL);

	engine->RegisterObjectMethod("sound", "bool play()const", asMETHOD(sound, play), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool play_looped()const", asMETHOD(sound, play_looped), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool pause()const", asMETHOD(sound, pause), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool play_wait()const", asMETHOD(sound, play_wait), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool stop()const", asMETHOD(sound, stop), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool close()const", asMETHOD(sound, close), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "void set_fx(const string &in, int=0)const", asMETHOD(sound, set_fx), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "void delete_fx(const string &in, int=0)const", asMETHOD(sound, delete_fx), asCALL_THISCALL);

	engine->RegisterObjectMethod("sound", "void set_reverb_parameters(float, float, float, float, float)const", asMETHOD(sound, set_reverb_parameters), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "void set_delay_parameters(float, float, float)const", asMETHOD(sound, set_delay_parameters), asCALL_THISCALL);

	engine->RegisterObjectMethod("sound", "void set_position(float, float, float, float, float, float, double=0.0, float=5, float=0.6, float=0, float=0, float=0, float=0)const", asMETHODPR(sound, set_position, (float listener_x, float listener_y, float listener_z, float source_x, float source_y, float source_z, double theta, float pan_step, float volume_step, float behind_pitch_decrease, float start_pan, float start_volume, float start_pitch), void), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "void set_position(const vector@=null, const vector@=null)const", asMETHODPR(sound, set_position, (const ngtvector*, const ngtvector*), void), asCALL_THISCALL);

	engine->RegisterObjectMethod("sound", "void set_hrtf(bool=true)const property", asMETHOD(sound, set_hrtf), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool get_hrtf()const property", asMETHOD(sound, get_hrtf), asCALL_THISCALL);

	engine->RegisterObjectMethod("sound", "void set_volume_step(float)const property", asMETHOD(sound, set_volume_step), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "void set_pan_step(float)const property", asMETHOD(sound, set_pan_step), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "void set_pitch_step(float)const property", asMETHOD(sound, set_pitch_step), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool seek(float)const", asMETHOD(sound, seek), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool get_looping() const property", asMETHOD(sound, get_looping), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "void set_looping(bool)const property", asMETHOD(sound, set_looping), asCALL_THISCALL);

	engine->RegisterObjectMethod("sound", "float get_pan() const property", asMETHOD(sound, get_pan), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "void set_pan(float)const property", asMETHOD(sound, set_pan), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "float get_volume() const property", asMETHOD(sound, get_volume), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "void set_volume(float)const property", asMETHOD(sound, set_volume), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "float get_pitch() const property", asMETHOD(sound, get_pitch), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "void set_pitch(float)const property", asMETHOD(sound, set_pitch), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "float get_speed() const property", asMETHOD(sound, get_speed), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "void set_speed(float)const property", asMETHOD(sound, set_speed), asCALL_THISCALL);

	engine->RegisterObjectMethod("sound", "bool get_active() const property", asMETHOD(sound, is_active), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool get_playing() const property", asMETHOD(sound, is_playing), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "bool get_paused() const property", asMETHOD(sound, is_paused), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "float get_position() const property", asMETHOD(sound, get_position), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "float get_length() const property", asMETHOD(sound, get_length), asCALL_THISCALL);
	engine->RegisterObjectMethod("sound", "void set_length(float=0.0) const property", asMETHOD(sound, set_length), asCALL_THISCALL);

	engine->RegisterObjectMethod("sound", "float get_sample_rate() const property", asMETHOD(sound, get_sample_rate), asCALL_THISCALL);
	engine->RegisterGlobalFunction("void set_sound_global_hrtf(bool)property", asFUNCTION(set_sound_global_hrtf), asCALL_CDECL);
	engine->RegisterGlobalFunction("bool get_sound_global_hrtf()property", asFUNCTION(get_sound_global_hrtf), asCALL_CDECL);
	engine->RegisterGlobalProperty("mixer sound_default_mixer", (void*)&sound_default_mixer);
	engine->RegisterObjectType("audio_recorder", sizeof(audio_recorder), asOBJ_REF | asOBJ_NOCOUNT);
	engine->RegisterObjectBehaviour("audio_recorder", asBEHAVE_FACTORY, "audio_recorder@outch()", asFUNCTION(faudio_recorder), asCALL_CDECL);
	engine->RegisterObjectMethod("audio_recorder", "void start()const", asMETHOD(audio_recorder, start), asCALL_THISCALL);
	engine->RegisterObjectMethod("audio_recorder", "void stop()const", asMETHOD(audio_recorder, stop), asCALL_THISCALL);
	engine->RegisterObjectMethod("audio_recorder", "string get_data()const property", asMETHOD(audio_recorder, get_data), asCALL_THISCALL);
	engine->RegisterObjectType("audio_encoder", sizeof(audio_encoder), asOBJ_REF | asOBJ_NOCOUNT);
	engine->RegisterObjectBehaviour("audio_encoder", asBEHAVE_FACTORY, "audio_encoder@ raw()", asFUNCTION(faudio_encoder), asCALL_CDECL);
	engine->RegisterObjectMethod("audio_encoder", "void encode(const string &in)const", asMETHOD(audio_encoder, encode), asCALL_THISCALL);
	engine->RegisterObjectMethod("audio_encoder", "string get_data()const property", asMETHOD(audio_encoder, get_data), asCALL_THISCALL);

}
