/**
 * \file	common/util.h
 * \author	Stefan Gast
 *
 * \brief	Deklarationen diverser Hilfsfunktionen für Ein-/Ausgabe, Fehlersuche usw.
 */

#ifndef QUIZ_UTIL_H
#define QUIZ_UTIL_H

#include <stdarg.h>
#include <stddef.h>

/* Wir benutzen die __attribute__ Erweiterung von GCC zur Überprüfung
 * der Argumente von debugPrint. Damit andere Compiler sich nicht beschweren,
 * definieren wir auf diesen __attribute__ als leeres Makro. */
#ifndef __GNUC__
#  define  __attribute__(x)  /* leer */
#endif

void setProgName(const char *argv0);
const char *getProgName(void);

void debugEnable(void);
int debugEnabled(void);
void debugDisable(void);

void debugPrint(const char *fmt, ...) __attribute__ ((format(printf, 1, 2)));
void infoPrint(const char *fmt, ...) __attribute__ ((format(printf, 1, 2)));
void errorPrint(const char *fmt, ...) __attribute__ ((format(printf, 1, 2)));
void errnoPrint(const char *prefix);

void debugHexdump(const void *ptr, size_t n, const char *fmt, ...) __attribute__ ((format(printf, 3, 4)));
void hexdump(const void *ptr, size_t n, const char *fmt, ...) __attribute__ ((format(printf, 3, 4)));
void vhexdump(const void *ptr, size_t n, const char *fmt, va_list args);

char *readLine(int fd);

/* Funktionen zur Berechnung Antwortzeit - aus dem Systemprogrammierung-Moodlekurs
 * https://www.elearning.hs-weingarten.de/mod/lesson/view.php?id=23738&pageid=6525
 */
struct timespec timespecAdd(const struct timespec*, const struct timespec*);
struct timespec timespecSub(const struct timespec*, const struct timespec*);
int cmpTimespec(const struct timespec*, const struct timespec*);

#endif
