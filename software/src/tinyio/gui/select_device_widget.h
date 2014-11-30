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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <libusbcc/libusbcc.h>

// TinyIO:
#include <tinyio/tinyio/device_manager.h>


namespace tinyiogui {

class SelectDeviceWidget: public QWidget
{
	Q_OBJECT

  public:
	// Ctor
	SelectDeviceWidget (QWidget* parent, tinyio::DeviceManager*);

	/**
	 * Return DeviceInfo for the selected device.
	 */
	Optional<tinyio::DeviceInfo>
	selected_device_info() const;

	/**
	 * Re-read USB device list and refresh the combo-box.
	 */
	void
	refresh_list();

  private:
	/**
	 * Called by the "Select" button.
	 */
	void
	select_pressed();

  private slots:
	/**
	 * Update UI state, enable/disable buttons, etc.
	 */
	void
	update_ui();

  signals:
	/**
	 * Emitted when user presses the "Select" button.
	 */
	void
	selected();

  private:
	tinyio::DeviceManager*	_device_manager;
	QComboBox*				_devices_combobox;
	QPushButton*			_select_button;
};

} // namespace tinyiogui

#endif

