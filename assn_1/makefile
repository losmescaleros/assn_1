# Executable
EXEC = assn_1
# Compiler
CC = g++
# Compiler options
CFLAGS = -c -Wall
# Files
OUT_FILE = output.txt
KEY_DB = data/example/key.db
SEEK_DB = data/example/seek.db
EXAMPLE_OUT_FILE = data/example/output.txt
TEST_RESULTS = test_results.txt

all: main

main: main.o
	$(CC) -o $(EXEC) main.o
	
main.o: main.c
	$(CC) $(CFLAGS) main.c
	
test: main
	rm -f $(TEST_RESULTS)
	rm -f $(OUT_FILE)
	./$(EXEC) --mem-lin $(KEY_DB) $(SEEK_DB) > $(OUT_FILE)
	echo "Test 1: In Memory Linear Search" >> $(TEST_RESULTS)
	-diff $(OUT_FILE) $(EXAMPLE_OUT_FILE) >> $(TEST_RESULTS)
	rm -f $(OUT_FILE)
	./$(EXEC) --mem-bin $(KEY_DB) $(SEEK_DB) > $(OUT_FILE)
	echo "Test 2: In Memory Binary Search" >> $(TEST_RESULTS)
	-diff $(OUT_FILE) $(EXAMPLE_OUT_FILE) >> $(TEST_RESULTS)
	rm -f $(OUT_FILE)
	./$(EXEC) --disk-lin $(KEY_DB) $(SEEK_DB) > $(OUT_FILE)
	echo "Test 3: On Disk Linear Search" >> $(TEST_RESULTS)
	-diff $(OUT_FILE) $(EXAMPLE_OUT_FILE) >> $(TEST_RESULTS)
	rm -f $(OUT_FILE)
	./$(EXEC) --disk-bin $(KEY_DB) $(SEEK_DB) > $(OUT_FILE)	
	echo "Test 4: On Disk Binary Search" >> $(TEST_RESULTS)
	-diff $(OUT_FILE) $(EXAMPLE_OUT_FILE) >> $(TEST_RESULTS)
	rm -f $(OUT_FILE)
	
	