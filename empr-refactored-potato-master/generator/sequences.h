//
// Created by John on 15/04/2018.
//
#include "JTsequencer.h"

typedef struct {
    char pattern;
    char repeats;
} MainSequenceProperties;

typedef struct {
    char sectionStart;
    char sectionEnd;
    char repeats;
    char pattern;
} Sect;

typedef struct {
    int size;
    int capacity;
    struct Sect* sect;
} Sections;

typedef struct {
    struct MainSequenceProperties prop;
    char mainSequence[SEQUENCE_LENGTH];
    struct Sections sections;
}* Sequence;

void sections_init(Sections *sections);
void sections_add_sect(Sections *sections, Sect *sData);
struct Sect sections_get_sect(Sections *sections, int index);
void sections_increase_capacity(Sections *sections);
void sections_free(Sections *sections);
int get_no_sections(Sections *sections);

#define SECTIONS_INITIAL_CAPACITY 1

