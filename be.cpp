/*========================================
 *    sl.c: SL version 5.03
 *        Copyright 1993,1998,2014-2015
 *                  Toyoda Masashi
 *                  (mtoyoda@acm.org)
 *        Last Modified: 2014/06/03
 *
 *	Modified by ya0201: 2019/12/22
 *========================================
 */
/* be version 1.00 : Initial build executioner.                              */
/*                                              by ya0201         2019/12/22 */
/* sl version 5.03 : Fix some more compiler warnings.                        */
/*                                              by Ryan Jacobs    2015/01/19 */
/* sl version 5.02 : Fix compiler warnings.                                  */
/*                                              by Jeff Schwab    2014/06/03 */
/* sl version 5.01 : removed cursor and handling of IO                       */
/*                                              by Chris Seymour  2014/01/03 */
/* sl version 5.00 : add -c option                                           */
/*                                              by Toyoda Masashi 2013/05/05 */
/* sl version 4.00 : add C51, usleep(40000)                                  */
/*                                              by Toyoda Masashi 2002/12/31 */
/* sl version 3.03 : add usleep(20000)                                       */
/*                                              by Toyoda Masashi 1998/07/22 */
/* sl version 3.02 : D51 flies! Change options.                              */
/*                                              by Toyoda Masashi 1993/01/19 */
/* sl version 3.01 : Wheel turns smoother                                    */
/*                                              by Toyoda Masashi 1992/12/25 */
/* sl version 3.00 : Add d(D51) option                                       */
/*                                              by Toyoda Masashi 1992/12/24 */
/* sl version 2.02 : Bug fixed.(dust remains in screen)                      */
/*                                              by Toyoda Masashi 1992/12/17 */
/* sl version 2.01 : Smoke run and disappear.                                */
/*                   Change '-a' to accident option.                         */
/*                                              by Toyoda Masashi 1992/12/16 */
/* sl version 2.00 : Add a(all),l(long),F(Fly!) options.                     */
/*                                              by Toyoda Masashi 1992/12/15 */
/* sl version 1.02 : Add turning wheel.                                      */
/*                                              by Toyoda Masashi 1992/12/14 */
/* sl version 1.01 : Add more complex smoke.                                 */
/*                                              by Toyoda Masashi 1992/12/14 */
/* sl version 1.00 : SL runs vomiting out smoke.                             */
/*                                              by Toyoda Masashi 1992/12/11 */

#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include "be.h"

// int add_BE();
// int my_mvaddstr(int y, int x, char *str);

int my_mvaddstr(int y, int x, char *str) {
    for ( ; x < 0; ++x, ++str)
        if (*str == '\0')  return ERR;
    for ( ; *str != '\0'; ++str, ++x)
        if (mvaddch(y, x, *str) == ERR)  return ERR;
    return OK;
}

int add_BE() {
  static char *BE[BEHEIGHT]
    = {BE01, 
      BE02, 
      BE03, 
      BE04, 
      BE05, 
      BE06, 
      BE07, 
      BE08, 
      BE09, 
      BE10, 
      BE11, 
      BE12, 
      BE13, 
      BE14, 
      BE15, 
      BE16, 
      BE17, 
      BE18, 
      BE19, 
      BE20, 
      BE21, 
      BE22, 
      BE23, 
      BE24, 
      BE25, 
      BE26, 
      BE27, 
      BE28, 
      BE29, 
      BE30, 
      BE31, 
      BE32, 
      BE33, 
      BE34, 
      BE35, 
      BE36, 
      BE37, 
      BE38, 
      BE39, 
      BE40, 
      BE41, 
      BE42, 
      BE43};
  int x, y, i;

  x = COLS / 2 - (BELENGTH / 2);
  y = LINES / 2 - (BEHEIGHT / 2);
  if (x < - BELENGTH)  return ERR;
  for (i = 0; i < BEHEIGHT; ++i) {
    my_mvaddstr(y + i, x, BE[i]);
  }
  return OK;
}


//check big vs little endian machine
static bool IsBigEndian(void) {
  int a = 1;
  return !((char*)&a)[0];
}

static int ConvertToInt(char* buffer, int len) {
  int a = 0;

  if(!IsBigEndian()) {
    for(int i = 0; i < len; ++i) {
      ((char*)&a)[i] = buffer[i];
    }
  } else {
    for(int i = 0; i < len; ++i) {
      ((char*)&a)[3-i] = buffer[i];
    }
  }

  return a;
}

//Location and size of data is found here: http://www.topherlee.com/software/pcm-tut-wavformat.html
static char* LoadWAV(std::string filename, int& channels, int& sampleRate, int& bps, int& size) {
  char buffer[4];

  std::ifstream in(filename.c_str());
  in.read(buffer, 4);

  if(strncmp(buffer, "RIFF", 4) != 0) {
    std::cout << "Error here, not a valid WAV file, RIFF not found in header\n This was found instead: "
              << buffer[0] << buffer[1] << buffer[2] << buffer[3] << std::endl;
  }

  in.read(buffer, 4);//size of file. Not used. Read it to skip over it.  

  in.read(buffer, 4);//Format, should be WAVE

  if(strncmp(buffer, "WAVE", 4) != 0) {
    std::cout << "Error here, not a valid WAV file, WAVE not found in header.\n This was found instead: "
              << buffer[0] << buffer[1] << buffer[2] << buffer[3] << std::endl;
  }

  in.read(buffer, 4);//Format Space Marker. should equal fmt (space)

  if(strncmp(buffer, "fmt ", 4) != 0) {
    std::cout << "Error here, not a valid WAV file, Format Marker not found in header.\n This was found instead: "
              << buffer[0] << buffer[1] << buffer[2] << buffer[3] << std::endl;
  }

  in.read(buffer, 4);//Length of format data. Should be 16 for PCM, meaning uncompressed.

  if(ConvertToInt(buffer, 4) != 16) {
    std::cout << "Error here, not a valid WAV file, format length wrong in header.\n This was found instead: "
              << ConvertToInt(buffer, 4) << std::endl;
  }

  in.read(buffer, 2);//Type of format, 1 = PCM

  if(ConvertToInt(buffer, 2) != 1) {
    std::cout << "Error here, not a valid WAV file, file not in PCM format.\n This was found instead: "
              << ConvertToInt(buffer, 4) << std::endl;
  }

  in.read(buffer, 2);//Get number of channels. 

  //Assume at this point that we are dealing with a WAV file. This value is needed by OpenAL
  channels = ConvertToInt(buffer, 2);

  in.read(buffer, 4);//Get sampler rate. 

  sampleRate = ConvertToInt(buffer, 4);

  //Skip Byte Rate and Block Align. Maybe use later?
  in.read(buffer, 4);//Block Align
  in.read(buffer, 2);//ByteRate

  in.read(buffer, 2);//Get Bits Per Sample

  bps = ConvertToInt(buffer, 2);

  //Skip character data, which marks the start of the data that we care about. 
  in.read(buffer, 4);//"data" chunk. 

  in.read(buffer, 4); //Get size of the data

  size = ConvertToInt(buffer, 4);

  if(size < 0) {
    std::cout << "Error here, not a valid WAV file, size of file reports 0.\n This was found instead: "
              << size << std::endl;
  }

  char* data = new char[size];

  in.read(data, size);//Read audio data into buffer, return.

  in.close();

  return data;    
}


int main(int argc, char *argv[]) {
  int x, ctr = 0;
  int total_duration=150;
  int animation_duration=12;

  int channel, sample_rate, bps, size;
  char* data;
  ALuint buffer, source, format;
  ALCdevice *device;
  ALCcontext *context;

  // initialize curses
  initscr();
  start_color(); // color setting
  init_pair(1, 232, COLOR_BLACK);
  init_pair(2, 234, COLOR_BLACK);
  init_pair(3, 238, COLOR_BLACK);
  init_pair(4, 244, COLOR_BLACK);
  init_pair(5, 252, COLOR_BLACK);
  init_pair(6, COLOR_WHITE, COLOR_BLACK);
  signal(SIGINT, SIG_IGN);
  noecho();
  curs_set(0);
  nodelay(stdscr, TRUE);
  leaveok(stdscr, TRUE);
  scrollok(stdscr, FALSE);
  x = COLS/2;
  attrset(COLOR_PAIR(1));

  // initialize OpenAL
  data = LoadWAV("data/shinobi_execution_se.wav", channel, sample_rate, bps, size);
  device = alcOpenDevice(nullptr);
  if (!device) {
    std::cout << "Could not open sound device" << std::endl;
    return 1;
  }
  context = alcCreateContext(device, nullptr);
  if (!context) {
    std::cout << "Could not open context" << std::endl;
    return 1;
  }
  alcMakeContextCurrent(context);
  alGenBuffers(1, &buffer);
  if (bps == 8) {
    format = AL_FORMAT_STEREO8;
  } else {
    format = AL_FORMAT_STEREO16;
  }
  alBufferData(buffer, format, data, size, sample_rate);
  alGenSources(1, &source);
  alSourcei(source, AL_BUFFER, buffer);

  // main loop
  alSourcePlay(source);
  for (ctr=0; ctr < total_duration; ctr++) {
    if (add_BE() == ERR) break;
    getch();
    switch (ctr / (animation_duration / 5)) {
      case 0:
        attrset(COLOR_PAIR(1));
        break;
      case 1:
        attrset(COLOR_PAIR(2));
        break;
      case 2:
        attrset(COLOR_PAIR(3));
        break;
      case 3:
        attrset(COLOR_PAIR(4));
        break;
      case 4:
        attrset(COLOR_PAIR(5));
        break;
      default:
        attrset(COLOR_PAIR(6));
        break;
    }
    refresh();
    usleep(40000);
  }

  // finalize OpenAL
  alSourceStop(source);
  alDeleteSources(1, &source);
  alDeleteBuffers(1, &buffer);
  alcMakeContextCurrent(nullptr);
  alcDestroyContext(context);
  alcCloseDevice(device);

  // finalize curses
  mvcur(0, COLS - 1, LINES - 1, 0);
  attrset(0);
  endwin();

  return 0;
}
