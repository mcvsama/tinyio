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

#ifndef TINYIO__GUI__MAIN_WINDOW_H__INCLUDED
#define TINYIO__GUI__MAIN_WINDOW_H__INCLUDED

// Lib:
#include <QtWidgets/QWidget>
#include <QtWidgets/QStackedLayout>

// TinyIO:
#include <tinyio/gui/select_device_widget.h>
#include <tinyio/gui/control_widget.h>


namespace tinyiogui {

class Application;


class MainWindow: public QWidget
{
  public:
	// Ctor
	MainWindow (Application*);

  private:
	/**
	 * Show device selector and refresh it.
	 */
	void
	show_selector();

	/**
	 * Show control widget, if possible.
	 */
	void
	show_control_widget();

	/**
	 * Create control widget wrapper.
	 */
	Unique<QWidget>
	make_control_widget_wrapper (ControlWidget*);

  private slots:
	/**
	 * Called when user selects a device.
	 */
	void
	device_selected();

  private:
	Application*				_application;
	Unique<QStackedLayout>		_stack;
	Unique<SelectDeviceWidget>	_select_device_widget;
	Unique<QWidget>				_control_widget_wrapper;
	ControlWidget*				_control_widget; // Will be owned by the wrapper.
};

} // namespace tinyiogui

#endif

