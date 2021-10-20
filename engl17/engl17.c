
//22/04/21 первый  вариант с передачей фиксиров. имени текста без ком стр
//


//надо бы проверить прав заполнения по умолчанию дин памяти и INI файла !!!


#define _CRT_SECURE_NO_WARNINGS  //   ?? подробнее об предупреждениях ??
//#define VARTOKEN nodebug
//#define RENAME ok
//#define ALPHABET ok
#define M_SORT
//#define NO_SORT

//#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>
#include"engl17.h"  //

 
FILE *pFtxt; //---> текст который читать - argv[1] первый аргумент ком строки "txt2.txt"
FILE *pFini;	//---> указатель на структ. файл в котором сохранять файлов fini.dat
FILE *pFnosort;	//---> указатель на структ. ф в котор сохр несорт базу слов - argv[1]_nosort.dat"
FILE *pFsort;	//---> указатель на структ. ф в котором сохр сорт базу слов - argv[1]_sort.dat"

//FILE *pFsort;	//---> указатель на структ. ф в котором сохр сорт базу слов - argv[1]_sort.dat"
FILE *pFfreqsort;//---> указ на  ф в котор сохр частотно-сорт базу слов - argv[1]_freqsort.dat"

char *pnamenosort = NULL;		//-->указат на имя ф-ла с запис несорт масс стр 

//struct inidat settings;     // структура ОПЕРАТИВНОЙ памяти с пользавательскими настройками
//struct inidat *psettings = &settings;	//каз на структ в ОПЕРАТИВНОЙ пам с пользават-ми настройками

struct word *pmemword = NULL;	//-->глоб указатна первичное выделеие несорт ДИН памяти 
					//под МАССИВ СТРУКТУР (word) для отсепарирования token()
					//далее память будет перерасширятся по этому указателю
struct word *pmemsortword = NULL;  //--> указ на д пам стрктур с отсортированными словами
//struct word *pmemalphabetword = NULL;  //не приг-сь ук на д пам с отсорт-ми и сокращенными словами
int amountword = 0;		//---> РАЗМ дин пам В ЗАПИСЯХ  под структуры word
int *pamountword = NULL;		//---> указ на РАЗМ дин пам В ЗАПИСЯХ  под структуры word
int countnumword = 0;	//---счётчик инкремента слов а значит и стр-р при сепар-и
int *pcountnumword = NULL;		//--->указатель на счетч слов
int countnosort = 0;	//---общее количество отсепарированных несортированных слов в тексте
struct inidat *pmemini ;	//--->указ на ДИН пам стр-ры базы ini имён прог-мы
int flagtext = NEWTEXT;	//переменная -ФЛАГ режима работы со стар или нов текстом
						//т е будет ли сепарироваться по новой и писаться в нофую базу слов 
						//!!!!  попробуем убрать и не использовать  !!!!
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%       main     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
int main(int argc, const char ** argv, const char** env)
{

	system("chcp 1251 > nul");
	errno_t err;			//  переменная (int?) для вывода ошибок ? внутр переменная (int?)
	//pamountword = &amountword;  !!!!!!!!!!!!!!!!
	//pcountwordlok = &countnumword;
	pamountword = &amountword;  // указ на РАЗМ дин пам В ЗАПИСЯХ для сепарир стр-р(пока= 8 стр)
	pcountnumword = &countnumword; //указ на счётч инкр-та СЛОВ = СТРУКТ при сепар-и 
	int flagkey = 0; // локальный флаг-клавиша для передачи кода наж клавиши
	int ttime = DELAYKEY;	//лок перем - задержка для выбора по клавише или таймеру

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 1 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//// 1)созд-ся д пам pmemini и заполн 0, 2)провер наличие старого ini файла, 
//// если нет то делаем новый, д пам pmemini заполн по ум-нию и запис в файл, 
//// 3)переоткрывается pFini на чтение с дозаписью и перенос содерж в pmemini  
//// 4)если был стар ini ф то переносится в pmemini его содерж , 
//// 5)показ 5 сек имеющихся настроек из pmemini т е =ini файла, с ним
//// 6) закрыть ini файл , брать настройки из д п pmemini  =ini файлу

	//1)созд д пам pmemini
	pmemini = (struct inidat*)malloc(sizeof(struct inidat)); //созд ДИН пам под стркт
	if (pmemini == NULL)printf("Не выделена память ini настройки программы \n");
	else printf("  Выделена дин пам pmemini = %d Bytes \n\
 под ini структуру-настройки программы \n",sizeof(struct inidat));	// debug
	
	//присвоить имя динамической струтуре = struct inidat --- settings  ---
	//struct inidat settings = *pmemini;

	
	memset(pmemini, '\0', sizeof(struct inidat)); //  пока  заполн. нулями, ?

	//	ТУТ  взамен 0 - СТАНДАРТНЫЕ настройки (из шаблона)
	pmemini->sorttype = 1; // по умолч алфав сортировка
	strncpy(pmemini->name, "text00.txt", EN1); // имя  ф
	strncpy(pmemini->ininamenosortf, "text00_nosort.dat",EN1); // имя несорт ф
	strncpy(pmemini->ininamealphsortf, "text00_alphsort.dat", EN1); // имя алфав сорт ф
	//strncpy(pmemini->ininameafreqsortf, "text00_freqalphsort.dat", EN1); //имя част-алфав ф


	strncpy(pmemini->ininameafreqsortf, "text00_freqalphsort.dat", EN1); //имя част-алфав ф

	//и возможны !!!! дальнейшие настройки дополнительно к настр по умолчани
							
	//2) есть ли ф fini.dat  ==============================================================

	//2) открытие=проверка сущ-вания стар ini файла пользов настр "fini.dat"=============		
	err = fopen_s(&pFini, "fini.dat", "r+b");	 // открывается ли на ЧТЕНИЕ с дозаписью
	if (err)	// НЕТ  СТАРОГО fini.dat "r+b" = нет. Делаем новый = "w+b"							
	{			// (!!! только "r" в VS чтото не получилось)
		perror("fini.dat");
		puts("\n !!! НЕ найден ini файл пользователя \n чтобы созд его = нажать ENYKEY  \n");
		system("pause");

	//// открывается НОВЫЙ fini.dat "w+b" на ЗАПИСЬ-ЧТЕНИЕ
		err = fopen_s(&pFini, "fini.dat", "w+b");
		if (err)	// значит почему-то всётаки не создался fini.dat и выход								
		{
			puts("\n !!! почемуто не создался новый ini файл пользователя \n");
			perror("fini.dat");
			free(pmemini);				//освоб памяти
			system("pause");
			exit(1);					//урок сорвался
		}

		else
		{	// открылся НОВЫЙ fini.dat на ЗАПИСЬ-ЧТЕНИЕ
			// (^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^)
			// ПОПОЗЖЕ тут дописать в дин пам pmemini настр по умолчанию !!! ^^^^^^
			pmemini->sorttype = 1; // по умолч алфав сортировка
			strncpy(pmemini->ininamenosortf, "text00_nosort.dat", EN1); // имя несорт ф
			strncpy(pmemini->ininamealphsortf, "text00_alphsort.dat", EN1);// имя алфав сорт ф
			strncpy(pmemini->ininameafreqsortf, "text00_freqalphsort.dat", EN1);//имя част-алфав ф
			
			//запись  стандартн по умлч настроек ИЗ ДИН ПАМЯТИ pmeminidat в ф fini.dat  ======		
			size_t result = fwrite(pmemini, sizeof(struct inidat), QUANTITYNAME, pFini);
			//puts("\n +++  создался  НОВЫЙ ini файл пользователя со стнд настр \n");	//debug			
			puts("\n Записан из Д ПАМ на HDD Новый стандартный файл пользователя \
с стнд настр\"fini.dat\" \n");
			
			fclose(pFini);	// закрыть ф-й поток в который записаны настройки ini по умолчанию
			//puts("\n +++  создался  НОВЫЙ ini файл пользователя со стнд настр \n");	//debug			
		}
	
	//3) ПЕРЕОТКРЫВАЕТСЯ ли на чтен с дозаписью
		err = fopen_s(&pFini, "fini.dat", "r+b");// ПЕРЕОТКРЫВАЕТСЯ ли на чте с дозаписью
		if (err)	// значит не открылся fini.dat  - и выход								
		{
			perror("fini.dat");
			puts("\n !!!!! НЕ переоткрылся на чт с дозап ini файл пользователя   \n");
			system("pause");
			exit(1);					//НЕ переоткрылся на чт с дозап ini файл
		}
		// получилось создать новый закрыть и по новой открыть "fini.dat"
		else 
		{
			puts("\n Открылся на чт с дозап нов fini.dat файл польз-ля с стандартн настр  \n");
		}
		system("pause");
	} // end if "нет  стар fini.dat r+b" = нет. Сделан новый ,,,,,,,,,,,,,,,,,,,,,,,,

	  //4) так как fini.dat открылся т е - ЕСТЬ старый то Новый fini.dat не нужен
	else
	{	//4) Новый fini.dat не нужен
		size_t result = fread(pmemini, sizeof(struct inidat), QUANTITYNAME, pFini);
	// ввод в pmemini имеющихся СТАРЫХ настроек и имени уже разб-го т-та - для открытия ф баз 
		puts("\n Открылись и скоп в pmemini СТАРЫЕ имеющиеся настр-ки (чт с дозап ini ф) \n");
	} // end if else"есть ли  стар fini.dat r+b" = да  ......................................
	  
	  //5) значит теперь у нас есть УЖЕ ПО ЛЮБОМУ созданный и закрытый fini.dat  _______________________________________
	  
	 // все настройки будут идти с структурой settings созданной и заполн из фай ла в 
	//д память - pmemini
	// (^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^)
	// тут надо бы секунд на 5 вывести на экран имеющиеся стнд настройки pmemini
	  puts("\n - показ 5 сек структуру settings из pmemini н-ки ур.(по умолчанию ???)\n");
 
	  printf(" ~~~ pmemini->idname = %d \n", pmemini->idname);
	  printf(" ~~~ pmemini->sorttype = %d \n", pmemini->sorttype);
	  printf(" ~~~ (*pmemini).sorttype = %d \n", (*pmemini).sorttype);
	  printf(" ~~~ pmemini->name[EN1] = %s \n", pmemini->name);
	  printf(" ~~~ pmemini->ininamenosortf[EN1] = %s \n", pmemini->ininamenosortf);
	  printf(" ~~~ pmemini->ininamealphsortf[EN1] = %s \n", pmemini->ininamealphsortf);
	  printf(" ~~~ pmemini->ininameafreqsortf[EN1] = %s \n", pmemini->ininameafreqsortf);
	  puts("    --- показания счетчика слов:");
	  printf(" ~~~ pmemini->inicountnumword = %d \n", pmemini->inicountnumword);
	  //printf(" ~~~ idname = %d \n", settings.idname);printf(" ~~~ idname = %d \n", settings.idname);
	  
	  /*
	  printf(" ~~~ pmemini->name[EN1] = %s \n", pmemini->name);
	  printf(" ~~~ pmemini->ininamenosortf[EN1] = %s \n", pmemini->ininamenosortf);
	  printf(" ~~~ pmemini->ininamealphsortf[EN1] = %s \n", pmemini->ininamealphsortf);
	  printf(" ~~~ pmemini->ininameafreqsortf[EN1] = %s \n", pmemini->ininameafreqsortf);
	  puts("    --- показания счетчика слов:");
	  printf(" ~~~ pmemini->inicountnumword = %d \n", pmemini->inicountnumword);
	  */

	  puts("    ================================= \n");

	  printf(" ~~~ pmemini->sorttype = %d \n", pmemini->sorttype);
	  // незаполнено name !!!!  
	  printf("\n ~~~ pmemini->name[] = %s \n", pmemini->name);
	  printf("\n ~~~ Отсортированных англ слов countnumword = %d и т д \n ", countnumword);
	  printf("\n ~~~ Всего в тексте англ слов =  %d и т д \n ", countnosort);
	  printf("\n ~~~ Уже изученных из них англ слов =  и т д \n ");

	  Sleep(3000);  // задержка мс
	  puts("\n ^^^ закончился показ 5 сек из Д ПАМ-pmemini настройки польз-ля  ..\n");

	  //6)закрыть ф поток в который запис ini настройки СТАРЫЕ ИЛИ НОВЫЕ ПО УМОЛЧАНИЮ  
	  fclose(pFini);	//end теперь УЖЕ есть ПО ЛЮБОМУ созданный и потом закрытый fini.dat 
		
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 2 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	//// 1-й оставить урок КАК ЕСТЬ ИЛИ ЕСЛИ ХОТИТЕ СДЕЛАТЬ УРОК ПО НОВОМУ 
	puts("\n 1) ЕСЛИ ХОТИТЕ СДЕЛАТЬ УРОК ПО НОВОМУ    ?  - тогда нажмите 'y'  \n");
	if ('y' == _getch(stdin))//  ==================================================
	{
		// 2) если открыть новый текст ? - тогда нажм 'y' откр-тие нового входн ф text00.txt 

//=====######======######=========#########=======######=================================

//_ 	2a)  ================================================
		puts("\n 2) если открыть и сортировать НОВЫЙ текст ? - тогда нажмите 'y'\n");
		if ('y' == _getch(stdin))
			//  открытие нового входного text00.txt файла  ==============
		{
			puts("\n пробуем открыть text00.txt и обновляем содержимое fini.dat ___ \n");
			//// открытие входного text00.txt файла и базовая сортировка   ===================
			err = fopen_s(&pFtxt, TEXTIN, "rb");// инициал-тся указ=ль FILE *pFtxt 
												//T  и открывается в режиме (rb)-  "txt2.txt" 
			if (err) {
				printf("В папке нет требуемого входного ф-ла %s \n", TEXTIN);
				perror("нет требуемого "TEXTIN); 
				system("pause"); 
				exit(1);
			}
			// &&&&&& else continue текст ф из папки нашёлся-открылся начин сепарироват  &&&&&&&
			else {  // "новый" текстовый файл из папки нашёлся-открылся 
				puts("открывается указ-ль FILE *pFtxt ф потока на text00.txt \n");
				//// перенос ф-ла текста в оперативную динам память, для цього ---> ======================
				
				long txtSize = 0;	////~~~~~~~  определяем РАЗМЕР входн ***.txt файла в байтах  -----	 
				fseek(pFtxt, 0, SEEK_END);// уст-ем текущ поз в смещ на 0 относ конца ф-ла
				txtSize = ftell(pFtxt); //в txtSize = ПОЛУЧАЕМ РАЗМЕР В БАЙТАХ
				fseek(pFtxt, 0, SEEK_SET);	// перевести текущую поз на начало файла
				
				printf("Размер памяти входного текста из ф-ла .txt = %d Bytes \n", txtSize);

				//// выделить дин память pmemtxtbuf для хран-я содерж-го из ф-л а =====================
				char *pmemtxtbuf;			//--->  локальн указатель на дин пам. pmemtxtbuf 
											//		для хранения содерж-го из вх-го файла...........
											// копируемого и подлежащего разбиению токенами
				pmemtxtbuf = (char*)malloc(sizeof(char) * (txtSize)+1);
				if (pmemtxtbuf == NULL)
				{
					fputs("Ошибка памяти", stderr);
					exit(2);
				}
				printf("\n~ Д пам. pmemtxtbuf для хранения текста из ф-ла %s = %d byts ~\n\n", TEXTIN, (sizeof(char) * txtSize) + 1);

				//// ----- из pFtxt считываем файл в буфер	pmemtxtbuf!!!  ---------------------
				size_t result = fread(pmemtxtbuf, sizeof(char), txtSize, pFtxt);  // СЧИТЫВАЕМ файл в буфер!!!
				if (result != txtSize)  //если не совпало число считанных байт
				{
					if (feof(pFtxt)) printf("Преждевременное достижение конца файла.\n");
					else printf("Ошибка при чтении файла.\n  result=%d\t txtSize=%d\n", result, txtSize);
					fclose(pFtxt);	//так как был открыт то закрыть файл ввод  входного текста
					system("pause");
					exit(3);
				}
				fclose(pFtxt);	//поработал и закрыл )) файл ввод  входного текста
				pmemtxtbuf[txtSize] = '\0';  //!!!!!!!!!!!!!!!!!!!!!! ИНАЧЕ В КОНЦЕ КРЯКОЗЯБЫ

#ifdef TEXT0//~~~ неразб ткст выв-ся в станд. поток консоли НА ЭКРАН (для  отладки)============
				if (puts(pmemtxtbuf) == EOF) {
					printf("Ошибка при записи из текстов файла \n");
				}  // -> -> -> 
				else {
					printf(" Файл текста  считан-записан в ф-ции main нормально\n");
					printf(" ~~~ !!!!!Текст выше - это содержащийся в файле %s ~~~       \n \
	!!!!!!!!на который указывала ком строка, далее сепарирование \n", TEXTIN);	//  ???*
				}
#endif//~~~~~~~~~~~~~~ TEXT НА ЭКРАН после отл можнои убрать  ~~~~~~~~~~~~~~~

				//2a-a) Начальн самое первое выдел пам *pmemword под и поехали СЕПАР! прост блок ~~~~~
				{
					printf("  Размер памяти под одну структуру %d байт\n", sizeof(struct word));
					amountword = MAX_WORD;  //размер ЗАПИСЕЙ начально выделенн 
					pmemword = (struct word *) malloc(amountword * sizeof(struct word));
					//самое первое выделение памяти под сепарацию
					// и занесения строк в структуры временно - начальное количество MAX_WORD 
					if (pmemword == NULL)printf("Не выделенна память под punsort \n");
					else printf("  Выделенна память punsort = %d Bytes \n  под %d неотсортированных структур \
  и ПОЕХАЛИ! СЕПАРИРОВАТЬ\n",
						MAX_WORD * sizeof(struct word), MAX_WORD);				//    отладка
				}
				pmemword = sepmini2(pmemword, pamountword, pcountnumword, pmemtxtbuf);
				free(pmemtxtbuf);	//освободить пам буфер входн текста

				//======  переименование и запись в файл базу XX_nosort сепар-х но несорт-х структ ===========				
				
				// 2a-b~~ для несортировнного массива преобразов имени XXX_nosort.dat вызовом ф-и rename2()
				char *pnamewordnosort;  //указ д строки для преобраз.rename имя ф "TEXTIN_nosort.dat"
				pnamewordnosort = rename2(TEXTIN, "_nosort.dat", 4);
 
				//2a-c~~~~ базу несортир слов запись в WORD hdd файл(заранее переим)--///////////////  
				
				//writebase2(pFnosort, pnamewordnosort, pmemword, countnumword);
				writehdd(1, pFnosort, pnamewordnosort, sizeof(struct word), countnumword, pmemword);
				puts(pnamewordnosort);		//debug вывод имени .hdd несортированных слов

				//2a-d== изменение и занесение в ф ini "fini.dat" <- ИМЕНИ - XXX_nosort.dat из д п  ====   	
				
				//~~~~  Новая запись в поле дин пам pFini <- ИМЕНИ - XXX_nosort.dat ~~~~~~~~~
				{pmemini->idname = 0;  // ?????
				strncpy(pmemini->ininamenosortf, pnamewordnosort, EN1);
				}
				// запись  ф INI (имени "XXX_nosort.dat")
				writehdd(0, pFini, "fini.dat", sizeof(struct inidat), 1, pmemini);

				// end 2) если разбить НОВЫЙ текст
			}     //   end текстовый файл из папки открылся, отсепар-ся и запис-ся в pFnosort

// имеем  НОВЫЙ БАЗОВЫЙ ФАЙЛ сепарированный без сортировки 

// создаём НОВЫЙ БАЗОВЫЙ ФАЙЛ разбитого с АЛФАВИТНОЙ СОРТИРОРВКОЙ

			//2a-e т е это будет БАЗОВЫЙ ФАЙЛ разбитого с АЛФАВИТНОЙ СОРТИРОРВКОЙ
			// Выделение д памяти pmemsortword ПОД СОРТИРОВАННЫЙ МАССИВ [pcountnumword] =====
			pmemsortword = (struct word *) malloc((*pcountnumword) * sizeof(struct word));
			//pmemsortword-глоб указ = выделение д пам стрктур под сортировку слов
			if (pmemsortword == NULL)printf("Не выделенна память под pmemsortword \n");
			else printf("  Выделенна память psort = %d Bytes \n  под %d сортированных структур \
  и поехали! сортировать\n",(*pcountnumword) * sizeof(struct word), (*pcountnumword));

			//=== pmemsortword <- pmemword Перенос в эту д память  структур ============
			int temp = 0;
			for (temp = 0; temp < *pcountnumword; temp++)
			{
				*(pmemsortword + temp) = *(pmemword + temp); //копирование
#ifdef NO_SORT 
				printf("m_sort %d - %s \n", temp, pmemsortword[temp].en);  // отладка
#endif
			}
			// надов конце блока free(pmemword);  // освободить д память pmemword
			printf("Структуры скопированы в нов массb для ???-ной сортировки --> \n");

			// 2a-f)  БАЗОВАЯ алфавитн сортировка
			int(*pfTemp) = measurealph; //указ на функц алфавитн сортировки
			int disloc = 0;  //            далее Сортировка id по разным критериям
			pmemsortword = idsort(pmemsortword, pcountnumword, pfTemp, disloc);
			pmemsortword = reduct3(pmemsortword, pcountnumword);
			//!!!!! теперь в д п pmemsortword алф сортированный массив слов !!!!!!!!!

			//~~~~ После алф сорт преобраз-ние имени в XXX_alphsort.dat и запись в ini  ======
			char *pnamesortword = NULL;  //укз дин строка c преобраз-ным имя ф "XXX_sort.dat"
			pnamesortword = rename2(TEXTIN, "_alphsort.dat", 4);  // д п выдел ф rename2()
			puts(pnamesortword);

			////////// Далее запись в ф "text00_alphsort.dat" БАЗУ СЛОВ pmemsortword по алфавиту
			////////writebase2(pFsort, pnamesortword, pmemsortword, countnumword);

			//2a-g)~~~ запись в нов hdd файл(заранее переим) WORD базу алф-сортир структур ---
			writehdd(1, pFnosort, pnamesortword, sizeof(struct word), countnumword, pmemsortword);
		
			//2a-h) запись старый ф INI (имени "XXX_alphsort.dat")
			strncpy(pmemini->ininamealphsortf, pnamesortword, EN1);//зап поля в дин пам INI
			writehdd(0, pFini, "fini.dat", sizeof(struct inidat), 1, pmemini);
			
			//,,,,,,,,,, Temp отладка print ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
			printf("This \"Print\" create alphabetwordbase inside to engl17.c after idsort() reduct3() - \n");
			int m;    // Temp отладка
			for (m = 0; m < *pcountnumword; m++)
			{
				//pmemsortword[m].repeat = 0;
				printf(" _ %3d.  alphabet_id=%3d id=%3d  _( %s ) [repeat= %d ]   \n", m, pmemsortword[m].repeat_id, pmemsortword[m].id, pmemsortword[m].en, pmemsortword[m].repeat);    // temp
			}

			free(pmemword);  // освободить д память pmemword c несортированными словами
							 //!!!!! алф сортированный массив слов в д п pmemsortword !!!
							 //!!!!! алф сортированный массив слов в ф text00_alphsort.dat 
		}  // end открытие и сортировка нового входного text00.txt файла ,,,,,,,,,,,,,,,,


//________________________________________________________________________________________

//_  2b) continue &&&& ЕСЛИ СЧИТЫВАТЬ И РАБ С СТАРЫМ ТXT ФАЙОМ  &&&&&

//из INI узнать 

		else	//	2b) ~~~ ЕСЛИ ОТКРЫВАТЬ НЕ НОВ ТЕКСТ ФАЙЛ А ПЕРЕОТКРЫТЬ СТАРЫЙ ~~~~
		{
			// открыть ф ini и считать из него ИМЯ уже ранее алф-сортированной базы ??????
			// и считать из него тип сортировки
			err = fopen_s(&pFini, "fini.dat", "r+b");//XXX_alphsort.dat сохр в ф-л "fini.dat"
			if (err)
			{
				puts("\n Ошибка! \n Неудача отытия ранее созданного ф-ла имён пользователя \n");
				//????????????????????
				//считать из д памяти pmemini->ininamenosortf <- ИМЕНИ - XXX_alphsort.dat 
				//size_t tlen = strlen(pmemini->ininamealphsortf);
				// memset(pmemini->ininamealphsortf, NULL, tlen);
				system("pause");
				exit(1);
			}
			else  // &&&&&&&&&&&&&&&  РАБ С д пам из СТАРОГО ini Ф  &&&&&&&&&&&&
			{
				size_t result = fread(pmemini, sizeof(struct inidat), QUANTITYNAME, pFini);
				// теперь имеем переносены настройки из ini файла в дин память pmemini 
				//из pmemini->sorttype видим какая была ранее выбрана сортировка
				switch (pmemini->sorttype)
			{
			case 0:// 2b1) считываем в д пам АЛФ сорт файл
				err = fopen_s(&pFsort, pmemini->ininamealphsortf, "r+b");//XXX_alphsort.dat сохр в ф-л "fini.dat"
				if (err) //   a)
				{
					puts("\n Ошибка! \n Неудача отытия ранее созданного ф-ла алфавитно-сортированных слов \n");
					system("pause");
					exit(1);
				}
				else   //	b) Удачно откр-ся и далее считываем в д пам алф сорт файл
				{
					long fileSize = 0;	////~~~~~~~  определяем РАЗМЕР входн ***.txt файла в байтах  -----	 
					fseek(pFsort, 0, SEEK_END);// уст-ем текущ поз в смещ на 0 относ конца ф-ла
					fileSize = ftell(pFsort); //в txtSize = ПОЛУЧАЕМ РАЗМЕР В БАЙТАХ
					fseek(pFsort, 0, SEEK_SET);	// перевести текущую поз на начало файла

					printf("Размер д пам для алфав-сортированной базы из ф-ла = %d Bytes \n", fileSize);
					//выделяем эту д память под алф-сорт
					pmemsortword = (byte*)malloc(fileSize);
					if (pmemsortword == NULL)
					{
						printf("Не выделенна память под pmemsortword \n");
						system("pause");
						exit(1);
					}
						
					size_t result = fread(pmemsortword, sizeof(byte), fileSize, pFsort);
					// алф сортиров-й массив слов считан повторн из ф в д п pmemsortword 
					puts("\n Алфавитно сортированный файл считан повторно! \n  \n");
				}
				fclose(pFsort);
			
				break;
			case 1: // 2b2) считываем в д пам ЧАСТОТНО сорт файл
				err = fopen_s(&pFfreqsort, pmemini->ininameafreqsortf, "r+b");//XXX_freqsort.dat 
				if (err) //   a)	Неудача отытия ранее созданного XXX_freqsort.dat
				{
					puts("\n Ошибка! \n Неудача отытия ранее созданного ф-ла частотно-сортированных слов \n");
					system("pause");
					exit(1);
					
					
				}
				else   //	 Успешн откр и Считывание существ ранее частот-сортир файла
				{
					//!!!! !!!! !!!! Считывание частотно сортированного файла !!!!
					long fileSize = 0;	////~~~~~~~  определяем РАЗМЕР входн ***.txt файла в байтах  -----	 
					fseek(pFfreqsort, 0, SEEK_END);// уст-ем текущ поз в смещ на 0 относ конца ф-ла
					fileSize = ftell(pFfreqsort); //в txtSize = ПОЛУЧАЕМ РАЗМЕР В БАЙТАХ
					fseek(pFfreqsort, 0, SEEK_SET);	// перевести текущую поз на начало файла

					printf("Размер д пам для частотно-сортированной базы из ф-ла = %d Bytes \n", fileSize);
					//выделяем эту д память под частот-сорт
					pmemsortword = (byte*)malloc(fileSize);
					if (pmemsortword == NULL)
					{
						printf("Не выделенна память под pmemsortword \n");
						system("pause");
						exit(1);
					}

/*
//switch (pmemini->sorttype)



*/
					size_t result = fread(pmemsortword, sizeof(byte), fileSize, pFfreqsort);//???????pFfreqsort 
					// частотн сортиров-й массив слов считан повторн из ф в д п pmemsortword 
					puts("\n Алфавитно сортированный файл считан повторно! \n  \n");
					
				}// end	b) Считывание частотно сортированного файла
				 //возможно надо закрыть считанный файл по указателю pFfreqsort ???
				fclose(pFfreqsort);
				break;
			}// нужной сортировки массив слов считан повторн из ф в д п pmemsortword
				// теперь в pmemsortword находится массив структур нужной сортировки
			}	// end else
			// в зависимости от типа сортировки считать алфав или частотн базу слов......
									
		}  // end 2b) else ЕСЛИ СЧИТЫВАТЬ И РАБ С СТАРЫМ ТXT ФАЙЛОМ  
// Конец выбора стар или нов txt. Массив слов нужной сорт-ки находится в д п pmemsortword ____

//=====|||||=====||||||=====||||||======||||||=======|||||||===============================

// здесь if( наж Y если изменить ТОЛЬКО НАСТРОЙКИ для изучения открытого текста)
		puts("\n 3) если ТОЛЬКО НОВЫЕ НАСТРОЙКИ ?  - тогда нажмите 'y'\n");
		if ('y' == _getch(stdin))//  вносить изменения в настройки урока ============
		{
			// есть алф или друг сорт текст из txt файла  ininamenosortf[EN1] 
			// ! он уже есть в pmemsortword ??????????????????????????????????????
			//  настройки нах-ся в pmemini из ini ф надо запросить нов настр вручную
			//РУЧНОЙ ВЫБОР СОРТИРОВКИ===============================================================
			//далее вызов АЛФАВИТНОЙ или другой сортировки  =======
			// созд указат на ф-ю сортировки

			puts("\n если Алфавитная сортировка  ?  - тогда нажмите '1'\n \
    Частотная сортировка - тогда нажмите  '2' \n");

			while ((ttime--) >= 0) // (DELAYKEY)
			{
				Sleep(500);  // задержка мс
				flagkey = 1; // значение по концу времени
				if (_kbhit())  //--- ф-я видит любое наж клав
				{
					flagkey = _getch();
					break;
				}
			}  //далее алф сорт
			
			if (flagkey == '1')//  Алфавитная сортировка - настройки урока ============
			{
				int(*pfTemp) = measurealph;
				int disloc = 0;  // далее Сортировка id по разным критериям 
				pmemsortword = idsort(pmemsortword, pcountnumword, pfTemp, disloc);
				//  pmemsortword = reduct3(pmemsortword, pcountnumword);

				printf("This \"Print\" inside настр урока engl17.c after idsort() reduct3() - \n");
				int m;
				for (m = 0; m < *pcountnumword; m++)
				{
					//pmemsortword[m].repeat =  0;
					printf(" _ %3d.  alphabet_id=%3d id=%3d  _( %s ) [repeat= %d ]   \n", m, pmemsortword[m].repeat_id, pmemsortword[m].id, pmemsortword[m].en, pmemsortword[m].repeat);    // temp
				}

				// Записать в файл XXX_alphsort.dat !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				writebase2(pFsort, pmemini->ininamealphsortf, pmemsortword, countnumword);


				// printf("\n Отсортированных англ слов =  и т д \n ");
				printf("\n ~~~ Отсортированных англ слов =  и т д \n ");
				printf("\n ~~~ Всего в тексте англ слов =  и т д \n ");
				printf("\n ~~~ Уже изученных из них англ слов =  и т д \n ");

			}	//end вызов АЛФАВИТНОЙ сортировки

			if (flagkey == '2')//  Частотная сортировка - настройки урока ============
			{
				int(*pfTemp) = measurerepeatalph;
				int disloc = 0;  // далее Сортировка id по разным критериям 
				pmemsortword = idsort(pmemsortword, pcountnumword, pfTemp, disloc);
				//  pmemsortword = reduct3(pmemsortword, pcountnumword);

				// Записать в файл XXX_freqsort.dat !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


				//  flagtext ????????????????????
 				writebase2(pFfreqsort, pmemini->ininameafreqsortf, pmemsortword, countnumword);

				printf("This \"Print\" inside настр урока engl17.c after measurerepeatalph() - \n");
				int m;
				for (m = 0; m < *pcountnumword; m++)
				{
					//pmemsortword[m].repeat =  0;
					printf(" _ %3d.  alphabet_id=%3d id=%3d  _( %s ) [repeat= %d ]   \n", m, pmemsortword[m].repeat_id, pmemsortword[m].id, pmemsortword[m].en, pmemsortword[m].repeat);    // temp
				}
				// printf("\n Отсортированных англ слов =  и т д \n ");
				printf("\n ~~~ Отсортированных англ слов =  и т д \n ");
				printf("\n ~~~ Всего в тексте англ слов =  и т д \n ");
				printf("\n ~~~ Уже изученных из них англ слов =  и т д \n ");

			}//   end  Частотная сортировка - настройки урока
		}	//end 3) если вносить изменения в настройки урока старую (уже существ-ую) базу слов  

/*
//     ____________________________  БУДЕТ ПОВТОРЕНИЕ ???? _________________
// Перед сортировкой преобразование имени в XXX_sort.dat  =====================
			char *pnamesortword = NULL;  // указат на дин строка-имя  файла "argv[1]_sort.dat"
			pnamesortword = rename2(TEXTIN, "_sort.dat", 4);  // д п выдел ф rename2()
			puts(pnamesortword); 		
// Выделение д памяти pmemsortword под сортированный массив [pcountnumword]  ===========
			pmemsortword = (struct word *) malloc((*pcountnumword) * sizeof(struct word));
			//pmemsortword-глоб указ = выделение д пам стрктур под сортировку слов
			if (pmemsortword == NULL)printf("Не выделенна память под pmemsortword \n");
			else printf("  Выделенна память psort = %d Bytes \n  под %d сортированных структур \
  и поехали! сортировать\n",
				(*pcountnumword) * sizeof(struct word), (*pcountnumword));
//Перенос в эту д память pmemsortword структур из pmemword  =============
			int temp = 0;
			for (temp = 0; temp < *pcountnumword; temp++)
			{
				*(pmemsortword + temp) = *(pmemword + temp); //копирование
#ifdef NO_SORT 
				printf("m_sort %d - %s \n", temp, pmemsortword[temp].en);  // отладка
#endif
			}
			printf("Структуры скопированы в нов массb для алфавитной сортировки --> \n");
*/
			else  // else - нет, старые настройки сортировки ????????????????????????
			{
				
			}	// end - нет, старые настройки сортировки

// далее - Сортировка по укаазанным в ini дин памяти настройкам


	}	// end " if ЕСЛИ ХОТИТЕ СДЕЛАТЬ ПО НОВОМУ УРОК"
// end " if ЕСЛИ ХОТИТЕ СДЕЛАТЬ ПО НОВОМУ УРОК"

//=============================================================================================

// &&&&&&&&&&&&  else   continue  " ОСТАВИТЬ ОТКРЫВАВШИЙСЯ УРОК КАК ЕСТЬ" &&&&&&&&&&&
	else// 1b) ПРОДОЛЖАЕМ СТАРЫЙ УР т е извлекаем стар базу слов и старые настр. fini.dat
	{  // 1b) извлекаем настр. стар fini.dat и работаем с уже ранее открывавшимся уроком и 
		// ранее первично указанно тип сорт в pmemini->sorttype  
		puts("\n извлекаем настройки старого fini.dat_____ ");
		puts(" работаем с отсортированным ранее текстом_____ \n");
		
	} //end else "Продолжаем старый ур "
	  //end if else "оставить урок КАК ЕСТЬ ИЛИ СДЕЛАТЬ ПО НОВОМУ"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	  printf(" ~~~ pmemini->sorttype = %d \n", pmemini->sorttype);
	  // незаполнено name !!!!  
	  printf("\n ~~~ pmemini->name[] = %s \n", pmemini->name);
	  printf("\n ~~~ Отсортированных англ слов countnumword = %d и т д \n ", countnumword);
	  printf("\n ~~~ Всего в тексте англ слов =  %d и т д \n ", countnosort);
	  printf("\n ~~~ Уже изученных из них англ слов =  и т д \n ");
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	free(pmemini); free(pmemsortword); printf("\n\n  The END!   -     конец урока! \n\n\n");
	system("pause");
 }  //end main

 //  синхронизация ??????????
 //  синхронизация ??????????

