#include <SDL2/SDL.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define NANOSEC_IN_SEC 1000000000L

#define MUS_PATH "../../../sound_files/goat.wav"

timer_t timerID;

// prototype for our audio callback
// see the implementation for more information
void my_audio_callback(void *userdata, Uint8 *stream, int len);

// timer callback function
void timer_handler (int signum) {
	printf("starting audio!\n");
	SDL_PauseAudio(0);
}

// variable declarations
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play

/*
** PLAYING A SOUND IS MUCH MORE COMPLICATED THAN IT SHOULD BE
*/
int main(int argc, char* argv[]){

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return 1;

	// added for testing purposes
	printf("SDL initialized!\n");

	// local variables
	static Uint32 wav_length; // length of our sample
	static Uint8 *wav_buffer; // buffer containing our audio file
	static SDL_AudioSpec wav_spec; // the specs of our piece of music
	
	/* Load the WAV */
	// the specs, length and buffer of our wav are filled
	if( SDL_LoadWAV(MUS_PATH, &wav_spec, &wav_buffer, &wav_length) == NULL ){
		printf("couldn't load wav\n");
		return 1;
	}

	// set the callback function
	wav_spec.callback = my_audio_callback;
	wav_spec.userdata = NULL;

    //NEED TO CONFIGURE THE SPEC -> https://wiki.libsdl.org/SDL_AudioSpec
    //HOW TO GET THE SPECS?
    // more https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideaudioexamples.html

	// set our global static variables
	audio_pos = wav_buffer; // copy sound buffer
	audio_len = wav_length; // copy file length
	
	/* Open the audio device */
	if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	
	// Start playing
	//SDL_PauseAudio(0); <-- removing in favor of timer logic

	// BEGIN timer logic
	struct sigaction sa;
    struct itimerspec timer;

    memset (&sa, 0, sizeof (sa));
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timer_handler;

    sigemptyset(&sa.sa_mask);
    sigaction(SIGRTMIN, &sa, NULL);
    struct sigevent te;
    memset(&te, 0, sizeof(struct sigevent));

    te.sigev_notify = SIGEV_SIGNAL;
    te.sigev_signo = SIGRTMIN;
    te.sigev_value.sival_ptr = &timerID;
    timer_create(CLOCK_REALTIME, &te, &timerID);

    // create a buffer so the user can see times of interest
    char time_string[26];
    uint32_t epoch_target_secs;

    printf("use https://www.epochconverter.com to get epoch time\n");
    printf("enter target epoch time (sec): ");
    // have the user input the offset in seconds to start playback
    scanf("%d", &epoch_target_secs);

    struct timespec curr_pi_time;

    // the target time that we want audio playback to begin
    struct timespec target_time;

    // hardcode an arbitrary future time to start playback
    target_time.tv_sec = epoch_target_secs;
    target_time.tv_nsec = 0;

    timer.it_value.tv_sec = epoch_target_secs;
    timer.it_value.tv_nsec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_nsec = 0;

    timer_settime(timerID, TIMER_ABSTIME, &timer, NULL);

    strftime(time_string, 26, "%Y-%m-%d %H:%M:%S", localtime(&(target_time.tv_sec)));

    // print out pi's system time and target time
    printf("Playback should begin at: %s\n\n", time_string);

    printf("pi sec                : %d\n", curr_pi_time.tv_sec);
    printf("pi nsec               : %d\n", curr_pi_time.tv_nsec);

    // END timer logic

	// wait until we're don't playing
	while ( audio_len > 0 ) {
		SDL_Delay(5); 
	}
	
	// shut everything down
	SDL_CloseAudio();
	SDL_FreeWAV(wav_buffer);
}

// audio callback function
// here you have to copy the data of your audio buffer into the
// requesting audio buffer (stream)
// you should only copy as much as the requested length (len)
void my_audio_callback(void *userdata, Uint8 *stream, int len) {
	
	if (audio_len ==0)
		return;
	
	len = ( len > audio_len ? audio_len : len );

	// simply copy from one buffer into the other
	SDL_memcpy (stream, audio_pos, len);

	// mix from one buffer into another <-- doesn't work properly!!!
	//SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
	
	audio_pos += len;
	audio_len -= len;
}
