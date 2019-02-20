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
#if ! defined(FASTQRESULT_HPP)
#define FASTQRESULT_HPP

#include "VGP_NumberType.hpp"
#include <ostream>

struct FastQResult
{
	char const * forward_name;
	vgp_number_type forward_name_o;

	char const * forward;
	vgp_number_type forward_o;
	char const * q_forward;

	char const * reverse_name;
	vgp_number_type reverse_name_o;

	char const * reverse;
	vgp_number_type reverse_o;
	char const * q_reverse;

	vgp_number_type p;

	FastQResult() {}
	FastQResult(
		char const * rforward_name,
		vgp_number_type rforward_name_o,

		char const * rforward,
		vgp_number_type rforward_o,
		char const * rq_forward,

		char const * rreverse_name,
		vgp_number_type rreverse_name_o,

		char const * rreverse,
		vgp_number_type rreverse_o,
		char const * rq_reverse,

		vgp_number_type rp
	) :
		forward_name(rforward_name),
		forward_name_o(rforward_name_o),
		forward(rforward),
		forward_o(rforward_o),
		q_forward(rq_forward),
		reverse_name(rreverse_name),
		reverse_name_o(rreverse_name_o),
		reverse(rreverse),
		reverse_o(rreverse_o),
		q_reverse(rq_reverse),
		p(rp)
	{

	}
};

std::ostream & operator<<(std::ostream & out, FastQResult const & R);
#endif
