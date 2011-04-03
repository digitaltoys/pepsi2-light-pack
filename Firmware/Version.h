/*
 * Version.h
 *
 *  Created on: 03.11.2010
 *      Author: Mike Shatohin (brunql)
 *     Project: Lightpack
 *
 *  Lightpack is very simple implementation of the backlight for a laptop
 *
 *  Copyright (c) 2010, 2011 Mike Shatohin, mikeshatohin [at] gmail.com
 *
 *  Lightpack is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Lightpack is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
 
#ifndef VERSION_H_
#define VERSION_H_

//   Firmware version:
#define VERSION_OF_FIRMWARE				(0x0402UL)
#define VERSION_OF_FIRMWARE_MAJOR		((VERSION_OF_FIRMWARE >> 8) & 0xff)
#define VERSION_OF_FIRMWARE_MINOR		(VERSION_OF_FIRMWARE & 0x00ff)


#endif /* VERSION_H_ */
