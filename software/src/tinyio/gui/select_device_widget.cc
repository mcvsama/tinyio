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
#include <QtCore/QTimer>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>

// TinyIO:
#include <tinyio/gui/gui.h>

// Local:
#include "select_device_widget.h"


namespace tinyiogui {

SelectDeviceWidget::SelectDeviceWidget (QWidget* parent, tinyio::DeviceManager* device_manager):
	QWidget (parent),
	_device_manager (device_manager)
{
	auto label = new QLabel ("Select TinyIO device:", this);

	auto refresh_button = new QPushButton ("&Refresh", this);
	prepare_button (refresh_button);
	QObject::connect (refresh_button, &QPushButton::clicked, this, &SelectDeviceWidget::refresh_list);

	_select_button = new QPushButton ("&Select ✓", this);
	prepare_button (_select_button);
	QObject::connect (_select_button, &QPushButton::clicked, this, &SelectDeviceWidget::select_pressed);

	_devices_combobox = new QComboBox (this);

	auto info_label = new QLabel ("If device list is empty, check your USB permissions\n"
								  "for TinyIO. Perhaps you need to install udev .rules file\nand reload udev.", this);

	auto layout = new QVBoxLayout (this);
	auto buttons_layout = new QHBoxLayout();
	layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
	layout->addWidget (label);
	layout->addWidget (_devices_combobox);
	layout->addLayout (buttons_layout);
	layout->addItem (new QSpacerItem (0, 10, QSizePolicy::Minimum, QSizePolicy::Fixed));
	layout->addWidget (info_label);
	layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
	buttons_layout->addItem (new QSpacerItem (0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	buttons_layout->addWidget (refresh_button);
	buttons_layout->addWidget (_select_button);

	refresh_list();
}


Optional<QString>
SelectDeviceWidget::selected_device_serial() const
{
	auto serial_number_variant = _devices_combobox->currentData();

	if (serial_number_variant.isValid())
		return serial_number_variant.toString();
	else
		return { };
}


void
SelectDeviceWidget::refresh_list()
{
	_devices_combobox->clear();

	for (auto const& dev_info: _device_manager->find_devices())
	{
		try {
			auto tinyio = dev_info.open();
			_devices_combobox->addItem ("TinyIO " + QString::fromStdString (tinyio.release_version_str()) +
										", S/N: " + QString::fromStdString (tinyio.serial_number()),
										QString::fromStdString (tinyio.serial_number()));
		}
		// Don't care if we couldn't open device now:
		catch (libusb::Exception const&)
		{ }
	}

	update_ui();
}


void
SelectDeviceWidget::select_pressed()
{
	if (_devices_combobox->count() > 0 && _devices_combobox->currentIndex() != -1)
		emit selected();
}


void
SelectDeviceWidget::update_ui()
{
	_select_button->setEnabled (_devices_combobox->count() > 0 && _devices_combobox->currentIndex() != -1);
}

} // namespace tinyiogui

