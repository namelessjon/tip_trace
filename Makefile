# Makefile
# Jonathan D. Stott <jonathan.stott@gmail.com>

CFLAGS=-Wall -g


core_trace: core_trace.o libtiptrace.a
	$(CC) $(CFLAGS) -o $@ core_trace.o -L. -ltiptrace -lz -lm


libtiptrace.a: find_tips.o find_isoline.o calculate_tip_coordinates.o
	$(AR) rcs $@ $^

# Make the components of the library

find_tips.o: find_tips.c tip_trace.h point_t.h

find_isoline.o: find_isoline.c point_t.h

calculate_tip_coordinates.o: calculate_tip_coordinates.c point_t.h


.PHONY: clean

clean:
	@rm -f *.o
