#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

void main ()
{
    pid_t processA, processB;
    int fd1, fd2, fd3, fd4, readed, i;
    char textH1[30];
    char textH2[30];

    fd1 = mknod("T_H1_H2", S_IFIFO | 0600, 0);
    fd2 = mknod("T_H2_H1", S_IFIFO | 0600, 0);

    if(fd1 == -1) printf("Problemas tuberia T_H1_H2\n");
    else if(fd2 == -1) printf("Problemas tuberia T_H2_H1\n");
    else printf("FIFOS CREATED\n\n");

    processA = fork();

    if(processA == 0) //HIJO 1
    {
        fd1 = open("T_H1_H2", O_WRONLY);
        fd3 = open("T_H2_H1", O_RDONLY);

        if(fd1 == -1) printf("Error en la conexion de la tuberia \n");
        else printf("Conexion exitosa Hijo 1 (%d)\n", getpid());

        while(strncmp(textH1,"exit",4)!=0)
        {
            sleep(1);
			printf("\nIngrese su mensaje (%d)---> ", getpid());
			fgets(textH1, 30, stdin);
			write(fd1, textH1, strlen(textH1)+1);

            readed = read(fd3, textH2, sizeof(textH2));
            if(strncmp(textH2,"\n",1)!=0)
                printf("Mensaje Leido (%d)---> %s", getpid(), textH2);
        }
    }
    else
    {
        processB = fork();

        if(processB == 0)// HIJO 2
        {
            fd2 = open("T_H1_H2", O_RDONLY);
            fd4 = open("T_H2_H1", O_WRONLY);

            if(fd2 == -1) printf("Error en la conexion de la tuberia \n");
            else printf("Conexion exitosa Hijo 2 (%d)\n", getpid());

            while(strncmp(textH2,"exit",4)!=0)
            {
                readed = read(fd2, textH1, sizeof(textH1));
                if(strncmp(textH1,"\n",1)!=0)
			        printf("- Mensaje Leido (%d)---> %s", getpid(), textH1);

                printf("\n- Ingrese su mensaje (%d)---> ", getpid());
			    fgets(textH2, 30, stdin);
			    write(fd4, textH2, strlen(textH2)+1);
                sleep(1);
            }
        } 
        else
        {
            while(wait(NULL) != -1);
        }
    }
}