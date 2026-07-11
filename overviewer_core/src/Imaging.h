#ifndef IMAGING_H
#define IMAGING_H

#include "ImPlatform.h"
#include <stddef.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define IMAGING_MODE_LENGTH (6 + 1)
#define IMAGING_MAGIC "PIL Imaging"

#define IMAGING_TYPE_UINT8 0
#define IMAGING_TYPE_INT32 1
#define IMAGING_TYPE_FLOAT32 2
#define IMAGING_TYPE_SPECIAL 3

typedef struct ImagingMemoryInstance *Imaging;
typedef struct ImagingPaletteInstance *ImagingPalette;
typedef struct ImagingHistogramInstance *ImagingHistogram;
typedef struct ImagingAccessInstance *ImagingAccess;
typedef struct ImagingOutlineInstance *ImagingOutline;

typedef struct {
    char *ptr;
    int size;
} ImagingMemoryBlock;

typedef struct ImagingMemoryArena {
    int alignment;
    int block_size;
    int blocks_max;
    int blocks_cached;
    ImagingMemoryBlock *blocks_pool;
    int stats_new_count;
    int stats_allocated_blocks;
    int stats_reused_blocks;
    int stats_reallocated_blocks;
    int stats_freed_blocks;
} *ImagingMemoryArena;

extern struct ImagingMemoryArena ImagingDefaultArena;

struct ImagingMemoryInstance {
    char mode[IMAGING_MODE_LENGTH];
    int type;
    int depth;
    int bands;
    int xsize;
    int ysize;
    ImagingPalette palette;
    UINT8 **image8;
    INT32 **image32;
    char **image;
    char *block;
    ImagingMemoryBlock *blocks;
    int pixelsize;
    int linesize;
    void (*destroy)(Imaging im);
};

struct ImagingPaletteInstance {
    char mode[IMAGING_MODE_LENGTH];
    int size;
    UINT8 palette[1024];
    INT16 *cache;
    int keep_cache;
};

struct ImagingAccessInstance {
    const char *mode;
    void (*get_pixel)(Imaging im, int x, int y, void *pixel);
    void (*put_pixel)(Imaging im, int x, int y, const void *pixel);
};

struct ImagingHistogramInstance {
    char mode[IMAGING_MODE_LENGTH];
    int bands;
    long *histogram;
};

typedef void *ImagingSectionCookie;
extern void ImagingSectionEnter(ImagingSectionCookie *cookie);
extern void ImagingSectionLeave(ImagingSectionCookie *cookie);

typedef void (*ImagingShuffler)(UINT8 *out, const UINT8 *in, int pixels);

typedef int (*ImagingCodec)(Imaging im, void *state, UINT8 *buffer, int bytes);

struct ImagingCodecStateInstance {
    int count;
    int state;
    int errcode;
    int x, y;
    int ystep;
    int xsize, ysize, xoff, yoff;
    ImagingShuffler shuffle;
    int bits, bytes;
    UINT8 *buffer;
    void *context;
    PyObject *fd;
};

typedef struct ImagingCodecStateInstance *ImagingCodecState;

#endif
