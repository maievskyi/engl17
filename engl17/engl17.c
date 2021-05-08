
//22/04/21 первый  вариант с передачей фиксиров. имени текста без ком стр
// 


#define _CRT_SECURE_NO_WARNINGS  //   ?? подробнее об предупреждениях ??
//#define VARTOKEN nodebug
//#define RENAME ok
//#define ALPHABET ok
#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include"engl17.h"  //


FILE *pFtxt; //---> текст который читать - argv[1] первый аргумент ком строки "txt2.txt"
FILE *pFini;	//---> указатель на структ. файл в котором сохранять файлов fini.dat
FILE *pFnosort;	//---> указатель на структ. ф в котор сохр несорт базу слов - argv[1]_nosort.dat"
FILE *pFsort;	//---> указатель на структ. ф в котором сохр сорт базу слов - argv[1]_sort.dat"

char *pnamenosort;		//-->указат на имя ф-ла с запис несорт масс стр 
struct word *pmemword;		//-->глоб указат в main()на первичное выделеие ДИН памяти 
							//под МАССИВ СТРУКТУР (word) для отсепарирования token()
							//далее память будет перерасширятся по этому указателю
struct word *pmemsortword;  //--> указ на д пам стрктур с отсортированными словами
struct word *pmemalphabetword;  //--> указ на д пам стр-р с отсорт-ми и сокращенными словами
long amountmem = 0;		//---> РАЗМЕР в байтах дин пам выде-мых под структуры word
long *pamountmem;		//--->укз РАЗМ в б-тах ДИН пам выде-мых под несорт структ word
int countnumword = 0;	//---счётчик инкремента слов а значит и стр-р при сепар-и
int *pcountnumword;		//--->указатель на счетч слов
struct inidat *pmemini;	//--->указ на ДИН пам с стр-й базы ini имён прог-мы
int flagtext = NEWTEXT;	//переменная -ФЛАГ режима работы со стар или нов текстом
						//т е будет ли сепарироваться по новой и писаться в нофую базу слов 

						//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%       main     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
int main(int argc, const char ** argv, const char** env)
{

	system("chcp 1251 > nul");
	errno_t err;			//  переменная (int?) для вывода ошибок ? внутр переменная (int?)


							// есть ли fini.dat  ==============================================================
							// открытие=проверка сущ-вания ini файла пользов настр "fini.dat"__________________		
	err = fopen_s(&pFini, "fini.dat", "r+b");// открывается ли на чтение с дозаписью
	if (err)	// значит не открылся fini.dat								
	{
		perror("fini.dat");
		puts("\n !!! ранее НЕ существовал ini файл пользователя  \n");
		system("pause");

		//// далее созд-ся д пам pmemini, заполн по ум-нию, откр pFini и зап д пам в ф поток

		pmemini = (struct inidat*)malloc(sizeof(struct inidat)); //созд д пам
		if (pmemini == NULL)printf("Не выделена память ini настройки программы \n");
		else printf("  Выделена дин пам din1name = %d Bytes \n\
 под ini структуру-настройки программы \n",
			sizeof(struct inidat));			// debug

											//и !!!! дальнейшие настроек дополнительно к настр по умолчани
		memset(pmemini, NULL, sizeof(struct inidat)); //    заполн. нулями

													  // открывается нов fini.dat на запись-чтение
		err = fopen_s(&pFini, "fini.dat", "w+b");
		if (err)	// значит нет fini.dat								
		{
			puts("\n !!! не создался ini файл пользователя \n");
			perror("fini.dat");
			system("pause");
		}
		else
		{
			puts("\n +++  создался НОВЫЙ ini файл пользователя \n");	//debug
		}
		//запись  содержимого дин память pmeminidat в ф fini.dat (?надоли обн  = 0) ============
		size_t result = fwrite(pmemini, sizeof(struct inidat), QUANTITYNAME, pFini);
		puts("\n Записан на HDD Новый файл пользователя \"fini.dat\" \n");

		fclose(pFini);	// закрыть файловый поток из которого читаются имена файлов
		free(pmemini);	// освободить память pmemini
		system("pause");
	} // end if "есть ли fini.dat" ...................................................
	  /*else   // значит есть fini.dat  ____________________________________________
	  {
	  }*/

	  // хотите ли изменить настройки урока =========================================
	puts("\n если хотите изменить настройки урока ?  - тогда нажмите 'y'  \n");
	if ('y' == getch(stdin))//  ==================================================
	{
		puts("\n создаём другой fini.dat и  пробуем открыть text00.txt _____ \n");
		// открытие входного text00.txt файла =======================================
		err = fopen_s(&pFtxt, TEXTIN, "rb");// инициал-тся указ=ль FILE *pFtxt 
											//T  и открывается в режиме (rb)-  "txt2.txt" 
		if (err) {
			printf("В папке нет требуемого входного ф-ла %s \n", TEXTIN);
			perror("нет требуемого "TEXTIN); system("pause"); exit(1);
		}
		else {
			puts("открывается указ-ль FILE *pFtxt ф потока на text00.txt \n");

			//~~~~~~~  определяем РАЗМЕР входн ***.txt файла в байтах  ---------------
			long txtSize = 0;	//--- размер в байтах файла котор будет считан в дин память
								// устанавливаем текущ позицию в конец файла, т е (смещ на 0 относ конца ф-ла)	 
			fseek(pFtxt, 0, SEEK_END);
			txtSize = ftell(pFtxt); //в txtSize = ПОЛУЧАЕМ РАЗМЕР В БАЙТАХ

			fseek(pFtxt, 0, SEEK_SET);	// перевести текущую поз на начало файла

			printf("Размер памяти входного текста из ф-ла .txt = %d Bytes \n", txtSize);

			//// перенос ф-ла текста в оперативную динам память, для цього ---> ======================
			// созд-ся д пам pmemini, счит из ф поток и дополняем настр к умолч-ю 
			//потом записываем в ф поток 

			pmemini = (struct inidat*)malloc(sizeof(struct inidat)); //созд д пам
			if (pmemini == NULL)printf("Не выделена память ini настройки программы \n");
			else
			{
				// СЧИТЫВАЕМ INI файл в д пам pmemini!!!
				size_t result = fread(pmemini, sizeof(struct inidat), QUANTITYNAME, pFini);
				fclose(pFini);  // cчитал и закрыл ф-л ))
				printf("  Выделена дин пам din1name = %d Bytes \n\
 под ini структуру-настройки программы и заполнена настр из ini файла\n",
					result);			// debug sizeof(struct inidat));
				printf("  Выделена дин пам din1name = %d Bytes \n\
 под ini структуру-настройки программы и заполнена настр из ini файла\n",
					sizeof(struct inidat));			// debug sizeof(struct inidat));
			}
			

			//~~~~~~  выделение дин памяти буфеp *pmemtxtbuf---------------------------------
			char * pmemtxtbuf;			//--->  локальн указатель на дин пам. pmemtxtbuf 
										//		для хранения содерж-го из вх-го файла...........
			pmemtxtbuf = (char*)malloc(sizeof(char) * txtSize); // выделить дин память pmemtxtbuf для хран-я содерж-го из ф-л а
																// далее подлежащего разбиению токенами
			if (pmemtxtbuf == NULL)
			{
				fputs("Ошибка памяти", stderr);
				exit(2);
			}
			printf("~~ выделена дин пам. pmemtxtbuf для хранения текста из файла %d ~~\n", *pmemtxtbuf);
			// ------из pFtxt считываем файл в буфер	pmemtxtbuf!!!------------------------------------
			size_t result = fread(pmemtxtbuf, sizeof(char), txtSize, pFtxt);  // СЧИТЫВАЕМ файл в буфер!!!
			if (result != txtSize)  //если не совпало число считанных байт
			{
				if (feof(pFtxt)) printf("Преждевременное достижение конца файла.\n");
				else printf("Ошибка при чтении файла.\n");
				system("pause");
				exit(3);
			}
			fclose(pFtxt);	//поработал и закрыл )) файловый ввод из которого читается входной текст

#ifdef TEXT	//~~~~~ текст выв-ся в станд. поток вывода консоли НА ЭКРАН (для  отладки)============ =======
			if (puts(pmemtxtbuf) == EOF) {
				printf("Ошибка при записи из текстов файла \n");
			}  // -> -> -> 
			else {
				printf("Файл текста  считан-записан в ф-ции main нормально\n");
				printf("		~~~ !!!!!Текст выше - это содержащийся в файле %s ~~~       \n \
	!!!!!!!!на который указывала ком строка, далее сепарирование \n", argv[1]);	//  ???*
			}
#endif//~~~~~~~~~~~~~~~~~~~~~ после отл можнои убрать  ~~~~~~~~~~~~~~~

			//~~~~ Начальное самое первое выдел пам *pmemword под сеп и поехали! прост блок ==============
			{
				printf("  Размер памяти под одну структуру %d байт\n", sizeof(struct word));
				amountmem = MAX_WORD * sizeof(struct word);  //размер (байт) начально выд-мой памяти 
				pmemword = (struct word *) malloc(amountmem);   //самое первое выделение памяти 
																//  под сепарацию и занесения строк в структуры 
																//временно - начальное количество MAX_WORD 
				if (pmemword == NULL)printf("Не выделенна память под punsort \n");
				else printf("  Выделенна память punsort = %d Bytes \n  под %d неотсортированных структур \
  и ПОЕХАЛИ! сепарировать\n",
					MAX_WORD * sizeof(struct word), MAX_WORD);				//    отладка
			}

			//~~~~~~~~~    далее (подготовка аргументов?) вызов ф-ции сепаррования - sepmini() ------   
			pamountmem = &amountmem;  // указ на РАЗМ дин пам БАЙТ для сепарир стр-р(пока= 8 стр)
			pcountnumword = &countnumword; //указ на счётч инкр-та СЛОВ = СТРУКТ при сепар-и 
										   // pmemword - указ на МАССИВ СТРУКТУР (word) для отсепарирования token()
										   //pmemtxtbuf - указ на дин массив неразбитого текста - копии входн файла
										   //argv[1] - из ком строки имя для отладки printf

										   // ВЫЗОВ СЕПАРИРОВАНИЯ  long amountword = *pcountnumword / sizeof(struct word); 
			pmemword = sepmini(pmemword, pamountmem, pmemtxtbuf, pcountnumword, TEXTIN);

			free(pmemtxtbuf);

			//===~~~~~~~~  далее запись в файл базу WORD_nosort сепарированных но несортированных структур ===========				

			//---~~~~~~ для несортировнного массива преобразов имени XXX_nosort.dat вызовом ф-и rename2()
			char *pnamewordnosort;  //указ д строки для преобраз.rename имя ф "argv[1]_nosort.dat"
			{	pnamewordnosort = rename2(TEXTIN, "_nosort.dat", 4);
			}
			//~~~~~~~~~~~~ запись в WORD hdd файл(заранее переим) базу несортир структур ---///////////////  
			writebase2(pFnosort, pnamewordnosort, pmemword, countnumword);//
																		  //pnosortFile - указ на откр внутр ф-ции hdd файл в котором сохранять базу слов 
																		  //pnamewordnosort - уже сформированное ранее имя ф-ла для hdd ("argv[1]_nosort.dat")
																		  // pmemword - указ на дин массив НЕСОРТ структур, 
																		  // countnumword - число несорт структур
																		  //,?? возврат указ имя файла с  структурами ( ----- )???? 
			puts(pnamewordnosort);		//debug вывод имени .hdd несортированных слов

										//~~~~~~~~~~  занесение в ф ini "fini.dat" <- ИМЕНИ XXX_nosort.dat из дин памяти  ~~~~~~~~   	

										//~~~~~~~  сначала изменение в дин пам pFini <- ИМЕНИ  XXX_nosort.dat ~~~~~~~~~
			{pmemini->idname = 0;
			strncpy(pmemini->ininamenosortf, pnamewordnosort, EN1);
			}

			//~~~~~~~~~~  запись в ф ini "fini.dat" <- ИМЕНИ XXX_nosort.dat из дин памяти  ~~~~~~~~   	
			err = fopen_s(&pFini, "fini.dat", "r+b");//XXX_nosrt.dat сохр в ф-л "fini.dat"
			if (err)
			{
				puts("\n Ошибка! \n Неудача отытия ранее созданного ф-ла имён пользователя \n");
				system("pause");
				exit(1);
			}
			fwrite(pmemini, sizeof(struct inidat), QUANTITYNAME, pFini);//fini.dat
			fclose(pFini);	//поработал и закрыл )) 

							//=========================================================================================================
							//=========================================================================================================
		};  // end открытие входного text00.txt файла ................................




	}	// end "если хотите изменить настройки урока ? - нажмите 'y'"...............
	else
	{  // извлекаем настройки старого fini.dat_______________________________
		puts("\n извлекаем настройки старого fini.dat_____ \n");


	} //end "хотите ли создать другой fini.dat"

	free(pmemini);
	
	printf("\n\n             The END!   -     конец урока! \n\n\n");
	system("pause");
}  //end main