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
#if ! defined(IRPRESULT_HPP)
#define IRPRESULT_HPP

#include "VGP_ReadGroup.hpp"

struct IRPResult
{
	char const * forward;
	vgp_number_type forward_o;
	char const * reverse;
	vgp_number_type reverse_o;

	char const * q_forward;
	char const * q_reverse;

	ReadGroup const * RG;
	bool newrg;

	vgp_number_type p;

	IRPResult() {}
	IRPResult(
		char const * rforward,
		vgp_number_type rforward_o,
		char const * rreverse,
		vgp_number_type rreverse_o,
		char const * rq_forward,
		char const * rq_reverse,
		ReadGroup const * rRG,
		bool const rnewrg,
		vgp_number_type const rp
	) : forward(rforward), forward_o(rforward_o), reverse(rreverse), reverse_o(rreverse_o), q_forward(rq_forward), q_reverse(rq_reverse), RG(rRG), newrg(rnewrg), p(rp) {}
};

std::ostream & operator<<(std::ostream & out, IRPResult const & R);
#endif
