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

// Lib:
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QComboBox>

// Local:
#include "select_device_widget.h"


namespace tinyiogui {

SelectDeviceWidget::SelectDeviceWidget (QWidget* parent, tinyio::DeviceManager* device_manager):
	QWidget (parent),
	_device_manager (device_manager)
{
	auto label = new QLabel ("Select TinyIO device:", this);
	auto devices_combobox = new QComboBox (this);
	for (auto const& dev_info: device_manager->find_devices())
	{
		auto tinyio = dev_info.open();
		devices_combobox->addItem ("TinyIO " + QString::fromStdString (tinyio.release_version_str()) +
								   ", S/N: " + QString::fromStdString (tinyio.serial_number()));
	}

	auto layout = new QVBoxLayout (this);
	layout->addWidget (label);
	layout->addWidget (devices_combobox);
}

} // namespace tinyiogui

