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
#include <iostream>

// Lib:
#include <libusbcc/libusbcc.h>

// Local:
#include "device_manager.h"
#include "exception.h"


namespace tinyio {

DeviceInfoList
DeviceManager::find_devices() const
{
	DeviceInfoList result;

	for (auto const& descriptor: _bus.device_descriptors())
	{
		try {
			if (descriptor.vendor_id() == kTinyIOVendorID && descriptor.product_id() == kTinyIOProductID)
			{
				libusb::Device device = descriptor.open();

				if (device.manufacturer() == kTinyIOManufacturer && device.product() == kTinyIOProduct)
				{
					result.emplace_back (descriptor);
				}
			}
		}
		catch (libusb::StatusException const& e)
		{
			std::cerr << "USB bus scan error: " << e.what() << std::endl;
			// The only serious problem that should be forwarded:
			if (e.status() == LIBUSB_ERROR_NO_MEM)
				std::throw_with_nested (Exception ("USB search failure"));
		}
	}

	return result;
}

} // namespace tinyio

