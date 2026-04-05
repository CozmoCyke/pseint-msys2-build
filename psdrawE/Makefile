include ../configs/config.${ARCH}
OBJS_DIR=../${TEMP}/psdrawE
CXXFLAGS+=${WX_CXXFLAGS} -D_FOR_EXPORT -I. -Wno-narrowing
LDFLAGS+=${WX_LDFLAGS}

ifdef WINDOWS
ICON=${OBJS_DIR}/icon.o
else
ICON=
endif


all: ${OBJS_DIR} ../${BIN}/psdrawE${EXT}

include Makefile.common

OBJS+=${OBJS_DIR}/LocalizationManager.o ${OBJS_DIR}/KeywordForms.o

${OBJS_DIR}/LocalizationManager.o: ../wxPSeInt/LocalizationManager.cpp
	${GPP} ${CXXFLAGS} -c ../wxPSeInt/LocalizationManager.cpp -o $@

${OBJS_DIR}/KeywordForms.o: ../pseint/KeywordForms.cpp ../pseint/KeywordForms.h ../pseint/Keywords.hpp ../pseint/LocalizationManager.h ../pseint/utils.h
	${GPP} ${CXXFLAGS} -c ../pseint/KeywordForms.cpp -o $@


clean:
	${RM} ${OBJS} ../${BIN}/psdrawE${EXT}

../${BIN}/psdrawE${EXT}: ../${BIN} ${OBJS} ${ICON}
	${GPP} ${OBJS} ${LDFLAGS} ${ICON} -o $@

${OBJS_DIR}:
	${MKDIR} ${OBJS_DIR}

ifdef WINDOWS
${ICON}: ../wxPSeInt/icon.rc ../bin/imgs/icon.ico
	${WINDRES} ${WX_CXXFLAGS} -i ../wxPSeInt/icon.rc -o $@
endif

../${BIN}:
	${MKDIR} ../${BIN}
