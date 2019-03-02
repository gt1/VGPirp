/**
    Copyright (C) 2009-2019 German Tischler
    Copyright (C) 2011-2014 Genome Research Limited

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
#if ! defined(HEADERTOOLS_HPP)
#define HEADERTOOLS_HPP

#include <string>
#include <sstream>

struct HeaderTools
{
	static std::string quote(std::string const s)
	{
		std::ostringstream ostr;

		ostr << '"';

		for ( std::string::size_type i = 0; i < s.size(); ++i )
			if ( s[i] == '"' )
			{
				ostr.put('\\');
				ostr.put('#');
			}
			else
			{
				ostr.put(s[i]);
			}

		ostr << '"';

		return ostr.str();
	}
};
#endif
