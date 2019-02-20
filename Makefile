CXXFLAGS=-O3 -g

BINARIES=VGP_fastqToIRP

HEADERS = VGP_AutoArray.hpp VGP_BaseOp.hpp VGP_BaseOpException.hpp VGP_BaseValid.hpp VGP_FastQReader.hpp VGP_FastQReaderException.hpp \
	VGP_FileType.hpp VGP_FileTypeException.hpp VGP_IRPHeader.hpp VGP_IRPReadData.hpp VGP_IRPReader.hpp VGP_IRPReaderException.hpp \
	VGP_IRPResult.hpp VGP_LineBuffer.hpp VGP_LineBufferException.hpp VGP_MaxNumberPrint.hpp VGP_NumberType.hpp VGP_Provenance.hpp \
	VGP_ProvenanceStep.hpp VGP_QValid.hpp VGP_ReadGroup.hpp

all: ${BINARIES}

VGP_BaseOpException.o: VGP_BaseOpException.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c VGP_BaseOpException.cpp -o VGP_BaseOpException.o
VGP_FileType.o: VGP_FileType.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c VGP_FileType.cpp -o VGP_FileType.o
VGP_IRPHeader.o: VGP_IRPHeader.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c VGP_IRPHeader.cpp -o VGP_IRPHeader.o
VGP_IRPResult.o: VGP_IRPResult.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c VGP_IRPResult.cpp -o VGP_IRPResult.o
VGP_LineBufferException.o: VGP_LineBufferException.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c VGP_LineBufferException.cpp -o VGP_LineBufferException.o
VGP_MaxNumberPrint.o: VGP_MaxNumberPrint.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c VGP_MaxNumberPrint.cpp -o VGP_MaxNumberPrint.o
VGP_Provenance.o: VGP_Provenance.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c VGP_Provenance.cpp -o VGP_Provenance.o
VGP_ProvenanceStep.o: VGP_ProvenanceStep.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c VGP_ProvenanceStep.cpp -o VGP_ProvenanceStep.o
VGP_ReadGroup.o: VGP_ReadGroup.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c VGP_ReadGroup.cpp -o VGP_ReadGroup.o
VGP_fastqToIRP.o: VGP_fastqToIRP.cpp ${HEADERS}
	${CXX} ${CXXFLAGS} -c VGP_fastqToIRP.cpp -o VGP_fastqToIRP.o

LIBVGP_OBJECTS = VGP_BaseOpException.o VGP_FileType.o VGP_IRPHeader.o VGP_IRPResult.o VGP_LineBufferException.o VGP_MaxNumberPrint.o VGP_Provenance.o \
	VGP_ProvenanceStep.o VGP_ReadGroup.o
libVGP.a: ${LIBVGP_OBJECTS}
	ar rc libVGP.a ${LIBVGP_OBJECTS}

VGP_fastqToIRP: libVGP.a VGP_fastqToIRP.o
	${CXX} -o VGP_fastqToIRP VGP_fastqToIRP.o -L. -lVGP

clean:
	rm -f ${LIBVGP_OBJECTS}
distclean: clean
	rm -f ${BINARIES} libVGP.a
	
