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
#include "VGP_Provenance.hpp"

std::ostream & operator<<(std::ostream & out, Provenance const & P)
{
	out << "! " << P.steps.size() << "\n";
	
	for ( std::size_t i = 0; i < P.steps.size(); ++i )
		out << P.steps[i] << "\n";
	
	return out;
}
