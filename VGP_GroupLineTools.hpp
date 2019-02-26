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
#if ! defined(GROUPLINETOOLS_HPP)
#define GROUPLINETOOLS_HPP

#include "VGP_MaxNumberPrint.hpp"
#include "VGP_HeaderTools.hpp"
#include <string>
#include <vector>
#include <sstream>

struct GroupLineTools
{
	static std::string getGroupLineMax(std::string const & lane, vgp_number_type n)
	{
		std::ostringstream ostr;

		ostr << "g"
			<< " " << MaxNumberPrint(n)
			<< " " << lane.size() << " " << lane;

		return ostr.str();
	}

	static std::string getGroupLineMax(std::string const & lane, std::vector<std::string> Vin, vgp_number_type n)
	{
		std::ostringstream ostr;

		ostr << "g"
			<< " " << MaxNumberPrint(n)
			<< " " << lane.size() << " " << lane;

		std::ostringstream fnostr;

		for ( std::vector<std::string>::size_type i = 0; i < Vin.size(); ++i )
		{
			if ( i )
				fnostr << ",";

			fnostr << HeaderTools::quote(Vin[i]);
		}

		std::string const fn = fnostr.str();

		ostr << " " << fn.size() << " " << fn;

		return ostr.str();
	}

	static std::string getGroupLine(std::string const & lane, std::vector<std::string> Vin, vgp_number_type n, std::size_t const minlen)
	{
		std::ostringstream ostr;

		ostr << "g"
			<< " " << n
			<< " " << lane.size() << " " << lane;

		std::ostringstream fnostr;

		for ( std::vector<std::string>::size_type i = 0; i < Vin.size(); ++i )
		{
			if ( i )
				fnostr << ",";

			fnostr << HeaderTools::quote(Vin[i]);
		}

		std::string const fn = fnostr.str();

		ostr << " " << fn.size() << " " << fn;

		std::string s = ostr.str();

		if ( s.size() < minlen )
		{
			std::string const pad(minlen-s.size(),' ');
			s += pad;
		}

		return s;
	}
};
#endif
