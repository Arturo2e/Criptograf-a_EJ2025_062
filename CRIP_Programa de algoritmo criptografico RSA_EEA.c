/**---Program for the RSA cryptografhic algorithm---
 * University: Autonomous University of Nuevo Leon (UANL)
 * Dependence: Faculty of Physical and Mathemathics Sciences (FCFM)
 * Carrier: Licence of Security of Information Thecnologies (LSTI)
 * Matter: Cryptografy 
 * Student-name: Escalera Elizondo Arturo
 * Id-number: 2092649
 * Theacher: Alejandro Muñiz Solorio 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define HASH_LENGHT 128

// Default message that establish if the user had not enter a message to cipher
char deflt_message[1000] = "";

//Verify if the input of the user is a positive integer
int is_uninteger(const char *input) {
    // Verify if the input is empty
    if (strlen(input) == 0) return 0;
    
    //Verify if a something in the input not is a digit
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) return 0;
    }

    return 1;
}

// Exponentiation of a integer that has a result of a b**a for a iterations
int powint(const int base, int exp)
{
    int pwint = 1;
    for (int i=0; i < exp; i++) pwint = base*pwint;
    return pwint;
}

// Gets the unique identifier of the message (the number m)
unsigned int hash(char message[1000])
{
    unsigned int dec_sum = 0;
    for(int i=0; message[i] != '\0'; i++)
    {
        unsigned int dec_val = (unsigned int) message[i];
        dec_sum += dec_val;
    }
    // Gets the actual local time in seconds
    time_t actime;
    time(&actime);
    srand( (unsigned int) actime );
    unsigned int randnum = (unsigned int) rand();
    return (dec_sum + randnum)%HASH_LENGHT;
}

// Returns the 'c' decimal value of the message encrypted, identifies by number 'm'
unsigned int rsa_cipher(unsigned int e, unsigned int n)
{
    char message[1000];
    printf("\nEscriba el mensaje a cifrar. [Límite de 1000 carácteres]\n");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';
    strcpy(deflt_message, message);
    if (strcmp(message, "") == 0)
    {
        printf("\nIngrese un mensaje para cifrar he intente de nuevo.\n");
        return -1;
    }
    unsigned int m = hash(message);
    return (powint(m, e))%n; // c value
}

// Returns the 'm' decimal value of the original message from the number 'c'
unsigned int rsa_uncipher(int c, unsigned int e, unsigned int n, int phi_n)
{
    int d = (1/e)%phi_n;
    return (powint(c, d))%n; // m value
}

// Calculate the Euler's totient function
int phi(int p, int q) { return (p-1)*(q-1); }

// Verify if a number given is a prime
int is_prime(int number)
{
    if (number == 2 || number == 3) return 1;
    else
    {
        // Verify if the number given is a even number
        if (number % 2 == 0) return 0;

        // Finds any divisor of the number
        // by verify if any other number less than the number is a divisor
        // omitting the even denominators
        for(int i=3; i < number; i += 2)
        {
            if (number%i == 0) return 0;
        }
        return 1;
    }
}

void main(void)
{
    char input1[128];
    char input2[128];
    char input3[128];
    unsigned int p;
    unsigned int q;
    unsigned int e;

_start:
    printf("\n        ---Programa de algoritmo RSA [1,024 bits]---         \n");
    
    printf("Ingrese el número primo 'p': ");
    fgets(input1, sizeof(input1), stdin);
    input1[strcspn(input1, "\n")] = '\0';

    printf("Ingrese el número primo 'q': ");
    fgets(input2, sizeof(input2), stdin);
    input2[strcspn(input2, "\n")] = '\0';
    
    if (!is_uninteger(input1) || !is_uninteger(input2))
    { 
        printf("Alguno de los números ingresados ('p' ó 'q') no es entero.\n"); 
        goto _start;
    }
    else 
    {
        p = (unsigned int) strtoul(input1, NULL, 10);
        q = (unsigned int) strtoul(input2, NULL, 10);
    }
    
    // Verify if the number p and q given are prime
    if (is_prime(p) && is_prime(q))
    {

        int const n = p*q;
        int phi_n = phi(p, q);

        while(1)
        {
            printf("Elija un número primo 'e' divisor y menor que phi(n): ");
            fgets(input3, sizeof(input3), stdin);
            input3[strcspn(input3, "\n")] = '\0';

            if (!is_uninteger(input3))
            {
                printf("\nEl número 'e' ingresado no es un entero.\n");
                continue;
            }

            e = (unsigned int) strtoul(input3, NULL, 10);

            if (is_prime(e) && e < phi_n && phi_n%e == 0)
            {
            _choice:
                printf("¿Qué desea hacer: o (C) Cifrar o (D) Descifrar o (S) Salir? ");
                char choice;
                scanf("%c", &choice);
                getchar();

                if (choice == 'C')
                {
                    unsigned int c = rsa_cipher(e, n);
                    if (c == -1) goto _choice;
                    printf("\n[Valor de c]: El identificador del mensaje cifrado es %u \n", c);
                    goto _choice;
                }
                else if (choice == 'D')
                {
                    unsigned int c = rsa_cipher(e, n);
                    if (c == -1) goto _choice;
                    unsigned int m = rsa_uncipher(c, e, n, phi_n);
                    printf("\n[Valor de m]: El identificador del mensaje original es %u \n", m);
                    goto _choice;
                }
                else if (choice == 'S') exit(0);
                else
                {
                    printf("\nIngrese una opción válida, opción ingresada no válida.\n");
                    goto _choice;
                }
            }
            else
            {
                printf("\nEl número 'e' que se ingresó no es un primo o es negativo y no menor que phi(n), vuelva a intentarlo por favor.\n");
                continue;
            }
        }

    }
    else
    {
        printf("\nIngrese número que sean válidos: %u ó %u no son primos y son negativos.\n", p, q);
        goto _start;
    }
}