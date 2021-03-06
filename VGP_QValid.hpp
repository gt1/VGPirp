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
#if ! defined(QVALID_HPP)
#define QVALID_HPP

#include "VGP_AutoArray.hpp"

struct QValid
{
	AutoArray<char> qValid;

	QValid() : qValid(256)
	{
		for ( int i = 0; i < 256; ++i )
			if ( i >= '!' && i <= '~' )
				qValid[i] = 1;
			else
				qValid[i] = 0;
	}
};
#endif
