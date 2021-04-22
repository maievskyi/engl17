
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
struct inidat *pmeminidat;	//--->указ на ДИН пам с стр-й базы ini имён прог-мы
int flagtext = NEWTEXT;	//переменная -ФЛАГ режима работы со стар или нов текстом
						//т е будет ли сепарироваться по новой и писаться в нофую базу слов 

						//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%       main     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
int main(int argc, const char ** argv, const char** env)
{

	system("chcp 1251 > nul");
	errno_t err;			//  переменная (int?) для вывода ошибок ? внутр переменная (int?)



	printf("\n\n             The END!   -     конец урока! \n\n\n");
	system("pause");
}  //end main