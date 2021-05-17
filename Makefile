#general params
CC=gcc
CFLAGS=-Wall -Werror -g -march=native -std=gnu11 -fopt-info-vec
LIBS=-lcunit -lpthread -lm
INCLUDE_HEADERS_DIRECTORY=-Iheaders
INCLUDES=src/cluster.o src/distance.o src/input.o src/output.o src/kmeans.o src/threading.o src/runner.o src/test.o
CLUSTERS_BUFFER_PATH=clusters
POINTS_BUFFER_PATH=points

kmeans: main.c src/algorithme.o src/createCSV.o src/distance.o src/distortion.o  src/parseBinary.o src/launcher.o src/combinations.c # add your other object files needed to compile your program here. !! The ordering is important !! if file_a.o depends on file_b.o, file_a.o must be placed BEFORE file_b.o in the list !
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LIBS)    # this will run the following command: gcc -Wall -Werror -g -o kmeans src/distance.o other_object_filespresent_above.o ... -lcunit -lpthread

%.o: %.c                  # if for example you want to compute example.c this will create an object file called example.o in the same directory as example.c. Don't forget to clean it in your "make clean"
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

clean:
	rm -f src/*.o
	rm -f kmeans
	rm -f tests/*.o

testing: tests/main_Tests.c src/algorithme.o src/createCSV.o src/distance.o src/distortion.o  src/parseBinary.o src/launcher.o src/combinations.o tests/Tests_algo.o tests/Tests_binary.o tests/Tests_combinations.o tests/Tests_createCSV.o tests/Tests_distance.o tests/Tests_distortion.o tests/Tests_globaux.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo
	@echo START
	@echo
	valgrind --leak-check=full -s ./testing 
	@echo
	./testing
	@echo 
	@echo

test : kmeans
	@echo
	@echo START
	@echo
	valgrind --leak-check=full -s ./kmeans -p 7  -k 5 -n 4 -f outputTest1.csv InputFiles/test200.bin
	make clean

# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests

