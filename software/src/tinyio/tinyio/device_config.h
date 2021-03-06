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

#ifndef TINYIO__TINYIO__DEVICE_CONFIG_H__INCLUDED
#define TINYIO__TINYIO__DEVICE_CONFIG_H__INCLUDED

// Lib:
#include <libusbcc/libusbcc.h>
#include <boost/optional.hpp>


namespace tinyio {

enum PinDirection {
	Input	= 0, // Binary match to protocol requests.
	Output	= 1,
};


/**
 * Represents device configuration (pin directions, levels, etc).
 */
class DeviceConfig
{
	friend class Device;

  public:
	static constexpr std::size_t	kPinsCount = 24;

  public:
	DeviceConfig();

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
	 * Return configured pin direction.
	 */
	PinDirection
	get_pin_direction (uint8_t pin) const;

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
	std::array<PinDirection, kPinsCount>	_pin_directions;
	std::array<bool, kPinsCount>			_pin_levels;
};

} // namespace tinyio

#endif


