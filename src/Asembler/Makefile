# Ovde idu svi folderi koji treba da se include-uju.
INCLUDE_FOLDERS = h reflex/include

# Ovo samo namesti da odgovara gcc-ovom formatu za inkludovane foldere
INCLUDES = $(addprefix -I,$(INCLUDE_FOLDERS))

# Direktorijum sa nasim source fajlovima
SRC_DIR := ./src
# Direktorijumi sa reflex-ovim source fajlovima
REFLEX_SRC_DIRS := reflex/lib reflex/unicode

# Fleg-ovi koji se koriste pri prevodjenju. Moze se dodati i -g
# ukoliko se zeli debagovanje, najbolje za debagovanje doduse da se radi iz Visual Studio-a
CPPFLAGS := -std=c++11
# kompajler koji koristimo
CC = g++

# wildcard funkcija uzima sve fajlove/foldere iz zadate putanje, ovde se subdirs
# sastoji iz dva dela, jedan je gde se uzimaju folderi iz root foldera, a drugi wildcard
# nam sluzi za dohvatanje iz reflex-ovih source foldera
subdirs := $(wildcard */) $(wildcard reflex/*/)

# opet wildcard, samo sto sada trazimo fajlove koji imaju ekstenziju .cpp
sources := $(wildcard $(addsuffix *.cpp,$(subdirs)))

# Ovde samo menjamo ekstenziju fajla iz .cpp u .o
OBJ_FILES := $(patsubst %.cpp,%.o,$(sources))


# Glavno pravilo za prevodjenje, zavisi od svih objektnih fajlova
build: $(OBJ_FILES)
	$(CC) $(CPPFLAGS) $(INCLUDES) $(OBJ_FILES) -o assembler


# pravilo za prevodjenje .cpp fajlova u objektne fajlove
$(OBJ_FILES) : %.o : %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDES) -c -o $@ $<

# Pravilo za ciscenje, odnosno brisanje svih generisanih .o fajlova
.PHONY: clean
clean: 
	rm  $(OBJ_FILES)
	rm assembler
