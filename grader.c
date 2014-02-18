#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef int bool;
#define true 1
#define false 0

typedef struct question{
	int correct;
	int incorrect;
	int unanswered;
	int invalid;
} question;

typedef struct student{
	char* answers;
	char* name; 
	int correct;
    int incorrect;
    int invalid;
} student;


int numQuestions = 0, numStudents = 0;
char* answers;
student** students;
question* questions;

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
				fprintf(stderr, "Problem allocating dynamic string for input");
				return NULL;
			}
		}
			
	}
	
	newStr[i++] = '\0';		
	
	return newStr;
}

bool processAnswerKey(char* answerKey){
	int len = strlen(answerKey);
	bool ret = 1;
	int i;
	
	for(i = 0; i < len; i++){
		char ch = answerKey[i];
		if(ch < 'a' || ch > 'e'){
			ret = 0;
			fprintf(stderr, "Error - answer '%c' to question %d is invalid", ch, i+1);
		}
		
		numQuestions++;
	}
	
	return ret;
}	

student** processStudents(){
	int capacity = 2;
	
	// List of students
	student** students = malloc(capacity*sizeof(student*));
	
	char* studLine;
	
	// As long as students are being entered
	while(strlen(studLine = readLine()) != 0){
		student* s = malloc(sizeof(student));
		
		// Process answers
		int i, invalid = 0, incorrect = 0, correct = 0;
		for(i = 0; i < numQuestions; i++){

				if((studLine[i] > 'e' || studLine[i] < 'a') && studLine[i] != ' '){
					invalid += 1;
				
					questions[i].invalid++;
				}
				else if(studLine[i] != answers[i]){
					incorrect += 1;
					if(studLine[i] != ' '){		
						questions[i].incorrect++;
					}
					else{
						questions[i].unanswered++;
					}
				}
				else{
					correct += 1;
					questions[i].correct++;
				}
					
				//printf("%c vs %c - %d correct", studLine[i], answers[i], correct);
									
		}
		
		s->invalid = invalid;
		s->incorrect = incorrect;
		s->correct = correct;
		
		char* answers = malloc(numQuestions * sizeof(char));
		strncpy(answers, studLine, numQuestions);
		
		s->answers = malloc(sizeof(char**));
		s->answers = answers;
		
		int nameLen = strlen(studLine-numQuestions);
		char* name = malloc( nameLen * sizeof(char));
		strncpy(name, studLine+numQuestions, nameLen);
		
		s->name = malloc(sizeof(char**));
		s->name = name;
		
		students[numStudents] = s;
		numStudents++;
		
		if(numStudents == capacity){
			students = realloc(students, sizeof(student *) * (capacity*=2));
		}
	}
	
	return students;
}

/*
 * 
 * name: main
 * @return int exitCode
 * 
 * The main method of execution
 * 
 */

int main(int argc, char **argv){
	if(argc < 2){
		fprintf(stderr, "No answer key specified");
		return 1;
	}
	
	answers = argv[1];
	
	bool validKey = processAnswerKey(answers);
	
	if( !validKey ){
		return 2;
	}
	
	int i;
	
	//Allocate question memory now that we know # of questions
	questions = malloc(numQuestions*sizeof(question));
	for(i = 0; i < numQuestions; i++){
		question q = questions[i];

		q.correct = 0;
		q.invalid = 0;
		q.incorrect = 0;
		q.unanswered = 0;
	}
	
	//Process student input
	students = processStudents();
	
	// get question num leading zeroes
	int questionLead = 0;
	int places = numQuestions / 10;
	while(places != 0){
		questionLead++;
		places /= 10;
	}
	
	for(i = 0; i < numQuestions; i++){
		question q = questions[i];
		printf("Question %0*d:  %d  %d  %d  %d\n", questionLead, i+1,  
			q.correct, q.incorrect, q.unanswered, q.invalid);
	}
	
	printf("\n");
	
	for(i = 0; i < numStudents; i++){
		student* s = students[i];
		printf("%s  %d  %d  %d  %s", s->answers, s->correct,
			s->incorrect, s-> invalid, s->name);
	}
	
	//int i;
	//for(i = 0; i < numStudents; i++){
	//	printf("%d correct", students[i]->correct);
	//}
		
	//char* answerKeyInput = NULL;
	
	//printf("Enter the answer key: ");
	
	//answerKeyInput = readLine();
	
	//printf("\n\n%s", answerKeyInput);
	
	//free(answerKeyInput);
	
	return 0;
}
