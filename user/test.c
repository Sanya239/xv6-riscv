//
// Created by Sasha on 21.04.2025.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

// Function to print hex dump of data
void hexdump(void *data, int size) {
  for(int i = 0; i < size; i++) {
    printf("%x ", ((uint8*)data)[i]);
  }
  printf("\n");
}

int main() {
  int fd0, fd1, fd2, fd3;
  char buf[16];
  uint test_data = 0x12345678;
  
  printf("Testing mydevice interfaces...\n");
  
  // Test null
  fd0 = open("null", O_RDWR);
  if(fd0 < 0) {
    printf("Failed to open null\n");
    exit(1);
  }
  printf("Writing 4 bytes to null...\n");
  write(fd0, &test_data, sizeof(uint));
  printf("Reading from null...\n");
  memset(buf, 0, sizeof(buf));
  int n = read(fd0, buf, sizeof(uint));
  printf("Read %d bytes from null:\n", n);
  printf("read returned %d\n", n);
  if (n > 0) {
    hexdump(buf, n);
  }
  close(fd0);
  
  // Test zero
  fd1 = open("zero", O_RDWR);
  if(fd1 < 0) {
    printf("Failed to open zero\n");
    exit(1);
  }
  printf("Writing 4 bytes to zero...\n");
  n = write(fd1, &test_data, sizeof(uint));
  printf("Write returned %d\n", n);
  printf("Reading from zero...\n");
  memset(buf, 0, sizeof(buf));
  n = read(fd1, buf, sizeof(uint));
  printf("Read %d bytes from zero:\n", n);
  if (n > 0) {
    hexdump(buf, n);
  }
  close(fd1);
  
  // Test urandom
  fd2 = open("urandom", O_RDWR);
  if(fd2 < 0) {
    printf("Failed to open urandom\n");
    exit(1);
  }
  printf("Writing seed to urandom...\n");
  n = write(fd2, &test_data, sizeof(uint64));
  printf("Write returned %d\n", n);
  printf("Reading random numbers from urandom...\n");
  
  for(int i = 0; i < 3; i++) {
    memset(buf, 0, sizeof(buf));
    n = read(fd2, buf, sizeof(uint64));
    printf("Read %d bytes, random value:\n", n);
    if (n > 0) {
      hexdump(buf, n);
    }
  }
  close(fd2);
  
  // Test nullstat (null stats)
  fd3 = open("nullstat", O_RDWR);
  if(fd3 < 0) {
    printf("Failed to open nullstat\n");
    exit(1);
  }
  printf("Writing 4 bytes to nullstat...\n");
  n = write(fd3, &test_data, sizeof(uint));
  printf("Write returned %d\n", n);
  
  printf("Reading stats from nullstat...\n");
  memset(buf, 0, sizeof(buf));
  n = read(fd3, buf, sizeof(uint64));
  printf("Read %d bytes, count value:\n", n);
  if (n > 0) {
    hexdump(buf, n);
  }
  
  // Write more bytes and read again
  printf("Writing 8 more bytes to nullstat...\n");
  write(fd3, &test_data, sizeof(uint));
  write(fd3, &test_data, sizeof(uint));
  memset(buf, 0, sizeof(buf));
  n = read(fd3, buf, sizeof(uint64));
  printf("After more writes, count value:\n");
  if (n > 0) {
    hexdump(buf, n);
  }

  close(fd3);

  printf("Test completed successfully!\n");
  exit(0);
}
