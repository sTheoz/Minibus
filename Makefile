# CHAPITRE 1 : BUT FINAL

but : ST

# CHAPITRE 2 : VARIABLES
IA_OFILES = ligueIA/ST.o

CC = gcc

CFLAGS = -Wall -Wextra -std=c99

# CHAPITRE 3 : DEPENDANCES (REGLES IMPLICITES)

ligueIA/ST.o : ligueIA/ST.c

#CHAPITRE 4 : DEPENDANCES AVEC COMMANDES

all : ST

ST : $(IA_OFILES)
	$(CC) $(CFLAGS) -o ligueIA/$@ $^ -lm


#CHAPITRE 5 : NETTOYAGE DES FICHIERS GENERES

clean :
	-rm -f $(IA_OFILES)

play :
	java -jar minibus.jar -b2 -ld -lo C:ligueIA/ST C:ligueIA/IA_LEANNA;\
	cd html;\
	php7.3 -S localhost:8888

help :
	clear;\
	echo "To compile the game do 'make'";\
	echo "To clear o_files do 'make clean'";\
	echo "To launch the game do 'make play'";\
	echo "To display this manual do 'make help'"

#CHAPITRE 6 : BUTS FACTICES

.PHONY : but clean

.PHONY : but play

.PHONY : but help