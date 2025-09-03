#include <memory.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/unistd.h>
#include <unistd.h>

int main() {

  size_t d = 11;

	// request memory from kernel
	// with READ + WRITE
  void *mapped_memory =
      mmap(NULL, d, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

  if (mapped_memory == MAP_FAILED) {
    printf("map failure\n");
    return 1;
  }

  // sample instruction that just return `33`
	//
  unsigned char code[11] = {
      0x48, 0xB8, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov rax, 33
      0xC3                                                        // ret
  };


	memcpy(mapped_memory,code,11);

  // change permission to READ + EXECUTE
  //
  if (mprotect(mapped_memory, d, PROT_READ | PROT_EXEC) == -1) {
    printf("failed to change mod\n");
    return 1;
  }

	int res = ((int(*)()) mapped_memory)();  // call the array

	printf("res = %d" , res);

	return 0;
}
