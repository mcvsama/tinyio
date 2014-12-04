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

#ifndef TINYIO__TINYIO__DEVICE_H__INCLUDED
#define TINYIO__TINYIO__DEVICE_H__INCLUDED

// Lib:
#include <libusbcc/libusbcc.h>
#include <boost/optional.hpp>

// Local:
#include "device_config.h"


namespace tinyio {

/**
 * Represents open state of a TinyIO device.
 */
class Device
{
  public:
	Device (libusb::Device&&);

	Device (Device const&) = delete;

	Device (Device&&) = default;

	Device&
	operator= (Device const&) = delete;

	Device&
	operator= (Device&&) = default;

	/**
	 * Return true if device is connected/valid.
	 */
	bool
	good();

	/**
	 * Return device version as string.
	 */
	std::string
	release_version_str() const;

	/**
	 * Return serial number of the device.
	 */
	std::string
	serial_number() const;

	/**
	 * Commit changes.
	 */
	void
	commit (DeviceConfig const&, bool force = false);

	/**
	 * Get all pin directions.
	 */
	std::array<PinDirection, DeviceConfig::kPinsCount>
	get_pin_directions();

	/**
	 * Get all pin levels.
	 */
	std::array<bool, DeviceConfig::kPinsCount>
	get_pin_levels();

  private:
	/**
	 * Return vector of bytes ready to use in USB control transfer.
	 * Middle-byte bits are swapped by this function for device's
	 * convenience.
	 * Resulting vector is big-endian.
	 */
	template<class T>
		static std::vector<uint8_t>
		get_mask (std::array<T, DeviceConfig::kPinsCount> const& bit_array);

	/**
	 * Return array of bits obtained from the vector sent
	 * by device. Middle-byte bits are swapped (again).
	 */
	static std::array<bool, DeviceConfig::kPinsCount>
	get_bits (std::vector<uint8_t> const& bytes);

	/**
	 * Reverse order of bits in a byte.
	 */
	static void
	swap_bits (uint8_t&);

  private:
	libusb::Device		_usb_device;
	DeviceConfig		_config;
};

} // namespace tinyio

#endif


