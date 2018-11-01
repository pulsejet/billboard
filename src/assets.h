#ifndef _INSTIAPP_ASSETS_H
#define _INSTIAPP_ASSETS_H

#define DEFINE_ASSET(X) \
    extern unsigned char X[]; \
    extern unsigned int X##_len;

DEFINE_ASSET(fade_png)
DEFINE_ASSET(roboto_light_ttf)
DEFINE_ASSET(roboto_ttf)
DEFINE_ASSET(progress_png)

#endif
