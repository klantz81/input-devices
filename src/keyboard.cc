#include "keyboard.h"

cKeyboard::cKeyboard() {
	active = false;
	keyboard_ev = new input_event();
	keyboard_st = new keyboard_state();
	for (int i = 0; i < KEY_CNT; i++) keyboard_st->key[i] = 0;

	keyboard_fd = open(KEYBOARD_DEV, O_RDONLY | O_NONBLOCK);
	if (keyboard_fd > -1) {
		ioctl(keyboard_fd, EVIOCGNAME(256), name);
		ioctl(keyboard_fd, EVIOCGVERSION, &version);
		std::cout << "   Name: " << name << std::endl;
		std::cout << "Version: " << version << std::endl << std::endl;
		active = true;
		pthread_create(&thread, 0, &cKeyboard::loop, this);
	}
}

cKeyboard::~cKeyboard() {
	if (keyboard_fd > -1) {
		active = false;
		pthread_join(thread, 0);
		close(keyboard_fd);
	}
	keyboard_fd = 0;
	delete keyboard_st;
	delete keyboard_ev;
}

void* cKeyboard::loop(void *obj) {
	while (reinterpret_cast<cKeyboard *>(obj)->active) reinterpret_cast<cKeyboard *>(obj)->readEv();
}

void cKeyboard::readEv() {
	int bytes = read(keyboard_fd, keyboard_ev, sizeof(*keyboard_ev));
	if (bytes > 0) {
		if (keyboard_ev->type & EV_KEY)
			keyboard_st->key[keyboard_ev->code] = keyboard_ev->value;
	}
}

__s32 cKeyboard::keyState(int key) {
	return keyboard_st->key[key];
}
