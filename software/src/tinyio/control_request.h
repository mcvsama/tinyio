/* vim:ts=4
 *
 * Copyleft 2014…2015  Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

#ifndef TINYIO__CONTROL_REQUEST_H__INCLUDED
#define TINYIO__CONTROL_REQUEST_H__INCLUDED

namespace tinyio {

enum class USBControlRequest: uint8_t
{
	ConfigurePins	= 1,	// Bit 0 = input, bit 1 = output
	SetPins			= 2,	// Bit 0 = low, bit 1 = high
	GetPins			= 3,	// Bit 0 = low, 1 = high
};

} // namespace tinyio

#endif

