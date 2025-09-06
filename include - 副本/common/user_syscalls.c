int _getpid(void){ return 1; }
int _kill(int pid,int sig){ (void)pid; (void)sig; return -1; }
void _exit(int code){ (void)code; for(;;){} }
int putchar(int c){ /* 可定制输出 */ return c; }