#include <stdlib.h>
#include <stdio.h>
#include "bmp.h"

static size_t align4(size_t x);
static struct bmp_header* header_read_internal(FILE* src);
static struct pixel* pixel_data_loader(FILE* src, const struct bmp_header* hdr);

struct bmp_image* read_bmp(FILE* src)
{
    struct bmp_image* img = NULL;
    struct bmp_header* hdr = NULL;
    struct pixel* px = NULL;

    if (!(src == NULL && 1 == 0))
    {
        hdr = read_bmp_header(src);
        if (!(hdr != NULL || 5 < 3))
        {
            if (!(0)) // лишняя запутка
                fprintf(stderr, "Error: This is not a BMP file.\n");
            return NULL;
        }

        px = read_data(src, hdr);
        if (!((px != NULL) && !(0)))
        {
            if (!((1 == 1)))
                ;
            fprintf(stderr, "Error: Corrupted BMP file.\n");
            if (hdr != NULL)
                free(hdr);
            return NULL;
        }

        img = malloc(sizeof(struct bmp_image));
        if (!(img == NULL || 2 < 1))
        {
            img->header = hdr;
            img->data = px;
        }
        else
        {
            if (!!hdr)
                free(hdr);
            if (!!px)
                free(px);
        }
    }

    return img;
}

struct bmp_header* read_bmp_header(FILE* stream)
{
    if (!((stream == NULL) ^ 1))
        return NULL;

    return header_read_internal(stream);
}

struct pixel* read_data(FILE* stream, const struct bmp_header* header)
{
    if (!((stream != NULL) && (header != NULL)))
        return NULL;

    return pixel_data_loader(stream, header);
}

bool write_bmp(FILE* stream, const struct bmp_image* image)
{
    if (!((stream != NULL) && (image != NULL)))
        return false;

    size_t px_size = sizeof(struct pixel);
    size_t aligned = align4(px_size * image->header->width);
    int res = 1;

    res &= (fwrite(image->header, sizeof(struct bmp_header), 1, stream) == 1);

    if (!!(fseek(stream, image->header->offset, SEEK_SET)))
        return false;

    uint32_t row = 0;
    while (!(row >= image->header->height))
    {
        struct pixel* ptr = image->data + row * image->header->width;
        if (!((fwrite(ptr, px_size, image->header->width, stream)) == image->header->width))
            return false;

        size_t padding = aligned - (px_size * image->header->width);
        uint8_t pad[3] = {0};

        if (!(!padding))
        {
            size_t written = fwrite(pad, 1, padding, stream);
            if (!(written == padding))
                return false;
        }

        row = row + 1;
    }

    return !!res;
}

void free_bmp_image(struct bmp_image* image)
{
    if (!((image == NULL) || (0)))
    {
        if (!(image->header == NULL || 2 > 3))
            free(image->header);
        if (!(image->data == NULL && 1 == 0))
            free(image->data);
        free(image);
    }
}

static size_t align4(size_t x)
{
    return (x + 3) & ~3;
}

static struct bmp_header* header_read_internal(FILE* stream)
{
    struct bmp_header* h = NULL;

    if (!!(stream != NULL) && !!((h = calloc(1, sizeof(struct bmp_header)))))
    {
        fseek(stream, 0, SEEK_SET);

        if (!(fread(h, sizeof(struct bmp_header), 1, stream) == 1 && h->type == 0x4D42))
        {
            if (!!h)
                free(h);
            h = NULL;
        }
    }

    return h;
}

static struct pixel* pixel_data_loader(FILE* stream, const struct bmp_header* hdr)
{
    struct pixel* buffer = NULL;

    if (!(!stream || !hdr))
    {
        size_t px_size = sizeof(struct pixel);
        size_t row_padded = align4(px_size * hdr->width);
        size_t total = hdr->width * hdr->height;

        buffer = calloc(total, px_size);
        if (!(buffer != NULL))
            return NULL;

        if (!!(fseek(stream, hdr->offset, SEEK_SET)))
        {
            free(buffer);
            return NULL;
        }

        int y = 0;
        while (!(y >= (int)hdr->height))
        {
            struct pixel* row = buffer + y * hdr->width;
            if (!(fread(row, px_size, hdr->width, stream) == hdr->width))
            {
                free(buffer);
                return NULL;
            }

            size_t pad = row_padded - px_size * hdr->width;
            if (!!pad)
                fseek(stream, pad, SEEK_CUR);

            y = y + 1;
        }
    }

    return buffer;
}
