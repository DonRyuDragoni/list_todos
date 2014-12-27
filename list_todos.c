/*
 * An attempt to build my first usefull program in C
 *
 * This is supposed to read a file and search in it
 * for lines beggining with "TODO" and list them in
 * order, printing something like:
 *
 *     On line 15:
 *         list files used untill now
 *     On line 122:
 *         Rewrite the function, using recursion
 * 
 * (Note that the notes are printed ignoring the "TODO"
 * strings. After all, this program is already about
 * printing TODOs, so no need to remaind it in every
 * f*cking line.)
 *
 */

#include<stdio.h>
#include<string.h>

int main()
{
    // Note that THIS IS NOT WORKING!
    
    char str[] = "asdlkfj";
    char* c = &str[0];
    while(c != &str[7]) {
        printf("%c", c);
        ++c;
    }

    return 0;
}
