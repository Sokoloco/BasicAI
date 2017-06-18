/*
 * BasicAI.c
 * 
 * Copyright 2016 Luis <luis@luis-VirtualBox>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 
 
/*             Librerias Incluidas
 * 
 * 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*                  Estructura del arbol
 * 
 * 
 * 
 */
struct Conocimiento{ //estructura de arbol cuaternario (con cuatro hojas)
	struct Conocimiento* arriba; //Hoja que apnta a hoja pasada, si nulo primera hoja
	char pregunta[512]; //contiene la pregunta especificade esa hoja, si vacio es una hoja linker, osea una hoja que se utiliza para apuntar a preguntas extras
	char respuesta[200];
	struct Conocimiento* primero; //primer paso a hoja 1
	struct Conocimiento* segundo;//apunta a segunda hoja
	struct Conocimiento* tercero; //tercera hoja
	struct Conocimiento* cuarto; //y por ultimo a cuarta
};
/*                Definicion De Tipos
 * 
 * 
 * 
 */
typedef struct Conocimiento conoce; //tipo definido de estructura
typedef conoce* ptrConoce; //tipo definido de puntero a/de arbol

/*                 Declaracion de funciones
 * 
 * 
 */
void cargartxt(); //funcion encargada de cargar el inicio (si hay uno) de un .txt
ptrConoce crearHoja(char nuevoArg[512], char nuevaResp[200]); //inserta una nueva hoja a el arbol, recibe la pregunta
int leeractual();
void jugarD();
void agregarNuevaHoja(char nuevoArg[512],char nuevaPre[200]); //agrega una nueva hoja a el arbol, recibe la hoja actual  su pregunta
void cargarHojaTxt(ptrConoce *hjaAct,int caso,char cual[512],char resp[200]); //Encargado de leer el TXT y crear las hojas una por una
void crearTxt(); //Define si se va a crear el nuevo archivo txt o no, si no se crea nada mas se lee.
void savetxt(); //guarda el txt, con el arbol actual
/* Variables Globales
 * 
 */
 ptrConoce primerHoja = NULL;//"cabeza" del arbol, iniciada en nulo cada vez.
 char pregBlanc[3]; //variable de pregunta blanca encargada de cargar preguntas en blanco(nulas)
 //las preguntas en blanco estan encargadas de poner preguntas sin hijos juntas
/*
 * 
 * 
 * 
 *#############################################MAIN########################################################### 
 * 
 * 
 * 
 */
int main(){ //funcion main, corre al inicio
	strcpy(pregBlanc," "); //primer pregunta debe estar en blanco para que esta se regisre como nula
	primerHoja = crearHoja(pregBlanc,pregBlanc); //le pone a la primer pregunta en nula
	crearTxt();//funcion con la intencion de crear o nada mas leer el txt dependiente de si este existe o no
	cargartxt(); //se cambia la cabeza a por lo que se tenga en el .txt
	char agregar[3], nuevPre[512], nuevRes[200];//Tipo string para la creacion de nuevos arboles y condiciones de parada
	while(primerHoja->arriba != NULL){
		primerHoja = primerHoja->arriba;
	}
	int parar = 0;
	parar = 0;
	while(parar == 0){
		parar = leeractual();
	}
	if(parar == 2){
		printf("la respuesta es %s (s = si)",primerHoja->respuesta);
		scanf("%s",agregar);
		if(agregar[0] == 115){
			printf("La computadora adivino su respuesta!\n");
			jugarD();
		}
	}
	printf("La computadora perdio\nProfavor ingresar la pregunta: ");
	scanf("%s",nuevPre);
	printf("\nY su respuesta: ");
	//getchar();
	//getchar();
	//getchar();
	scanf("%s",nuevRes);
	agregarNuevaHoja(nuevPre,nuevRes);
	jugarD(); //retorna cero, ya que main es de tipo int
	return 0;
}
/*
 * Funciones de creacion de arbol
 * 
 * 
 * 
 */
void jugarD(){
	char jugar[3];
	printf("desea jugar de nuevo? (s = si)");
	scanf("%s",jugar);
	if(jugar[0] == 115)
		main();
	else
		printf("Gracias por jugar");
	savetxt();
}
ptrConoce crearHoja(char nuevoArg[512], char nuevaResp[200]){ //funcion llamada por agregar hoja, encargada de insertar una nueva hoja, nuevoArg seria la pregunta
	ptrConoce ptr; //puntero desechable que va a obtener los valores y pasarlos a la nueva hoja
	ptr = (ptrConoce)malloc(sizeof(conoce)); //reserva el tamanho de la hoja, para que esta no se borre una vez completada la funcion
	char linea[5];
	char lineaP[5];
	strcpy(linea,"\n");
	strcpy(lineaP,"? \n");
	strcpy(ptr->pregunta,strcat(nuevoArg,linea)); // nuevoAeg(la pregunta se le asigna a el puntero de pregunta(hoja actual)
	strcpy(ptr->respuesta,strcat(nuevaResp,lineaP));
	ptr->primero = ptr->segundo = ptr->tercero = ptr->cuarto = ptr->arriba = NULL; //se ponene todas los punteros a diferentes hojas en nulo
	return ptr; //se retona a el puntero
}
void agregarNuevaHoja(char nuevoArg[512],char nuevaPre[200]){ //encargado de agreagar una nueva hoja
	ptrConoce nuevo; //puntero desechable que va a contener la nueva hoja
	nuevo = crearHoja(nuevoArg,nuevaPre); //crea el nuevo nodo a insertar
	nuevo->arriba = primerHoja; //asigna a la hoja actual como nodo de arriba
	if(primerHoja->primero == NULL) //caso uno si en la hoja actual no se tiene nada
		primerHoja->primero = nuevo;
	else if(primerHoja->segundo == NULL)//caso que se tenga un elemento
		primerHoja->segundo = nuevo;
	else if(primerHoja->tercero == NULL)//caso que se tengasn dos elementos
		primerHoja->tercero = nuevo;
	else if(primerHoja->cuarto == NULL)//caso que se tengan 
		primerHoja->cuarto = nuevo;
	else{ //caso adonde todas las hojas ya esten ocupada
		nuevo = crearHoja(pregBlanc,pregBlanc);//crea un nodo vacio, el cual va a apuntar a todos los posibles extras que hayan
		nuevo->arriba = primerHoja; //asigna elpuntero anterior (de vacio) a hoja actual
		ptrConoce cambio = primerHoja->primero; //se hace un cambio el primero que estaba(hoja acutal) se pone de primero(vacio)
		primerHoja->primero = nuevo; // vacio se pone de primero
		primerHoja = nuevo; //la hoja actual ahora se convierte en laque esta vacia
		agregarNuevaHoja(cambio->pregunta,cambio->respuesta); //cambio(pimero de la hoja pasada se agrega a hoja vacio(nuevo)
		agregarNuevaHoja(nuevoArg,nuevaPre); //y el nuevo argumento se agrega a (vacia)
	}
	printf("%s, %s\n",nuevo->pregunta,nuevo->respuesta);
}

/*                         Funciones del .TXT
 * 
 * 
 * 
 * 
 */
 int leeractual(){
	char seguir[3];
	if(primerHoja->pregunta == pregBlanc){
		int paso = 0;
		primerHoja= primerHoja->primero;
		paso = leeractual();
		if (paso == 1)
			return 0;
		else
			primerHoja = primerHoja->arriba;
	}
	if(primerHoja->primero != NULL){
		printf("%s (s = si)",primerHoja->primero->pregunta);
		scanf("%s",seguir);
		if(seguir[0]== 115){
			primerHoja = primerHoja->primero;
			leeractual();
		}
	}
	if(primerHoja->segundo != NULL){
		printf("%s (s = si)",primerHoja->segundo->pregunta);
		scanf("%s",seguir);
		if(seguir[0] == 115){
			primerHoja = primerHoja->segundo;
			leeractual();
		}
	}
	if(primerHoja->tercero != NULL){
		printf("%s (s = si)",primerHoja->tercero->pregunta);
		scanf("%s",seguir);
		if(seguir[0] == 115){
			primerHoja = primerHoja->tercero;
			leeractual();
		}
	}
	if(primerHoja->cuarto != NULL){
		printf("%s (s = si)",primerHoja->cuarto->pregunta);
		scanf("%s",seguir);
		if(seguir[0] == 115){
			primerHoja = primerHoja->cuarto;
			leeractual();
		}
	}
	if((primerHoja->primero == NULL) && (primerHoja->segundo == NULL) && (primerHoja->tercero == NULL) && (primerHoja->cuarto == NULL))
		return 2;
	return 1;
 }
 
 void crearTxt(){
	 FILE *read = fopen("Conocimiento.txt", "r");
	 if(read == NULL){
		 FILE *create = fopen("Conocimiento.txt", "w");
		 fprintf(create,"1\nEs del reino animal?\nReino animal?\n5\n\n\n2\nEs del reino vegetal?\nReino Vegetal?\n5\n\n\n3\nEs del reino mineral?\nReino mineral?\n");
		 fclose(create);
		 return;
	 }
	 fclose(read);
}

void cargarHojaTxt(ptrConoce *hjaAct,int caso,char cual[512],char resp[200]){
	if(caso == 53)
		*hjaAct = (*hjaAct)->arriba;
	else{
		ptrConoce nuevaHoja = crearHoja(cual,resp);
		switch(caso){
		case 49:
			(*hjaAct)->primero = nuevaHoja;
			//printf("1");
			break;
		case 50:
			(*hjaAct)->segundo = nuevaHoja;
			//printf("2");
			break;
		case 51:
			(*hjaAct)->tercero = nuevaHoja;
			//printf("3");
			break;
		case 52:
			(*hjaAct)->cuarto = nuevaHoja;
			//printf("4");
			break;
	}
	nuevaHoja->arriba = *hjaAct;
	*hjaAct = nuevaHoja;
	}
}

void cargartxt(){//carga el .txt en un arbol https://www.youtube.com/watch?v=9KVqEpiKxTY
	char casoA[5];
	char buff[512];
	char respues[200];
	FILE *f = fopen("Conocimiento.txt", "r");
	while(fgets (casoA,5,f)){
		fgets (buff, 510, f);
		fgets (respues, 200, f);
		cargarHojaTxt(&primerHoja,casoA[0],buff,respues);
	}
	fclose(f);
}

void savetxt(){
	FILE *f = fopen("Conocimiento.txt", "w");
	for(int i = 0;i<3;i++){
		while(primerHoja->arriba != NULL){
			primerHoja = primerHoja->arriba;
		}
		if(i == 1){
			primerHoja = primerHoja->segundo;
			fprintf(f,"2\n%s%s",primerHoja->pregunta,primerHoja->respuesta);
		}
		else if(i == 2){
			primerHoja = primerHoja->tercero;
			fprintf(f,"3\n%s%s",primerHoja->pregunta,primerHoja->respuesta);
		}
		while(primerHoja->primero != NULL){
			primerHoja = primerHoja->primero;
			fprintf(f,"1\n%s%s",primerHoja->pregunta,primerHoja->respuesta);
		}
		while(1){
			if(primerHoja->segundo != NULL)
				fprintf(f,"2\n%s%s5\n0\n0\n",primerHoja->segundo->pregunta,primerHoja->segundo->respuesta);
			if(primerHoja->tercero != NULL)
				fprintf(f,"3\n%s%s5\n0\n0\n",primerHoja->tercero->pregunta,primerHoja->tercero->respuesta);
			if(primerHoja->cuarto != NULL)
				fprintf(f,"4\n%s%s5\n0\n0\n",primerHoja->cuarto->pregunta,primerHoja->cuarto->respuesta);
			primerHoja = primerHoja->arriba;
			fprintf(f,"5\n0\n0\n");
			if(primerHoja->arriba == NULL)
				break;
		}
	}
	fclose(f);
}


