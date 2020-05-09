#include "PngLoader.h"
#include <stdlib.h>
#include <memory.h>
#include <png.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

using namespace std;


// constructor
ImageData::ImageData(size_t inDataSize, char* inData, int inWidth, int inHeight, bool inWithAlpha){
    width = inWidth;
    height = inHeight;
    withAlpha = inWithAlpha;
    if(inDataSize > 0){
        data = inData;
        loaded = true;
    }else{
        data = NULL;
        loaded = false;
    }
}

// destructor
ImageData::~ImageData(){
    if(data != NULL){
        delete data;
    }
}


ImageData loadPngImage(const char* fileName){
//    // текущая папка
//    char* curPath = NULL;
//    size_t size = 0;
//    curPath = getcwd(curPath, size);
//    // полный путь
//    string fullPath = string(curPath) + "/" + inputFile;
//    const char* fileName = fullPath.c_str();

    // проверяем сигнатуру файла (первые 4 байт)
    size_t headerSize = 8;
    png_byte header[headerSize];
    memset(header, 0, headerSize);
    FILE* fp = fopen(fileName, "rb");
    if(fp == NULL){
        printf("File %s nor found\n", fileName);
        return ImageData(0, NULL, 0, 0, false);
    }

    fread(header, 1, headerSize, fp);
    if (png_check_sig(header, headerSize) == false) {
        fclose(fp);
        printf("Is not png: %s\n", fileName);
        return ImageData(0, NULL, 0, 0, false);
    }

    // создаем внутреннюю структуру png для работы с файлом
    // последние параметры - структура, для функции обработки ошибок и варнинга (последн. 2 параметра)
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    // создаем структуру с информацией о файле
    png_infop info_ptr = png_create_info_struct(png_ptr);

    //> Save calling environment for long jump
    setjmp(png_jmpbuf(png_ptr));

    //> Initialize input/output for the PNG file
    png_init_io(png_ptr, fp);

    // говорим библиотеке, что мы уже прочли 4 байт, когда проверяли сигнатуру
    png_set_sig_bytes(png_ptr, headerSize);
    // читаем всю информацию о файле
    png_read_info(png_ptr, info_ptr);
    // Эта функция возвращает инфу из info_ptr
    // размер картинки в пикселях
    png_uint_32 width = 0;
    png_uint_32 height = 0;
    // глубина цвета (одного из каналов, может быть 1, 2, 4, 8, 16)
    int bit_depth = 0;
    // описывает какие каналы присутствуют:
    // PNG_COLOR_TYPE_GRAY, PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE,
    // PNG_COLOR_TYPE_RGB, PNG_COLOR_TYPE_RGB_ALPHA...
    int color_type = 0;
    // последние 3 параметра могут быть нулями и обозначают: тип фильтра, тип компрессии и тип смещения
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);

    // png формат может содержать 16 бит на канал, но нам нужно только 8, поэтому сужаем канал
    if (bit_depth == 16) png_set_strip_16(png_ptr);
    // преобразуем файл если он содержит палитру в нормальный RGB
    if (color_type == PNG_COLOR_TYPE_PALETTE && bit_depth <= 8) png_set_palette_to_rgb(png_ptr);
    // если в грэйскейле меньше бит на канал чем 8, то конвертим к нормальному 8-битному
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png_ptr);
    // и добавляем полный альфа-канал
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
    // В нашей игре допустимы картинки, содержащие информацию только об оттенках серого цвета (grayscale картинки).
    // Если же, необходимо преобразование к RGB формату,
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png_ptr);

    double gamma = 0.0f;
    // если есть информация о гамме в файле, то устанавливаем на 2.2
    if ( png_get_gAMA(png_ptr, info_ptr, &gamma) ) png_set_gamma(png_ptr, 2.2, gamma);
    // иначе ставим дефолтную гамму для файла в 0.45455 (good guess for GIF images on PCs)
    else png_set_gamma(png_ptr, 2.2, 0.45455);

    // после всех трансформаций, апдейтим информацию в библиотеке
    png_read_update_info(png_ptr, info_ptr);
    // опять получаем все размеры и параметры обновленной картинки
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);

    // определяем кол-во байт нужных для того чтобы вместить строку
    png_uint_32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    // теперь, мы можем выделить память чтобы вместить картинку
    size_t dataSize = row_bytes * height;
    png_byte* data = new png_byte[dataSize];
    memset(data, 0, dataSize);
    // выделяем память, для указателей на каждую строку
    png_byte **row_pointers = new png_byte * [height];
    // сопоставляем массив указателей на строчки, с выделенными в памяти (res)
    // т.к. изображение перевернутое, то указатели идут снизу вверх
    for (unsigned int i = 0; i < height; i++){
        row_pointers[height - i - 1] = data + i * row_bytes;
    }

    // все, читаем картинку (данные)
    png_read_image(png_ptr, row_pointers);

    // освобождаем память от указателей на строки
    delete [] row_pointers;

    // освобождаем память выделенную для библиотеки libpng
    png_destroy_read_struct(&png_ptr, 0, 0);

    // закрываем файл
    fclose(fp);

    bool withAlpha = color_type & PNG_COLOR_MASK_ALPHA;

    // TODO: формат
    return ImageData(dataSize, (char*)data, width, height, withAlpha);
}
