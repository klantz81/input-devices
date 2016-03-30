#include <iostream>
#include <iomanip>

#include "src/keyboard.h"
#include "src/mouse.h"
#include "src/joystick.h"
#include "src/touch.h"

int main(int argc, char* argv[]) {

	bool active = true;

	cKeyboard keyboard;
	cMouse    mouse;
	cJoystick joystick;
        cTouch    touch;

	mouse_position mouse_axis;
	bool mouse_state0,
	     mouse_state1;

	joystick_position joystick_axis0,
			  joystick_axis1;
	bool joystick_state0,
	     joystick_state1;
             

	while (active) {

		mouse_axis   = mouse.mousePosition();
		mouse_state0 = mouse.buttonState(BTN_MOUSE+0);
		mouse_state1 = mouse.buttonState(BTN_MOUSE+1);

		joystick_axis0  = joystick.joystickPosition(0);
		joystick_axis1  = joystick.joystickPosition(1);
		joystick_state0 = joystick.buttonState(BTN_JOYSTICK+0);
		joystick_state1 = joystick.buttonState(BTN_JOYSTICK+1);

                bool hit = false;
                for (int i = 0; i < 255; i++) {
                        if (touch.touch_st->id[i] > -1) {
                                std::cout << i << " " << touch.touch_st->id[i] << ": "
                                        << touch.touch_st->x[i] << ","
                                        << touch.touch_st->y[i] << "\t\t";
                                hit = true;
                        }
                }
                if (hit)
                        std::cout << '\n';
/*
 		std::cout << "x = " << std::setiosflags(std::ios::fixed) << std::setw(5) << std::setprecision(0) << mouse_axis.x <<
			    " y = " << std::setiosflags(std::ios::fixed) << std::setw(5) << std::setprecision(0) << mouse_axis.y << " ";
		std::cout << (mouse_state0 ? "1 " : "0 ") << (mouse_state1 ? "1 " : "0 ") << " ";


		std::cout << "theta = " << std::setiosflags(std::ios::fixed) << std::setw(10) << std::setprecision(7) << joystick_axis0.theta <<
				" r = " << std::setiosflags(std::ios::fixed) << std::setw(10) << std::setprecision(7) << joystick_axis0.r     << " ";
		std::cout << "theta = " << std::setiosflags(std::ios::fixed) << std::setw(10) << std::setprecision(7) << joystick_axis1.theta <<
				" r = " << std::setiosflags(std::ios::fixed) << std::setw(10) << std::setprecision(7) << joystick_axis1.r     << " ";
		std::cout << (joystick_state0 ? "1 " : "0 ") << (joystick_state1 ? "1 " : "0 ") << "    ";

		std::cout << '\r';
*/
		
		if (keyboard.keyState(KEY_ESC)) {
			std::cout << std::endl << "escape detected" << std::endl << std::endl;
			active = false;
		}
	}

	return 0;
}