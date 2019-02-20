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
#if ! defined(BASEVALID_HPP)
#define BASEVALID_HPP

#include "VGP_AutoArray.hpp"

struct BaseValid
{
	AutoArray<char> bValid;

	BaseValid()
	: bValid(256)
	{
		for ( int i = 0; i < 256; ++i )
			if (
				i == 'a' ||
				i == 'c' ||
				i == 'g' ||
				i == 't' ||
				i == 'n' ||
				i == 'A' ||
				i == 'C' ||
				i == 'G' ||
				i == 'T' ||
				i == 'N'
			)
				bValid[i] = 1;
			else
				bValid[i] = 0;

	}
};
#endif
