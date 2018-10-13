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
	void *readBitmapHeader(std::fstream &fh, bitmap_header &hp) {
	    //variable dec:
	    //Citire header bitmap
  		fh.read((char*)&hp, sizeof(bitmap_header));
	}

	/**
	 *
	 * @param fp
	 * @param hp
	 * @return
	 */
	void readBitmapData(std::fstream &fh, bitmap_header &hp, bmp_data &bmp) {
	    bmp.originalLineSize = hp.width * 3;
	    bmp.workLineSize = bmp.originalLineSize + 4 - bmp.originalLineSize % 4;
	    bmp.padding = bmp.workLineSize - bmp.originalLineSize;

	    if (bmp.padding == 4) {
	        bmp.workLineSize = bmp.originalLineSize; //in cazul in care nu trebuie sa mai adaugam padding
	        bmp.padding = 0;
	    }
	    std::cout<<"Padding:"<<bmp.padding<<std::endl;


	    bmp.size = (size_t) (hp.height * bmp.workLineSize);

	    //Alocare buffer pentru citire si vectorul cu biti unde va fi plasata imaginea completa
	    bmp.data = new char[bmp.size];

	    fh.seekg(sizeof(char) * hp.fileheader.dataoffset, ios_base::beg);

	    //fseek(fp, sizeof(char) * hp.fileheader.dataoffset, SEEK_SET);
	    std::cout<<"Offset: " << hp.fileheader.dataoffset << std::endl;

	    std::cout<<"Size: " << bmp.size << std::endl;

	    //size_t n = fread(bmp.data, sizeof(char), bmp.size, fp);
	    fh.read(bmp.data, bmp.size);

	    for (int i = 0; i < bmp.size; ++i) {
	    	// std::cout << (int) bmp.data[i];
	    }
	}

	void test() {
		std::cout << "sal" << std::endl;
		std::fstream fbitmap;
		fbitmap.open("fonts/font.bmp" , std::fstream::in | std::fstream::binary);

		bitmap_header header;
		bmp_data bmp;

		readBitmapHeader(fbitmap, header);
		std::cout << "W: " << header.width << "H: " << header.height << std::endl;

		readBitmapData(fbitmap, header, bmp);
	}
}


#endif

