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
#if ! defined(PROVENANCESTEP_HPP)
#define PROVENANCESTEP_HPP

#include "VGP_BaseOp.hpp"
#include "VGP_LineBuffer.hpp"

struct ProvenanceStep
{
	std::string name;
	std::string version;
	std::string command;
	std::string date;
	
	ProvenanceStep()
	{
	}
	ProvenanceStep(
		std::string rname,
		std::string rversion,
		std::string rcommand,
		std::string rdate
	) : name(rname), version(rversion), command(rcommand), date(rdate)
	{
	}
	ProvenanceStep(LineBuffer & LB)
	{
		char const * la = NULL;
		char const * le = NULL;
		
		if ( ! LB.getline(&la,&le) )
		{
			throw LineBufferException("Expected ProvenanceStep line not found");
		}
		
		name = BaseOp::parseString(la,le);
		version = BaseOp::parseString(la,le);
		command = BaseOp::parseString(la,le);
		date = BaseOp::parseString(la,le);
	}
};

std::ostream & operator<<(std::ostream & out, ProvenanceStep const & P);
#endif
