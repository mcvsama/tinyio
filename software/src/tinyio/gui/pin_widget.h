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

// TinyIO:
#include <tinyio/tinyio/device.h>


namespace tinyiogui {

class PinWidget: public QWidget
{
  public:
	// Ctor
	PinWidget (QWidget* parent, uint8_t pin);

  private:
	void
	set_button_highlighted (QPushButton* button, bool logic_level);

  private:
	uint8_t				_pin;
	Unique<QPushButton>	_button;
	Unique<QLabel>		_label;
	Unique<QLabel>		_io_label;
	QColor				_std_button_bg;
	QColor				_std_button_fg;
};

} // namespace tinyiogui

#endif

