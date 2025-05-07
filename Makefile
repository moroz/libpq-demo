LDFLAGS += -lpq

UNAME_S = $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CFLAGS += -I$(shell brew --prefix libpq)/include
	LDFLAGS += -L$(shell brew --prefix libpq)/lib
else
	CFLAGS += -I/usr/share/include/postgresql
endif

% : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
