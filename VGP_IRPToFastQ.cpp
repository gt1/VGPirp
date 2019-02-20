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
#include "VGP_IRPReader.hpp"
#include "VGP_FastQReader.hpp"
#include <fstream>

int main(int argc, char * argv[])
{
	try
	{
		IRPReader IRP(std::cin,1024*1024);

		// std::cout << IRP.header;
		IRPResult const * R = NULL;
		for ( vgp_number_type i = 0; (R = IRP.readData()); ++i )
		{
			if ( R->q_forward && R->q_reverse )
			{
				std::cout << '@' << i << "_F" << '\n';
				std::cout.write(R->forward,R->forward_o);
				std::cout.put('\n');
				std::cout << '+' << '\n';
				std::cout.write(R->q_forward,R->forward_o);
				std::cout.put('\n');

				std::cout << '@' << i << "_R" << '\n';
				std::cout.write(R->reverse,R->reverse_o);
				std::cout.put('\n');
				std::cout << '+' << '\n';
				std::cout.write(R->q_reverse,R->reverse_o);
				std::cout.put('\n');
			}
			else
			{
				std::cout << '>' << i << "_F" << '\n';
				std::cout.write(R->forward,R->forward_o);
				std::cout.put('\n');

				std::cout << '>' << i << "_R" << '\n';
				std::cout.write(R->reverse,R->reverse_o);
				std::cout.put('\n');
			}
		}
	}
	catch(std::exception const & ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}
