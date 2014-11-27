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

#ifndef TINYIO__EXCEPTION_H__INCLUDED
#define TINYIO__EXCEPTION_H__INCLUDED

// Standard:
#include <exception>


namespace tinyio {

class Exception: public std::runtime_error
{
	using runtime_error::runtime_error;
};

} // namespace tinyio

#endif


