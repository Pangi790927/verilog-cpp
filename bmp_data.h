//
// Created by Ionescu Alex on 13/01/2018.
//

#ifndef GIGEL_BMP_DATA_H
#define GIGEL_BMP_DATA_H
typedef struct {
    char *data;
    char *filename;
    int originalLineSize;
    int workLineSize;
    int padding;
    size_t size;
} bmp_data;
#endif //GIGEL_BMP_DATA_H
