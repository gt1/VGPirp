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
#include "VGP_ProvenanceStep.hpp"

std::ostream & operator<<(std::ostream & out, ProvenanceStep const & P)
{
	BaseOp::printString(out,P.name);
	out.put(' ');
	BaseOp::printString(out,P.version);
	out.put(' ');
	BaseOp::printString(out,P.command);
	out.put(' ');
	BaseOp::printString(out,P.date);
	
	return out;
}
