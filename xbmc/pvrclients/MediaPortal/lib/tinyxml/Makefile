INCLUDES=-I.
SRCS=tinystr.cpp \
     tinyxml.cpp \
     tinyxmlerror.cpp \
     tinyxmlparser.cpp
LIB=tinyxml.a

include ../../../Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))
