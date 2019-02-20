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
#include "VGP_FastQResult.hpp"

std::ostream & operator<<(std::ostream & out, FastQResult const & R)
{
	out.put('@');
	out.write(R.forward_name,R.forward_name_o);
	out.put('\n');
	out.write(R.forward,R.forward_o);
	out.put('\n');
	out.put('+');
	out.put('\n');
	out.write(R.q_forward,R.forward_o);
	out.put('\n');

	out.put('@');
	out.write(R.reverse_name,R.reverse_name_o);
	out.put('\n');
	out.write(R.reverse,R.reverse_o);
	out.put('\n');
	out.put('+');
	out.put('\n');
	out.write(R.q_reverse,R.reverse_o);
	out.put('\n');

	return out;
}
