//
// Created by John on 15/04/2018.
//

#include "sequences.h"


void sections_init(Sections *sections)
{
    sections->size = 0;
    sections->capacity = SECTIONS_INITIAL_CAPACITY;

    sections->sect = malloc(sizeof(Sect) * sections->capacity);
}

void sections_add_sect(Sections *sections, Sect *sData)
{
    sections_increase_capacity(sections);

    sections->sect[sections->size++] = sData;
}

struct Sect sections_get_sect(Sections *sections, int index)
{
    if (index >= sections->size || index < 0)
    {
        printf("Index %d out of bounds for vector of size %d\n", index, sections->size);
        exit(1);
    }
    return sections->sect[index];
}

void sections_increase_capacity(Sections *sections)
{
    if (sections->size >= sections->capacity)
    {
        sections->capacity += 1;
        sections->sect = realloc(sections->sect, sizeof(Sect) * sections->capacity);
    }
}

void sections_free(Sections *sections)
{
    free(sections->sect);
}

int get_no_sections(Sections *sections)
{
    return sections->size;
}

