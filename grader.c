#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * 
 * name: readLine
 * @return char* newStr
 * 
 * Takes in the next line of input and returns a pointer to a string
 * containing that input.
 */
char* readLine(){
	int i = 0, size = 2, ch = 0;
	char* newStr = NULL;
	
	newStr = malloc(2*sizeof(char));
	
	if(!newStr){
		return NULL;
	}
	
	*newStr = '\0';
	
	while( ( ch = getchar()) != EOF && ch != '\n' ){		
		newStr[i++] = ch;
		
		if(i == size){
			newStr = realloc(newStr, sizeof(char)*(size+=16));
            if(!newStr){
				printf("Problem allocating dynamic string for input");
				return NULL;
			}
		}
			
	}
	
	newStr[i++] = '\0';		
	
	return newStr;
}

/*
 * 
 * name: main
 * @return int exitCode
 * 
 * The main method of execution
 * 
 */

int main(){	
	char* answerKeyInput = NULL;
	
	printf("Enter the answer key: ");
	
	answerKeyInput = readLine();
	
	printf("\n\n%s", answerKeyInput);
	
	free(answerKeyInput);
	
	return 0;
}
