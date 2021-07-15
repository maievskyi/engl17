
//22/04/21 первый  вариант с передачей фиксиров. имени текста без ком стр
// 


#define _CRT_SECURE_NO_WARNINGS  //   ?? подробнее об предупреждениях ??
//#define VARTOKEN nodebug
//#define RENAME ok
//#define ALPHABET ok
#define M_SORT
//#define NO_SORT
#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include"engl17.h"  //


FILE *pFtxt; //---> текст который читать - argv[1] первый аргумент ком строки "txt2.txt"
FILE *pFini;	//---> указатель на структ. файл в котором сохранять файлов fini.dat
FILE *pFnosort;	//---> указатель на структ. ф в котор сохр несорт базу слов - argv[1]_nosort.dat"
FILE *pFsort;	//---> указатель на структ. ф в котором сохр сорт базу слов - argv[1]_sort.dat"

char *pnamenosort = NULL;		//-->указат на имя ф-ла с запис несорт масс стр 
struct word *pmemword = NULL;		//-->глоб указат в main()на первичное выделеие ДИН памяти 
							//под МАССИВ СТРУКТУР (word) для отсепарирования token()
							//далее память будет перерасширятся по этому указателю
struct word *pmemsortword = NULL;  //--> указ на д пам стрктур с отсортированными словами
struct word *pmemalphabetword = NULL;  //--> указ на д пам стр-р с отсорт-ми и сокращенными словами
int amountword = 0;		//---> РАЗМ дин пам В ЗАПИСЯХ  под структуры word
int *pamountword = NULL;		//---> указ на РАЗМ дин пам В ЗАПИСЯХ  под структуры word
int countnumword = 0;	//---счётчик инкремента слов а значит и стр-р при сепар-и
int *pcountnumword = NULL;		//--->указатель на счетч слов
struct inidat *pmemini = NULL;	//--->указ на ДИН пам с стр-й базы ini имён прог-мы
int flagtext = NEWTEXT;	//переменная -ФЛАГ режима работы со стар или нов текстом
						//т е будет ли сепарироваться по новой и писаться в нофую базу слов 

						//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%       main     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
int main(int argc, const char ** argv, const char** env)
{

	system("chcp 1251 > nul");
	errno_t err;			//  переменная (int?) для вывода ошибок ? внутр переменная (int?)
	//pamountword = &amountword;  !!!!!!!!!!!!!!!!
	//pcountwordlok = &countnumword;
	pamountword = &amountword;  // указ на РАЗМ дин пам В ЗАПИСЯХ для сепарир стр-р(пока= 8 стр)
	pcountnumword = &countnumword; //указ на счётч инкр-та СЛОВ = СТРУКТ при сепар-и 

/////////////////////////////////////////////////////////////////////////////////
//// созд-ся д пам pmemini, заполн по ум-нию, откр pFini и зап д пам в ф поток
	pmemini = (struct inidat*)malloc(sizeof(struct inidat)); //созд д пам
	if (pmemini == NULL)printf("Не выделена память ini настройки программы \n");
	else printf("  Выделена дин пам din1name = %d Bytes \n\
 под ini структуру-настройки программы \n",
		sizeof(struct inidat));			// debug

	//и !!!! дальнейшие настроек дополнительно к настр по умолчани
	//  пока  заполн. нулями, далее стандартн настройки
	//	ТУТ позже взамен 0 - стандартн настройки
	memset(pmemini, '\0', sizeof(struct inidat)); 

							
//// есть ли fini.dat  ==============================================================
//// открытие=проверка сущ-вания стар ini файла пользов настр "fini.dat"==================		
	err = fopen_s(&pFini, "fini.dat", "r+b");// открывается ли на чтение с дозаписью
	if (err)	// значит не открылся стар fini.dat								
	{
		perror("fini.dat");
		puts("\n !!! ранее НЕ существовал ini файл пользователя  \n");
		system("pause");

		

	//// открывается НОВЫЙ fini.dat на запись-чтение
		err = fopen_s(&pFini, "fini.dat", "w+b");
		if (err)	// значит почемуто всётаки не создался fini.dat								
		{
			puts("\n !!! не создался новый ini файл пользователя \n");
			perror("fini.dat");
			free(pmemini);				//освоб памяти
			system("pause");
			exit(1);					//урок сорвался
		}
		else
		{
			puts("\n +++  создался НОВЫЙ ini файл пользователя \n");	//debug
		}
	////запись  содержимого дин память pmeminidat в ф fini.dat (?надоли обн  = 0) ========
		size_t result = fwrite(pmemini, sizeof(struct inidat), QUANTITYNAME, pFini);
		puts("\n Записан на HDD Новый файл пользователя \"fini.dat\" \n");
		//
		fclose(pFini);	// закрыть ф-й поток в который записаны настройки ini по умолчанию
		free(pmemini);	// освоб память pmemini   ???? а надо ли ведь всё равно перевыделять
		//
		err = fopen_s(&pFini, "fini.dat", "r+b");// ПЕРЕОТКРЫВАЕТСЯ ли на чте с дозаписью
		if (err)	// значит не открылся fini.dat								
		{
			perror("fini.dat");
			puts("\n !!!!! НЕ переоткрылся на чт с дозап ini файл пользователя   \n");
			system("pause");
		}
		else
		{
			puts("\n Переоткрылся на чт с дозап ini файл пользователя с стандартн настр  \n");
		}

		system("pause");
	} // end if "есть ли  стар fini.dat r+b" = нет  ,,,,,,,,,,,,,,,,,,,,,,,,
	else
	{
// вывод имени старого уже разбитого текста


	}// end if else"есть ли  стар fini.dat r+b" = да  ......................................

	  // значит есть УЖЕ ПО ЛЮБОМУ открытый fini.dat  ____________________________________________
	// тут надо бы секунд на 5 вывести на экран имеющиеся настройки pmemini
	//// 1-й оставить урок КАК ЕСТЬ ИЛИ ИЗМЕНИТЬ ТЕКСТ или НАСТРОЙКИ для нового урока =======
	puts("\n если хотите изменить текст или настройки урока   ?  - тогда нажмите 'y'  \n");
	if ('y' == getch(stdin))//  ==================================================
	{
		//если разбить новый текст ? - тогда нажмите 'y' откр-е нового входн text00.txt файла
		puts("\n если разбить НОВЫЙ текст ?  - тогда нажмите 'y'\n");
		if ('y' == getch(stdin))//  открытие нового входного text00.txt файла ==============
		{
			puts("\n пробуем открыть text00.txt и создаём другой fini.dat  _____ \n");
			//// открытие входного text00.txt файла =======================================
			err = fopen_s(&pFtxt, TEXTIN, "rb");// инициал-тся указ=ль FILE *pFtxt 
												//T  и открывается в режиме (rb)-  "txt2.txt" 
			if (err) {
				printf("В папке нет требуемого входного ф-ла %s \n", TEXTIN);
				perror("нет требуемого "TEXTIN); system("pause"); exit(1);
			}
			else {  //текстовый файл из папки открылся
				puts("открывается указ-ль FILE *pFtxt ф потока на text00.txt \n");
				//// перенос ф-ла текста в оперативную динам память, для цього ---> ======================

				long txtSize = 0;	////~~~~~~~  определяем РАЗМЕР входн ***.txt файла в байтах  -----	 
				fseek(pFtxt, 0, SEEK_END);// уст-ем текущ поз в конец ф-ла, и смещ на 0 относ конца ф-ла
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

				//// ------из pFtxt считываем файл в буфер	pmemtxtbuf!!!------------------------------------
				size_t result = fread(pmemtxtbuf, sizeof(char), txtSize, pFtxt);  // СЧИТЫВАЕМ файл в буфер!!!
				if (result != txtSize)  //если не совпало число считанных байт
				{
					if (feof(pFtxt)) printf("Преждевременное достижение конца файла.\n");
					else printf("Ошибка при чтении файла.\n  result=%d\t txtSize=%d\n", result, txtSize);
					system("pause");
					exit(3);
				}
				fclose(pFtxt);	//поработал и закрыл )) файл ввод  входного текста
				pmemtxtbuf[txtSize] = '\0';  //!!!!!!!!!!!!!!!!!!!!!!!!!!! иначе в конце крякозябы

#ifdef TEXT	//~~~~~ текст выв-ся в станд. поток вывода консоли НА ЭКРАН (для  отладки)============ =======
				if (puts(pmemtxtbuf) == EOF) {
					printf("Ошибка при записи из текстов файла \n");
				}  // -> -> -> 
				else {
					printf(" Файл текста  считан-записан в ф-ции main нормально\n");
					printf(" ~~~ !!!!!Текст выше - это содержащийся в файле %s ~~~       \n \
	!!!!!!!!на который указывала ком строка, далее сепарирование \n", TEXTIN);	//  ???*
				}
#endif//~~~~~~~~~~~~~~~~~~~~~ после отл можнои убрать  ~~~~~~~~~~~~~~~

				// INI созд-ся д пам pmemini, счит из ф поток и дополняем настр к умолч-ю 
				//потом записываем в ф поток 
//???????????????????????????????????????????????????????????????????????????????????????
				pmemini = (struct inidat*)malloc(sizeof(struct inidat)); //созд д пам
				if (pmemini == NULL)printf("Не выделена память ini настройки программы \n");
				else
				{
					// СЧИТЫВАЕМ INI файл в д пам pmemini!!!
					size_t result = fread(pmemini, sizeof(struct inidat), QUANTITYNAME, pFini);
					if (result != 0)
					{
						printf("  Выделена дин пам din1name = %d Bytes \n\
 под ini структуру-настройки программы и заполнена настр из ini файла\n",
							sizeof(struct inidat));			// debug sizeof(struct inidat));
					}
					fclose(pFini);  // cчитал и закрыл ф-л ))

					printf("  Выделена дин пам din1name = %d Bytes \n\
 под ini структуру-настройки программы и заполнена настр из ini файла\n",
						sizeof(struct inidat));			// debug sizeof(struct inidat));
				}
//????????????????????????????  зачем здесь открыался и перезаписывался ini файл и д память
				//~~~~ Начальное самое первое выдел пам *pmemword под сепар и поехали! прост блок ==============
				{
					printf("  Размер памяти под одну структуру %d байт\n", sizeof(struct word));
					amountword = MAX_WORD;  //размер ЗАПИСЕЙ начально выделенн 
					pmemword = (struct word *) malloc(amountword * sizeof(struct word));   //самое первое выделение памяти 
																						   //  под сепарацию и занесения строк в структуры 
																						   //временно - начальное количество MAX_WORD 
					if (pmemword == NULL)printf("Не выделенна память под punsort \n");
					else printf("  Выделенна память punsort = %d Bytes \n  под %d неотсортированных структур \
  и ПОЕХАЛИ! СЕПАРИРОВАТЬ\n",
						MAX_WORD * sizeof(struct word), MAX_WORD);				//    отладка
				}

				//~~~~~~~~~    далее (подготовка аргументов?) вызов ф-ции сепаррования - sepmini() ------   

				// pmemword - указ на МАССИВ СТРУКТУР (word) для отсепарирования token()
				//pmemtxtbuf - указ на дин массив неразбитого текста - копии входн файла

				// ВЫЗОВ СЕПАРИРОВАНИЯ  long amountword = *pcountnumword / sizeof(struct word); 
				//pmemword = sepmini(pmemword, pamountword, pmemtxtbuf, pcountnumword, TEXTIN);
				// sepmini2(struct word *pmemarray, int *pamountword, pcountnumword, char *pmemtxtbuf);
				pmemword = sepmini2(pmemword, pamountword, pcountnumword, pmemtxtbuf);
				free(pmemtxtbuf);	//освободить пам буфер входн текста

									//==========  далее запись в файл базу WORD_nosort сепарированных но несортированных структур ===========				
									//---~~~~~~ для несортировнного массива преобразов имени XXX_nosort.dat вызовом ф-и rename2()
				char *pnamewordnosort;  //указ д строки для преобраз.rename имя ф "TEXTIN_nosort.dat"
				{	pnamewordnosort = rename2(TEXTIN, "_nosort.dat", 4);
				}
				//~~~~~~~~~~~~ запись в WORD hdd файл(заранее переим) базу несортир структур ---///////////////  
				writebase2(pFnosort, pnamewordnosort, pmemword, countnumword);
					//pnosortFile - указ на откр внутр ф-ции hdd файл в котором сохранять базу слов 
					//pnamewordnosort - уже сформированное ранее имя ф-ла для hdd ("argv[1]_nosort.dat")
					// pmemword - указ на дин массив НЕСОРТ структур, 
					// countnumword - число несорт структур
					//,?? возврат указ имя файла с  структурами ( ----- )???? 
				puts(pnamewordnosort);		//debug вывод имени .hdd несортированных слов

				//============= занесение в ф ini "fini.dat" <- ИМЕНИ - XXX_nosort.dat из дин памяти  =======   	
				//~~~~~~~  сначала изменение в дин пам pFini <- ИМЕНИ - XXX_nosort.dat ~~~~~~~~~
				{pmemini->idname = 0;
				strncpy(pmemini->ininamenosortf, pnamewordnosort, EN1);
				}

				//~~~~~~~~~~  запись в ф ini "fini.dat" <- ИМЕНИ XXX_nosort.dat из дин памяти  ~~~~~~~~   	
				err = fopen_s(&pFini, "fini.dat", "r+b");//XXX_nosrt.dat сохр в ф-л "fini.dat"
				if (err)
				{
					puts("\n Ошибка! \n Неудача отытия ранее созданного ф-ла имён пользователя \n");
					//удалить из д памяти pmemini->ininamenosortf <- ИМЕНИ - XXX_nosort.dat 
					size_t tlen = strlen(pmemini->ininamenosortf);
					memset(pmemini->ininamenosortf, NULL, tlen);
					system("pause");
					exit(1);
				}
				else
				{
					fwrite(pmemini, sizeof(struct inidat), QUANTITYNAME, pFini);//fini.dat
					fclose(pFini);	//поработал и закрыл )) или ещё добавлять настойки???????? 
				}
				
			}     //   end текстовый файл из папки открылся, отсепар-ся и запис-ся в pFnosort
		}  // end открытие нового входного text00.txt файла ...........
//_________________________________________________________________________________
// здесь if( нажать Y если изменить только настройки для существующего сепарированного текста)
		puts("\n если НОВЫЕ НАСТРОЙКИ ?  - тогда нажмите 'y'\n");
		if ('y' == getch(stdin))//  вносить изменения в настройки урока ============
		{


		}	//end   вносить изменения в настройки урока .................



//_________________________________________________________________________________________
// Перед сортировкой преобразование имени в XXX_sort.dat  =====================
			char *pnamesortword = NULL;  // указат на дин строка-имя  файла "argv[1]_sort.dat"
			pnamesortword = rename2(TEXTIN, "_sort.dat", 4);  // д п выдел ф rename2()
			puts(pnamesortword); 		//debug
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

			//далее вызов АЛФАВИТНОЙ или другой сортировки из ini и сокращение повторов =======
			//
			puts("\n если Алфавитная сортировука  ?  - тогда нажмите '1'\n");
			if ('1' == getch(stdin))//  вносить изменения в настройки урока ============
			{


			}	//end вызов АЛФАВИТНОЙ сортировки
//=========================================================================================================
//=========================================================================================================
		





	}	// end "как есть или хотите изменить текст или настройки урока ? - нажмите 'y'"...............
	else
	{  // извлекаем настр. старого fini.dat и работаем с уже ранее открывавшимся уроком и 
		// отсортированным текстом
		puts("\n извлекаем настройки старого fini.dat_____ \n");
		puts(" работаем с отсортированным ранее текстом_____ \n");


	} //end if else "хотите ли создать другой fini.dat"

	free(pmemini);
	
	printf("\n\n             The END!   -     конец урока! \n\n\n");
	system("pause");
}  //end main