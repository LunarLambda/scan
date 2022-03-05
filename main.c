/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <stdio.h>
#include "scan.h"

int main(int argc, const char *argv[])
{
    argv++; argc--; // Skip program name (argv[0])

    struct state s = {0};
    int res;
    while ((res = scan(argv, argv + argc, "abc::f:", &s)) != -1)
    {
        switch (res)
        {
            case 'a':
                puts("Got -a");
                break;

            case 'b':
                puts("Got -b");
                break;

            case 'c':
                printf("Got -c with optional argument %s\n", s.argument ? s.argument : "<none>");
                break;

            case 'f':
                printf("Got -f with argument %s\n", s.argument);
                break;

            case 1: // non-option argument
                printf("Got non-option argument %s\n", s.argument);
                break;

            case '?': // unknown option
                printf("Got unknown option %c in argument %s\n", s.option, s.argument);
                break;

            case ':': // missing argument
                printf("Missing argument for for -%c\n", s.option);
                break;

            default:
                puts("Something else...");
                break;
        }
    }

    for (int i = s.position; i < argc; i++)
    {
        printf("Extra argument %s\n", argv[i]);
    }

    puts("");
}
