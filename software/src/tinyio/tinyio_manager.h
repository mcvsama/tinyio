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

#ifndef TINYIO__TINYIO_MANAGER_H__INCLUDED
#define TINYIO__TINYIO_MANAGER_H__INCLUDED

// Lib:
#include <boost/optional.hpp>

// Local:
#include "tinyio.h"


namespace tinyio {

/**
 * Searches for TinyIO devices.
 */
class TinyIOManager
{
	static constexpr libusb::VendorID	kTinyIOVendorID		= 0x16c0;
	static constexpr libusb::ProductID	kTinyIOProductID	= 0x05dc;
	static constexpr auto				kTinyIOManufacturer	= "mulabs.org";
	static constexpr auto				kTinyIOProduct		= "TinyIO";

  public:
	/**
	 * Searches the USB bus for TinyIO devices.
	 */
	TinyIOList
	find_devices() const;

  private:
	libusb::Bus _bus;
};

} // namespace tinyio

#endif


