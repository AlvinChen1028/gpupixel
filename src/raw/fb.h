#ifndef __FACE_BEAUTY_H__
#define __FACE_BEAUTY_H__

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void fb_init(void);
void fb_process(unsigned char *frame, int width, int height, int fmt);
void fb_exit(void);

#ifdef __cplusplus
}
#endif

#endif
