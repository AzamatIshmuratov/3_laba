#include <QCoreApplication>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <clocale>
using namespace std;
const int l_name = 40;
const int num = 20;
struct Book {
    char name[l_name], author[l_name], number_UDK[num];
    int year_of_publication, copies;
    Book *next;
};

Book* add( Book *beg, const Book &book );
//int edit( Book *beg );
Book* find( Book *beg, char *name, Book *& prev );
/*void find( Book *beg, int birth_day );
void find( Book *beg, float pay );*/
void find_book_return( Book *beg );
void find_book_take( Book *beg );
void get_author( char *name );
void get_name( char *name );
void get_UDC( char *name );
int menu();
void print_dbase( Book *beg );
Book* read_dbase( char *filename );
Book read_book();
Book* remove( Book *beg );
int write_dbase( char *filename, Book *beg );
// --------------------------------------Главная функция -------------------------
int main() {
    setlocale(LC_ALL, "rus");
    Book *beg = read_dbase( "C:\\Structures\\build-Book-Desktop_Qt_5_8_0_MinGW_32bit-Debug\\debug\\dbase.txt" );
    if ( !beg ) return 1;
    puts( "===============================================================" );
    puts("Welcome to our library!");
    while ( true ) {
        switch ( menu() ) {
            case 1: find_book_return( beg );
            break;
            case 2: find_book_take( beg );
            break;
            case 3: add( beg, read_book() );
            break;
            case 4: print_dbase( beg );;
            break;
            case 5: write_dbase( "C:\\Structures\\build-Book-Desktop_Qt_5_8_0_MinGW_32bit-Debug\\debug\\dbase.txt", beg );
            break;
            case 6: remove(beg);
            case 7: return 0;
            break;
            default: puts( " Nado vvodit' chislo ot 1 do 7" ); break;
        }
    }
}

Book* add( Book *beg, const Book &book ) { // -------------------Добавление книги
    Book* pv = new Book; // Формирование нового элемента
    *pv = book;
    pv->next= 0;
    if ( beg ) { // Список не пуст
        Book* temp = beg;
        while ( temp->next) temp =temp->next; // Поиск конца списка
        temp->next= pv; // Привязывание нового элемента
    }
    else beg = pv; // Список пуст
    return beg;
}

// ----------------------------------------------------Поиск книги по названию
Book* find(Book* pv, char* name, Book *& prev ) {
    prev = 0;
    while(pv) {
        if ( strstr( pv->name,name ) )
            if ( pv->name[strlen( name )] == ' ' ) {
                printf( "%s%s%s%5i%3i\n", pv->number_UDK, pv->author, pv->name, pv->year_of_publication, pv->copies);
                return pv;
            }
        prev = pv;
        pv = pv->next;
    }
    puts( "There is no such book\n" );
    return 0;
}

void find_book_return( Book* beg ) { // ----------------------------------------------Поиск
    char buf[l_name];
    Book *prev = 0, *pv = 0;
    get_name( buf );
    pv = find( beg, buf, prev );
    if (pv == 0)
        return;
    pv->copies++;
    printf( "Thanks for the return!\n", pv->number_UDK, pv->author, pv->name, pv->year_of_publication, pv->copies);
}

void find_book_take( Book* beg ) { // ----------------------------------------------Поиск
    char buf[l_name];
    int choice;
    Book *prev = 0, *pv = 0;
    get_name( buf );
    pv = find( beg, buf, prev );
    if (pv == 0)
        return;
    else {
        printf("\nDo you want to take a book? (1 - Yes; 2 - No)\n");
        scanf ("%i", &choice);
        if (choice == 1){
            if ((pv->copies)==0){
               printf("\nBook in stock, but is on the hands of\n");
            } else {
                printf("\nThe book in stock\n");
                pv->copies--;
                //printf( "%s%s%s%5i%3i\n", pv->number_UDK, pv->author, pv->name, pv->year_of_publication, pv->copies);
            }
       }else return;
    }
}

void get_author( char* name ) { // -----------------------------------Запрос автора
    puts( "Enter author" ); gets( name );
}

void get_name( char* name ) { // -----------------------------------Запрос названия
    puts( "Enter book title" ); gets( name );
}

void get_UDC( char* name ) { // -----------------------------------Запрос номера УДК
    puts( "Enter the number of UDC" ); gets( name );
}

int menu() { // --------------------------------------------------------Вывод меню
    char buf[10];
    int option;
    do {
        puts( "===============================================================" );
        puts( "\n1 - Return a book\t 4 - Display base on the screen");
        puts( "2 - Take a book\t\t 5 - Outputting the database to a file" );
        puts( "3 - Add new book\t 6 - Delete the book\t\t\t 7 - Exit" );
        gets( buf );
        option = atoi( buf );
    } while ( !option );
    return option;
}

void print_dbase( Book* beg ) { // -----------------------------Вывод базы на экран
    Book* pv = beg;
    while ( pv ) {
        printf( "%s%s%s%5i%3i\n", pv->number_UDK, pv->author, pv->name, pv->year_of_publication, pv->copies);
        pv = pv->next;
    }
}

Book* read_dbase( char* filename ) { // -----------------------Чтение базы из файла
    FILE* fin;
    Book book, *beg = 0;
    if ( ( fin = fopen( filename, "r" ) ) == 0 ) {
        printf( "No such file %s\n", filename );
    return NULL;
    }
    while ( !feof( fin ) ) {
        fgets( book.number_UDK, num, fin );
        fgets( book.author, l_name, fin );
        fgets( book.name, l_name, fin );
        fscanf( fin, "%i%i\n", &book.year_of_publication, &book.copies );
        beg = add( beg, book );
    }
    fclose( fin );
    return beg;
}

Book read_book() { // ----------------------------Ввод информации о новой книге
    Book book; char buf[80];
    get_UDC( book.number_UDK );
    for ( int i = strlen( book.number_UDK ); i < num; i++ )
        book.number_UDK[i] = ' ';
    book.number_UDK[num - 1] = '\0';
    get_author( book.author );
    for ( int i = strlen( book.author ); i < l_name; i++ )
        book.author[i] = ' ';
    book.author[l_name - 1] = '\0';
    get_name( book.name );
    for ( int i = strlen( book.name ); i < l_name; i++ )
        book.name[i] = ' ';
    book.name[l_name - 1] = '\0';
    do {
        puts( "Enter the year of publication" ); gets(buf);
    }
    while ( !( book.year_of_publication = atof( buf ) ) );
    do {
        puts( "Enter the number of copies" ); gets(buf);
    }
    while ( !( book.copies = atof( buf ) ) );
    return book;
}
Book* remove( Book* beg ) { // ----------------------------------Удаление книги
    char name [l_name];
    get_name( name ); // Кого удаляем?
    Book* prev;
    Book* pv = find( beg, name, prev );
    if ( pv ) { // Если нашли
        if ( pv == beg ) beg = beg->next; // Удаление из начала списка
        else// Удаление из середины или конца списка
            prev->next=pv->next;
        delete pv; // Освобождение памяти из-подэлемента
    }
    return beg;
}
// --------------------------------------------------------------Вывод базы в файл
int write_dbase( char *filename, Book *pv ) {
    FILE *fout;
    if ( ( fout = fopen( filename, "w" ) ) == NULL ) {
        puts( "Error opening the file" );
        return 1;
    }
    while ( pv ) {
        fprintf( fout, "%s%s%s%5i%3i\n", pv->number_UDK, pv->author, pv->name, pv->year_of_publication, pv->copies);
        pv = pv->next;
    }
    fclose( fout );
    return 0;
}
