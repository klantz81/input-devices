#ifndef MOUSE_H
#define MOUSE_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/input.h>

#define MOUSE_DEV "/dev/input/event2"

struct mouse_position {
	int x, y;
};

struct mouse_state {
	__s32 button[KEY_CNT];
	__s32 axis[REL_CNT];
};

class cMouse {
  private:
	pthread_t thread;
	bool active;
	int mouse_fd;
	input_event *mouse_ev;
	mouse_state *mouse_st;
	char name[256];
	__u32 version;

  protected:
  public:
	cMouse();
	~cMouse();
	static void* loop(void* obj);
	void readEv();
	mouse_position mousePosition();
        __s32 buttonState(int n);
};

#endif
