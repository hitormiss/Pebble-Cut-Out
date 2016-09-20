#include <pebble.h>
#include "locale.h"

//https://github.com/redus/hex_clock/tree/master/src
//https://github.com/freakified/TimeStylePebble/blob/master/src/languages.h

static const char* MONTH[][12] = {
	{"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"},	// EN *
	//{"JAN", "FÉV", "MAR", "AVR", "MAI", "JUI", "JUL", "AOÛ", "SEP", "OCT", "NOV", "DÉC"},	// FR
	{"JAN", "FEV", "MAR", "AVR", "MAI", "JUI", "JUL", "AOU", "SEP", "OCT", "NOV", "DEC"},	// FR *
	//{"JAN", "FEB", "MÄR", "APR", "MAI", "JUN", "JUL", "AUG", "SEP", "OKT", "NOV", "DEZ"},	// DE
	{"JAN", "FEB", "MAR", "APR", "MAI", "JUN", "JUL", "AUG", "SEP", "OKT", "NOV", "DEZ"},	// DE *
	{"ENE", "FEB", "MAR", "ABR", "MAY", "JUN", "JUL", "AGO", "SEP", "OCT", "NOV", "DIC"},	// ES *
	{"GEN", "FEB", "MAR", "APR", "MAG", "GIU", "LUG", "AGO", "SET", "OTT", "NOV", "DIC"},	// IT *
	{"JAN", "FEB", "MRT", "APR", "MEI", "JUN", "JUL", "AUG", "SEP", "OKT", "NOV", "DEC"},	// NL *
	//{"OCA", "ŞUB", "MAR", "NİS", "MAY", "HAZ", "TEM", "AĞU", "EYL", "EKİ", "KAS", "ARA"},	// TR
	{"OCA", "SUB", "MAR", "NIS", "MAY", "HAZ", "TEM", "AGU", "EYL", "EKI", "KAS", "ARA"},	// TR *
	{"LED", "ÚNO", "BŘE", "DUB", "KVĚ", "ČRV", "ČVC", "SRP", "ZÁŘ", "ŘÍJ", "LIS", "PRO"},	// CZ
	{"JAN", "FEV", "MAR", "ABR", "MAI", "JUN", "JUL", "AGO", "SET", "OUT", "NOV", "DEZ"},	// PT *
	{"ΙΑΝ", "ΦΕΒ", "ΜΑΡ", "ΑΠΡ", "ΜΑΪ", "ΙΟΝ", "ΙΟΛ", "ΑΥΓ", "ΣΕΠ", "ΟΚΤ", "ΝΟΕ", "ΔΕΚ"},	// GK
	{"JAN", "FEB", "MAR", "APR", "MAJ", "JUN", "JUL", "AUG", "SEP", "OKT", "NOV", "DEC"},	// SE *
	{"STY", "LUT", "MAR", "KWI", "MAJ", "CZE", "LIP", "SIE", "WRZ", "PAŹ", "LIS", "GRU"},	// PL
	{"JAN", "FEB", "MAR", "APR", "MÁJ", "JÚN", "JÚL", "AUG", "SEP", "OKT", "NOV", "DEC"},	// SK
	{"Th1", "Th2", "Th3", "Th4", "Th5", "Th6", "Th7", "Th8", "Th9", "T10", "T11", "T12"},	// VN
	{"IAN", "FEB", "MAR", "APR", "MAI", "IUN", "IUL", "AUG", "SEP", "OCT", "NOI", "DEC"},	// RO *
	{"GEN", "FEB", "MAR", "ABR", "MAI", "JUN", "JUL", "AGO", "SET", "OCT", "NOV", "DES"},	// CA *
	{"JAN", "FEB", "MAR", "APR", "MAI", "JUN", "JUL", "AUG", "SEP", "OKT", "NOV", "DES"},	// NO *
	{"ЯНВ", "ФЕВ", "МАР", "АПР", "МАЙ", "ИЮН", "ИЮЛ", "АВГ", "СЕН", "ОКТ", "НОЯ", "ДЕК"},	// RE
	{"JAN", "VEB", "MÄR", "APR", "MAI", "JUN", "JUL", "AUG", "SEP", "OKT", "NOV", "DET"},	// EE *
	{"URT", "OTS", "MAR", "API", "MAI", "EKA", "UZT", "ABU", "IRA", "URR", "AZA", "ABE"},	// EU *
	{"TAM", "HEL", "MAA", "HUH", "TOU", "KES", "HEI", "ELO", "SYY", "LOK", "MAR", "JOU"},	// FI *
	{"JAN", "FEB", "MAR", "APR", "MAJ", "JUN", "JUL", "AUG", "SEP", "OKT", "NOV", "DEC"}	// DA *
};

static const char* WEEKDAY[][7] = {
	{"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"},	// EN *
	{"DIM", "LUN", "MAR", "MER", "JEU", "VEN", "SAM"},	// FR *
	{"SO",  "MO",  "DI",  "MI",  "DO",  "FR",  "SA"},		// DE *
	//{"DOM", "LUN", "MAR", "MIÉ", "JUE", "VIE", "SÁB"},	// ES
	{"DOM", "LUN", "MAR", "MIE", "JUE", "VIE", "SAB"},	// ES *
	{"DOM", "LUN", "MAR", "MER", "GIO", "VEN", "SAB"},	// IT *
	{"ZO",  "MA",  "DI",  "WO",  "DO",  "VR",  "ZA"},		// NL *
	//{"PAZ", "PTS", "SAL", "ÇAR", "PER", "CUM", "CTS"},	// TR
	{"PAZ", "PTS", "SAL", "CAR", "PER", "CUM", "CTS"},	// TR *
	{"NE",  "PO",  "ÚT",  "ST",  "ČT",  "PÁ",  "SO"},		// CZ *
	//{"DOM", "SEG", "TER", "QUA", "QUI", "SEX", "SÁB"},	// PT
	{"DOM", "SEG", "TER", "QUA", "QUI", "SEX", "SAB"},	// PT *
	{"ΚΥΡ", "ΔΕΥ", "ΤΡΙ", "ΤΕΤ", "ΠΕΜ", "ΠΑΡ", "ΣΑΒ"},	// GK
	{"SÖN", "MÅN", "TIS", "ONS", "TOR", "FRE", "LÖR"},	// SE
	{"NDZ", "PON", "WTO", "ŚRO", "CZW", "PIĄ", "SOB"},	// PL
	{"NE", "PO", "UT", "ST", "ŠT", "PI", "SO"},					// SK
	{"CN", "T2", "T3", "T4", "T5", "T6", "T7"},					// VN
	{"DUM", "LUN", "MAR", "MIE", "JOI", "VIN", "SÂM"},	// RO
	{"DG", "DL", "DT", "DC", "DJ", "DV", "DS"},					// CA *
	//{"SØN", "MAN", "TIR", "ONS", "TOR", "FRE", "LØR"},	// NO 
	{"SON", "MAN", "TIR", "ONS", "TOR", "FRE", "LOR"},	// NO *
	{"ВС", "ПН", "ВТ", "СР", "ЧТ", "ПТ", "СБ"},					// RU
	{"P", "E", "T", "K", "N", "R", "L"},								// EE *
	{"IG", "AL", "AR", "AZ", "OG", "OL", "LR"},					// EU *
	{"SU", "MA", "TI", "KE", "TO", "PE", "LA"},					// FI * 
	//{"SØN", "MAN", "TIR", "ONS", "TOR", "FRE", "LØR"}		// DA 
	{"SON", "MAN", "TIR", "ONS", "TOR", "FRE", "LOR"}		// DA *
};

static const char* LOCALE_LIST[] = {
	"EN", "FR", "DE", "ES", "IT", "NL", "TR", "CZ", "PT", "GK", "SE", "PL", "SK", "VN", "RO", "CA", "NO", "RU", "EE", "EU", "FI", "DA"	
};
const int LOCALE_SIZE = 22;

Locale set_locale(const char* locale){
	for (uint8_t i = 0; i < LOCALE_SIZE; ++i){
		if (strcmp(locale, LOCALE_LIST[i]) == 0){
			return i;
		}
	}
	return EN;
}

const char* get_weekday(int day_number, const Locale locale){
	return WEEKDAY[locale][day_number];
}

const char* get_month(int month_number, const Locale locale){
	return MONTH[locale][month_number];
}

