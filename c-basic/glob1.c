 #include <stdio.h>
 #include <glob.h>
 
int main(int argc, const char *argv[]){
    glob_t buf;
    int i;
    glob("/home/*",GLOB_NOSORT, NULL, &buf);
    for(i=0; i < buf.gl_pathc; i++)
    {
        printf("buf.gl_pathv[%d]= %s \n", i, (buf.gl_pathv[i]));
    }
  
    globfree(&buf);
    return 0;
}