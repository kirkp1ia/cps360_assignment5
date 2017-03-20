/* Names: Ian Kirkpatrick, Benjamin Groseclose
 * Class: CPS 360,                            Section: Spring 2017
 * Times: Tuesday, Thursday 9:30 - 11:00      Assignment: 05
 * Due: March 21, 2017                      Started: March 19, 2017
 * Credit: 15 points.
 *
 * Problem: To create a program that will read in a File-path, startingAddress
 * and the lastaddress. Then print out the starting address and 16 bytes in
 * hexidecimal form that correspound with that starting address. Also print
 * out the ASCII symbols that are associated with the given bytes.Do this all
 * in the correct output format.
 *
 * Input Format:
 * <file-path> <start-address> <end-address>
 *
 * Output Format:
 * address            Words in hexidecimal         ASCII
 * --------  ------------------------------------  --------
 * aaaaaaaa   xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx  cccccccc
 *
 * Solution: To collect the given arguments by using the system library
 * function strtol(). The collect the given data from the file through the
 * open(), read(), close() system calls. Transfer the data into hexidecimal
 * form by breaking into chunks and pulling char values of them. To get them
 * into sets of 2. Place all the hexidecimal value collected into the correct
 * string format. Place these hex values into an array that can be used to
 * print out the corresponding ASCII codes. Create the header for the ouput.
 * Then correctly print out each line. Only prints out until you have exhaust
 * the word count that was entered as the 3rd argument on the command line.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


/*
 * Problem: To direct the program flow and actions. Call correct method and
 * give the correct paramters. Reads input and displays the file in hexidecimal
 * form according to the desired format. Checks for bad input and gives the
 * correct error connect to it.
 *
 * Solution: Declare prototypes for the method we shall use main. Declares also
 * the variables needed in main. Then sets filepth to equal the return value
 * of strfromcomline after the paramters are argv, 1. Then calls readargs to set
 * the values of filepth, beginaddr, and endaddr. Also Checks for bad input. It
 * then calls the loadimage method and will set the array filebytes to equal
 * the data within the file given in the command line. Checks whether any data
 * was stored in filebytes. If so will call the method mdump which will
 * ultimatly be what prints out the correct output. After this it shall exit
 * the program.
 */
int main(int argc, char *argv[]) {
  int getfilesize(char *path);
  int loadimage(char *path, int mem[], int size);
  void mdump(int mem[], int size, int first, int last);
  char *readargs(int, char*[], int*, int*);

  char *filepth;
  int beginaddr;
  int endaddr;
  int filesize;
  int loadimagestatus;
  static int filebytes[5000000];

  filepth = readargs(argc, argv, &beginaddr, &endaddr);

  filesize = getfilesize(filepth);

  loadimagestatus = loadimage(filepth, filebytes, filesize);
  if (loadimagestatus) mdump(filebytes, filesize, beginaddr, endaddr);
  else fprintf(stderr, "Please input a valid file path. File %s could not be found.\n", filepth);
  exit(0);
}


/* Problem: Set command line argument variables to their corresponding value
 * passed in through the command line. This also checks to see if the correct
 * number of arguments have been passed. 3 values must have been passed or the
 * program will close and display a usage message.
 *
 * argc and argv are the same things as in main. These are used to get the
 * actual values for beginaddr, endaddr and file path.
 *
 * Solution: Compare argc against 4 (3 arguments plus the program name). If it
 * is not equal to 4, then the usage message is displayed and the program exits.
 * Otherwise, beginaddr and endaddr are set to their corresponding values passed
 * in through the command line. The file path is returned for ease of writing.
 * This way, the size of the array of chars can be determined based on the
 * actual value and not be a fixed "catch-all" length string.
 */
char *readargs(int argc, char *argv[], int *beginaddr, int *endaddr) {
  int intfromcomline(char *argv[], int index);
  char *strfromcomline(char *argv[], int index);
  char *fpth;
  
  if (argc != 4) {
    fprintf(stderr, "Please enter 3 arguments. A file name,\nthe address at which to begin printing the file,\nand the address at which to stop printing the file.\n");
    exit(1);
  } else {
    fpth = strfromcomline(argv, 1);
    *beginaddr = intfromcomline(argv, 2);
    *endaddr = intfromcomline(argv, 3);
    return fpth;
  }
}


/*
 * Problem: To collect the integer arguments passed on the command line, at
 * index
 *
 * Solution: intfromcomline requires an array of chars 'argv[]' and a integer
 * 'index'. This to pass the argument command line and then a index for that
 * array. Will declare i, which will be equal to what the system library
 * function strtol returns. We pass argv with the index of 'index'. Also a
 * null and 16. Returns the value of i.
 */
int intfromcomline(char *argv[], int index) {
  int i = strtol(argv[index], NULL, 16);
  return i;
}


/*
 * Problem: Collect the address of the file passed in the commandline.
 *
 * Solution: *strfromcomline requires an array of chars 'argv[]' and a
 * integer 'index'. This to pass the argument command line and then a index
 * for that array. Simply returns then value of the array at the index 'index'.
 */
char *strfromcomline(char *argv[], int index) {
  return argv[index];
}


/*
 * Problem: To collect the size of the file given in the command line
 * arguments.
 *
 * Solution: getfilesize requires you to pass the base to the file. Then
 * declares a struct stat sb and int filestats. It will then set filestats
 * to equal the stat system call method using the filepath and the struct
 * stat sb as parameters. Compares filestats to equal -1 if so returns 0.
 * This would mean stat returned and error and then getfilesize will return
 * a 0 saying an error occured. If filestats does NOT equal -1 it will
 * then return the size of the file.
 */
int getfilesize(char *path) {
  struct stat sb;
  int filestats;

  filestats = stat(path, &sb);
  if (filestats == -1) return 0;
  else return (int) sb.st_size;
}


/*
 * Problem: To open the file. To read from the file and place the data in the
 * file into an array that is connected to mem[] array. Then to close the file
 * up again.
 *
 * Solution: Loadimage expects the filename, the memory array, and size.
 * Creates a variable named int fileopen. Then is set fileopen to equal what
 * the system function open() returns. open() returns the file descriptor or
 * -1 on error. Checks for the -1 error and will return 0 if the case due to
 * the open() error. Will use the read() system function to read from the file.
 * Read() requires fileopen (whatever is returned from open(). ) Also the
 * memory array and size. Whatever is return from read this is compared to
 * != Size. Because read() only worked correctly in our case when it returns
 * the same number as size. If read() returns a number that's does not equal
 * size. Loadimage returns 0, due to the error that occurred in read() If
 * this if statement goes to the else. We call the system function close()
 * that requires the fileopen variable. The last we do is return 1 from
 * loadimage.
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


/* Problem: Return a word sized set of data (4 bytes) as a string. Also, set
 * the corresponding index in the char array ascii for that word to the
 * character values of the word.
 *
 * For example, if int_word is 0x23a5f6 and wordid is 2, then the string
 * representation of int_word will be returned ("23a5f6") and the 2 chunk in
 * ascii (index 4, 5, 6, 7 which are the second group of 4 bytes (the size of
 * a word.)) will be set to 23, a5 and f6 in that order. The chunk number
 * corresponds to wordid.
 *
 * Solution: Declare variables needed in the function. A pointer is declared
 * pointing to int_word. The data in that pointer is then broken up into 4
 * bytes. These are then strung together as strings using sprintf. Also, each
 * byte is put into ascii using the equation "bytenum + (4*wordid)" to retrieve
 * the index at which to put the byte. The string with the bytes are returned
 * and ascii is accessible by the caller.
 */
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


/* Problem: Print a line of hexidecimal values corresponding to the file data
 * found in mem. Each line will have the following format:
 *
 * aaaaaaaa  xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx  cccccccc
 *
 * The first bit, aaaaaaaa, is the first address in that line. The second bit,
 * the group of xxxxxxxx's are the actual hexidecimal values of the file at
 * the corresponding addresses in mem. The next bit, cccccccc, is the ASCII
 * values of the 8 hexidecimal words.
 *
 * It can be assumed that the address for each word in the line can be found by
 * adding it's index in this line (0-indexed) to the value of aaaaaaaa.
 *
 * Solution: Begin by printing the address out. This will always be the value of
 * start. Then, print each word starting at start untill the address at start +
 * limit. Retrieve the string representation of each word through strword.
 * Each call to strword adds the word to ascii. This allows ascii to then be
 * iterated over and printed out as the cccccccc part of the line in ascii form.
 *
 * NOTE: If start + limit < 4, blank spaces will replace the missing words.
 */
void dumpline(int mem[], int start, int limit) {
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


/* Problem: Dump the contents of the file contained in mem to stdout. Only print
 * the data beginning at first and ending at last inclusively. If first and
 * last are outside of the range of mem (0..size), then the entirety of mem is
 * printed.
 *
 * Solution: Check to see if first and last are out of bounds and if so, set
 * start and end to 0 and size respectively. Otherwise, set them to first and
 * last.
 * Next, after calling dumpheader to print the header of the table, iterate from
 * start to end and print every 4 words using dumpline to stdout. Incrementation
 * will increase by 4 because each 4 words are printed at a time.
 */
void mdump(int mem[], int size, int first, int last) {
  void dumpline(int[], int, int);
  void dumpheader();
  int start, end, count;

  if (first < 0 || first > size) {
    start = 0; end = size+1;
  } else if (last < 0 || last > size) {
    start = 0; end = size+1;
  } else {
    start = first; end = last+1;
  }

  dumpheader();

  count = start;
  for (; count < end; count += 4) {
    dumpline(mem, count, end - count);
  }
}


/*
 * Problem: To return the header of the desired output.
 *
 * Solution: Declare two arrays of chars. Each will hold specific strings that
 * will print out as the header. The prints out the two msg value seperated by
 * a new line.
 */
void dumpheader() {
  char msg1[] = "Address           Words in Hexadecimal         ASCII characters";
  char msg2[] = "--------  -----------------------------------  ----------------";
  printf("%s\n%s\n", msg1, msg2);
}
