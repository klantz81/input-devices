#include "mouse.h"

cMouse::cMouse() {
	active = false;
	mouse_ev = new input_event();
	mouse_st = new mouse_state();
	for (int i = 0; i < KEY_CNT; i++) mouse_st->button[i] = 0;
	for (int i = 0; i < REL_CNT; i++) mouse_st->axis[i] = 0;

	mouse_fd = open(MOUSE_DEV, O_RDONLY | O_NONBLOCK);
	if (mouse_fd > -1) {
		ioctl(mouse_fd, EVIOCGNAME(256), name);
		ioctl(mouse_fd, EVIOCGVERSION, &version);
		std::cout << "   Name: " << name << std::endl;
		std::cout << "Version: " << version << std::endl << std::endl;
		active = true;
		pthread_create(&thread, 0, &cMouse::loop, this);
	}
}

cMouse::~cMouse() {
	if (mouse_fd > -1) {
		active = false;
		pthread_join(thread, 0);
		close(mouse_fd);
	}
	mouse_fd = 0;
	delete mouse_st;
	delete mouse_ev;
}

void* cMouse::loop(void *obj) {
	while (reinterpret_cast<cMouse *>(obj)->active) reinterpret_cast<cMouse *>(obj)->readEv();
}

void cMouse::readEv() {
	int bytes = read(mouse_fd, mouse_ev, sizeof(*mouse_ev));
	if (bytes > 0) {
		if (mouse_ev->type & EV_REL)
			mouse_st->axis[mouse_ev->code] += mouse_ev->value;
		if (mouse_ev->type & EV_KEY)
			mouse_st->button[mouse_ev->code] = mouse_ev->value;
	}
}

mouse_position cMouse::mousePosition() {
	mouse_position m;
	m.x = mouse_st->axis[REL_X];
	m.y = mouse_st->axis[REL_Y];
	return m;
}

__s32 cMouse::buttonState(int n) {
	return mouse_st->button[n];
}
