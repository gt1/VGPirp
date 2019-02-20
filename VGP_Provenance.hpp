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
#if !defined(PROVENANCE_HPP)
#define PROVENANCE_HPP

#include "VGP_ProvenanceStep.hpp"
#include "VGP_NumberType.hpp"
#include <vector>

struct Provenance
{
	std::vector<ProvenanceStep> steps;

	Provenance() {}
	Provenance(LineBuffer & LB)
	{
		char const * la = NULL;
		char const * le = NULL;

		if ( ! LB.getline(&la,&le) )
		{
			throw LineBufferException("Expected Provenance line not found");
		}

		if ( la == le || *la != '!' )
		{
			throw LineBufferException(std::string("Expected Provenance line not found (no ! symbol)") + std::string(la,le));
		}

		++la;

		if ( la == le || *la != ' ' )
		{
			throw LineBufferException("Expected Provenance line not found (no space symbol after !)");
		}

		++la;

		vgp_number_type const n = BaseOp::getNumber(la,le);

		for ( vgp_number_type i = 0; i < n; ++ i )
		{
			steps.push_back(ProvenanceStep(LB));
		}
	}
};

std::ostream & operator<<(std::ostream & out, Provenance const & P);
#endif
