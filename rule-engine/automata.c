#include <stdlib.h>
#include <stdio.h>

typedef struct{
	char pattern;
	char result;
} Rule;

Rule ** rules;
int numRules = 8;

void printCells(char *cells, int *currentLine, int width){
	int i = 0;
	printf("[%04d]: ", *currentLine);
	for(i = 0; i < width; i ++){
		if(cells[i] == 1) printf("#");
		else printf(" ");
	}
	printf("\n");
}

char* getEmptyCells(int width){
	char* cells = calloc(sizeof(char), width);
	return cells;
}

void automate(char *cells, int width){
	char *newCells = getEmptyCells(width);
	int i, pos;
	for(pos = 0; pos < width; pos ++){
		char pattern;
		if(pos <= 1) pattern = cells[pos]; // Get the left most cells padded with 0.
		else if(pos == width - 1) pattern = cells[pos] << 2;
		else if(pos == width - 2) pattern = (cells[pos - 1] << 2) | (cells[pos] << 1);
		else pattern = (cells[pos - 2] << 2) | (cells[pos - 1] << 1) | (cells[pos]);
		for(i = 0; i < numRules; i ++){
			if(rules[i]->pattern == pattern){
				newCells[pos] = rules[i]->result;
			}
		}
	}
	for(i = 0; i < width; i ++){
		cells[i] = newCells[i];
	}
	free(newCells);
}

void runOneLine(char *cells, int width, int *currentLine){
	automate(cells, width);
	*currentLine = (*currentLine) + 1;
	printCells(cells, currentLine, width);
}

void runAllLines(char *cells, int width, int height, int *currentLine){
	while(*currentLine < height){
		runOneLine(cells, width, currentLine);
	}
}

int processInput(char input, char *cells, int width, int maxHeight, int *currentLine){
	printf("The current line is %d\n", *currentLine);
	if(input == 's') return -1;	// User wants to stop, so let's.
	else if((*currentLine) >= maxHeight) return -1; // User has hit last line.
	else if(input == 'o') runOneLine(cells, width, currentLine);
	else if(input == 'a') runAllLines(cells, width, maxHeight, currentLine);
	else return -1;		// Ran none of the commands, so we have an error.
	return 0;		// Exit successfully.
}

Rule **createRules(){
	Rule **rules = malloc(sizeof(Rule*) * numRules);
	int i;

	for(i = 0; i < 8; i ++) rules[i] = malloc(sizeof(Rule));

	rules[0]->pattern = 0b111;
	rules[0]->result = 0;
	rules[1]->pattern = 0b110;
	rules[1]->result = 1;
	rules[2]->pattern = 0b101;
	rules[2]->result = 1;
	rules[3]->pattern = 0b100;
	rules[3]->result = 0;
	rules[4]->pattern = 0b011;
	rules[4]->result = 1;
	rules[5]->pattern = 0b010;
	rules[5]->result = 1;
	rules[6]->pattern = 0b001;
	rules[6]->result = 1;
	rules[7]->pattern = 0b000;
	rules[7]->result = 0;
	return rules;
}

int main(int argc, char **argv){
	if(argc != 3){
		printf("Usage: ./automata width height\n");
		return -1;
	}

	rules = createRules();

	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	int *currentLine;
	*currentLine = 0;
	char input;

	printf("Creating automaton with a width of %d and height of %d\n", width, height);

	char *cells = getEmptyCells(width);
	cells[width / 2] = 1;

	printf("Next line? [o = one line, a = all lines, s = stop]\n");
	while( (input = getchar()) != 's' ){
		if(processInput(input, cells, width, height, currentLine) == -1) return 0;		// -1 indicates the program is finished execution.
		printf("Next line? [o = one line, a = all lines, s = stop]\n");
		getchar();	// Burn EOF
	}

	return 0;

}
