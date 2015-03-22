/*
 * coWPAtty - Brute-force dictionary attack against WPA-PSK.
 *
 * Copyright (c) 2004, Joshua Wright <jwright@hasborg.com>
 *
 * $Id: utils.c,v 1.1.1.1 2004/11/02 11:43:30 jwright Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. See COPYING for more
 * details.
 *
 * coWPAtty is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * Significant code is graciously taken from the following:
 * wpa_supplicant by Jouni Malinen.  This tool would have been MUCH more
 * difficult for me if not for this code.  Thanks Jouni.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <crypt.h>
#include <unistd.h>
#include <ctype.h>
#include <netinet/in.h> /* for ntohs() */
#include "utils.h"


void lamont_hdump(unsigned char *bp, unsigned int length);
char *printmac(unsigned char *mac);
int IsBlank(char *s);

/* A better version of hdump, from Lamont Granquist.  Modified slightly
   by Fyodor (fyodor@DHP.com) */
void lamont_hdump(unsigned char *bp, unsigned int length) {

  /* stolen from tcpdump, then kludged extensively */

  static const char asciify[] = "................................ !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~.................................................................................................................................";

  const unsigned short *sp;
  const unsigned char *ap;
  unsigned int i, j;
  int nshorts, nshorts2;
  int padding;

  printf("\n\t");
  padding = 0;
  sp = (unsigned short *)bp;
  ap = (unsigned char *)bp;
  nshorts = (unsigned int) length / sizeof(unsigned short);
  nshorts2 = (unsigned int) length / sizeof(unsigned short);
  i = 0;
  j = 0;
  while(1) {
    while (--nshorts >= 0) {
      printf(" %04x", ntohs(*sp));
      sp++;
      if ((++i % 8) == 0)
        break;
    }
    if (nshorts < 0) {
      if ((length & 1) && (((i-1) % 8) != 0)) {
        printf(" %02x  ", *(unsigned char *)sp);
        padding++;
      }
      nshorts = (8 - (nshorts2 - nshorts));
      while(--nshorts >= 0) {
        printf("     ");
      }
      if (!padding) printf("     ");
    }
    printf("  ");

    while (--nshorts2 >= 0) {
      printf("%c%c", asciify[*ap], asciify[*(ap+1)]);
      ap += 2;
      if ((++j % 8) == 0) {
        printf("\n\t");
        break;
      }
    }
    if (nshorts2 < 0) {
      if ((length & 1) && (((j-1) % 8) != 0)) {
        printf("%c", asciify[*ap]);
      }
      break;
    }
  }
  if ((length & 1) && (((i-1) % 8) == 0)) {
    printf(" %02x", *(unsigned char *)sp);
    printf("                                       %c", asciify[*ap]);
  }
  printf("\n");
}

int IsBlank(char *s) {

    int len, i;
    if (s == NULL) { 
        return(1); 
    }

    len = strlen(s);

    if (len == 0) { 
        return(1); 
    }

    for (i=0; i < len; i++) {
       if (s[i] != ' ') {
           return(0);
       }
    }
    return(0);
}

char *printmac(unsigned char *mac) {
    static char macstring[18];

    memset(&macstring, 0, sizeof(macstring));
    (void)snprintf(macstring, sizeof(macstring), 
        "%02x:%02x:%02x:%02x:%02x:%02x",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return(macstring);
}
