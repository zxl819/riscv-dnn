#ifndef __PARAMS_H__
#define __PARAMS_H__

#ifndef HIN
#define HIN 13
#endif

#ifndef WIN
#define WIN 13
#endif

#ifndef CIN
#define CIN 256
#endif

#ifndef KH
#define KH 3
#endif

#ifndef KW
#define KW 3
#endif

#ifndef STRIDE_H
#define STRIDE_H 2
#endif

#ifndef STRIDE_W
#define STRIDE_W 2
#endif

#ifndef PAD_TOP
#define PAD_TOP 0
#endif

#ifndef PAD_BOTTOM
#define PAD_BOTTOM 0
#endif

#ifndef PAD_LEFT
#define PAD_LEFT 0
#endif

#ifndef PAD_RIGHT
#define PAD_RIGHT 0
#endif


#define COUT CIN

#define HOUT ( (HIN + PAD_TOP + PAD_BOTTOM - KH) / STRIDE_H + 1)
#define WOUT ( (WIN + PAD_LEFT + PAD_RIGHT - KW) / STRIDE_W + 1)

#define OUT_SIZE (HOUT * WOUT * COUT)

#ifndef NLOOPS
#define NLOOPS 1
#endif

#endif
