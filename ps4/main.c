#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"
#include "transformations.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Использование: %s <входной_файл.bmp> <выходной_файл.bmp> <операция> [параметры]\n", argv[0]);
        printf("Операции:\n");
        printf("  flip_h\n  flip_v\n  rotate_r\n  rotate_l\n");
        printf("  scale <factor>\n  crop <x> <y> <width> <height>\n  extract <channels>\n");
        return 1;
    }

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];
    const char *operation = argv[3];

    FILE *input_file = fopen(input_filename, "rb");
    if (!input_file) {
        perror("Ошибка при открытии входного файла");
        return 1;
    }

    struct bmp_image *image = read_bmp(input_file);
    fclose(input_file);

    if (!image) {
        fprintf(stderr, "Ошибка при чтении BMP-изображения.\n");
        return 1;
    }

    struct bmp_image *result = NULL;

    if (strcmp(operation, "flip_h") == 0) {
        result = flip_horizontally(image);
    } else if (strcmp(operation, "flip_v") == 0) {
        result = flip_vertically(image);
    } else if (strcmp(operation, "rotate_r") == 0) {
        result = rotate_right(image);
    } else if (strcmp(operation, "rotate_l") == 0) {
        result = rotate_left(image);
    } else if (strcmp(operation, "scale") == 0) {
        if (argc < 5) {
            fprintf(stderr, "Не указан масштаб (например: scale 0.5)\n");
            free_bmp_image(image);
            return 1;
        }
        float factor = atof(argv[4]);
        result = scale(image, factor);
    } else if (strcmp(operation, "crop") == 0) {
        if (argc < 8) {
            fprintf(stderr, "Недостаточно аргументов для crop (нужно: x y w h)\n");
            free_bmp_image(image);
            return 1;
        }
        int x = atoi(argv[4]);
        int y = atoi(argv[5]);
        int w = atoi(argv[6]);
        int h = atoi(argv[7]);
        result = crop(image, x, y, w, h);
    } else if (strcmp(operation, "extract") == 0) {
        if (argc < 5) {
            fprintf(stderr, "Не указаны каналы (например: extract rg)\n");
            free_bmp_image(image);
            return 1;
        }
        result = extract(image, argv[4]);
    } else {
        fprintf(stderr, "Неизвестная операция: %s\n", operation);
        free_bmp_image(image);
        return 1;
    }

    if (!result) {
        fprintf(stderr, "Ошибка при выполнении операции.\n");
        free_bmp_image(image);
        return 1;
    }

    FILE *output_file = fopen(output_filename, "wb");
    if (!output_file) {
        perror("Ошибка при открытии выходного файла");
        free_bmp_image(image);
        free_bmp_image(result);
        return 1;
    }

    if (!write_bmp(output_file, result)) {
        fprintf(stderr, "Ошибка при записи BMP-изображения.\n");
        fclose(output_file);
        free_bmp_image(image);
        free_bmp_image(result);
        return 1;
    }

    fclose(output_file);
    free_bmp_image(image);
    free_bmp_image(result);

    printf("Изображение успешно сохранено в %s\n", output_filename);
    return 0;
}
