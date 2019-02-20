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

std::string quote(std::string const s)
{
	std::ostringstream ostr;

	ostr << '"';

	for ( uint64_t i = 0; i < s.size(); ++i )
		if ( s[i] == '"' )
		{
			ostr.put('\\');
			ostr.put('#');
		}
		else
		{
			ostr.put(s[i]);
		}

	ostr << '"';

	return ostr.str();
}

std::string getGroupLineMax(std::string const & lane, std::vector<std::string> Vin, vgp_number_type n)
{
	std::ostringstream ostr;

	ostr << "g"
		<< " " << MaxNumberPrint(n)
		<< " " << lane.size() << " " << lane;

	std::ostringstream fnostr;

	for ( uint64_t i = 0; i < Vin.size(); ++i )
	{
		if ( i )
			fnostr << ",";

		fnostr << quote(Vin[i]);
	}

	std::string const fn = fnostr.str();

	ostr << " " << fn.size() << " " << fn;

	return ostr.str();
}

std::string getGroupLine(std::string const & lane, std::vector<std::string> Vin, vgp_number_type n, std::size_t const minlen)
{
	std::ostringstream ostr;

	ostr << "g"
		<< " " << n
		<< " " << lane.size() << " " << lane;

	std::ostringstream fnostr;

	for ( uint64_t i = 0; i < Vin.size(); ++i )
	{
		if ( i )
			fnostr << ",";

		fnostr << quote(Vin[i]);
	}

	std::string const fn = fnostr.str();

	ostr << " " << fn.size() << " " << fn;

	std::string s = ostr.str();

	if ( s.size() < minlen )
	{
		std::string const pad(minlen-s.size(),' ');
		s += pad;
	}

	return s;
}

int main(int argc, char * argv[])
{
	try
	{
		if ( argc < 2 )
		{
			std::cerr << "usage: " << argv[0] << " <out.irp> <in.fastq> [<in.fastq>]" << std::endl;
			return EXIT_FAILURE;
		}

		std::vector<std::string> Vin;
		for ( uint64_t i = 2; i < argc; ++i )
			Vin.push_back(argv[i]);

		std::string const lane = "lane";
		std::size_t const mingrouplensize = getGroupLineMax(lane,Vin,0).size();
		std::string readgroupline = getGroupLine(lane,Vin,0,mingrouplensize);

		IRPHeader header;

		header.FT.filetype = "irp";
		header.FT.fileversion = 1;
		header.FT.filesubversion = 0;

		std::ostringstream comostr;
		for ( uint64_t i = 0; i < argc; ++i )
		{
			if ( i )
				comostr << ' ';
			comostr << quote(argv[i]);
		}
		std::string const com = comostr.str();

		header.VP.push_back(
			ProvenanceStep(
				argv[0],
				"0.0",
				com,
				"now"
			)
		);

		header.headerMap['#']['P'] = 0;
		header.headerMap['#']['S'] = 0;
		header.headerMap['@']['S'] = 0;
		header.headerMap['+']['S'] = 0;
		header.headerMap['#']['!'] = 1;

		std::fstream out(argv[1], std::ios::in | std::ios::out | std::ios::trunc);

		std::ifstream * pistr0(new std::ifstream(argv[2]));
		std::ifstream * pistr1 = NULL;

		if ( ! pistr0->is_open() )
		{
			std::cerr << "[E] unable to open " << argv[2] << std::endl;
			return EXIT_FAILURE;
		}

		if ( 3 < argc )
		{
			std::ifstream * tistr1(new std::ifstream(argv[3]));
			pistr1 = tistr1;

			if ( !pistr1->is_open() )
			{
				std::cerr << "[E] unable to open " << argv[3] << std::endl;
				return EXIT_FAILURE;
			}
		}

		out << header;
		out << readgroupline << "\n";

		// std::cout << header;

		vgp_number_type numpairs = 0;
		vgp_number_type numseq = 0;
		vgp_number_type maxreadlen = 0;
		vgp_number_type totalbp = 0;

		FastQReader * pFQR = NULL;

		if ( pistr1 )
			pFQR = new FastQReader(*pistr0,*pistr1,1024*1024);
		else
			pFQR = new FastQReader(*pistr0,1024*1024);

		FastQReader & FQR = *pFQR;

		FastQResult const * R = NULL;
		while ( (R = FQR.readData()) )
		{
			numpairs += 1;
			numseq += 2;

			out << 'P' << '\n';

			out << 'S' << ' ' << R->forward_o << ' ';
			out.write(R->forward,R->forward_o);
			out << '\n';

			out << 'Q' << ' ' << R->forward_o << ' ';
			out.write(R->q_forward,R->forward_o);
			out << '\n';

			out << 'S' << ' ' << R->reverse_o << ' ';
			out.write(R->reverse,R->reverse_o);
			out << '\n';

			out << 'Q' << ' ' << R->reverse_o << ' ';
			out.write(R->q_reverse,R->reverse_o);
			out << '\n';

			maxreadlen = std::max(maxreadlen,R->forward_o);
			maxreadlen = std::max(maxreadlen,R->reverse_o);

			totalbp += R->forward_o;
			totalbp += R->reverse_o;

			#if 0
			std::cout << "pos " << R->p << std::endl;
			std::cout << *R;
			#endif
		}

		header.headerMap['#']['P'] = numpairs;
		header.headerMap['#']['S'] = numseq;
		header.headerMap['@']['S'] = maxreadlen;
		header.headerMap['+']['S'] = totalbp;

		// rewrite header and group line
		out.seekp(0);
		out << header;

		readgroupline = getGroupLine(lane,Vin,numpairs,mingrouplensize);
		out << readgroupline;

		delete pFQR;
		delete pistr1;
		delete pistr0;

		#if 0
		IRPReader IRP(std::cin,1024*1024);

		std::cout << IRP.header;
		IRPResult const * R = NULL;
		while ( (R = IRP.readData()) )
		{
			if ( R->newrg )
				std::cout << *(R->RG) << "\n";
			std::cerr << "p=" << R->p << std::endl;
			std::cout << *R;
		}
		#endif
	}
	catch(std::exception const & ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}
