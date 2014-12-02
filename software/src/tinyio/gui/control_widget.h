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

#ifndef TINYIO__GUI__CONTROL_WIDGET_H__INCLUDED
#define TINYIO__GUI__CONTROL_WIDGET_H__INCLUDED

// Stantard:
#include <array>

// Lib:
#include <QtCore/QTimer>
#include <QtWidgets/QWidget>

// TinyIO:
#include <tinyio/tinyio/device.h>
#include <tinyio/gui/pin_widget.h>


namespace tinyiogui {

class ControlWidget: public QWidget
{
  public:
	// Ctor
	ControlWidget (QWidget* parent, tinyio::Device&&);

	/**
	 * Enable/disable "hold" function (inhibit commiting
	 * changes until "hold" is disabled).
	 */
	void
	set_hold (bool enabled);

  public slots:
	/**
	 * Set pin direction (input/output).
	 */
	void
	set_pin_direction (uint8_t pin, tinyio::PinDirection);

	/**
	 * Flip pin output level.
	 */
	void
	flip_pin_level (uint8_t pin);

	/**
	 * Read all pin levels.
	 */
	void
	get_pin_levels();

  private:
	/**
	 * Gets called periodically.
	 */
	void
	timeout();

  private:
	tinyio::Device						_device;
	std::array<Unique<PinWidget>, 24>	_pin_widgets;
	Unique<QTimer>						_refresh_timer;
	// If true, exceptions are silenced:
	bool								_failsafe		= false;
	bool								_hold_enabled	= false;
};

} // namespace tinyiogui

#endif

