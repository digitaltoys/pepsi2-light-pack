/*
 * commands.h
 *
 *  Created on: 04.08.2010
 *      Author: Mike Shatohin (brunql)
 *     Project: AmbilightUSB
 *
 *  AmbilightUSB is very simple implementation of the backlight for a laptop
 *
 *  Copyright (c) 2010 Mike Shatohin, mikeshatohin [at] gmail.com
 *
 *  AmbilightUSB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  AmbilightUSB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
 

#ifndef COMMANDS_H_
#define COMMANDS_H_

// Commands to device, sends it in first byte of data[]
enum COMMANDS{
	CMD_LEDS_1_2, 					/* sw version < sw_v3.0.2, fw < fw_v3.2: CMD_RIGHT_SIDE = 0 */
	CMD_LEDS_3_4, 					/* sw version < sw_v3.0.2, fw < fw_v3.2: CMD_LEFT_SIDE = 1 */
	CMD_OFF_ALL, 					/* switch off all leds */
	CMD_SET_TIMER_OPTIONS, 			/* timer settings */
	CMD_SET_PWM_LEVEL_MAX_VALUE, 	/* each color max value */
	CMD_SMOOTH_CHANGE_COLORS,

	CMD_LEDS_5_6 = 0x10, 			/* data comes for LEDs N5 and N6 */
	CMD_LEDS_7_8,

	CMD_NOP = 0xff
};

enum PRESCALLERS{
	CMD_SET_PRESCALLER_1,
	CMD_SET_PRESCALLER_8,
	CMD_SET_PRESCALLER_64,
	CMD_SET_PRESCALLER_256,
	CMD_SET_PRESCALLER_1024,
};


enum DATA_VERSION_INDEXES{
	INDEX_HW_VER_MAJOR,
	INDEX_HW_VER_MINOR,
	INDEX_FW_VER_MAJOR,
	INDEX_FW_VER_MINOR,
};


#endif /* COMMANDS_H_ */
