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
#if ! defined(SAMREADER_HPP)
#define SAMREADER_HPP

#include "VGP_FileType.hpp"
#include "VGP_IRPReadData.hpp"
#include "VGP_IRPReaderException.hpp"
#include "VGP_IRPResult.hpp"
#include "VGP_Provenance.hpp"
#include "VGP_MaxNumberPrint.hpp"
#include "VGP_IRPHeader.hpp"
#include "VGP_BaseValid.hpp"
#include "VGP_QValid.hpp"
#include "VGP_SAMReaderException.hpp"
#include <sstream>
#include <iostream>
#include <map>
#include <algorithm>

struct PBRRecord
{
	vgp_number_type well;
	vgp_number_type firstpulse;
	vgp_number_type lastpulse;
	double score;

	char const * sequence;
	std::size_t sequence_o;

	char const * N;
	std::size_t N_o;

	char const * PW;
	std::size_t PW_o;

	PBRRecord() {}
	PBRRecord(
		vgp_number_type rwell,
		vgp_number_type rfirstpulse,
		vgp_number_type rlastpulse,
		double rscore,
		char const * rsequence,
		std::size_t rsequence_o,
		char const * rN,
		std::size_t rN_o,
		char const * rPW,
		std::size_t rPW_o
	) : well(rwell), firstpulse(rfirstpulse), lastpulse(rlastpulse), score(rscore),
	    sequence(rsequence),
	    sequence_o(rsequence_o),
	    N(rN), N_o(rN_o),
	    PW(rPW), PW_o(rPW_o)
	{

	}
};

/*
 */
struct SAMReader : public BaseValid, public QValid
{

	struct TagField
	{
		std::pair<char,char> K;
		char type;
		std::pair<char const *, char const *> T;

		TagField() {}
		TagField(std::pair<char,char> const & rK) : K(rK)
		{
		}
		TagField(
			std::pair<char,char> rK,
			char const rtype,
			std::pair<char const *, char const *> rT
		) : K(rK), type(rtype), T(rT)
		{
		}

		bool operator<(TagField const & O) const
		{
			return K < O.K;
		}
	};

	struct ReadGroup
	{
		unsigned long h;
		std::string ID;
		std::string PU;

		ReadGroup()
		{

		}
		ReadGroup(unsigned long const rh) : h(rh) {}
		ReadGroup(
			unsigned long const rh,
			std::string const & rID,
			std::string const & rPU
		) : h(rh), ID(rID), PU(rPU) {}

		bool operator<(ReadGroup const & O) const
		{
			return h < O.h;
		}
	};

	static unsigned long hash(char const * a, char const * e)
	{
		unsigned long hash = 2166136261;;

		for ( ; a != e; ++a )
		{
			hash = hash ^ static_cast<unsigned long>(static_cast<unsigned char>(*a));
			hash = hash * 16777619UL;
			hash &= 0xFFFFFFFFUL;
		}

		return hash;
	}

	std::size_t numfields;
	std::size_t numtags;
	std::size_t numrg;

	std::istream & in;
	LineBuffer LB;

	AutoArray< std::pair<char const *,char const *> > T;
	AutoArray< TagField > TAGS;
	AutoArray< ReadGroup > RG;
	AutoArray< char > PW;
	AutoArray< char > N;

	std::istringstream rqstr;

	PBRRecord record;

	void readHeader()
	{
		char const * la = NULL;
		char const * le = NULL;

		// do we have a next line?
		while ( LB.getline(&la,&le) )
		{
			if ( la != le && *la == '@' )
			{
				if ( le-la >= 4 && la[1] == 'R' && la[2] == 'G' && la[3] == '\t' )
				{
					char const * linestart = la;

					la += 4;

					// std::cerr << "Read group " << std::string(la,le) << std::endl;

					std::size_t i = 0;

					while ( la != le )
					{
						char const * lc = la;

						while ( lc != le && *lc != '\t' )
							++lc;

						while ( i >= T.size() )
							T.bump();

						assert ( i < T.size() );

						T[i++] = std::pair<char const *,char const *>(la,lc);

						if ( lc != le )
						{
							assert ( *lc == '\t' );
							++lc;
						}


						la = lc;
					}

					std::size_t const numfields = i;

					std::size_t t = 0;
					for ( std::size_t j = 0; j < numfields; ++j )
					{
						if ( T[j].second - T[j].first < 3 || T[j].first[2] != ':' )
						{
							throw SAMReaderException(std::string("SAMReader::readHeader: broken RG field : ")+ std::string(T[j].first,T[j].second));
						}

						while ( t >= TAGS.size() )
							TAGS.bump();
						assert ( t < TAGS.size() );

						TAGS[t++] = TagField(
							std::pair<char,char>(T[j].first[0],T[j].first[1]),
							0,
							std::pair<char const *,char const *>(T[j].first+3,T[j].second)
						);
					}
					std::sort(TAGS.begin(),TAGS.begin()+t);

					numtags = t;

					TagField const * ID = getTag('I','D');

					if ( ! ID )
						throw SAMReaderException(std::string("SAMReader::readHeader: broken RG field (no ID tag) : ")+ std::string(linestart,le));

					TagField const * PU = getTag('P','U');

					if ( ! PU )
						throw SAMReaderException(std::string("SAMReader::readHeader: broken RG field (no PU tag) : ")+ std::string(linestart,le));

					std::string const sID(ID->T.first,ID->T.second);
					std::string const sPU(PU->T.first,PU->T.second);

					unsigned long const h = hash(ID->T.first,ID->T.second);

					//std::cerr << "sID=" << sID << " h=" << h << " sPU=" << sPU << std::endl;

					while ( numrg >= RG.size() )
						RG.bump();

					RG[numrg++] = ReadGroup(h,sID,sPU);

					numtags = 0;
				}
			}
			else
			{
				LB.putback(la,le);
				break;
			}
		}

		std::sort(RG.begin(),RG.begin()+numrg);

		#if 0
		for ( uint64_t i = 0; i < numrg; ++i )
			std::cerr << RG[i].h << "," << RG[i].ID << "," << RG[i].PU << std::endl;
		#endif
	}

	SAMReader(::std::istream & rin, std::size_t buffersize = 1)
	: numfields(0), numtags(0), numrg(0), in(rin), LB(in,buffersize)
	{
		readHeader();
	}

	TagField const * getTag(char const a, char const b) const
	{
		std::pair<TagField const *, TagField const *> P =
			std::equal_range(
				TAGS.begin(),
				TAGS.begin() + numtags,
				TagField(std::pair<char,char>(a,b))
			);

		// std::cerr << "range " << P.second-P.first << std::endl;

		if ( (P.second - P.first) == 1 )
			return P.first;
		else
			return NULL;

	}

	static vgp_number_type parseNumber(char const * c, char const * e)
	{
		vgp_number_type v = 0;

		while ( c != e )
		{
			v *= 10;
			v += *(c++)-'0';
		}

		return v;
	}

	PBRRecord * readData()
	{
		char const * la = NULL;
		char const * le = NULL;

		if ( LB.getline(&la,&le) )
		{
			std::size_t i = 0;
			char const * linestart = la;

			while ( la != le )
			{
				char const * lc = la;

				while ( lc != le && *lc != '\t' )
					++lc;

				while ( i >= T.size() )
					T.bump();

				assert ( i < T.size() );

				T[i++] = std::pair<char const *,char const *>(la,lc);

				if ( lc != le )
				{
					assert ( *lc == '\t' );
					++lc;
				}


				la = lc;
			}

			if ( i < 11 )
			{
				throw SAMReaderException(std::string("SAMReader::readData: line is missing fixed fields: ")+ std::string(linestart,le));
			}

			#if 0
			for ( std::size_t j = 0; j < i; ++j )
			{
				std::cerr << "T[" << j << "]=" << std::string(T[j].first,T[j].second) << std::endl;
			}
			#endif

			numfields = i;

			std::size_t t = 0;
			for ( std::size_t j = 11; j < i; ++j )
			{
				if ( T[j].second - T[j].first < 5 || T[j].first[2] != ':' || T[j].first[4] != ':' )
				{
					throw SAMReaderException(std::string("SAMReader::readData: broken tag field : ")+ std::string(T[j].first,T[j].second));
				}

				while ( t >= TAGS.size() )
					TAGS.bump();
				assert ( t < TAGS.size() );

				TAGS[t++] = TagField(
					std::pair<char,char>(T[j].first[0],T[j].first[1]),
					T[j].first[3],
					std::pair<char const *,char const *>(T[j].first+5,T[j].second)
				);
			}
			std::sort(TAGS.begin(),TAGS.begin()+t);

			numtags = t;

			#if 0
			for ( std::size_t j = 0; j < numtags; ++j )
			{
				std::cerr << TAGS[j].K.first << TAGS[j].K.second << "=" << TAGS[j].type << " " << std::string(TAGS[j].T.first,TAGS[j].T.second) << std::endl;
			}
			#endif

			TagField const * zmtag = getTag('z','m');

			if ( ! zmtag || zmtag->type != 'i' )
				throw SAMReaderException(std::string("SAMReader::readData: required zm tag not found in ") + std::string(linestart,le));

			TagField const * qstag = getTag('q','s');

			if ( ! qstag || qstag->type != 'i' )
				throw SAMReaderException(std::string("SAMReader::readData: required qs tag not found in ") + std::string(linestart,le));

			TagField const * qetag = getTag('q','e');

			if ( ! qetag || qetag->type != 'i' )
				throw SAMReaderException(std::string("SAMReader::readData: required qe tag not found in ") + std::string(linestart,le));

			TagField const * rqtag = getTag('r','q');

			if ( ! rqtag || rqtag->type != 'f' )
				throw SAMReaderException(std::string("SAMReader::readData: required rq tag not found in ") + std::string(linestart,le));

			rqstr.str(std::string(rqtag->T.first,rqtag->T.second));
			rqstr.clear();

			double rq;
			rqstr >> rq;

			if ( ! rqstr || rqstr.peek() != std::istream::traits_type::eof() )
			{
				throw SAMReaderException(std::string("SAMReader::readData: unable to parse rq tag in ") + std::string(linestart,le));
			}

			TagField const * RGtag = getTag('R','G');
			if ( !RGtag || RGtag->type != 'Z' )
				throw SAMReaderException(std::string("SAMReader::readData: required RG tag missing in ") + std::string(linestart,le));

			unsigned long const h = hash(RGtag->T.first,RGtag->T.second);

			vgp_number_type const qs = parseNumber(qstag->T.first,qstag->T.second);
			vgp_number_type const qe = parseNumber(qetag->T.first,qetag->T.second);
			vgp_number_type const zm = parseNumber(zmtag->T.first,zmtag->T.second);

			std::pair<ReadGroup const *, ReadGroup const *> const PRG = std::equal_range(RG.begin(),RG.begin()+numrg,ReadGroup(h));
			if (
				PRG.second - PRG.first != 1
				||
				(RGtag->T.second-RGtag->T.first) != static_cast< std::ptrdiff_t> (PRG.first->ID.size())
				||
				strncmp(
					RGtag->T.first,
					PRG.first->ID.c_str(),
					PRG.first->ID.size()
				) != 0
			)
			{
				throw SAMReaderException(std::string("SAMReader::readData: no read group found for ") + std::string(RGtag->T.first,RGtag->T.second));
			}

			// std::string const * sPU = &(PRG.first->PU);

			char const * nstart = NULL;
			char const * nend = NULL;

			TagField const * sntag = getTag('s','n');

			if (
				sntag && sntag->type == 'B'
				&&
				sntag->T.second-sntag->T.first >= 2 &&
				sntag->T.first[0] == 'f' &&
				sntag->T.first[1] == ','
			)
			{
				nstart = sntag->T.first + 2;
				nend = sntag->T.second;

				std::size_t o = 0;
				for ( char const * c = nstart; c != nend; ++c )
				{
					char const d = (*c == ',') ? ' ' : *c;

					while ( o >= N.size() )
						N.bump();

					N[o++] = d;
				}

				nstart = N.begin();
				nend = nstart + o;
			}

			TagField const * pwtag = getTag('p','w');
			std::size_t PWo = 0;

			char const * pwstart = NULL;
			char const * pwend = NULL;

			if ( pwtag && pwtag->type == 'B' &&
				pwtag->T.second-pwtag->T.first >= 2 &&
				pwtag->T.first[0] == 'C' &&
				pwtag->T.first[1] == ','
			)
			{
				char const * a = pwtag->T.first+2;
				char const * e = pwtag->T.second;

				while ( a != e )
				{
					char const * c = a;
					vgp_number_type n = 0;
					unsigned int ni = 0;
					while ( c != e && *c >= '0' && *c <= '9' )
					{
						n *= 10;
						n += *(c++) - '0';
						++ni;
					}

					if ( ! ni )
						throw SAMReaderException(std::string("SAMReader::readData: pw tag contains empty number ") + std::string(pwtag->T.first,pwtag->T.second));

					while ( PWo >= PW.size() )
						PW.bump();

					PW[PWo++] = std::min(n,static_cast<vgp_number_type>(4)) + '0';

					a = c;

					if ( a != e )
					{
						if ( *a == ',' )
							++a;
						else
							throw SAMReaderException(std::string("SAMReader::readData: pw tag malford (no comma after number) ") + std::string(pwtag->T.first,pwtag->T.second));
					}
				}

				pwstart = PW.begin();
				pwend = pwstart + PWo;
			}

			// pw=B C,5,6,3,3,19,15,16,3,4,3,4,5,7,13,10,3,14,1,15,6,20,15,3,28,6

			#if 0
			std::cerr << "rq=" << rq << " zm=" << zm << " qs=" << qs << " qe=" << qe << " PU=" << (*sPU) << std::endl;
			#endif

			#if 0
			if ( nend != nstart )
				std::cerr << "sn=" << std::string(nstart,nend) << std::endl;
			#endif

			record = PBRRecord(
				zm,
				qs,
				qe,
				rq,
				T[9].first,
				T[9].second-T[9].first,
				nstart,
				nend-nstart,
				pwstart,
				pwend-pwstart
			);

			return &record;
		}
		else
		{
			return NULL;
		}
	}
};
#endif
