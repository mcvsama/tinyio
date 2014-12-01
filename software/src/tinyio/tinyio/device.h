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


namespace tinyio {

enum PinDirection {
	Input	= 0, // Binary match to protocol requests.
	Output	= 1,
};


/**
 * Represents open state of a TinyIO device.
 */
class Device
{
  public:
	static constexpr std::size_t	kPinsCount = 24;

  public:
	Device (libusb::Device&&);

	Device (Device const&) = delete;

	Device (Device&&) = default;

	Device&
	operator= (Device const&) = delete;

	Device&
	operator= (Device&&) = default;

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
	 * Reset device to default state: set all pins as inputs and commit().
	 */
	void
	reset();

	/**
	 * Configure given pin.
	 * Nothing is done until commit() is called.
	 */
	void
	configure_pin (uint8_t pin, PinDirection);

	/**
	 * Set given pin output level.
	 * Nothing is done until commit() is called.
	 */
	void
	set_pin_level (uint8_t pin, bool logic_level);

	/**
	 * Return configured pin level (might be different than
	 * actual pin level, eg. in input+pull-up configuration).
	 */
	bool
	get_pin_level (uint8_t pin) const;

	/**
	 * Set pin logic level to the opposite.
	 */
	void
	flip_pin_level (uint8_t pin);

	/**
	 * Commit changes.
	 */
	void
	commit();

	/**
	 * Get all pin levels.
	 */
	std::array<bool, kPinsCount>
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
		get_mask (std::array<T, kPinsCount> const& bit_array);

	/**
	 * Return array of bits obtained from the vector sent
	 * by device. Middle-byte bits are swapped (again).
	 */
	static std::array<bool, kPinsCount>
	get_bits (std::vector<uint8_t> const& bytes);

	/**
	 * Reverse order of bits in a byte.
	 */
	static void
	swap_bits (uint8_t&);

  private:
	libusb::Device							_usb_device;
	std::array<PinDirection, kPinsCount>	_pin_directions;
	bool									_pin_directions_changed	= false;
	std::array<bool, kPinsCount>			_pin_levels;
	bool									_pin_levels_changed		= false;
};

} // namespace tinyio

#endif


