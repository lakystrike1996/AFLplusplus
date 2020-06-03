#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

// libFuzzer interface is thin, so we don't include any libFuzzer headers.
int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size);
__attribute__((weak)) int LLVMFuzzerInitialize(int *argc, char ***argv);

static const size_t kMaxAflInputSize = 1 << 20;
static uint8_t AflInputBuf[kMaxAflInputSize];

void __attribute__ ((noinline)) afl_qemu_driver_stdin_input(void) {

  size_t l = read(0, AflInputBuf, kMaxAflInputSize);
  LLVMFuzzerTestOneInput(AflInputBuf, l);

}

int main(int argc, char **argv) {

  if (LLVMFuzzerInitialize) LLVMFuzzerInitialize(&argc, &argv);
  // Do any other expensive one-time initialization here.

  if (getenv("AFL_QEMU_DRIVER_NO_HOOK")) {

    afl_qemu_driver_stdin_input();

  } else {

    uint8_t dummy_input[1] = {0};
    LLVMFuzzerTestOneInput(dummy_input, 1);

  }
  
  return 0;

}
