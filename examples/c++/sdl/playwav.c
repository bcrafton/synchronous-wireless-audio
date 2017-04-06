#include <SDL2/SDL.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define NANOSEC_IN_SEC 1000000000L

#define MUS_PATH "../../../sound_files/goat.wav"

// prototype for our audio callback
// see the implementation for more information
void my_audio_callback(void *userdata, Uint8 *stream, int len);

// timer callback function
void timer_handler (int signum) {
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
    struct itimerval timer;

    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction (SIGALRM, &sa, NULL);

    // create a buffer so the user can see times of interest
    char time_string[26];
    uint32_t offset;

    struct timespec curr_pi_time;
    uint32_t sec_offset;
    uint32_t usec_offset;
    uint32_t nsec_offset;

    // get the current time on the pi
    clock_gettime(CLOCK_REALTIME, &curr_pi_time);

    strftime(time_string, 26, "%Y-%m-%d %H:%M:%S", localtime(&(curr_pi_time.tv_sec)));

    printf("Current time is: [ %s ]. Please enter offset in seconds: \n", time_string);
    // have the user input the offset in seconds to start playback
    scanf("%d", &offset);
    printf("offset is: %d seconds\n", offset);

    // the target time that we want audio playback to begin
    struct timespec target_time;

    // hardcode an arbitrary future time to start playback
    target_time.tv_sec = curr_pi_time.tv_sec + offset;
    target_time.tv_nsec = 0;

    sec_offset = target_time.tv_sec - curr_pi_time.tv_sec;

    if (curr_pi_time.tv_nsec > target_time.tv_nsec)
    {
        sec_offset--;
        nsec_offset = NANOSEC_IN_SEC - curr_pi_time.tv_nsec + target_time.tv_nsec;
    } else
    {
        nsec_offset = target_time.tv_nsec - curr_pi_time.tv_nsec;
    }

    usec_offset = nsec_offset / 1000;

    timer.it_value.tv_sec = sec_offset;
    timer.it_value.tv_usec = usec_offset;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    setitimer (ITIMER_REAL, &timer, NULL);

    strftime(time_string, 26, "%Y-%m-%d %H:%M:%S", localtime(&(target_time.tv_sec)));

    // print out pi's system time and target time
    printf("Playback should begin at: %s\n\n", time_string);

    printf("pi sec                : %d\n", curr_pi_time.tv_sec);
    printf("target sec            : %d\n", target_time.tv_sec);
    printf("pi nsec               : %d\n", curr_pi_time.tv_nsec);
    printf("target nsec           : %d\n", target_time.tv_nsec);
    printf("sec_offset            : %d\n", sec_offset);
    printf("usec_offset           : %d\n", usec_offset);

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
