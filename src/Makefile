all: clean main_seq main_mpi
	
main_seq: main_seq.o UsageUtility.o
	gcc main_seq.o UsageUtility.o -o main_seq
	
main_seq.o: main_seq.c UsageUtility.h
	gcc -c main_seq.c
	
main_mpi: main_mpi.o UsageUtility.o
	gcc main_mpi.o UsageUtility.o -o main_mpi
	
main_mpi.o: main_mpi.c UsageUtility.h
	gcc -c main_mpi.c

	

UsageUtility.o: UsageUtility.c UsageUtility.h
	gcc -c UsageUtility.c
	
clean:
	rm -f *.o all
	rm -f main_seq
	rm -f main_mpi
