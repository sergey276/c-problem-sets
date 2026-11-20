# Variables
CC = gcc
CFLAGS = -std=c11 -Wall -Werror
LDLIBS = -lm
OUTPUT = ps1
CPPCHECK = cppcheck --enable=performance --error-exitcode=1
FORMAT_CODE = uncrustify --no-backup --replace -q

all: $(OUTPUT)

$(OUTPUT): playfair.o bmp.o main.o
	@echo "Running compilation"
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $(OUTPUT)

main.o: main.c
	@echo "Building main module"
	$(FORMAT_CODE) $<
	$(CPPCHECK) $<
	$(CC) $(CFLAGS) -c $< $(LDLIBS) -o $@

playfair.o: playfair.c
	@echo "Building playfair module"
	$(FORMAT_CODE) $<
	$(CPPCHECK) $<
	$(CC) $(CFLAGS) -c $< $(LDLIBS) -o $@

bmp.o: bmp.c
	@echo "Building bmp module"
	$(FORMAT_CODE) $<
	$(CPPCHECK) $<
	$(CC) $(CFLAGS) -c $< $(LDLIBS) -o $@

clean:
	@echo "Cleanup"
	rm -rf $(OUTPUT) *.o
