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

#ifndef TINYIO__TINYIO_H__INCLUDED
#define TINYIO__TINYIO_H__INCLUDED

// Lib:
#include <lib/libusbcc/libusbcc.h>

// Local:
#include "control_request.h"


namespace tinyio {

/**
 * Represents open state of a TinyIO device.
 */
class Interface
{
  public:
	enum PinDirection {
		Input,
		Output,
	};

  public:
	/**
	 * Configure given pin.
	 * Nothing is done until commit() is called.
	 */
	void
	configure_pin (uint8_t pin, PinDirection);

	/**
	 * Set given pin output state.
	 * Nothing is done until commit() is called.
	 */
	void
	set_pin_level (uint8_t pin, bool logic_level);

	/**
	 * Commit changes.
	 */
	void
	commit();
};


/**
 * Represents TinyIO device.
 */
class TinyIO
{
  public:
	/**
	 * Open the device, return the handle.
	 */
	Interface
	open();
};


/**
 * Searches for TinyIO devices.
 */
class TinyIOManager
{
  public:
	void
	find_devices();
};

} // namespace tinyio

#endif


