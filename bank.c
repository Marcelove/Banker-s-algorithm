#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "b.h"

int errorr;

FILE *commands;
FILE *customer;
FILE *resultado;

void updateneed(int **maximum, int **allocation, int **need, int clientes, int limite)//atualizar o need
{
    int a, b;

    for(a = 0; a<clientes; a++){
        
        for(b=0; b<limite; b++){
            need[a][b] = maximum[a][b] - allocation[a][b];
        }
        
    }
}

int isDigit(char a)//indentificar digitos
{

        if ((a == '0') || (a == '1') || (a == '2') || (a == '3') || (a == '4') || (a == '5') || (a == '6') || (a == '7') || (a == '8') || (a == '9'))
                return 1;
        else
                return 0;
}



void pegadigitos(FILE *fp, int **matriz, int clientes, int limite)//extrair digitos
{
        int clienteAtual = 0, limiteAtual = 0, digitoAtual = 0;
        char c;

        for (c = getc(fp); c != EOF; c = getc(fp)){

                //printf("Estou lendo o %c\n", c);

                if (c == '\n'){
                        matriz[clienteAtual][limiteAtual] = digitoAtual;
                        //printf("peguei o %d\n", digitoAtual);

                        digitoAtual = 0;
                        clienteAtual++;
                        limiteAtual = 0;
                }

                else if (isDigit(c)){
                        digitoAtual = (digitoAtual *10) + (c - '0');
                }

                else if (c == ','){

                        matriz[clienteAtual][limiteAtual] = digitoAtual;

                        //printf("peguei o %d\n", digitoAtual);

                        digitoAtual = 0;
                        limiteAtual++;
                }

                else {
                        printf("Error: wrong format at customer.txt.\n");
                        exit(1);
                }
               
        }

       matriz[clienteAtual][limiteAtual] = digitoAtual;
}

void printaMatriz(int **maximum, int **allocation, int **need, int clientes, int limite)//printa as matrizes
{
        int a, b, yesirrr = limite*2, yesirrr2 = limite*2;

        //| ALLOCATION | NEED\n");

        fprintf(resultado,"MAXIMUM");

        for(a=0; a< (limite*2) -7; a++)
                fprintf(resultado," ");

        fprintf(resultado,"| ");

        fprintf(resultado,"ALLOCATION");

        for(a=0; a< (limite*2) -10; a++)
                fprintf(resultado," ");

        fprintf(resultado,"| ");

        fprintf(resultado,"NEED\n");

        for (a=0; a < clientes; a++){

                for(b=0; b < limite; b++){

                        fprintf(resultado,"%d ", maximum[a][b]);
                }

                while(yesirrr < 7){
                        fprintf(resultado," ");
                        yesirrr++;
                }
                yesirrr = limite*2;

                fprintf(resultado,"| ");

                for(b=0; b < limite; b++){

                        fprintf(resultado,"%d ", allocation[a][b]);
                }

                while(yesirrr2 < 10){
                        fprintf(resultado," ");
                        yesirrr2++;
                }
                yesirrr2 = limite*2;

                fprintf(resultado,"| ");

                for(b=0; b < limite; b++){

                        fprintf(resultado,"%d ", need[a][b]);
                }



                fprintf(resultado,"\n");
        }
}

int lines (FILE *fp)//retornar o número de clientes
{
        int count = 0;
        int prev = 0;
        char c;

        for (c = getc(fp); c != EOF; c = getc(fp)){
                if (c == '\n'){

                        count ++;
                        prev = 0;
                }

                else {
                        prev = 0;       
                }

                if (prev)
                        count ++;
        }
        return count;
}

void updateavailable(int **allocation, int *resources, int clientes, int limite)//atualizar recursos
{
        int a, b;
        int acumulador[limite];

        for(a=0; a<limite; a++){
                acumulador[a] = 0;
        }

        for(b=0; b<limite; b++){

                for(a=0; a<clientes; a++){
                        acumulador[b] += allocation[a][b];
                }
        }

        for(a=0; a<limite; a++){
                resources[a] -= acumulador[a];
        }

}

void verifyavailable(int **allocation, int *resources, int clientes, int limite)//atualizar recursos
{
        int a, b;
        int acumulador[limite];

        for(a=0; a<limite; a++){
                acumulador[a] = 0;
        }

        for(b=0; b<limite; b++){

                for(a=0; a<clientes; a++){
                        acumulador[b] += allocation[a][b];
                }
        }

        for(a=0; a<limite; a++){
                if ((resources[a] - acumulador[a]) < 0){
                        //printf("%d menos %d deu ruim\n",resources[a] , acumulador[a] );
                        errorr = 2;
                }
        }



}

int takeRequest(FILE *fp, int **allocation, int clientes, int limite, int *duplicatas)//realizar request
{
        int clienteAtual = 0, limiteAtual = 0, digitoAtual = 0, passada=0, a;
        char c;

        for (c = getc(fp); c != EOF; c = getc(fp)){

                if (c == '\n'){
                        allocation[clienteAtual][limiteAtual] += digitoAtual;
                        duplicatas[passada-1] = digitoAtual;

                        return clienteAtual;
                       
                }

                else if (isDigit(c)){

                        if(passada == 0)
                                clienteAtual = (clienteAtual *10) + (c - '0');
                        else
                                digitoAtual = (digitoAtual *10) + (c - '0');
                }

                else if (c == ' '){

                        if (passada == 0){
                                passada++;
                        }

                        else if(passada > limite){
                                printf("Error: mismatched number of arguments.\n");
                                exit(1);
                        }

                        else{

                                allocation[clienteAtual][limiteAtual] += digitoAtual;
                                duplicatas[passada-1] = digitoAtual;

                                digitoAtual = 0;
                                limiteAtual++;
                                passada++;
                        }
                }

                else {
                        printf("Error: wrong format at customer.txt.\n");
                        errorr=1;
                        return 0;
                }
               
        }

        allocation[clienteAtual][limiteAtual] += digitoAtual;
        duplicatas[passada-1] = digitoAtual;
       
        return clienteAtual;
}

int takeRelease(FILE *fp, int **allocation, int clientes, int limite, int *duplicatas)//realizar release
{
        int clienteAtual = 0, limiteAtual = 0, digitoAtual = 0, passada=0, a;
        char c;

        for (c = getc(fp); c != EOF; c = getc(fp)){

                if (c == '\n'){
                        allocation[clienteAtual][limiteAtual] -= digitoAtual;
                        duplicatas[passada-1] = digitoAtual;

                        return clienteAtual;
                       
                }

                else if (isDigit(c)){

                        if(passada == 0)
                                clienteAtual = (clienteAtual *10) + (c - '0');
                        else
                                digitoAtual = (digitoAtual *10) + (c - '0');
                }

                else if (c == ' '){

                        if (passada == 0){
                                passada++;
                        }

                        else if(passada > limite){
                                printf("Error: mismatched number of arguments.\n");
                                errorr=1;
                                return 0;
                        }

                        else{

                                allocation[clienteAtual][limiteAtual] -= digitoAtual;
                                duplicatas[passada-1] = digitoAtual;
                                
                                digitoAtual = 0;
                                limiteAtual++;
                                passada++;
                        }
                }

                else {
                        printf("Error: wrong format at customer.txt.\n");
                        errorr=1;
                        return 0;
                }
               
        }
       
}

void verifysafety (int **need, int *resources, int clientes, int limite)
{
    int a, b, count=0;

    for(a=0; a<clientes; a++){

        for(b=0; b<limite; b++){

            if((need[a][b] - resources[b]) <=0)
                count++;
        }

        if (count == limite)
            return;

        count = 0;
    }

    errorr = 5;

}

void verifymaximum(int **allocation, int **maximum, int clientes, int limite)
{
    int a, b;

    for(a=0; a<clientes; a++){

        for(b = 0; b < limite; b ++){

            if(allocation[a][b] > maximum[a][b]){
                errorr = 7;
                return;
            }
        }
    }
}

int main(int argc, char * argv[])
{


        resultado = fopen("result.txt", "w");

        errorr = 0;

        char c;

        int **maximum;
        int **allocation;
        int **need;

        //------------------------
        int **allocation2;
        //------------------------


        int clienteAtual;

        int resourcesFlag[argc-1];
        int resources[argc-1];
        int duplicatas[argc-1];

        int a, b, limite = argc, clientes;

        //Register resources
        for (a=1; a < limite; a++){ 
                resources[a-1] = atoi(argv[a]);
                resourcesFlag[a-1] = resources[a-1];
        }

        limite = argc-1;//update resources range

        customer = fopen("customer.txt", "r");
        clientes = lines(customer)+1; //ler numero de clientes 
        fclose(customer);

        maximum = malloc(clientes * sizeof * maximum);
        for (a=0; a<clientes; a++)
        {
                maximum[a] = malloc(clientes* sizeof * maximum[a]);
        }

        allocation = malloc(clientes * sizeof * allocation);
        for (a=0; a<clientes; a++)
        {
                allocation[a] = malloc(clientes * sizeof * allocation[a]);
        }

        need = malloc(clientes * sizeof * need);
        for (a=0; a<clientes; a++)
        {
                need[a] = malloc(clientes * sizeof * need[a]);
        }

        //---------------------------------------

        allocation2 = malloc(clientes * sizeof * allocation2);
        for (a=0; a<clientes; a++)
        {
                allocation2[a] = malloc(clientes * sizeof * allocation2[a]);
        }

        //---------------------------------------

        customer = fopen("customer.txt", "r");
        pegadigitos(customer, maximum, clientes, limite);//coletar os maximos e jogar na matriz
        fclose(customer);

        updateneed(maximum, allocation, need, clientes, limite);
        
        commands = fopen("commands.txt", "r");

        updateavailable(allocation, resources, clientes, limite);

        int request = 0;
        int release = 0;

        while((c = getc(commands)) != EOF){

                updateneed(maximum, allocation, need, clientes, limite);

                if (c== 'R'){
                        request++;
                        release++;
                }

                else if ((c == 'Q') && (request != 0)){
                        request = 2;
                        release = 0;
                }

                else if ((c == 'L') && (release != 0)){
                        request = 0;
                        release = 2;
                }

                else if (request == 2){

                        for(a=0; a<clientes; a++){
                                for(b=0; b<limite; b++){
                                        allocation2[a][b] = allocation[a][b];
                                }
                        }

                        //takeRequest(commands, matriz, clientes, limite, resources);
                        clienteAtual = takeRequest(commands, allocation, clientes, limite, duplicatas);

                        //verifica se os recursos disponíveis ficariam OK
                        verifyavailable(allocation, resourcesFlag, clientes, limite);

                        if(errorr == 0){

                                //reseta resources
                                for(a=0; a<limite; a++){
                                        resources[a] = resourcesFlag[a];

                                }

                                updateavailable(allocation, resources, clientes, limite);
                                updateneed(maximum, allocation, need, clientes, limite);

                                verifysafety(need, resources, clientes, limite);

                                if (errorr == 5){//The customer 0 request 0 2 0 was denied because result in an unsafe state
                                    fprintf(resultado,"The customer %d request ", clienteAtual);

                                    for(a=0; a<limite; a++){
                                        fprintf(resultado,"%d ", duplicatas[a]);
                                    }

                                    fprintf(resultado,"was denied because result in an unsafe state\n");

                                    for(a=0; a<clientes; a++){
                                        for(b=0; b<limite; b++){
                                                allocation[a][b] = allocation2[a][b];
                                        }
                                    }

                                }

                                else{

                                        verifymaximum(allocation, maximum, clientes, limite);

                                        if(errorr == 7){

                                                fprintf(resultado,"The customer %d request ", clienteAtual);

                                                for(a=0; a<limite; a++){
                                                        fprintf(resultado,"%d ", duplicatas[a]);
                                                }

                                                fprintf(resultado,"was denied because exceed its maximum allocation\n");

                                                for(a=0; a<clientes; a++){
                                                        for(b=0; b<limite; b++){
                                                                allocation[a][b] = allocation2[a][b];
                                                        }
                                                }
                                        }

                                        else{
                                                fprintf(resultado,"Allocate to customer %d the resources ", clienteAtual);

                                                for(a=0; a<limite; a++){
                                                        fprintf(resultado,"%d ", duplicatas[a]);
                                                }

                                                fprintf(resultado,"\n");
                                        }


                                }
                        }
                        else{

                                if(errorr == 2){
                                        fprintf(resultado,"The resources ");

                                        for(a=0; a<limite; a++){
                                                fprintf(resultado,"%d ", resources[a]);
                                        }

                                        fprintf(resultado,"are not enough to costumer %d request ", clienteAtual);

                                        for(a=0; a<limite; a++){
                                                fprintf(resultado,"%d ", duplicatas[a]);
                                        }

                                        fprintf(resultado,"\n");


                                }

                                for(a=0; a<clientes; a++){
                                        for(b=0; b<limite; b++){
                                                allocation[a][b] = allocation2[a][b];
                                        }
                                }
                        
                        }

                        request = 0;
                        errorr=0;
                }

                else if (release == 2){
                        //takeRelease(commands, matriz, clientes, limite);
                        clienteAtual = takeRelease(commands, allocation, clientes, limite, duplicatas);

                        if(errorr == 0){

                                fprintf(resultado,"Release from customer %d the resources ", clienteAtual);

                                for(a=0; a<limite; a++){
                                        fprintf(resultado,"%d ", duplicatas[a]);
                                }

                                fprintf(resultado,"\n");
                        }

                        release = 0;
                        errorr=0;
                }

                else if (c == '*'){

                        printaMatriz(maximum, allocation, need, clientes, limite);

                        
                        fprintf(resultado,"AVAILABLE ");
                        
                        for(a=0; a<limite; a++){
                                fprintf(resultado,"%d ", resources[a]);
                                //resources[a] = resourcesFlag[a];
                        }


                       fprintf(resultado,"\n");
                       
                }
                
                
        }

        fclose(commands);
        fclose(resultado);

}