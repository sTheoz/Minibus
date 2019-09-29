# CHAPITRE 1 : BUT FINAL

but : monIA

# CHAPITRE 2 : VARIABLES

#OFILES = clt.o \
         srv.o \
		 main.o


IA_OFILES = ligueIA/monIA.o

CC = gcc

CFLAGS = -Wall -pedantic -g

# CHAPITRE 3 : DEPENDANCES (REGLES IMPLICITES)

ligueIA/monIA.o : ligueIA/monIA.c

#CHAPITRE 4 : DEPENDANCES AVEC COMMANDES

all : monIA

monIA : $(IA_OFILES)
	$(CC) $(CFLAGS) -o ligueIA/monIA $(IA_OFILES)

#CHAPITRE 5 : NETTOYAGE DES FICHIERS GENERES

clean :
	-rm -f $(IA_OFILES)

play :
	java -jar minibus.jar -b2 C:ligueIA/monIA;\
	cd html;\
	php -S localhost:8888

#CHAPITRE 6 : BUTS FACTICES

.PHONY : but clean

.PHONY : but play