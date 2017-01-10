SRC_DIR = src
MAKE_FLAGS  = -j5 LAST=550

all clean:
	$(MAKE) $(MAKE_FLAGS) -C $(SRC_DIR) $@
