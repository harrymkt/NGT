#pragma once
#define MINIAUDIO_IMPLEMENTATION

#include "AL/al.h"
#include "AL/alc.h"
#include "sndfile.h"
#include "miniaudio.h"
#include "reverb/reverb.h"
#include <random>
#include <type_traits>
#include<chrono>
#include <string>
#include"sdl/SDL.h"
//#include "enet/enet.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "scriptdictionary/scriptdictionary.h"
class ngtvector {
public:
	void construct();
	void destruct();
	float x, y, z;
};


std::wstring wstr(const std::string & utf8String);

uint64_t get_time_stamp_millis();
uint64_t get_time_stamp_seconds();
void init_engine();
 long random(long min, long max);
 double randomDouble(double min, double max);
 int get_last_error();
void speak(const std::string &	 text, bool stop = true);
void speak_wait(const std::string &	 text, bool stop = true);
void stop_speech();
bool show_game_window(const std::string & title,int width=640, int height=480, bool closable=true);

void hide_game_window();
void set_game_window_title(const std::string & );
void set_game_window_closable(bool);
bool is_game_window_active();
void update_game_window();
void exit_engine(int=0);
bool clipboard_copy_text(const std::string&);
std::string clipboard_read_text();
std::string get_input();
bool key_pressed(SDL_Keycode);
bool key_released(SDL_Keycode);
bool key_down(SDL_Keycode);
bool key_repeat(SDL_Keycode);
bool alert(const std::string &	 title, const std::string &	 text, const std::string &button_name="OK");
int question(const std::string& title, const std::string &text);
void set_listener_position(float l_x, float l_y, float l_z);
void set_listener_position(ngtvector*);
void wait(int);
void delay(int);
void set_sound_storage(const std::string &);
std::string get_sound_storage();
void set_master_volume(float);
float get_master_volume();
std::string read_environment_variable(const std::string&);
class reverb {
public:

	ma_reverb_node_config c;
	void construct();
	void destruct();

	void set_input_gain(float);
	void set_reverb_mix(float);
	void  set_reverb_time(float);
	float get_input_gain();
	float get_reverb_mix();
	float  get_reverb_time();
};
class sound{
public:
	ALuint buffer_;
	ALuint source_;
	bool is_3d_;
	ma_sound handle_;
	short audio_system=0;
	bool playing=false, paused=false, active=false;
	ma_reverb_node rev;
	void construct();
	void destruct();

	bool load(const std::string &	 filename, bool set3d=false);
	bool load_from_memory(const std::string & data, bool set3d = false);
	bool play();
	bool play_looped();
	bool set_faid_parameters(float volume_beg, float volume_end, unsigned int time);
	bool pause();
	bool play_wait();
	bool stop();
	bool close();
	void set_sound_position(float s_x, float s_y, float s_z);
	void set_sound_position(ngtvector*);

	void set_sound_reverb(reverb* =NULL);
	bool seek(double);
	void set_sound_hrtf(bool = true);
	void cancel_reverb();
	double get_pan() const;
	void set_pan(double);
	double get_volume() const;
	void set_volume(double);
	double get_pitch() const;
	void set_pitch(double);
	double get_pitch_lower_limit() const;
	bool is_active() const;
	bool is_playing() const;
	bool is_paused() const;
	double get_position() const;
	double get_length() const;
	double get_sample_rate() const;
	double get_channels() const;
	double get_bits() const;
};
class timer {
public:

	std::chrono::time_point<std::chrono::steady_clock> inittime;
	uint64_t pausedNanos = 0;

	timer() {
		inittime = std::chrono::steady_clock::now();
		pausedNanos = 0;
	}
	void construct();
	void destruct();
	uint64_t elapsed_seconds();
	uint64_t elapsed_minutes();
	uint64_t elapsed_hours();
	uint64_t elapsed_millis();
	uint64_t elapsed_micros();
	uint64_t elapsed_nanos();
	void force_seconds(uint64_t);
	void force_minutes(uint64_t);
	void force_hours(uint64_t);
	void force_millis(uint64_t);
	void force_micros(uint64_t);
	void force_nanos(uint64_t);
	void restart();
	void pause();
	void resume();
	bool is_running();
};
class network_event {
public:
    const int EVENT_NONE = 0;
    const int EVENT_CONNECT = 1;
    const int EVENT_RECEIVE = 2;
    const int EVENT_DISCONNECT = 3;
	void construct();
	void destruct();
    int get_type() const {
        return m_type;
    }

unsigned    int get_peer_id() const {
        return m_peerId;
    }

unsigned    int get_channel() const {
        return m_channel;
    }

    std::string get_message() const {
        return m_message;
    }

    int m_type;
unsigned    int m_peerId;
    int m_channel;
    std::string m_message;
};

class network {
public:
	void construct();
	void destruct();
	unsigned    int connect(const std::string& host, int port);
	bool destroy();
	bool disconnect_peer(unsigned int);
	bool disconnect_peer_forcefully(unsigned int);
	bool disconnect_peer_softly(unsigned int);
	std::string get_peer_address(unsigned int);
	double get_peer_average_round_trip_time(unsigned int);
	CScriptArray* get_peer_list();
	network_event* request();
	bool send_reliable(unsigned int peerId, const std::string& packet, int channel);
	bool send_unreliable(unsigned int peerId, const std::string& packet, int channel);
	bool set_bandwidth_limits(double incomingBandwidth, double outgoingBandwidth);
	bool setup_client(int channels, int maxPeers);
	bool setup_server(int listeningPort, int channels, int maxPeers);

	int get_connected_peers() const;

	double get_bytes_sent() const;

	double get_bytes_received() const;

	bool is_active() const;

private:
	int m_connectedPeers;
	double m_bytesSent;
	double m_bytesReceived;
	bool m_active;
};
class library {
public:
	HMODULE lib;
	void construct();
	void destruct();
	bool load(const std::string &);
	CScriptDictionary* call(const std::string function_name, ...);
	void unload();
};
class instance {
private:
	HANDLE mutex;
public:
	instance(const std::string & application_name) {
		mutex = CreateMutexA(NULL, TRUE, application_name.c_str());
	}
	void construct();
	void destruct();
	bool is_running();
	~instance() {
		CloseHandle(mutex);
	}
};
class user_idle {
public:
    user_idle();
    uint64_t elapsed_millis();  // Pridaná nová funkcia
    uint64_t elapsed_seconds();
    uint64_t elapsed_minutes();
    uint64_t elapsed_hours();
    uint64_t elapsed_days();
    uint64_t elapsed_weeks();

private:
    uint64_t get_idle_time();
};
