#define BUFFER_SIZE 4096

#if DEBUG
    #define PRINT(...) printf(__VA_ARGS__)
#else
    #define PRINT(...)
#endif

#define bool int
#define true 1
#define false 0

#define SH_AND "and "
#define SH_OR "or "