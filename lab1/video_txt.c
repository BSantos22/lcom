#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

void vt_fill(char ch, char attr) {
  
	char *vptr;
	vptr = video_mem;

	int i;

	for (i = 0; i < scr_width * scr_lines; i++) {
		*vptr = ch;
		vptr++;
		*vptr = attr;
		vptr++;
	}
  
}

void vt_blank() {

	char *vptr;
	vptr = video_mem;

	int i;

	for (i = 0; i < scr_width * scr_lines; i++) {
		*vptr = ' ';
		vptr++;
		*vptr = 0x007;
		vptr++;
	}

}

int vt_print_char(char ch, char attr, int r, int c) {

	char *vptr;
	vptr = video_mem;

	vptr += 2*(scr_width*r + c);

	*vptr = ch;
	vptr++;
	*vptr = attr;

	return 0;

}

int vt_print_string(char *str, char attr, int r, int c) {

	char *vptr;
	vptr = video_mem;

	vptr +=  2*(scr_width*r + c);

	while (*str != '\0') {
		*vptr = *str;
		vptr++;
		*vptr = attr;
		vptr++;
		str++;
	}
	return 0;

}

int vt_print_int(int num, char attr, int r, int c) {

	char *vptr;
	vptr = video_mem;

	vptr +=  2*(scr_width*r + c);

	*vptr = 48 + num;
	vptr++;
	*vptr = attr;

	return 0;

}


int vt_draw_frame(int width, int height, char attr, int r, int c) {

  char *vptr;
  char *tmp;

  vptr = video_mem;
  vptr += 2*(scr_width*r + c);

  tmp = vptr;

  int i;

  for (i = 0; i < width; i++) {
	  *vptr = ' ';
	  vptr++;
	  *vptr = attr;
	  vptr++;
  }

  for (i = 1; i < height - 1; i++) {

	  vptr = tmp + 2*(scr_width*i);

	  *vptr = ' ';
	  vptr++;
	  *vptr = attr;
	  vptr++;


	  if(width % 2 == 0){
		  vptr += 2*(width-1);
	  }
	  if(width % 2 != 0){
		  vptr += 2*(width -2);
	  }

	  *vptr = ' ';
	  vptr++;
	  *vptr = attr;
  }

  vptr += 2*(scr_width - width+1)-1;

  for (i = 0; i < width; i++) {
  	  *vptr = ' ';
  	  vptr++;
  	  *vptr = attr;
  	  vptr++;
    }

  return 0;

}

/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}
