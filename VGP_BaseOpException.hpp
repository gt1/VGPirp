/**
    Copyright (C) 2009-2019 German Tischler

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/
#if ! defined(BASEOPEXCEPTION_HPP)
#define BASEOPEXCEPTION_HPP

#include <string>
#include <ostream>

struct BaseOpException : public std::exception
{
	std::string message;

	BaseOpException()
	{}
	BaseOpException(std::string const & rmessage) : message(rmessage) {}
	~BaseOpException() throw()
	{

	}

	char const * what() const throw()
	{
	    return message.c_str();
	}
};

std::ostream & operator<<(std::ostream & out, BaseOpException const & ex);
#endif
