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

// Standard:
#include <vector>

// Lib:
#include <libusbcc/libusbcc.h>
#include <boost/optional.hpp>

// Local:
#include "interface.h"


namespace tinyio {

/**
 * Represents TinyIO device.
 */
class TinyIO
{
  public:
	// Ctor
	explicit TinyIO (libusb::DeviceDescriptor const&);

	/**
	 * Open the device, return the handle.
	 */
	Interface
	open() const;

  private:
	libusb::DeviceDescriptor _usb_descriptor;
};


typedef std::vector<TinyIO> TinyIOList;

} // namespace tinyio

#endif


