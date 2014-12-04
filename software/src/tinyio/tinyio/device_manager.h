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

#ifndef TINYIO__TINYIO__DEVICE_MANAGER_H__INCLUDED
#define TINYIO__TINYIO__DEVICE_MANAGER_H__INCLUDED

// Lib:
#include <boost/optional.hpp>

// Local:
#include "device_info.h"


namespace tinyio {

/**
 * Searches for TinyIO devices.
 */
class DeviceManager
{
	static constexpr libusb::VendorID	kTinyIOVendorID		= 0x16c0;
	static constexpr libusb::ProductID	kTinyIOProductID	= 0x05dc;
	static constexpr auto				kTinyIOManufacturer	= "mulabs.org";
	static constexpr auto				kTinyIOProduct		= "TinyIO";

  public:
	/**
	 * Searches the USB bus for TinyIO devices.
	 */
	DeviceInfoList
	find_devices() const;

	/**
	 * Find device by USB device address.
	 */
	Optional<DeviceInfo>
	find_by_address (uint8_t address) const;

	/**
	 * Find device by serial number.
	 */
	Optional<DeviceInfo>
	find_by_serial (std::string const& serial) const;

  private:
	/**
	 * Rethrow only memory error, otherwise just log on std::cerr.
	 */
	static void
	rethrow_if_serious (libusb::StatusException const&);

  private:
	libusb::Bus _bus;
};

} // namespace tinyio

#endif


