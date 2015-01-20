#include <iostream>
#include <string>
#include <sys/time.h>
#include <stdio.h>

#include "main.h"

struct timeval startTime;
struct timeval endTime;
std::string searchMode;
std::string keyFile;
std::string seekFile;

FILE *key;

void
InMemoryLinearSearch(std::string keyFile, std::string seekFile, timeval *startTime, timeval *endTime);

void
InMemoryBinarySearch(std::string keyFile, std::string seekFile, timeval *startTime, timeval *endTime);

void
OnDiskLinearSearch(std::string keyFile, std::string seekFile, timeval *startTime, timeval *endTime);

void
OnDiskBinarySearch(std::string keyFile, std::string seekFile, timeval *startTime, timeval *endTime);

bool
BinarySearchOnDisk(int target, FILE *key, long lower, long upper);

bool
BinarySearch(int target, int K[], long lower, long upper);

void
PrintResults(timeval *startTime, timeval *endTime, int S[], int hit[], long seekCount);

int 
main(int argc, char* argv[])
{
	if (argc != 4)
	{
		exit(EXIT_ERROR);
	} 
	else
	{
		searchMode = argv[1];
		keyFile = argv[2];
		seekFile = argv[3];

		if (searchMode.compare(IN_MEMORY_LINEAR_SEARCH) == 0)
		{
			InMemoryLinearSearch(keyFile, seekFile, &startTime, &endTime);
		} 
		else if (searchMode.compare(IN_MEMORY_BINARY_SEARCH) == 0)
		{
			InMemoryBinarySearch(keyFile, seekFile, &startTime, &endTime);
		} 
		else if (searchMode.compare(ON_DISK_LINEAR_SEARCH) == 0)
		{
			OnDiskLinearSearch(keyFile, seekFile, &startTime, &endTime);
		} 
		else if (searchMode.compare(ON_DISK_BINARY_SEARCH) == 0)
		{
			OnDiskBinarySearch(keyFile, seekFile, &startTime, &endTime);
		} 
		else
		{
			exit(EXIT_ERROR);
		}
	}	

	exit(EXIT_SUCCESS);
}

void 
InMemoryLinearSearch(std::string keyFile, std::string seekFile, timeval *startTime, timeval *endTime)
{
	long keySizeInBytes = 0;
	long seekSizeInBytes = 0;
	// Open and read seek db into array S
	FILE *seek = NULL;
	FILE *key = NULL;

	seek = fopen(seekFile.c_str(), "rb");
	if (seek == NULL)
	{
		exit(EXIT_ERROR);
	}

	fseek(seek, 0, SEEK_END);
	seekSizeInBytes = ftell(seek);
	long seekCount = seekSizeInBytes / sizeof(int);
	int S[seekCount];
	// Move back to the beginning
	fseek(seek, 0, SEEK_SET);

	for (long i = 0; i < seekCount; i++)
	{
		fseek(seek, i * sizeof(int), SEEK_SET);
		fread(&S[i], sizeof(int), 1, seek);
	}

	fclose(seek);
	// Open and read key db into K
	// Start recording time
	gettimeofday(startTime, NULL);

	key = fopen(keyFile.c_str(), "rb");
	if (key == NULL)
	{
		exit(EXIT_ERROR);
	}
	fseek(key, 0, SEEK_END);
	keySizeInBytes = ftell(key);
	long keyCount = keySizeInBytes / sizeof(int);
	int K[keyCount];

	// Move back to the beginning
	fseek(key, 0, SEEK_SET);

	for (long i = 0; i < keyCount; i++)
	{
		fseek(key, i * sizeof(int), SEEK_SET);
		fread(&K[i], sizeof(int), 1, key);
	}

	fclose(key);

	int hit[seekCount];

	// Linear search
	for (long i = 0; i < seekCount; i++)
	{
		hit[i] = 0;
		for (long j = 0; j < keyCount; j++)
		{
			if (S[i] == K[j])
			{
				hit[i] = 1;
				break;
			}
		}
	}
	// Search procedure complete. Mark completion time
	gettimeofday(endTime, NULL);
	// Print results
	PrintResults(startTime, endTime, S, hit, seekCount);
	
}

void
InMemoryBinarySearch(std::string keyFile, std::string seekFile, timeval *startTime, timeval *endTime)
{
	long keySizeInBytes = 0;
	long seekSizeInBytes = 0;
	// Open and read seek db into array S
	FILE *seek = NULL;
	FILE *key = NULL;

	seek = fopen(seekFile.c_str(), "rb");
	if (seek == NULL)
	{
		exit(EXIT_ERROR);
	}

	fseek(seek, 0, SEEK_END);
	seekSizeInBytes = ftell(seek);
	long seekCount = seekSizeInBytes / sizeof(int);
	int S[seekCount];
	// Move back to the beginning
	fseek(seek, 0, SEEK_SET);

	for (long i = 0; i < seekCount; i++)
	{
		fseek(seek, i * sizeof(int), SEEK_SET);
		fread(&S[i], sizeof(int), 1, seek);
	}

	fclose(seek);
	// Open and read key db into K
	// Start recording time
	gettimeofday(startTime, NULL);

	key = fopen(keyFile.c_str(), "rb");
	if (key == NULL)
	{
		exit(EXIT_ERROR);
	}
	fseek(key, 0, SEEK_END);
	keySizeInBytes = ftell(key);
	long keyCount = keySizeInBytes / sizeof(int);
	int K[keyCount];

	// Move back to the beginning
	fseek(key, 0, SEEK_SET);

	for (long i = 0; i < keyCount; i++)
	{
		fseek(key, i * sizeof(int), SEEK_SET);
		fread(&K[i], sizeof(int), 1, key);
	}

	fclose(key);

	int hit[seekCount];

	// Binary search. K is assumed to be sorted
	for (long i = 0; i < seekCount; i++)
	{
		if (BinarySearch(S[i], K, 0, keyCount - 1))
		{
			hit[i] = 1;
		} 
		else
		{
			hit[i] = 0;
		}				
	}
	// Search procedure complete. Mark completion time
	gettimeofday(endTime, NULL);
	// Print results
	PrintResults(startTime, endTime, S, hit, seekCount);
}

void 
OnDiskLinearSearch(std::string keyFile, std::string seekFile, timeval *startTime, timeval *endTime)
{
	long seekSizeInBytes = 0;
	// Open and read seek db into array S
	FILE *seek = NULL;
	FILE *key = NULL;

	seek = fopen(seekFile.c_str(), "rb");
	if (seek == NULL)
	{
		exit(EXIT_ERROR);
	}

	fseek(seek, 0, SEEK_END);
	seekSizeInBytes = ftell(seek);
	long seekCount = seekSizeInBytes / sizeof(int);
	int S[seekCount];
	// Move back to the beginning
	fseek(seek, 0, SEEK_SET);

	for (long i = 0; i < seekCount; i++)
	{
		fseek(seek, i * sizeof(int), SEEK_SET);
		fread(&S[i], sizeof(int), 1, seek);
	}

	fclose(seek);

	// Open key db for reading
	key = fopen(keyFile.c_str(), "rb");
	if (key == NULL)
	{
		exit(EXIT_ERROR);
	}

	int hit[seekCount];
	// Start recording time
	gettimeofday(startTime, NULL);

	// Perform linear, on-disk search 
	for (long i = 0; i < seekCount; i++)
	{
		// Clear any EOF errors and start at beginning of file
		fseek(key, 0, SEEK_SET);
		clearerr(key);
		hit[i] = 0;

		while (true)
		{
			int current;
			if (!fread(&current, sizeof(int), 1, key))
			{
				break;
			}

			if (S[i] == current)
			{
				hit[i] = 1;
				break;
			}
		}		
	}
	// Search procedure complete. Mark completion time and close key db
	gettimeofday(endTime, NULL);

	fclose(key);

	// Print results
	PrintResults(startTime, endTime, S, hit, seekCount);
}

void
OnDiskBinarySearch(std::string keyFile, std::string seekFile, timeval *startTime, timeval *endTime)
{
	long seekSizeInBytes = 0;
	long keySizeInBytes = 0;
	// Open and read seek db into array S
	FILE *seek = NULL;
	FILE *key = NULL;

	seek = fopen(seekFile.c_str(), "rb");
	if (seek == NULL)
	{
		exit(EXIT_ERROR);
	}

	fseek(seek, 0, SEEK_END);
	seekSizeInBytes = ftell(seek);
	long seekCount = seekSizeInBytes / sizeof(int);
	int S[seekCount];
	// Move back to the beginning
	fseek(seek, 0, SEEK_SET);

	for (long i = 0; i < seekCount; i++)
	{
		fseek(seek, i * sizeof(int), SEEK_SET);
		fread(&S[i], sizeof(int), 1, seek);
	}

	fclose(seek);

	// Open key db for reading
	key = fopen(keyFile.c_str(), "rb");
	if (key == NULL)
	{
		exit(EXIT_ERROR);
	}
	fseek(key, 0, SEEK_END);
	keySizeInBytes = ftell(key);
	// We need to know how many integers are in key db
	long keyCount = keySizeInBytes / sizeof(int);
	fseek(key, 0, SEEK_SET);
	int hit[seekCount];
	// Start recording time
	gettimeofday(startTime, NULL);

	// Perform binary, on-disk search 
	for (long i = 0; i < seekCount; i++)
	{
		// Clear any EOF errors and start at beginning of file
		fseek(key, 0, SEEK_SET);
		clearerr(key);
		hit[i] = 0;
		
		if (BinarySearchOnDisk(S[i], key, 0, keyCount - 1))
		{
			hit[i] = 1;
		}
	}
	// Search procedure complete. Mark completion time and close key db
	gettimeofday(endTime, NULL);

	fclose(key);

	// Print results
	PrintResults(startTime, endTime, S, hit, seekCount);
}

bool
BinarySearchOnDisk(int target, FILE *key, long lower, long upper)
{
	if (lower > upper)
	{
		return false;
	}
	long n = upper - lower;
	long mid = lower + (n / 2);

	int current;
	fseek(key, mid * sizeof(int), SEEK_SET);
	// Make sure we didn't read past EOF
	if (!fread(&current, sizeof(int), 1, key))
	{
		return false;
	}
	// Make sure we exit if we reach the beginning of file and target isn't found
	if (upper == lower && current != target)
	{
		return false;
	}

	if (current == target)
	{
		return true;
	}
	else if (target < current)
	{
		return BinarySearchOnDisk(target, key, lower, mid - 1);
	} 
	else
	{
		return BinarySearchOnDisk(target, key, mid + 1, upper);
	}
}

bool
BinarySearch(int target, int K[], long lower, long upper)
{	
	
	if (lower > upper)
	{
		return false;
	}
	
	long n = upper - lower;
	long mid = lower + (n / 2);
	// Is this the last time we'll recurse?
	if (lower == upper && target != K[mid])
	{
		return false;
	}
	if (target == K[mid])
	{
		return true;
	}
	else if (target < K[mid])
	{
		return BinarySearch(target, K, lower, mid - 1);
	} 
	else
	{
		return BinarySearch(target, K, mid + 1, upper);
	}
}

void 
PrintResults(timeval *startTime, timeval *endTime, int S[], int hit[], long seekCount)
{
	for (int i = 0; i < seekCount; i++)
	{
		std::string hitString = "No";
		if (hit[i] == 1)
		{
			hitString = "Yes";
		}
		printf("%12d: %s\n", S[i], hitString.c_str());
	}

	int secElapsed = endTime->tv_sec - startTime->tv_sec;
	int usecElapsed = endTime->tv_usec - startTime->tv_usec;

	printf("Time: %1d.%06d\n", secElapsed, usecElapsed);
}
