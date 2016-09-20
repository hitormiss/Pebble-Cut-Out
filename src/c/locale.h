#pragma once

//https://github.com/redus/hex_clock/tree/master/src
//https://github.com/freakified/TimeStylePebble/blob/master/src/languages.h

typedef enum {
	EN, FR, DE, ES, IT, NL, TR, CZ, PT, GK, SE, PL, SK, VN, RO, CA, NO, RU, EE, EU, FI, DA
}	Locale;

// REQ: day_number counts from Sunday (0 == Sun, 1 == Mon, etc.)
// EFF: return weekday according to the locale language
const char* get_weekday(int day_number, const Locale locale);

// REQ: month_number counts from January (0 == JAN, 1 == FEB, etc.)
// EFF: return weekday according to the locale language
const char* get_month(int month_number, const Locale locale);

// EFF: char* to Locale converter
Locale set_locale(const char* locale);
