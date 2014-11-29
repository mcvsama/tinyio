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
#include <exception>
#include <iostream>
#include <string>
#include <iomanip>

// Lib:
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLayout>

// TinyIO:
#include <tinyio/gui/application.h>
#include <tinyio/gui/select_device_widget.h>

// Local:
#include "main_window.h"


namespace tinyiogui {

MainWindow::MainWindow (Application* application):
	_application (application)
{
	auto select_device_widget = new SelectDeviceWidget (this, application->device_manager());

	auto layout = new QVBoxLayout (this);
	layout->addWidget (select_device_widget);
}

} // namespace tinyiogui

