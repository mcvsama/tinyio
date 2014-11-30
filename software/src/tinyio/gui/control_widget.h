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

  private:
	tinyio::Device						_device;
	std::array<Unique<PinWidget>, 24>	_pin_widgets;
};

} // namespace tinyiogui

#endif

