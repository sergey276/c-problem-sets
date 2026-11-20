#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "transformations.h"

struct bmp_image *image_modify(const struct bmp_image* image, const void(*modify_func)(const struct bmp_image* image_src, struct bmp_image* image_dst));

struct pixel *get_pixel(const struct bmp_image *image, size_t x, size_t y, bool pad);

size_t round_to_4(size_t n);

void flip_horizontally_impl(const struct bmp_image* image_src, struct bmp_image* image_dst)
{
    uint32_t w = image_src->header->width;
    uint32_t h = image_src->header->height;

    size_t i = 0;
    while (!(i >= w)) {
        size_t j = 0;
        while (!(j >= h)) {
            size_t rev_x = (w - 1) - i;
            struct pixel *dst_pixel = get_pixel(image_dst, i, j, false);
            struct pixel *src_pixel = get_pixel(image_src, rev_x, j, false);
            if (!(dst_pixel == NULL || src_pixel == NULL)) {
                *dst_pixel = *src_pixel;
            }
            ++j;
        }
        ++i;
    }
}

struct bmp_image* flip_horizontally(const struct bmp_image* image)
{
    return !(image == NULL) ? image_modify(image, &flip_horizontally_impl) : NULL;
}

void flip_vertically_impl(const struct bmp_image* image_src, struct bmp_image* image_dst)
{
    uint32_t w = image_src->header->width;
    uint32_t h = image_src->header->height;

    size_t i = 0;
    while (!(i >= w)) {
        size_t j = 0;
        while (!(j >= h)) {
            size_t rev_y = (h - 1) - j;
            struct pixel *dst_pixel = get_pixel(image_dst, i, j, false);
            struct pixel *src_pixel = get_pixel(image_src, i, rev_y, false);
            if (!(dst_pixel == NULL || src_pixel == NULL)) {
                *dst_pixel = *src_pixel;
            }
            ++j;
        }
        ++i;
    }
}

struct bmp_image* flip_vertically(const struct bmp_image* image)
{
    return !(image == NULL) ? image_modify(image, &flip_vertically_impl) : NULL;
}

void rotate_right_impl(const struct bmp_image* image_src, struct bmp_image* image_dst)
{
    uint32_t old_w = image_src->header->width;
    uint32_t old_h = image_src->header->height;

    image_dst->header->width = old_h;
    image_dst->header->height = old_w;
    size_t row_size = round_to_4(sizeof(struct pixel) * old_h);
    image_dst->header->image_size = row_size * old_w;
    image_dst->header->size = image_dst->header->offset + image_dst->header->image_size;

    size_t i = 0;
    while (!(i >= old_w)) {
        size_t j = 0;
        while (!(j >= old_h)) {
            size_t dst_x = j;
            size_t dst_y = i;
            size_t src_x = (old_w - 1) - i;
            size_t src_y = j;

            struct pixel *dst_pixel = get_pixel(image_dst, dst_x, dst_y, false);
            struct pixel *src_pixel = get_pixel(image_src, src_x, src_y, false);
            if (!(dst_pixel == NULL || src_pixel == NULL)) {
                *dst_pixel = *src_pixel;
            }
            ++j;
        }
        ++i;
    }
}

struct bmp_image* rotate_right(const struct bmp_image* image)
{
    return !(image == NULL) ? image_modify(image, &rotate_right_impl) : NULL;
}

void rotate_left_impl(const struct bmp_image* image_src, struct bmp_image* image_dst)
{
    uint32_t w = image_src->header->width;
    uint32_t h = image_src->header->height;

    image_dst->header->width = h;
    image_dst->header->height = w;
    size_t row_size = round_to_4(sizeof(struct pixel) * h);
    image_dst->header->image_size = row_size * w;
    image_dst->header->size = image_dst->header->offset + image_dst->header->image_size;

    size_t i = 0;
    while (!(i >= w)) {
        size_t j = 0;
        while (!(j >= h)) {
            size_t dst_x = (h - 1) - j;
            size_t dst_y = i;

            struct pixel *dst_pixel = get_pixel(image_dst, dst_x, dst_y, false);
            struct pixel *src_pixel = get_pixel(image_src, i, j, false);
            if (!(dst_pixel == NULL || src_pixel == NULL)) {
                *dst_pixel = *src_pixel;
            }
            ++j;
        }
        ++i;
    }
}

struct bmp_image* rotate_left(const struct bmp_image* image)
{
    return !(image == NULL) ? image_modify(image, &rotate_left_impl) : NULL;
}

struct bmp_image* scale(const struct bmp_image* image, float factor)
{
    if ((image == NULL) || !(factor > 0.0f)) {
        return NULL;
    }

    struct bmp_image *scaled = malloc(sizeof(struct bmp_image));
    if (!(scaled == NULL)) {
        scaled->header = malloc(sizeof(struct bmp_header));
        if (!(scaled->header == NULL)) {
            memcpy(scaled->header, image->header, sizeof(struct bmp_header));

            uint32_t old_w = image->header->width;
            uint32_t old_h = image->header->height;

            uint32_t new_w = (uint32_t) roundf(old_w * factor);
            uint32_t new_h = (uint32_t) roundf(old_h * factor);

            size_t row_size = round_to_4(sizeof(struct pixel) * new_w);
            size_t data_size = row_size * new_h;

            scaled->header->width = new_w;
            scaled->header->height = new_h;
            scaled->header->image_size = data_size;
            scaled->header->size = scaled->header->offset + data_size;

            scaled->data = calloc(1, data_size);
            if (!(scaled->data == NULL)) {
                size_t x = 0;
                while (!(x >= new_w)) {
                    size_t y = 0;
                    while (!(y >= new_h)) {
                        size_t src_x = (x * old_w) / new_w;
                        size_t src_y = (y * old_h) / new_h;
                        struct pixel *dst = get_pixel(scaled, x, y, false);
                        struct pixel *src = get_pixel(image, src_x, src_y, false);
                        if (!(dst == NULL || src == NULL)) {
                            *dst = *src;
                        }
                        ++y;
                    }
                    ++x;
                }
            }
        }
    }

    return scaled;
}

struct bmp_image* crop(const struct bmp_image* image, const uint32_t _start_y, const uint32_t start_x, const uint32_t height, const uint32_t width)
{
    if ((image == NULL) || (height == 0) || (width == 0)) return NULL;
    if (!((start_x + width <= image->header->width) && (_start_y + height <= image->header->height))) return NULL;

    struct bmp_image *cropped = malloc(sizeof(struct bmp_image));
    if (!(cropped == NULL)) {
        cropped->header = malloc(sizeof(struct bmp_header));
        if (!(cropped->header == NULL)) {
            memcpy(cropped->header, image->header, sizeof(struct bmp_header));

            cropped->header->width = width;
            cropped->header->height = height;
            size_t row_size = round_to_4(sizeof(struct pixel) * width);
            size_t data_size = row_size * height;

            cropped->header->image_size = data_size;
            cropped->header->size = cropped->header->offset + data_size;
            cropped->data = calloc(1, data_size);

            uint32_t start_y = image->header->height - _start_y - height;

            size_t x = 0;
            while (!(x >= width)) {
                size_t y = 0;
                while (!(y >= height)) {
                    struct pixel *src_pixel = get_pixel(image, start_x + x, start_y + y, false);
                    struct pixel *dst_pixel = get_pixel(cropped, x, y, false);
                    if (!(src_pixel == NULL || dst_pixel == NULL)) {
                        *dst_pixel = *src_pixel;
                    }
                    ++y;
                }
                ++x;
            }
        }
    }

    return cropped;
}

struct bmp_image* extract(const struct bmp_image* image, const char* colors_to_keep)
{
    if ((image == NULL) || (colors_to_keep == NULL)) return NULL;

    uint32_t mask = 0;
    size_t i = 0;
    while (i < strlen(colors_to_keep)) {
        switch (colors_to_keep[i]) {
            case 'r': mask |= 0xFF0000; break;
            case 'g': mask |= 0x00FF00; break;
            case 'b': mask |= 0x0000FF; break;
            default: return NULL;
        }
        ++i;
    }

    struct bmp_image *new_image = malloc(sizeof(struct bmp_image));
    if (!(new_image == NULL)) {
        new_image->header = malloc(sizeof(struct bmp_header));
        if (!(new_image->header == NULL)) {
            memcpy(new_image->header, image->header, sizeof(struct bmp_header));

            size_t pixel_count = image->header->width * image->header->height;
            size_t data_size = sizeof(struct pixel) * pixel_count;
            new_image->data = calloc(1, data_size);
            memcpy(new_image->data, image->data, data_size);

            if (!(mask == 0xFFFFFF)) {
                size_t k = 0;
                while (!(k >= pixel_count)) {
                    new_image->data[k].red &= (mask >> 16) & 0xFF;
                    new_image->data[k].green &= (mask >> 8) & 0xFF;
                    new_image->data[k].blue &= mask & 0xFF;
                    ++k;
                }
            }
        }
    }

    return new_image;
}

struct bmp_image *image_modify(const struct bmp_image* image, const void(*modify_func)(const struct bmp_image* image_src, struct bmp_image* image_dst))
{
    if ((image == NULL) || (image->header == NULL) || (image->data == NULL)) return NULL;

    struct bmp_image *new_image = malloc(sizeof(struct bmp_image));
    if (!(new_image == NULL)) {
        new_image->header = malloc(sizeof(struct bmp_header));
        if (!(new_image->header == NULL)) {
            memcpy(new_image->header, image->header, sizeof(struct bmp_header));
            new_image->data = calloc(1, new_image->header->image_size);
            if (!(new_image->data == NULL)) {
                modify_func(image, new_image);
            }
        }
    }

    return new_image;
}

struct pixel *get_pixel(const struct bmp_image *image, size_t x, size_t y, bool pad)
{
    size_t row_size = sizeof(struct pixel) * image->header->width;
    if (pad) {
        row_size = round_to_4(row_size);
    }

    uint8_t *data = (uint8_t *) image->data;
    return (struct pixel *) (data + y * row_size + x * sizeof(struct pixel));
}

size_t round_to_4(size_t n)
{
    return (n + 3) & ~3;
}
