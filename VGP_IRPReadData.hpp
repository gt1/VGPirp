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
#if ! defined(IRPREADDATA_HPP)
#define IRPREADDATA_HPP

#include "VGP_AutoArray.hpp"
#include "VGP_NumberType.hpp"

struct IRPReadData
{
	AutoArray<char> N;
	vgp_number_type N_o;

	AutoArray<char> S;
	vgp_number_type S_o;
	AutoArray<char> Q;
	vgp_number_type Q_o;

	IRPReadData() : N(), N_o(0), S(), S_o(0), Q(), Q_o(0) {}
};
#endif
