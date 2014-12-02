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

#ifndef TINYIO__GUI__PIN_WIDGET_H__INCLUDED
#define TINYIO__GUI__PIN_WIDGET_H__INCLUDED

// Lib:
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMenu>

// TinyIO:
#include <tinyio/tinyio/device.h>


namespace tinyiogui {

class PinWidget: public QWidget
{
	Q_OBJECT

  public:
	// Ctor
	PinWidget (QWidget* parent, uint8_t pin);

	/**
	 * Set configured pin direction.
	 */
	void
	set_configured_pin_direction (tinyio::PinDirection);

	/**
	 * Set actual pin direction.
	 */
	void
	set_actual_pin_direction (tinyio::PinDirection);

	/**
	 * Set configured pin level.
	 */
	void
	set_configured_pin_level (bool logic_level);

	/**
	 * Set about actual pin level (as read by the µC).
	 */
	void
	set_actual_pin_level (bool logic_level);

	/**
	 * Update GUI.
	 */
	void
	update_gui();

  signals:
	/**
	 * Rename slot.
	 */
	void
	rename();

	/**
	 * Emitted when logic level should be switched.
	 */
	void
	flip_pin_level();

	/**
	 * Emitted when direction setting change.
	 */
	void
	set_pin_direction (tinyio::PinDirection);

  protected:
	// QWidget
	void
	mousePressEvent (QMouseEvent*) override;

  private slots:
	/**
	 * Request pin configuration as input.
	 */
	void
	configure_as_input();

	/**
	 * Request pin configuration as output.
	 */
	void
	configure_as_output();

	/**
	 * Callback from the push button.
	 */
	void
	button_pressed();

  private:
	/**
	 * Set look of a button.
	 */
	static void
	set_button_color (QPushButton* button, QColor const&);

  private:
	uint8_t					_pin;
	bool					_configured_level		= false;
	bool					_actual_level			= false;
	tinyio::PinDirection	_configured_direction	= tinyio::Input;
	tinyio::PinDirection	_actual_direction		= tinyio::Input;
	Unique<QPushButton>		_button;
	Unique<QLabel>			_label;
	Unique<QLabel>			_io_label;
	QMenu*					_menu;
};

} // namespace tinyiogui

#endif

