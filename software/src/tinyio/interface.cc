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
#include <tinyio/control_request.h>
#include <tinyio/exception.h>

// Local:
#include "interface.h"


namespace tinyio {

Interface::Interface (libusb::Device&& usb_device):
	_usb_device (std::move (usb_device))
{
	reset();
}


std::string
Interface::release_version_str() const
{
	return _usb_device.descriptor().release_version_str();
}


std::string
Interface::serial_number() const
{
	return _usb_device.serial_number();
}


void
Interface::reset()
{
	for (std::size_t i = 0; i < kPinsCount; ++i)
	{
		configure_pin (i, Input);
		set_pin_level (i, false);
	}
	commit();
}


void
Interface::configure_pin (uint8_t pin, PinDirection dir)
{
	if (pin < kPinsCount)
	{
		if (_pin_directions[pin] != dir)
		{
			_pin_directions[pin] = dir;
			_pin_directions_changed = true;
		}
	}
}


void
Interface::set_pin_level (uint8_t pin, bool logic_level)
{
	if (pin < kPinsCount)
	{
		if (_pin_levels[pin] != logic_level)
		{
			_pin_levels[pin] = logic_level;
			_pin_levels_changed = true;
		}
	}
}


void
Interface::commit()
{
	if (_pin_directions_changed)
	{
		_usb_device.send (libusb::ControlTransfer (static_cast<uint8_t> (USBControlRequest::ConfigurePins), 0, 0), 0, get_mask (_pin_directions));
		_pin_directions_changed = false;
	}

	if (_pin_levels_changed)
	{
		_usb_device.send (libusb::ControlTransfer (static_cast<uint8_t> (USBControlRequest::SetPins), 0, 0), 0, get_mask (_pin_levels));
		_pin_levels_changed = false;
	}
}


std::array<bool, Interface::kPinsCount>
Interface::get_pin_levels()
{
	return get_bits (_usb_device.receive (libusb::ControlTransfer (static_cast<uint8_t> (USBControlRequest::GetPins), 0, 0), 0));
}


template<class T>
	inline std::vector<uint8_t>
	Interface::get_mask (std::array<T, kPinsCount> const& bit_array)
	{
		std::size_t n_bytes = kPinsCount / 8;
		std::vector<uint8_t> result (n_bytes, 0);

		for (std::size_t byte = 0; byte < n_bytes; ++byte)
		{
			uint_least8_t mask = 0;
			for (std::size_t bit = 0; bit < 8; ++bit)
				mask |= static_cast<uint_least32_t> (bit_array[8 * byte + bit]) << bit;
			result[byte] = mask;
		}

		// Reverse order of bits in the middle byte (the one with index 1)
		swap_bits (result[1]);

		return result;
	}


inline std::array<bool, Interface::kPinsCount>
Interface::get_bits (std::vector<uint8_t> const& bytes)
{
	std::array<bool, kPinsCount> result;

	if (bytes.size() == kPinsCount / 8)
	{
		for (std::size_t byte = 0; byte < bytes.size(); ++byte)
		{
			uint8_t byte_val = bytes[byte];
			if (byte == 1)
				swap_bits (byte_val);
			for (std::size_t bit = 0; bit < 8; ++bit)
				result[8 * byte + bit] = (byte_val >> bit) & 1u;
		}
	}
	else
		throw Exception ("unexpected result size from the device");

	return result;
}


inline void
Interface::swap_bits (uint8_t& b)
{
	b = (b & 0xf0) >> 4 | (b & 0x0f) << 4;
	b = (b & 0xcc) >> 2 | (b & 0x33) << 2;
	b = (b & 0xaa) >> 1 | (b & 0x55) << 1;
}

} // namespace tinyio

