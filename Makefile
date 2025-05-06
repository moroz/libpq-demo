LDFLAGS += -lpq

% : %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
