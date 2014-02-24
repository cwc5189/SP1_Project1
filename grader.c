#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef int bool;
#define true 1
#define false 0

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

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
student* students;
question* questions;
char* newStr;

/*
 * 
 * name: readLine
 * @return char* newStr
 * 
 * Takes in the next line of input and returns a pointer to a string
 * containing that input.
 */
char* readLine(char* in){
	int i = 0, size = 1, ch = 0;
	newStr = in;
	
	newStr = calloc(1, sizeof(char));
	
	if(!newStr){
		free(newStr);
		fprintf(stderr, "Memory allocation failed!");
		exit(3);
	}
	
	newStr[0] = '\0';
	
	while( ( ch = getchar()) != EOF && ch != '\n' ){		
		newStr[i++] = ch;
		
		if(i == size){
			
			newStr = realloc(newStr, sizeof(char)*(size+=16));
			
            if(!newStr){
				free(newStr);
				fprintf(stderr, "Memory allocation failed!");
				exit(3);
			}
		}
			
	}
	
	newStr[i++] = '\0';		
	
	return newStr;
}

/*
 * 
 * name: processAnswerKey
 * @return bool valid
 * 
 * Checks the answer key to make sure there are no invalid answers
 */
bool processAnswerKey(char* answerKey){
	int len = strlen(answerKey);
	char ch = 0;
	bool ret = 1;
	int i;
	
	for(i = 0; i < len; i++){
		ch = answerKey[i];
		if(ch < 'a' || ch > 'e'){
			ret = 0;
			fprintf(stderr, "Error - answer '%c' to question %d is invalid", ch, i+1);
		}
		
		numQuestions++;
	}
	
	return ret;
}	

/*
 * 
 * name: trim
 * 
 * Trims leading and trailing whitespace on a string
 */
char* trim(char* in){
	int i;
	for(i = 0; i < strlen(in); i++){
		if(!isspace(in[i]))
			break;
	}
	
	int e;
	for(e = strlen(in)-1; e > 0; e--){
		if(!isspace(in[e]))
			break;
	}
	
	if(e == strlen(in)-1){
		e++;
	}
	
	if(e > i){
		char* n = calloc((e-i+1), sizeof(char));
		strncpy(n, &in[i], (e-i));
		//n[e-i+1] = '\0';
		free(in);
		
		return n;
	}
	
	return in;
}

/*
 * 
 * name: processStudents
 * @return student* students
 * 
 * Reads in the list of students as input, checks their answers
 * and records as appropriately in student and question records.
 * Then upon end of input, it returns a list of students to main
 * function.
 */
student* processStudents(){
	int capacity = 2;
	
	// List of students
	student* students = calloc(capacity, sizeof(student));
	
	char* studLine = NULL;
	
	// As long as students are being entered
	while(strlen(studLine = readLine(studLine)) != 0){
		student s = students[numStudents];
		
		// Process answers
		int i, invalid = 0, incorrect = 0, correct = 0;
		for(i = 0; i < numQuestions; i++){

				if((studLine[i] > 'e' || studLine[i] < 'a') && studLine[i] != ' '){
					invalid += 1;
					incorrect += 1;
					questions[i].incorrect++;
					questions[i].invalid++;
				}
				else if(studLine[i] != answers[i]){
					incorrect += 1;
					if(studLine[i] == ' '){		
						questions[i].unanswered++;
					}
					
					questions[i].incorrect++;
					
				}
				else{
					correct += 1;
					questions[i].correct++;
				}
					
				//printf("%c vs %c - %d correct", studLine[i], answers[i], correct);									
		}
		
		s.invalid = invalid;
		s.incorrect = incorrect;
		s.correct = correct;
		
		s.answers = calloc(numQuestions, sizeof(char*));
		strncpy(s.answers, studLine, numQuestions);
			
		int nameLen = strlen(studLine)-numQuestions;

		s.name = calloc(nameLen, sizeof(char*));
		strncpy(s.name, &studLine[numQuestions], nameLen);
		s.name = trim(s.name);
		
		students[numStudents] = s;
		numStudents++;
		
		if(numStudents == capacity){
			students = realloc(students, sizeof(student) * (capacity*=2));
		}
		
		free(studLine);
	}
	
	return students;
}

/*
 * name: cleanup
 * 
 * cleans up used memory - specifically student names, answers, 
 * then the student and question arrays
 */ 
void cleanup(){
	int i;
	for(i = 0; i < numStudents; i++){
		free(students[i].name);
		free(students[i].answers);
	}
	
	free(students);
	free(questions);
	free(newStr);
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
	// Check for answer key
	if(argc < 2){
		fprintf(stderr, "No answer key specified");
		return 1;
	}
	
	answers = argv[1];
	
	// Check for valid answer key
	bool validKey = 0;
	validKey = processAnswerKey(answers);
	
	if( !validKey ){
		return 2;
	}
	
	// Generic for-loop i counter.
	int i;
	
	// Allocate/initialize questions now that we know # of questions
	questions = calloc(numQuestions, sizeof(question));
	for(i = 0; i < numQuestions; i++){
		question q = questions[i];

		q.correct = 0;
		q.invalid = 0;
		q.incorrect = 0;
		q.unanswered = 0;
	}
	
	// Process student input
	students = processStudents();
	
	// Get the leading zeroes for question numbers
	int questionLead = 1;
	int places = numQuestions / 10;
	while(places != 0){
		questionLead++;
		places /= 10;
	}
	
	// Find largest question answer type count number
	// and then get the max # of places
	int max = 0;
	
	for(i = 0; i < numQuestions; i++){
		question q = questions[i];

		max = MAX(max, q.correct);
		max = MAX(max, q.invalid);
		max = MAX(max, q.incorrect);
		max = MAX(max, q.unanswered);
	}
	
	int answersLead = 1;
	max = max / 10;
	
	while(max != 0){
		answersLead++;
		max /= 10;
	}
	
	// Find max # of places for student answer type counts
	max = 0;
	
	for(i = 0; i < numStudents; i++){
		student s = students[i];

		max = MAX(max, s.correct);
		max = MAX(max, s.invalid);
		max = MAX(max, s.incorrect);
	}
	
	int sAnswersLead = 1;
	max = max / 10;
	
	while(max != 0){
		sAnswersLead++;
		max /= 10;
	}
	
	// Print Question stats
	for(i = 0; i < numQuestions; i++){
		question q = questions[i];
		
		printf("Question %*d:  %*d  %*d  %*d  %*d\n", 
			questionLead, i+1,  
			answersLead, q.correct, 
			answersLead, q.incorrect, 
			answersLead, q.unanswered, 
			answersLead, q.invalid);		
	}
	
	printf("\n");
	
	// Print Student stats
	for(i = 0; i < numStudents; i++){
		student s = students[i];
		printf("%s  %*d  %*d  %*d  %s\n", 
		s.answers, 
		sAnswersLead, s.correct,
		sAnswersLead, s.incorrect, 
		sAnswersLead, s.invalid, 
		s.name);
	}
	
	// clean up memory
	cleanup();
	
	return 0;
}
