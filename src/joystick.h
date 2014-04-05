#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/input.h>
#include <math.h>

#define JOYSTICK_DEV "/dev/input/event1"

struct joystick_position {
	float theta, r, x, y;
};

struct joystick_state {
	__s32 button[KEY_CNT];
	__s32 axis[ABS_CNT];
};

class cJoystick {
  private:
	pthread_t thread;
	bool active;
	int joystick_fd;
	input_event *joystick_ev;
	joystick_state *joystick_st;
	char name[256];
	__u32 version;

  protected:
  public:
	cJoystick();
	~cJoystick();
	static void* loop(void* obj);
	void readEv();
	joystick_position joystickPosition(int n);
        __s32 buttonState(int n);
};

#endif
