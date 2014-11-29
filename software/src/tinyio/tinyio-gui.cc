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
#include <memory>

// Lib:
#include <libusbcc/libusbcc.h>

// TinyIO:
#include <tinyio/gui/application.h>


void
print_exception (std::exception const& e, int level = 0)
{
	std::string indent (level, ' ');
	std::cerr << indent << indent;

	if (level == 0)
		std::cerr << "Exception: ";
	else
		std::cerr << "Reason: ";

	std::cerr << e.what() << std::endl;

	try {
		std::rethrow_if_nested (e);
	}
	catch (const std::exception& e)
	{
		print_exception (e, level + 1);
	}
	catch (...)
	{
		throw;
	}
}


int main (int argc, char** argv, char**)
{
	try {
		auto app = std::make_unique<tinyiogui::Application> (argc, argv);
		app->exec();
	}
	catch (std::exception const& e)
	{
		print_exception (e);
	}

	return EXIT_SUCCESS;
}

