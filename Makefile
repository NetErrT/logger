AR ?= ar
CC ?= cc
RM ?= rm -f

CFLAGS := -std=c99 -O2 -fPIC
CPPFLAGS := -DNDEBUG -D_XOPEN_SOURCE=600 -Wall -Wshadow -Wextra -Wpedantic -Wstrict-prototypes -Wmissing-prototypes -Walloca -Wmissing-include-dirs -Wold-style-definition

ifeq (${CC}, gcc)
CPPFLAGS += -Wlogical-op
endif

LDFLAGS :=

STATIC_LIB := liblogger.a
SHARED_LIB := liblogger.so

OBJS := logger.o

.PHONY: all
all: ${STATIC_LIB} ${SHARED_LIB}

${STATIC_LIB}: ${OBJS}
	${AR} rcs $@ ${OBJS}

${SHARED_LIB}: ${OBJS}
	${CC} ${LDFLAGS} -shared -o $@ ${OBJS}

%.o: %.c
	${CC} ${CFLAGS} ${CPPFLAGS} -c $< -o $@

.PHONY: clean
clean:
	${RM} ${OBJS} ${STATIC_LIB} ${SHARED_LIB}
