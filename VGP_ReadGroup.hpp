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
#if ! defined(READGROUP_HPP)
#define READGROUP_HPP

#include "VGP_AutoArray.hpp"
#include "VGP_NumberType.hpp"
#include <ostream>

struct ReadGroup
{
	AutoArray<char> groupheader;
	vgp_number_type groupheader_o;
	AutoArray<char> groupfile;
	vgp_number_type groupfile_o;
	vgp_number_type numreads;
	vgp_number_type numseen;

	ReadGroup() : groupheader(), groupheader_o(0), groupfile(), groupfile_o(0), numreads(0), numseen(0) {}
};

std::ostream & operator<<(std::ostream & out, ReadGroup const & G);
#endif
