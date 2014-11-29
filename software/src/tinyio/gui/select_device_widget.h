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

#ifndef TINYIO__GUI__SELECT_DEVICE_WIDGET_H__INCLUDED
#define TINYIO__GUI__SELECT_DEVICE_WIDGET_H__INCLUDED

// Lib:
#include <QtWidgets/QWidget>
#include <libusbcc/libusbcc.h>

// TinyIO:
#include <tinyio/tinyio/device_manager.h>


namespace tinyiogui {

class SelectDeviceWidget: public QWidget
{
  public:
	// Ctor
	SelectDeviceWidget (QWidget* parent, tinyio::DeviceManager*);

  private:
	tinyio::DeviceManager* _device_manager;
};

} // namespace tinyiogui

#endif

