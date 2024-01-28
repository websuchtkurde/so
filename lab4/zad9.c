#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int lacze[2];
    pipe(lacze);

    pid_t potomek1 = fork();

    int lacze2[2];
    pipe(lacze2);

    pid_t potomek2 = fork();

    if(potomek1 == 0)
    {
        if(potomek2 == 0)
        {
            int i;

            for(i = 1; i <= 4; i++)
            {
                int temp = 0;

                read(lacze[0], &temp, sizeof(temp));

                temp++;
                printf("Liczba wyslana przez proces 3: %d\n", temp);

                write(lacze2[1], &temp, sizeof(int));

                sleep(1);

                if(i == 4)
                {
                    close(lacze[0]);
                    close(lacze2[1]);
                }
            }
        }
        else
        {
            int i;

            for(i = 1; i <= 4; i++)
            {
                int temp = 0;

                read(lacze[0], &temp, sizeof(temp));

                temp++;
                printf("Liczba wyslana przez proces 2: %d\n", temp);

                write(lacze[1], &temp, sizeof(int));

                sleep(1);

                if(i == 4)
                {
                    close(lacze[0]);
                    close(lacze[1]);
                }
            }
        }
    }
    if(potomek2 == 0)
    {
        if(potomek1 == 0)
        {
            int i;

            for(i = 1; i <= 4; i++)
            {
                int temp = 0;

                read(lacze2[0], &temp, sizeof(temp));

                printf("Odczytana liczba: %d\n", temp);

                sleep(1);

                if(i == 4)
                {
                    close(lacze2[0]);
                }
            }
        }
        else
        {
            int i;

            for(i = 1; i <= 4; i++)
            {
                printf("Wyslana liczba: %d\n", i);

                write(lacze[1], &i, sizeof(i));

                sleep(1);

                if(i == 4)
                {
                    close(lacze[1]);
                }
            }
        }
    }

    return 0;
}
