#pragma once
#ifndef ENGL09_H 
#define ENGL09_H
#define TEXTIN "text00.txt"
#define EN 32 //размер англ текста (одного слова) в структ word
#define EN1 256 //размер имени файла уже созданного с структурами word 
#define RU 32 //размер рус текста в структ word
#define MAX_WORD 8//начальный размер массива структур word
#define MULT_DIN_MEM 2 // в сколько раз буд умн размер масс стр при недостижении EOF
#define NAMENOSORT 64 //длинна им-ни сепарированного несортированного ф-ла
#define NEWTEXT 1 //ФЛАГ ДЛЯ КОДА НОВОГО ТЕКСТА
#define OLDTEXT 0 //ФЛАГ ДЛЯ КОДА СТАРОГО ТЕКСТА
#define QUANTITYCHARNAME 256 //?? кол-во байт в одной записи в масс имён ф-в прогр ??
#define QUANTITYNAME 1 // кол-во записей элементов стр-тур в массиве имён ф-в программы
// #define d ((&struct word a.c)-(&struct word a)) 
#define TEXTD //вывод считанн текста до сепарац  debug
//#define SEPARATED  // sepmini()
//#define M_SORT		// main()
#define SORT
//#define ALPHABET // Отсортирован alphabet3()
#define IDSORT
//#define REDUCT3  //_________________________________


//#define MAX_WORD1 100 //размер массива структ word

//ф-я возвр указатель на д память с отсепар словами - str word *pmemword
// заранее созданный и переданный в ф-ю,
// pamountmem указ на РАЗМЕР д пам str word
//multipl коэфф умнож при нехват дин памяти при token() 
//pmemtxtbuf указ. на дин массив неразбитого текста - копии входн файла
//pcount - указатель числа подсчитанных слов при сепарир
// нужен для точного размера массива (несортированного) структур с англ словами
// arv1 имя передаваемого через ком строку входного файла - нужен только для fprint()????
struct word * sepmini(struct word *pmemword, long *pamountmem, char *pmemtxtbuf, int *pcount, char const *arv1);  //ф-я новая
	//ф-я увелич размер int size дин памяти struct word *mem до размера *pnewsize до 
	//размера *pnewsize  и передающая в него содерж старой памяти struct word *pmemword 
	// и возвр ????????????????? указ на нов память
struct word * sepmini2(struct word *pmemword, long *pamountmem, char *pmemtxtbuf, int *pcount, char *nametext);  //ф-я новая
struct word *extensmem(struct word *pmemword, long *pamountmem, long newamountword, int *pcountnumword);

char* writebase2(FILE *phddfile, char * namefilehdd, struct word *pmemword, int countnumword);	// 
	// phddfile указ на hdd файл в котором сохранять базу слов 
	//  namefilehdd - имя на диске; pmemword - указ на массив структур, 
	//countnumword - число несорт структур; возврат указ имя файла структур - namefilehdd;
	//ф-я сама открывает r+и и потом закрывает файл на hdd 

struct word * alphabet3(struct word *aa, int *psize);//
	// возврат указатель на память с отсортир по алфав массивом структ, 
	// aa дин память с маc структур,  psize количество структур 
struct word * alphabet4(struct word *aa, int *psize, int(*pmeasure)(const void *, const void*), int disloc);
	// возврат указатель на память с отсортир по алфав массивом структ, 
	// aa дин память с маc структур,  psize количество структур 
	// *pmeasure -указ на ф осуществляющую сортировку
	// disloc - смещение поля в структуре которое нумеруется при сортировке
int measurealph(const void *, const void *);   //
	// ф -я измеряющая ENGL алфавитн порядок располож двух стукт(по указателям на них)
	// если первый арг больше второго то возврат положительного числа

struct word * idsort(struct word *pin, int *psize, int(*pmeasure)(const void *, const void *), int disloc);
	// возврат указатель на память с отсортир по алфав массивом структ, 
	// aa дин память с маc структур, psize - указ количество структур 
	// *pmeasure -указ на ф осуществляющую сортировку
	// disloc - смещение поля в структуре которое нумеруется при сортировке
	// ЗАПОЛНЯЕТ ID поля масс ст-р в зависимости от парам ???????

int measurerepeat(const void * a, const void * b);//
// ф -я измеряющая ENGL частотный порядок располож двух стукт(по указателям на них) 

int measurerepeatalph(const void * a, const void * b);//
// ф -я измеряющая ENGL частотный (с учётом алфавита) порядок располож двух стукт(по указателям на них) 

struct word * reduct3(struct word *pa, int *psize);// pa-указ на массив стр-ур, 
												   //psize-указ на пер-ю числ  элем-тов в масс 

char* rename2(const char* poldname, char *paddstring, int del); // ф -я укорачивает имя poldname.ext
																//на del и  добавляет paddstring віделяет дин пам нужного разм и возвл указ на него

struct word       // стр для хранения слов с переводом и др в
{
	int id;  // алфавитная id нумерация
	int repeat_id;	// нумерация по повторениям в тексте
	int random_id;	// произвольная нумерация в тексте
	int temp_id;	//
	int temp2_id;	//

	int repeat;	// количество повторений слова в тексте


				//char rait;  // рейтинг
				//будут и другие поля;
	char en[EN];    //массив строчный - размером из #define
	char ru[RU];	// то же
	char tempchar; // отладка временно для выравн при просм  F3
	int tempint;
};
struct inidat       // стр для хранения польз настроек имён файлов и др
{				//будут и другие поля;
	int idname;
	char name[EN1];    //имя файла
	char ininamenosortf[EN1];	//имя файла с несортрованн структ
	char ininamesortf[EN1];		//имя файла с сортрованн структ

					   //char namepath[EN1];	// c полным путём
					   //int number;  // размер файла
					   //char namehelp[EN1]; // пояснения к файлу
	int inicountnumword;//--->копия в ini файле числа  счетч слов
};

#endif