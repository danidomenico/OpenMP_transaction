#CC  = icc -openmp -g 
#CFLAGS  = -O2 
CC  = g++ -fopenmp 
CFLAGS  = -O3 
OBJS	= ListItemset.o
OBJS2   = Database.o Itemset.o HashTree.o
OPTS	= -DBALT 
LIBS =  -lm 
LD = ld

TARGET  = no_output_apriori

default:	$(TARGET)

clean:
	rm -rf *~ *.o $(TARGET)

no_output_apriori: no_output_parapr.C $(OBJS) $(OBJS2) 
	$(CC) $(CFLAGS) $(OPTS)\
	-o no_output_apriori no_output_parapr.C $(OBJS) $(OBJS2) $(LIBS)

Database.o: Database.C Database.h
	$(CC) $(CFLAGS) $(OPTS) -c Database.C

Itemset.o: Itemset.C Itemset.h 
	$(CC) $(CFLAGS) $(OPTS) -c Itemset.C

ListItemset.o: ListItemset.C ListItemset.h
	$(CC) $(CFLAGS) -c ListItemset.C

HashTree.o: HashTree.C HashTree.h
	$(CC) $(CFLAGS) $(OPTS) -c HashTree.C

dec_shlib.o: dec_shlib.C dec_shlib.h
	$(CC) $(CFLAGS) $(OPTS) -c dec_shlib.C 

dec_llsc.o: dec_llsc.s llsc.h
	cc  -c dec_llsc.s 

