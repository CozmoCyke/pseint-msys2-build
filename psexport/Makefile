include ../configs/config.${ARCH}

OBJS_DIR=../${TEMP}/psexport
CXXFLAGS+=-I../pseint -D_FOR_PSEXPORT

ifdef WINDOWS
GPP := g++
GCC := gcc
WINDRES := windres
MKDIR := mkdir -p
RM := rm -f
endif

all: ${OBJS_DIR} ../${BIN}/psexport${EXT}

include Makefile.common

clean:
	${RM} ${OBJS} ../${BIN}/psexport${EXT}

../${BIN}/psexport${EXT}: ../${BIN} ${OBJS}
	${GPP} ${OBJS} ${LDFLAGS} -o $@

${OBJS_DIR}:
	${MKDIR} ${OBJS_DIR}

../${BIN}:
	${MKDIR} ../${BIN}