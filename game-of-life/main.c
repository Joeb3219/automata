#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int width = 40;
int height = 40;

typedef enum CellType{
	LIFE, EMPTY
} Cell;

char getDisplaySymbol(Cell type){
	switch(type){
		case LIFE:
			return '*';
		case EMPTY:
			return ' ';
		default:
			return 'N';
	}
}

Cell getCell(Cell *cells, int x, int y){
	if(x >= width || y >= height || x < 0 || y < 0) return EMPTY;
	return cells[y * width + x];
}

void setCell(Cell *cells, int x, int y, Cell value){
	if(x >= width || y >= height || x < 0 || y < 0) return;
	cells[y * width + x] = value;
}

void automate(Cell *cells){
	Cell newCells[width * height];
	int x, y;

	for(x = 0; x < width; x ++){
		for(y = 0; y < height; y ++){
			int neighborCount = 0;
			int xDelta, yDelta;
			for(xDelta = -1; xDelta <= 1; xDelta ++){
				for(yDelta = -1; yDelta <= 1; yDelta ++){
					if(yDelta == 0 && xDelta == 0) continue;
					if(getCell(cells, x + xDelta, y + yDelta) == LIFE) neighborCount ++;
				}
			}
			if(getCell(cells, x, y) == LIFE){
				if(neighborCount <= 1) newCells[y * width + x] = EMPTY;
				else if(neighborCount == 2 || neighborCount == 3) newCells[y * width + x] = LIFE;
				else if(neighborCount > 3) newCells[y * width + x] = EMPTY;
			}else{
				if(neighborCount == 3) newCells[y * width + x] = LIFE;
				else newCells[y* width + x] = EMPTY;
			}
		}
	}

	for(x = 0; x < width * height; x ++){
		cells[x] = newCells[x];
	}
}

void displayBoard(Cell *cells){
	int x = 0, y = 0;
	for(x = 0; x < width; x ++){
		for(y = 0; y < height; y++){
			printf("%c", getDisplaySymbol(getCell(cells, x, y)));
		}
		printf("\n");
	}
}

void clearScreen(){
	system("clear");
}

int main(int argc, char **argv){

	Cell *cells = calloc(sizeof(Cell), width * height);
	int numSteps = 100;
	int step = 0;

	int i = 0;
	for(i = 0; i < width * height; i ++) cells[i] = ((rand() % 100) > 67) ? LIFE : EMPTY;

	while(step < numSteps){
		clearScreen();
		displayBoard(cells);
		sleep(1);
		automate(cells);
		step ++;
	}

	return 0;
}
