OUTDIR=/Object/gaklib
OBJDIR=${OUTDIR}/${HOSTNAME}
DESTFILE=${OUTDIR}/libgak${HOSTNAME}.a

BINDIR=/Object/bin/${HOSTNAME}
LIBRARIES=-lcrypt -lssl -lpthread

DEBUG=-ggdb
NO_DEBUG=-DNDEBUG -O3

CFLAGS=-I./INCLUDE -D_REENTRANT ${NO_DEBUG} -fsigned-char
CPPFLAGS=${CFLAGS} -Wno-deprecated -Wno-invalid-offsetof

OBJECTS=${OBJDIR}/acls.o \
	${OBJDIR}/ansiChar.o \
	${OBJDIR}/array.o \
	${OBJDIR}/cgitools.o \
	${OBJDIR}/cmdlineParser.o \
	${OBJDIR}/cppParser.o \
	${OBJDIR}/cppPreprocessor.o \
	${OBJDIR}/css.o \
	${OBJDIR}/csv.o \
	${OBJDIR}/crypto.o \
	${OBJDIR}/cryptoAES.o \
	${OBJDIR}/cryptoRSA.o \
	${OBJDIR}/cryptoShared.o \
	${OBJDIR}/date.o \
	${OBJDIR}/datetime.o \
	${OBJDIR}/diff.o \
	${OBJDIR}/directory.o \
	${OBJDIR}/directoryEntry.o \
	${OBJDIR}/dirLink.o \
	${OBJDIR}/dirlist.o \
	${OBJDIR}/dynamic.o \
	${OBJDIR}/exif.o \
	${OBJDIR}/exception.o \
	${OBJDIR}/fieldSet.o \
	${OBJDIR}/fileID.o \
	${OBJDIR}/filename.o \
	${OBJDIR}/fLink.o \
	${OBJDIR}/fmtNumber.o \
	${OBJDIR}/fProtect.o \
	${OBJDIR}/fraction.o \
	${OBJDIR}/freadSTR.o \
	${OBJDIR}/fstream.o \
	${OBJDIR}/getcwd.o \
	${OBJDIR}/gps.o \
	${OBJDIR}/hash.o \
	${OBJDIR}/hostResolver.o \
	${OBJDIR}/html.o \
	${OBJDIR}/htmlParser.o \
	${OBJDIR}/http.o \
	${OBJDIR}/httpBaseServer.o \
	${OBJDIR}/httpResponse.o \
	${OBJDIR}/int2mot.o \
	${OBJDIR}/list.o \
	${OBJDIR}/locker.o \
	${OBJDIR}/logfile.o \
	${OBJDIR}/makePath.o \
	${OBJDIR}/mathExpression.o \
	${OBJDIR}/mboxParser.o \
	${OBJDIR}/md5.o \
	${OBJDIR}/prime.o \
	${OBJDIR}/progParser.o \
	${OBJDIR}/quantities.o \
	${OBJDIR}/randomNumber.o \
	${OBJDIR}/relpath.o \
	${OBJDIR}/rfile.o \
	${OBJDIR}/setcwd.o \
	${OBJDIR}/sha.o \
	${OBJDIR}/soap.o \
	${OBJDIR}/socketbuf.o \
	${OBJDIR}/sslSocket.o \
	${OBJDIR}/strcmpi.o \
	${OBJDIR}/Strgclas.o \
	${OBJDIR}/string.o \
	${OBJDIR}/textReader.o \
	${OBJDIR}/thread.o \
	${OBJDIR}/time.o \
	${OBJDIR}/t_string.o \
	${OBJDIR}/tokens.o \
	${OBJDIR}/unitTest.o \
	${OBJDIR}/wideChar.o \
	${OBJDIR}/wideString.o \
	${OBJDIR}/wsdlImporter.o \
	${OBJDIR}/xml.o \
	${OBJDIR}/xmlCss.o \
	${OBJDIR}/xmlEntities.o \
	${OBJDIR}/xmlParser.o \
	${OBJDIR}/xmlValidator.o \
	${OBJDIR}/XPath.o

${BINDIR}/impwsdl: impwsdl.cpp ${DESTFILE}
	g++ ${CPPFLAGS} -o $@ $^  ${LIBRARIES}

${DESTFILE}: ${OBJECTS}
	ar -rs $@ $^

all: xclean ${DESTFILE} ${BINDIR}/impwsdl

clean:
	-rm ${OBJECTS}
	-rm test
	-rm -f /tmp/test.dat
	-rm -rf /tmp/FcopyTest
	-rm /tmp/rsaKey.*

xclean: clean
	-rm ${DESTFILE}
	-rm ${BINDIR}/impwsdl

${OBJDIR}/%.o: CTOOLS/%.c
	gcc ${CFLAGS} -c $^ -o $@

${OBJDIR}/%.o: CTOOLS/%.cpp
	g++ ${CPPFLAGS} -c $^ -o $@

test: TEST.CPP ${DESTFILE}
	g++ ${CPPFLAGS} -lpthread ${LIBRARIES} $^ -o $@
	./test
	rm test
