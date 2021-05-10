//


#define _CRT_SECURE_NO_WARNINGS  //   ?? подробнее об предупреждениях ??
#define TOKENDEBUG nodebug
//#define ALPHABET ok
#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include <ctype.h>
#include"engl17.h"  //

extern flagtext;

//////////-------  новая ф-ция  ------///////////////////////////////////////////////////////////////
// сепарирует строчный дин массив pmemtxtbuf[] (уже скопированный из ф-ла) в  массив структур word
// при работе с (большим текстом) из себя выз ф-ю extensmem()
struct word * sepmini(struct word *pmemword, long *pamountmem, char * pmemtxtbuf, int * pcountnumword, char const * name)
	//возвр указатель на д память с отсепар словами - str word *pmemword
	// заранее созданный (мал размера) и переданный в ф-ю,
	// pamountmem указ на РАЗМЕР д пам str word в б-тах
	//multipl коэфф умнож при нехват дин памяти при token() 
	//pmemtxtbuf указ. на дин массив неразбитого текста - копии входн файла
	//pcount - указатель числа подсчитанных слов при сепарир
	// нужен для точного размера массива (несортированного) структур с англ словами
	// arv1 имя передаваемого через ком строку входного файла - нужен только для fprint()????
{
	printf("~~   Начинает работать ф-ция sepmini()  ~~~\n \
	~~ Разбиение массива символов из ф-ла - ( %s ): ~~\
	~~  приведение к нижнему регистру и вывод в монитор  ~~\n", name);
	printf("Tokens - называется опознавательный знак\n\n");

	//int count = 0;  // КОЛ НЕОТСОРТ СТРУКТУР ????
	//printf("Tokens:\n");

	int countstr = 0;				// локальный счетчик заполняемых структур в цикле token
	long maxmemstr = MAX_WORD;		// начальная верхняя граница счетчика для сравнения = 8
									//потом перехода к увеличению памяти в цикле token при нехватке
	int * pmaxmemstr = &maxmemstr;		//--> указ на верхн гран счетч для перехода к увел-ию памяти 
	struct word *plocseptempstr2 = NULL;		//--> врем указ на увеличенную дин память

	printf("Выделенна начальная память = %d Bytes под  %d \
отсепарированных струтур \n", (*pamountmem), (*pamountmem) / sizeof(struct word));	// 

																					//11
	char seps[] = " ][()/,.\t\n\r\v\xA0\"\'\\?!-+*/<>=_:;0123456789#$%^&";	// строка символов - сепараторов
	char *token1 = NULL;		//--->char *token1 - это типа указ на первый кусок   
								//отделённого  char текста с \0 в конце
	char *next_token1 = NULL;	//--->буфер ?? думаю для хранения разбиваемой строки
								//22														//															Establish string and get the first token:
	token1 = strtok_s(pmemtxtbuf, seps, &next_token1);	//первый вызов ф сепарирования
	char tokenbuf[EN];			//---> лок буферный массив для переноса из token1 english
								//в нижний регистр = 32 ?что будет при превышении???
								// !каждая строка после strtok_s() будет заканчиваться \0
								//33
	while (token1 != NULL)
	{
		//*pcountnumword += 1;     // ???????????????????????????????????????
		///_______________________________________________________________________________________________________

		// Get next token:
		//  ПРИВОДИМ СИМВОЛЫ К НИЖН РЕГИСТРУ ==================================
		int ix = 0;				// СЧЕТЧИК в 0 , для перебора СИМВОЛОВ  (а если > 32 ???)
								// который в каждом новом слове сбрасывается в 0
		while (token1[ix])					// начиная с первого символа из токена (char-строки) 
											//  пока в  token1[ix] не встретится \0
		{									//  ПРИВОДИМ СИМВОЛЫ К НИЖН РЕГИСТРУ 
			tokenbuf[ix] = (char)tolower(token1[ix]);  //посимвольно преобраз-е текущ символа ст-ки в СТРОЧНЫЙ
			ix++;                                   // инкремент индекса символов в строке
		}
		tokenbuf[ix] = (char)tolower(token1[ix]);	
		// запмсь  последнего =/0 символа ?
		//- end - символы приведены к нижн регистру 
		//*pcountnumword += 1;     // ?????????????????

		// Но! Если счётч слов достиг предела макс памяти массива под структ тогда ========================
		if (*pcountnumword == maxmemstr - 1)  //pcount - аргумент ф-ции sepmini ??????????=========================
											  // счётчик слов достиг заранее введенной в перем границы MAX_WORD
											  // или предыдущего умножения maxmemstr = maxmemstr * MULT_DIN_MEM;
											  // тут нужно увелич память вызовом extensmem() !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
											  //maxmemstr = maxmemstr * MULT_DIN_MEM; //
		{
			long newamountword = maxmemstr * MULT_DIN_MEM; //
			maxmemstr = newamountword; //?? может лишнее дублирование верней границы струтур
									   // вызов ф-ии ???? и т д
			printf("~~   Вызов ф-ции extensmem()  ~~\n");
			pmemword = extensmem(pmemword, pamountmem, newamountword, pcountnumword);

		} // end if	4 .............................................................
		if (ix > 1)
		{
			strcpy_s((pmemword + *(pcountnumword))->en, EN, tokenbuf);	// заполнение поля 
											// структуры но ! надо сравнить на длинну ?
			((pmemword + *(pcountnumword)))->id = (int)(*(pcountnumword)+1); //заполнение id
						// id отличается от countnumword на 1 ---   id=1   countnumword=0
			((pmemword + *(pcountnumword)))->repeat = 0;   // наверно temp ??
#ifdef SEPARATED
			printf("%4d. r=%d  id= %4d. - { %s }  ->  [ %s ]\n", *pcountnumword, \
				pmemword[(*pcountnumword)].repeat, *pcountnumword + 1, token1, tokenbuf);// отладочная строка 
																						 //- вывод уже ОТДЕЛЁННОГО СЛОВА -temp-  pmemword[(*pcountnumword)].repeat,
																						 //*(plocsep + *pcount)->en = "NULL"; // заполнение поля структуры
#endif			 //55
			*pcountnumword += 1;     // ???????????????????????????????????????

		}


#ifndef TOKENDEBUG
		//printf("string1----------%s \n", buf);		//temp для пояснения работы strtok_s(string1, seps, &next_token1)
		// вывод НЕРАЗДЕЛЁННОГО ТЕКСТА
		printf("next_token1======%s \n\n", next_token1);//temp для пояснения работы strtok_s(string1, seps, &next_token1)
#endif
														//66								// тут  цикл вызова token  ??????????????? 
		token1 = strtok_s(NULL, seps, &next_token1);
		//цикл-ий  вызов фц strtok дальнейшег пословного сепар-я
	}	// end while (token1 != NULL)

		// "обрезание" лишней памяти в массиве струтур
	long realamountword = *pcountnumword;
	pmemword = extensmem(pmemword, pamountmem, realamountword, pcountnumword);
	printf("Выделенна память = %d Bytes под  %d \
реальное число отсепарированных струтур word \n",
(realamountword) * sizeof(struct word), (realamountword));			//   


//=================================================================================
//=================================================================================
//=================================================================================
	return pmemword;
}//    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   END sepmini()   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

 //////////-------  новая ф-ция  ------///////////////////////////////////////////////////////////////
 // сепарирует строчный дин массив pmemtxtbuf[] (уже скопированный из ф-ла) в заранее созданный 
 // (мал размера) массив структур pmemword,  при работе с (большим текстом)
 //  из себя вызывает ф-ю extensmem()
 //возвр указатель на д память с отсепар словами - str word *pmemword
 //	также переводит все символы в строчные   ____________________________________________
 // pamountmem указ на РАЗМЕР д пам str word в б-тах
 //MULT_DIN_MEMl коэфф умнож при нехват дин памяти при token() 
 //pmemtxtbuf указ. на дин массив неразбитого текста - копии входн файла
 //pcount - указатель числа подсчитанных слов при сепарир
 // нужен для точного размера массива (несортированного) структур с англ словами
 // nametext имя передаваемого входного файла - нужен только для fprint()????
struct word * sepmini2(struct word * pmemword, long * pamountmem, char * pmemtxtbuf, int * pcount, char * nametext)
{
	char seps[] = " ,\t\n";
	char *token1 = NULL;
	
	char *next_token1 = NULL;


	return NULL;
}
//  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  END sepmini2()   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//////////////==== увелич размер дин памяти и перенос в содерж старой пам  ======/////////////////////
struct word *extensmem(struct word *pmemword, long *pamountmem, long newamountword, int *pcountnumword)
	//при достижении счётчика заполнения struct word значения выделенного разм памяти size вызывается ф-я
	// котори увелич размер *pamountmem дин памяти struct word * до размера *pnewsize  и передёт в него содерж 
	// старой памяти struct word *pmemword и возвр ?????????????????????? указатель на нов память
{
	printf("\n~~   Начинает работать ф-ция extensmem()  ~~\n");
	printf("Аргументы *pamountmem-%d;  newamountword - { %d };sizeof(struct word)-{ %d }, *pcountnumword - [ %d ]\n",\
		*pamountmem, newamountword, sizeof(struct word), *pcountnumword);   // отладочная
																																		  // ~~~~~~~~~~~~~~~~~~~~~~~  выделение нов увел дин памяти   ~~~~~~~~~~~~~~~~~~~~~~~~~
   	struct word *ptempstrmemword = (struct word *) malloc((newamountword) * sizeof(struct word));
	//ptempstrmemword - временный указат на выделение другого размера памяти под структуры 
	if (ptempstrmemword == NULL)printf("Не выделенна память под расширение struct word[] \n");
	else printf("Выделенна увеличенн память = %d Bytes под  %d \
отсепарированных струтур word\n\n",
(newamountword) * sizeof(struct word), (newamountword));			//
	*pamountmem = newamountword * sizeof(struct word);       //для будущего использ в др ф-циях
	if (newamountword < *pcountnumword)
	{
		printf("Выделенная память под struct word[] меньше \n чем требует счётчик слов \n");
		//return (NULL);
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ копирование во временн память    ~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (int z = 0; z < *pcountnumword; z++)	// поэлементное копирование во временн память
	{											// начиная с адреса по указателю памяти + ...
		*(ptempstrmemword + z) = *(pmemword + z);	// плюс инкремент  ...

	}
	printf("Скопирован предыд масс структур в увелич память\n");
	//*pcountnumword += 1;  // ...
	free(pmemword);										// освободить старое содержим plocsep
	pmemword = ptempstrmemword;			 		// присв plocsep адрес нового содержимого plocseptempstr2
	return pmemword;
}//    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   END extensmem()   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


 /////////////////// ======== запись в файл(заранее переименов) базу  структур   ==========///////////  
char* writebase2(FILE *phddfile, char* pinidat, struct word *pmemword, int countnumword)//
					//phddfile указ на hdd файл в котором сохранять базу слов    ??("argv[1]_nosort.dat")?
					//pinidat -  уже сформированное ранее ИМЯ ф-ла для hdd
					// pmemword - указ на дин массив несорт структур, 
					//countnumword - число стр динмассива
					//, возврат указ имя файла с  структурами ( ----- )
					//ф-я сама открывает r+и и потом закрывает файл на hdd 
{
	printf("   ~~~ Запись в файл %s несортированного массива структур по адресу: ~~~  \n", pinidat);
	//puts(argv[0]);  // записывает в стандартный поток вывода stdout строку     //temp
	//printf("~~ argv[0] является собственное имя исп ф-ла: ( %s )~~\n", argv[0]);  // temp

	// ~~~~~~~~~~ открытие указателя на hdd файл базы слов с именем pinidat  ~~~~~~~
	errno_t err = 1;   // зачем 1 -  ?????????????????????????????????? отлака наверно
					   //FILE *hddfile = phddfile;					// указ на файл в котором сохранять базу слов-("argv[1]_nosort.dat")?
	if (flagtext)		//если нов текст то открывается новый файл базы на запись
	{
		err = fopen_s(&phddfile, pinidat, "w+b");
		if (err) {
			perror(pinidat);
			printf("   ~~~ Не открылся нов файл w+b %s  массива структур  ~~~  \n", pinidat);
			system("pause"); exit(1);
	 	}
		else printf("   ~~~ Записан новв файл %s  массива структур  ~~~  \n", pinidat);
	}
	else
	{
		err = fopen_s(&phddfile, pinidat, "r+b");	// открыть бинарн файл для чт и дозаписи 
		if (err) {
			perror(pinidat);
			printf("   ~~~ Не открылся старый файл r+b %s  массива структур  ~~~  \n", pinidat);
			system("pause"); exit(1);
		}
		else printf("   ~~~ Открыт старый файл %s  массива структур  ~~~  \n", pinidat);
	};



	fwrite(pmemword, sizeof(struct word), countnumword, phddfile);  // запись в файл hddfile = "*pinidat"

						   //fclose(pFtxt);	// из которого читается список слов
	fclose(phddfile);		// закр файл в котором сохранять базу слов
	printf("~~ Записан файл базы слов с пом-ю writebase2(): ( %s ) ~~\n", pinidat);  // temp
	//...............................................................
	//free(pnamewordnosort);	//осв-ие памяти с ИМЕНЕМ файла *_common.dat отсепарир-ных неотсортиров-х структ
	return(pinidat);
}//    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   END writebase2()   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


 ///////////////////////===========  Алфавитная сортировка 3  =============////////////////////////////
 struct word * alphabet3(struct word *aa, int *psize)// возврат указатель на память с отсортир по алфав массивом структ,
 // a дин память с маc структур, size количество структур
 {											// работает норм
 int(*pmeasure)(const void *, const void *);  //обьявл указатель на функцию для передачи параметром в  ф-ю qsort()
 //  и присвоение указателю адреса функции
 // вызов библиотечн функции qsort с передачей параметром по указателю моей функции measure
 pmeasure = measurealph;
 qsort(aa, *psize, sizeof(struct word), pmeasure);	//|1|1|1|1|1|1|1|1|1|1|1|1|
 #ifdef ALPHABET
 int i;
 for (i = 0; i < *psize ; i++)
 {
 printf(" ~~~ Отсортирован  %d - ( %s )   ~~~\n",i, aa[i].en);    // temp
 }
 #endif //ALPHABET
 return aa;
 };//    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   END alphabet3()   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


 ///////////////////////===========  Алфавитная сортировка 4  =============////////////////////////////
struct word * alphabet4(struct word *aa, int *psize, int(*pmeasure)(const void *, const void *), int disloc)
	// возврат указатель на память с отсортир по алфав массивом структ, 
	// aa дин память с маc структур, psize - указ количество структур 
	// *pmeasure -указ на ф осуществляющую сортировку
	// disloc - смещение поля в структуре которое нумеруется при сортировке

{	// ЗАПОЛНЯЕТ ID поля масс ст-р в зависимости от парам ???????
	//int(*pmeasure)(const void *, const void *);  
	//обьявл указатель на функц для передачи параметром в  ф-ю qsort()
	//  и присвоение указателю адреса функции
	// вызов библиотечн функции qsort с передачей параметром по указателю моей функции measure
	//pmeasure = measurealph;
	printf(" \n\n~~~ Далее находимся внутри inside alphabet4()\n");
	qsort(aa, *psize, sizeof(struct word), pmeasure);	//вызов сортировки
#ifdef ALPHABET
	int i;
	for (i = 0; i < *psize; i++)
	{
		printf(" ~~~ Отсортирован  %d - ( %s )   ~~~\n", i, aa[i].en);    // temp
	}
#endif //ALPHABET
	return aa;
};//    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   END alphabet4()   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

  ///////////////////==========     ф-я алфавитного сравнения строк  =======/////////////////////////////////
int measurealph(const void * a, const void * b)		//   
													// ф -я измеряющая ENGL алфавитн порядок располож двух стукт(по указателям на них) 
{
	const char * string1 = (((struct word *)a)->en);
	const char * string2 = (((struct word *)b)->en);
	int temp = _stricmp(string1, string2);	// если первый арг больше второго
											// то возврат положительного числа
	return temp;
};//    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   END measalph()   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

  ///////////////////////===========  Сортировка по разным id структуры word   ========////////////////////
struct word * idsort(struct word *pin, int *psize, int(*pmeasure)(const void *, const void *), int disloc)
	// возврат указатель на память с отсортир по алфав массивом структ, 
	// pin дин память с маc структур, psize - указ количество структур 
	// *pmeasure -указ на ф осуществляющую сортировку
	// disloc - смещение поля в структуре которое нумеруется при сортировке
	// ЗАПОЛНЯЕТ ID поля масс ст-р в зависимости от парам ???????
{
	struct word * ptemp = NULL;
	ptemp = (struct word *) malloc((*psize) * sizeof(struct word));
	if (ptemp == NULL)printf("idsort() Не выделенна память под врем массив ptemp \n");
	printf(" \n\n~~~ Далее находимся внутри inside idsort() -  id = %d \n", pin[0].id);
	memmove(ptemp, pin, (*psize) * sizeof(struct word));//* sizeof(struct word)

	printf(" \n~~~ Далее после применения qsort() выв Отсортир-й массив стр-р ~~~ id = %d \n", pin[0].id);
	// temp
	qsort(ptemp, *psize, sizeof(struct word), pmeasure);	//вызов сортировки
	
	int m;//printf("    \n");
#ifdef IDSORT
	printf("This \"Print\" inside to idsort() in engllib16.c - \n");
	
	for (m = 0; m < *psize; m++)				//
	{													//
		printf("m- %3d.  id=%3d ___ ( %s )___[ %d ]   \n", m, ptemp[m].id, ptemp[m].en, ptemp[m].repeat);    // temp
	}
#endif
	printf("    \n");
	for (m = 0; m < *psize; m++)
	{
		ptemp[m].repeat_id = m;
	}

	for (m = 0; m < *psize; m++)
	{
		pin[m] = ptemp[m];    // копирование массива (для return) из локального
	}
	free(ptemp);
	return pin;
};//    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   END idsort()   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 

  ///////////////////==========     ф-я сравнения строк по повторениям =======///////////
int measurerepeat(const void * a, const void * b)		//   
														// ф -я измеряющая ENGL алфавитн порядок располож двух стукт(по указателям на них) 
														// + если a>b 0 if a=b - if a<b
{
	const int  repeat1 = (((struct word *)a)->repeat);
	const int  repeat2 = (((struct word *)b)->repeat);
	int temp = (repeat2)-(repeat1);
	return temp;
};//    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   END measurerepeat()   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  

  ///////////////////==========     ф-я сравнения строк по повтор + алфавит =======///////////
int measurerepeatalph(const void * a, const void * b)		//   
// ф -я измеряющая ENGL алфавитн порядок располож двух стукт(по указателям на них) 
														// + если a>b 0 if a=b - if a<b
{
	const int  repeat1 = (((struct word *)a)->repeat);
	const int  repeat2 = (((struct word *)b)->repeat);
	int temp = (repeat2)-(repeat1);
	if(temp==0)
	{
		const char * string1 = (((struct word *)a)->en);
		const char * string2 = (((struct word *)b)->en);
		int temp = _stricmp(string1, string2);	// если первый арг больше второго
												// то возврат положительного числа
		return temp;
	}
	else
	{
		return temp;
	}


	
};//    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   END measurerepeatalph()   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 


  ////////////============   a-я изменения строки - имени файла   ============/////////////////////////
char* rename2(const char* poldname, char *paddstring, int del) // ф -я укорачивает имя poldname.ext
															   //на del и  добавляет paddstring віделяет дин пам нужного разм и возвл указ на него
{
	//дин стр oldname укорачивается на длинну del , дописывая стр pletteadd
	//и всё это возвращается в указателе на строку - pnewname
	char *ptempstrstr = (char*)malloc(strlen(poldname) + strlen(paddstring));
	// память для нов стр
	//printf(" ########  inside of rename2(); #########\n ");    // temp
	memset(ptempstrstr, '\0', (strlen(poldname) + strlen(paddstring)));			// заполнение \0 
	strncpy(ptempstrstr, poldname, (strlen(poldname) - del));	// копируем без .ext
																//printf(" ~~~внутр перем ptempstrstr, = ( %s )   ~~~\n", ptempstrstr );    // temp
	strcat(ptempstrstr, paddstring);  // добавл строку
	printf(" ~~~внутри rename2() вн перем ptempstrstr,= ( %s ) ~~~\n", ptempstrstr);  // temp
	return (ptempstrstr);
}; //    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   END rename2()   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 




   ////////===========  сокращение повторяющихся слов. запонение repeat  ===========///////
struct word * reduct3(struct word *pa, int *psize)	
	//сокращает повторяющиеся слова после алф сорт
	// заполняет поле id pa-указ на массив стр-ур, 
	//psize-указ на пер-ю числ  элем-тов в масс - уменьшается при повторяюцихся словах 
{
	///////////===========  заполн поля repeat = 1 для всех структур в массиве ---------------
	int n = *psize;  //переданное начальное количество структур в массиве, внутри ф-ции уменьшается
	int i = 0;
	for (i = 0; i < n; i++)				//заполн поля repeat = 1 для всех структур в массиве
	{
		(((struct word *)pa + i)->repeat) = 1;
	}

	////////// ======= созд временн динам массив ptempstr с незаполенными индексами   ----------------
	struct word *ptempstr = (struct word*)malloc(*psize * sizeof(struct word));  // указ на 
																				 //временн массив с незаполенными индексами
																				 // в нём будут записываться эл-ты мас-ва структур но уже без повторений
	if (ptempstr == NULL)printf("не выделенна память под temp в reduct3() \n");
	struct word *prev = (struct word*)malloc(sizeof(struct word));  //нужен указатель 
																	//на единичный (текущий) элемент структуры (для сравнения)  
	if (prev == NULL)printf("не `	````````````````выделенна память под prev в reduct3() \n");
	for (i = 0; i<n; i++)	(((struct word *)ptempstr + i)->repeat) = 1;		//заполн 1-цей всех полей repeat массива ptempstr[]
	i = 0;
	ptempstr[0] = pa[0];	// перенос в врем масс нулевого элемента массива переданного через аргументы
	prev[0] = ptempstr[0]; // запомнен предыдущий элемент (для сравнения)
	int t = 0;		// счётчик  индексов нового массива ptempstr[t] (меньше чем i)
					// 
	for (i = 1; i<n; i++) // перебор c 1-го индекса всех индексов только входного массива pa[i] переданного 
	{		//через аргумент  в функц и сравнение с следующим запомненным в *prev предыдущим текст полем *->en
		if (measurealph(prev, (pa + i)) == 0)  // если следующий равен текущему.....
		{
			(((struct word *)ptempstr + t)->repeat) += 1;  // увелич поле repeat текущего ptempstr[t] не инкрементируя t
		}
		// иначе повторений поля нет и элемент вх массива с другим полем ->en 
		else
		{
			t += 1;					// инкр индекс врем массива ptempstr[t]
			ptempstr[t] = pa[i];		// заносим во врем масс из перебираемого pa[i] уже элемент с новым полем ->en
			prev[0] = ptempstr[t];		// запомнен в указатель текущий  элемент временного массива
		}
	}								// конец перебора всех индексов массива pa[i] переданного через аргумент в функц
	*psize = (t + 1);		// новый размер дин массива структур
	struct word *pret = (struct word*)malloc(*psize * sizeof(struct word));  // указ на заполенн временн массив для возвр из ф
	if (pret == NULL)printf("не выделенна память под temp в reduct() \n");
	// перезапись сокращённого массива temp в уменьшенн разм д п pret
	int k = 0;
	for (i = 0, k = 0; i <= t; i++, k++)
	{
		pret[i] = ptempstr[k];    // копирование массива (для return) из локального
	}
	for (i = 0; i < *psize; i++)  // заполнение "алфавитного" id
	{
		pret[i].id = i;
	}

#ifdef REDUCT3
	printf(" \n\n~~~ далее in reduct3() - отсортированный массив структур ~~~ \n");
	int m;												// temp
	for (m = 0; m <= t; m++)				//
	{													//
		printf(" _ %3d.  id=%3d --- ( %s )---[ %d ]   \n", m, pret[m].id, pret[m].en, pret[m].repeat);    // temp
	}
	printf(" \n~~~ далее выход из reduct3() - алф отсорт и уменьш повтор массив структур ~~~ pret[0].id = %d\n\n", pret[0].id);
#endif// REDUCT3
	free(ptempstr);
	free(prev);
	return pret;
}//    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%   end reduct3()   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 




