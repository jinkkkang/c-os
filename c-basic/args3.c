#include <stdio.h>
#include <unistd.h>


static void help(void) {
        printf("%s\n\n"
               "Report whether we are connected to an external power source.\n\n"
               "  -h --help             Show this help\n"
               "     --version          Show package version\n"
               "  -v --verbose          Show state as text\n",
               "demo");
}


/*

optstring
没有冒号： 说明该选项没有参数，不需要参数
有1个冒号： 该选项有一个参数
单个字符后接两个冒号”::"，表示该选项Option的选项参数Option argument是可选的。当提供了Option argument时，必须紧跟Option之后，不能以空格隔开
*/


int main(int argc, char *argv[])
{
    int opt;
    const char *optstring = "ab:c:d::e"; // 
    help();
    while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
            case 'a':
                printf("Option -a was triggered.\n");
                break;
            case 'b':
                printf("Option -b with argument '%s' was triggered.\n", optarg);
                break;
            case 'c':
                printf("Option -c with argument '%s' was triggered.\n", optarg);
                break;
            case 'd':
                printf("Option -d with argument '%s' was triggered.\n", optarg);
                break;     
            case '?':
                fprintf(stderr, "Unknown option character '%c'.\n", optopt);
                break;
            default:
                help();
        }
    }

    // Process remaining arguments after the options
    for (int i = optind; i < argc; i++) {
        printf("optind: %d\n",i);
        printf("Non-option argument: %s\n", argv[i]);
    }

    return 0;
}

