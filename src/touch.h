#ifndef TOUCH_H
#define TOUCH_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/input.h>

#define TOUCH_DEV "/dev/input/event10"

struct touch_state {
        __s32 id[255];
        __s32 x[255];
        __s32 y[255];
};

class cTouch {
  private:
	pthread_t thread;
	bool active;
	int touch_fd;
	input_event *touch_ev;
	char name[256];
	__u32 version;

  protected:
  public:
        __s32 slot;
	touch_state *touch_st;
        
	cTouch();
	~cTouch();
	static void* loop(void* obj);
	void readEv();
};

#endif
