#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/input.h>

#define KEYBOARD_DEV "/dev/input/event0"

struct keyboard_state {
	__s32 key[KEY_CNT];
};

class cKeyboard {
  private:
	pthread_t thread;
	bool active;
	int keyboard_fd;
	input_event *keyboard_ev;
	keyboard_state *keyboard_st;
	char name[256];
	__u32 version;

  protected:
  public:
	cKeyboard();
	~cKeyboard();
	static void* loop(void* obj);
	void readEv();
	__s32 keyState(int key);
};

#endif
