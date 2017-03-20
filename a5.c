/* Names: Ian Kirkpatrick, Benjamin Groseclose, Nathan Johnson,
 * Class: CPS 360,                            Section: Spring 2017
 * Times: Tuesday, Thursday 9:30 - 11:00      Assignment: 04
 * Due: March 2, 2017                      Started: February 27, 2017
 * Credit: 15 points.
 *
 * Problem:
 *
 * Solution:
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  int intfromcomline(char *argv[], int index);
  char *strfromcomline(char *argv[], int index);
  int getfilesize(char *path);
  int loadimage(char *path, int mem[], int size);
  void mdump(int mem[], int size, int first, int last);

  char *filepth;
  int beginaddr;
  int endaddr;
  int filesize;
  int loadimagestatus;
  static int filebytes[5000000];

  filepth = strfromcomline(argv, 1);
  beginaddr = intfromcomline(argv, 2);
  endaddr = intfromcomline(argv, 3);

  filesize = getfilesize(filepth);

  loadimagestatus = loadimage(filepth, filebytes, filesize);
  if (loadimagestatus) mdump(filebytes, filesize, beginaddr, endaddr);
  exit(0);
}

/*
 *

 * intfromcomline requires an array of chars 'argv[]' and a integer 'index'. This to pass the
 * argument command line and then a index for that array. Will declare i, which
 * will be equal to what the system library function strtol returns. We pass
 * argv with the index of 'index'. Also a null and 16. Returns the value of i.
 */
int intfromcomline(char *argv[], int index) {
  int i = strtol(argv[index], NULL, 16);
  return i;
}

/*
* *strfromcomline requires an array of chars 'argv[]' and a integer 'index'. This to pass the
* argument command line and then a index for that array. Simply returns then
* value of the array at the index 'index'.
*/
char *strfromcomline(char *argv[], int index) {
  return argv[index];
}

/*
* getfilesize requires you to pass the base to the file. Then declares a
* struct stat sb and int filestats. It will then set filestats to equal
* the stat system call method using the filepath and the struct stat sb as
* parameters. Compares filestats to equal -1 if so returns 0. This would mean
* stat returned and error and then getfilesize will return a 0 saying
* an error occured. If filestats does NOT equal -1 it will then return
* the size of the file.
*/
int getfilesize(char *path) {
  struct stat sb;
  int filestats;

  filestats = stat(path, &sb);
  if (filestats == -1) return 0;
  else return (int) sb.st_size;
}

/*
* Loadimage expects the filename, the memory array, and size. Creates a
* variable named int fileopen. Then is set fileopen to equal what the system
* function open() returns. open() returns the file descriptor or -1 on error.
* Checks for the -1 error and will return 0 if the case due to the open() error.
* Will use the read() system function to read from the file. Read() requires
* fileopen (whatever is returned from open(). ) Also the memory array and size.
* Whatever is return from read this is compared to != Size. Because read() only
* worked correctly in our case when it returns the same number as size. If
* read() returns a number that's does not equal size. Loadimage returns 0,
* due to the error that occurred in read() If this if statement goes to the
* else. We call the system function close() that requires the fileopen
* variable. The last we do is return 1 from loadimage.
*
* Limitations: Technically the read() function only has an error when it
* returns -1. There can be cases where read() works properly and returns a
* number they does not equal size. However we do not desire for this to occur
*in our program.
*/
int loadimage(char *path, int mem[], int size) {
  int fileopen = open(path, O_RDWR);
  if (fileopen < 0) return 0;

  if (read(fileopen, mem, size) != size) return 0;
  else close(fileopen);

  return 1;
}

char *strword(int int_word, char ascii[], int wordid) {
  unsigned int *ptrint_word = malloc(sizeof(int));
  int first_byte, second_byte, third_byte, fourth_byte;
  char *str_word = malloc(4),
  *ptrchar_word;

  *ptrint_word = int_word;
  ptrchar_word = (char *) ptrint_word;

  first_byte = (ptrchar_word[0]) & 0xff;
  second_byte = (ptrchar_word[1]) & 0xff;
  third_byte = (ptrchar_word[2]) & 0xff;
  fourth_byte = (ptrchar_word[3]) & 0xff;

  sprintf(str_word, "%02x%02x%02x%02x", first_byte, second_byte, third_byte, fourth_byte);

  ascii[0 + (4*wordid)] = first_byte;
  ascii[1 + (4*wordid)] = second_byte;
  ascii[2 + (4*wordid)] = third_byte;
  ascii[3 + (4*wordid)] = fourth_byte;

  return str_word;
}

void dumpline(int mem[], int size, int start, int limit) {
  char *strword(int, char[], int);

  int i = start, numwords = 4, chunk;
  char *current, ascii[16], fst, scd, thd, fth;
  printf("%08x  ", start);

  if (limit != -1 && limit <= numwords) numwords = limit;

  for (; i < start + numwords; i++) {
    current = strword(mem[i], ascii, i-start);
    printf("%s ", current);
  }

  i = 0;
  for (; i < 4-numwords; i ++) printf("         ");

  printf(" ");

  i = start;
  for (; i < start + numwords; i ++) {
    chunk = 4*(i-start);
    fst = ascii[0 + chunk];
    scd = ascii[1 + chunk];
    thd = ascii[2 + chunk];
    fth = ascii[3 + chunk];

    if (fst < 0x20) fst = '.';
    if (scd < 0x20) scd = '.';
    if (thd < 0x20) thd = '.';
    if (fth < 0x20) fth = '.';

    printf("%c%c%c%c", fst, scd, thd, fth);
  }

  printf("\n");
}

void mdump(int mem[], int size, int first, int last) {
  void dumpline(int[], int, int, int);
  void dumpheader();
  int start, end, count;

  if (first < 0 || first > size) {
    start = 0; end = size+1;
  } else if (last < 0 || last > size) {
    start = 0; end = size+1;
  } else {
    start = first; end = last+1;
    printf("%d, %d\n", start, end);
  }

  dumpheader();

  count = start;
  for (; count < end; count += 4) {
    dumpline(mem, size, count, end - count);
  }
}

void dumpheader() {
  char msg1[] = "Address           Words in Hexadecimal         ASCII characters";
  char msg2[] = "--------  -----------------------------------  ----------------";
  printf("%s\n%s\n", msg1, msg2);
}
