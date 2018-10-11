#ifndef FONTS_H
#define FONTS_H

#include <fstream>
#include "bmp_data.h"
#include "bmp_header.h"

namespace fonts {
	/**
	 *
	 * @param fp
	 * @return
	 */
	bitmap_header *readBitmapHeader(FILE *fp) {
	    //variable dec:
	    size_t n;

	    //Citire header bitmap
	    bitmap_header *hp = (bitmap_header *) malloc(sizeof(bitmap_header));
	    check(hp);

	    n = fread(hp, sizeof(bitmap_header), 1, fp);
	    if (n < 1) {
	        free(hp);
	        exit(-1);
	    }

	    return hp;
	}

	/**
	 *
	 * @param fp
	 * @param hp
	 * @return
	 */
	bmp_data *readBitmapData(FILE *fp, bitmap_header *hp) {
	    bmp_data *bmp = (bmp_data *) malloc(sizeof(bmp_data));

	    bmp->originalLineSize = hp->width * 3;
	    bmp->workLineSize = bmp->originalLineSize + 4 - bmp->originalLineSize % 4;
	    bmp->padding = bmp->workLineSize - bmp->originalLineSize;

	    if (bmp->padding == 4) {
	        bmp->workLineSize = bmp->originalLineSize; //in cazul in care nu trebuie sa mai adaugam padding
	        bmp->padding = 0;
	    }
	    printf("Padding: %d\n", bmp->padding);


	    bmp->size = (size_t) (hp->height * bmp->workLineSize);

	    //Alocare buffer pentru citire si vectorul cu biti unde va fi plasata imaginea completa
	    bmp->data = (char *) malloc(sizeof(char) * (bmp->size));
	    check(bmp->data);

	    fseek(fp, sizeof(char) * hp->fileheader.dataoffset, SEEK_SET);
	    printf("Offset: %d\n", hp->fileheader.dataoffset);


	    size_t n = fread(bmp->data, sizeof(char), bmp->size, fp);
	    if (n < 1) {
	        exit(-1);
	    }

	    return bmp;
	}

	void test() {
		std::cout << "sal" << std::endl;
	}
}


#endif

