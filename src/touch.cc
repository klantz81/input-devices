#include "touch.h"

cTouch::cTouch() {
	active = false;
	touch_ev = new input_event();
	touch_st = new touch_state();
	for (int i = 0; i < 255; i++) touch_st->id[i] = -1;
	for (int i = 0; i < 255; i++) touch_st->x[i] = 0;
 	for (int i = 0; i < 255; i++) touch_st->y[i] = 0;

	touch_fd = open(TOUCH_DEV, O_RDONLY | O_NONBLOCK);
	if (touch_fd > -1) {
		ioctl(touch_fd, EVIOCGNAME(256), name);
		ioctl(touch_fd, EVIOCGVERSION, &version);
		std::cout << "   Name: " << name << std::endl;
		std::cout << "Version: " << version << std::endl << std::endl;
		active = true;
		pthread_create(&thread, 0, &cTouch::loop, this);
                
                slot = 0;
	}
}

cTouch::~cTouch() {
	if (touch_fd > -1) {
		active = false;
		pthread_join(thread, 0);
		close(touch_fd);
	}
	touch_fd = 0;

	delete touch_st;
	delete touch_ev;
}

void* cTouch::loop(void *obj) {
	while (reinterpret_cast<cTouch *>(obj)->active) reinterpret_cast<cTouch *>(obj)->readEv();
}

void cTouch::readEv() {
	int bytes = read(touch_fd, touch_ev, sizeof(*touch_ev));
	while (bytes > 0 && touch_ev->type != EV_SYN) {
                
                if (touch_ev->type == EV_ABS && touch_ev->code == ABS_MT_SLOT) {
                        slot = touch_ev->value;
                        
                } else if (touch_ev->type == EV_ABS && touch_ev->code == ABS_MT_TRACKING_ID) {
                        touch_st->id[slot] = touch_ev->value;
                        
                } else if (touch_ev->type == EV_ABS && touch_ev->code == ABS_MT_POSITION_X) {
                        touch_st->x[slot] = touch_ev->value;
                        
                } else if (touch_ev->type == EV_ABS && touch_ev->code == ABS_MT_POSITION_Y) {
                        touch_st->y[slot] = touch_ev->value;
                        
                }
                
                bytes = read(touch_fd, touch_ev, sizeof(*touch_ev));
	}
}
