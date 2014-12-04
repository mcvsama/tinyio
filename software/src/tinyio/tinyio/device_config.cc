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

// Standard:
#include <cstddef>
#include <cstdlib>
#include <utility>

// Lib:
#include <libusbcc/libusbcc.h>

// TinyIO:
#include <tinyio/tinyio/exception.h>
#include <tinyio/control_request.h>

// Local:
#include "device_config.h"


namespace tinyio {

DeviceConfig::DeviceConfig()
{
	reset();
}


void
DeviceConfig::reset()
{
	for (std::size_t i = 0; i < kPinsCount; ++i)
	{
		configure_pin (i, Input);
		set_pin_level (i, false);
	}
}


void
DeviceConfig::configure_pin (uint8_t pin, PinDirection dir)
{
	if (pin < kPinsCount)
		if (_pin_directions[pin] != dir)
			_pin_directions[pin] = dir;
}


PinDirection
DeviceConfig::get_pin_direction (uint8_t pin) const
{
	if (pin < kPinsCount)
		return _pin_directions[pin];
	return Input;
}


void
DeviceConfig::set_pin_level (uint8_t pin, bool logic_level)
{
	if (pin < kPinsCount)
		if (_pin_levels[pin] != logic_level)
			_pin_levels[pin] = logic_level;
}


bool
DeviceConfig::get_pin_level (uint8_t pin) const
{
	if (pin < kPinsCount)
		return _pin_levels[pin];
	return false;
}


void
DeviceConfig::flip_pin_level (uint8_t pin)
{
	if (pin < kPinsCount)
		_pin_levels[pin] = !_pin_levels[pin];
}

} // namespace tinyio

