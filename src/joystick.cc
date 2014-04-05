#include "joystick.h"

cJoystick::cJoystick() {
	active = false;
	joystick_ev = new input_event();
	joystick_st = new joystick_state();
	for (int i = 0; i < KEY_CNT; i++) joystick_st->button[i] = 0;
	for (int i = 0; i < ABS_CNT; i++) joystick_st->axis[i] = 128;

	joystick_fd = open(JOYSTICK_DEV, O_RDONLY | O_NONBLOCK);
	if (joystick_fd > -1) {
		ioctl(joystick_fd, EVIOCGNAME(256), name);
		ioctl(joystick_fd, EVIOCGVERSION, &version);
		std::cout << "   Name: " << name << std::endl;
		std::cout << "Version: " << version << std::endl << std::endl;
		active = true;
		pthread_create(&thread, 0, &cJoystick::loop, this);
	}
}

cJoystick::~cJoystick() {
	if (joystick_fd > -1) {
		active = false;
		pthread_join(thread, 0);
		close(joystick_fd);
	}
	joystick_fd = 0;
	delete joystick_st;
	delete joystick_ev;
}

void* cJoystick::loop(void *obj) {
	while (reinterpret_cast<cJoystick *>(obj)->active) reinterpret_cast<cJoystick *>(obj)->readEv();
}

void cJoystick::readEv() {
	int bytes = read(joystick_fd, joystick_ev, sizeof(*joystick_ev));
	if (bytes > 0) {
		if (joystick_ev->type & EV_ABS)
			joystick_st->axis[joystick_ev->code] = joystick_ev->value;
		if (joystick_ev->type & EV_KEY)
			joystick_st->button[joystick_ev->code] = joystick_ev->value;
	}
}

joystick_position cJoystick::joystickPosition(int n) {
	joystick_position pos;
	int i0 = n == 0 ? ABS_X : ABS_Z;
	int i1 = n == 0 ? ABS_Y : ABS_RZ;

	float x0 =  (joystick_st->axis[i0] - 128.f) / 128.f,
	      y0 = -(joystick_st->axis[i1] - 128.f) / 128.f;
	float x  = x0 * sqrt(1 - pow(y0, 2) / 2.0f),
	      y  = y0 * sqrt(1 - pow(x0, 2) / 2.0f);

	pos.x = x0;
	pos.y = y0;

	pos.theta = atan2(y, x);
	pos.r = sqrt(pow(y, 2) + pow(x, 2));

	return pos;
}

__s32 cJoystick::buttonState(int n) {
	return joystick_st->button[n];
}
