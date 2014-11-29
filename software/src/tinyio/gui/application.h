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

#ifndef TINYIO__GUI__APPLICATION_H__INCLUDED
#define TINYIO__GUI__APPLICATION_H__INCLUDED

// Lib:
#include <QtWidgets/QApplication>

// TinyIO:
#include <tinyio/config/all.h>
#include <tinyio/gui/main_window.h>
#include <tinyio/tinyio/device_manager.h>


namespace tinyiogui {

class Application: public QApplication
{
  public:
	// Ctor
	Application (int& argc, char** argv);

	/**
	 * Override and catch exceptions.
	 */
	bool
	notify (QObject* receiver, QEvent* event) override;

	/**
	 * Collect info from exception, prepare a long string
	 * with dependent exceptions.
	 */
	static std::string
	stringify_exception (std::exception const&);

	/**
	 * Return app's DeviceManager.
	 */
	tinyio::DeviceManager*
	device_manager() const noexcept;

  private:
	Unique<tinyio::DeviceManager>	_device_manager;
	Unique<MainWindow>				_main_window;
};


inline tinyio::DeviceManager*
Application::device_manager() const noexcept
{
	return _device_manager.get();
}

} // namespace tinyiogui

#endif

