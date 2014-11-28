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

// Local:
#include "device_info.h"


namespace tinyio {

DeviceInfo::DeviceInfo (libusb::DeviceDescriptor const& usb_descriptor):
	_usb_descriptor (usb_descriptor)
{ }


Device
DeviceInfo::open() const
{
	return Device (_usb_descriptor.open());
}

} // namespace tinyio

