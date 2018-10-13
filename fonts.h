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
	    bmp.originalLineSize = hp.width * 4;
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

	    /*for (int i = 0; i < hp.height; ++i) {
	    	for (int j = 0; j < bmp.workLineSize / 3; ++j) {
	    		for (int k = 0; k < 3; ++k) {
	    			std::cout << (int) bmp.data[i *  bmp.workLineSize + j * 3 + k];
	    		}
	    	}
	    }*/

	    for (int i = 0; i < 256; ++i) {
	    	for (int k = 13; k >= 0; k--) {
    			for (int j = 0; j < 8; ++j) {
    				// i = index ascii caracter
    				// k pixel relativ y
    				// j pixel relativ x
    				//(i * 8 + j) * 3
    				//std::cout << (int) bmp.data[(i * 8 + j) * 4 + k * bmp.workLineSize + 1];

	    			if ( bmp.data[(i * 8 + j) * 4 + k * bmp.workLineSize + 1] == 0) {
	    				std::cout<<"*";
	    			} else {
	    				std::cout<<" ";
	    			}
	    		}
	    		std::cout << std::endl;
    		}
    		std::cout << std::endl;
    		std::cout << std::endl;
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

