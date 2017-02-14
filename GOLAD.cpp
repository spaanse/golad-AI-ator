#include <stdio.h>
#include <utility>
#include <algorithm>

#define NumGenerations 3

struct MoveStruct{
	std::pair<int,int> Changed;
	//FirstSource and SecondSource don't matter if the location in Changed isn't empty
	std::pair<int,int> FirstSource;
	std::pair<int,int> SecondSource;
};

int Width;
int Height;
int BirthBM;
int SurviveBM;
int Board[20][20]; //Maxsize
bool MyTurn = true;
bool FirstPlayer = true;
MoveStruct Move;

int CalculateNextGens(int*** NextGenerations){
	for (int CurrentGen = 0;CurrentGen < (NumGenerations);CurrentGen++){
		for (int a = 0;a<Height;a++){
			for (int b = 0;b<Width;b++){
				int FirstCount = 0;
				int SecondCount = 0;
				int NoneCount = 0;
				if (a>0 and b>0 and NextGenerations[CurrentGen][a-1][b-1] == 1){FirstCount++;}
				if (a>0 and NextGenerations[CurrentGen][a-1][b] == 1){FirstCount++;}
				if (a>0 and b<Width-1 and NextGenerations[CurrentGen][a-1][b+1] == 1){FirstCount++;}
				if (b>0 and NextGenerations[CurrentGen][a][b-1] == 1){FirstCount++;}
				if (b<Width-1 and NextGenerations[CurrentGen][a][b+1] == 1){FirstCount++;}
				if (a<Height-1 and b>0 and NextGenerations[CurrentGen][a+1][b-1] == 1){FirstCount++;}
				if (a<Height-1 and NextGenerations[CurrentGen][a+1][b] == 1){FirstCount++;}
				if (a<Height-1 and b<Width-1 and NextGenerations[CurrentGen][a+1][b+1] == 1){FirstCount++;}

				if (a>0 and b>0 and NextGenerations[CurrentGen][a-1][b-1] == 2){SecondCount++;}
				if (a>0 and NextGenerations[CurrentGen][a-1][b] == 2){SecondCount++;}
				if (a>0 and b<Width-1 and NextGenerations[CurrentGen][a-1][b+1] == 2){SecondCount++;}
				if (b>0 and NextGenerations[CurrentGen][a][b-1] == 2){SecondCount++;}
				if (b<Width-1 and NextGenerations[CurrentGen][a][b+1] == 2){SecondCount++;}
				if (a<Height-1 and b>0 and NextGenerations[CurrentGen][a+1][b-1] == 2){SecondCount++;}
				if (a<Height-1 and NextGenerations[CurrentGen][a+1][b] == 2){SecondCount++;}
				if (a<Height-1 and b<Width-1 and NextGenerations[CurrentGen][a+1][b+1] == 2){SecondCount++;}

				if (a>0 and b>0 and NextGenerations[CurrentGen][a-1][b-1] == 3){NoneCount++;}
				if (a>0 and NextGenerations[CurrentGen][a-1][b] == 3){NoneCount++;}
				if (a>0 and b<Width-1 and NextGenerations[CurrentGen][a-1][b+1] == 3){NoneCount++;}
				if (b>0 and NextGenerations[CurrentGen][a][b-1] == 3){NoneCount++;}
				if (b<Width-1 and NextGenerations[CurrentGen][a][b+1] == 3){NoneCount++;}
				if (a<Height-1 and b>0 and NextGenerations[CurrentGen][a+1][b-1] == 3){NoneCount++;}
				if (a<Height-1 and NextGenerations[CurrentGen][a+1][b] == 3){NoneCount++;}
				if (a<Height-1 and b<Width-1 and NextGenerations[CurrentGen][a+1][b+1] == 3){NoneCount++;}

				if (NextGenerations[CurrentGen][a][b] == 0 and ((BirthBM & (1<<(FirstCount+SecondCount+NoneCount))) != 0)){
					if (FirstCount > SecondCount){NextGenerations[CurrentGen+1][a][b] = 1;}
					if (FirstCount == SecondCount){NextGenerations[CurrentGen+1][a][b] = 3;}
					if (FirstCount < SecondCount){NextGenerations[CurrentGen+1][a][b] = 2;}
				}
				else if (NextGenerations[CurrentGen][a][b] != 0 and ((SurviveBM & (1<<(FirstCount+SecondCount+NoneCount))) == 0)){
					NextGenerations[CurrentGen+1][a][b] = 0;
				}
				else {
					NextGenerations[CurrentGen+1][a][b] = NextGenerations[CurrentGen][a][b];
				}
			}
		}
	}
	return 0;
}

int GenerateMove(){
	//Making NextGenerations in such an manner that it's usable as argument
	int** NextGenerations[NumGenerations+1];
	for (int a = 0;a<NumGenerations+1;a++){
		NextGenerations[a] = new int*[Height];
		for (int b = 0;b<Height;b++){
			NextGenerations[a][b] = new int[Width];
		}
	}
	for (int a = 0;a<Height;a++){
		for (int b = 0;b<Width;b++){
			NextGenerations[0][a][b] = Board[a][b];
		}
	}
	CalculateNextGens(NextGenerations);
	int Improvement[Height][Width];
	for (int a = 0;a<Width;a++){
		for (int b = 0;b<Height;b++){
			int CurrentScore = 0;
			for (int c = std::max(a-NumGenerations,0);c<std::min(a+NumGenerations+1,Height);c++){
				for (int d = std::max(b-NumGenerations,0);d<std::min(b+NumGenerations+1,Width);d++){
					if (NextGenerations[NumGenerations][c][d] == 1){CurrentScore++;}
					if (NextGenerations[NumGenerations][c][d] == 2){CurrentScore--;}
				}
			}
			if (!FirstPlayer){CurrentScore = -CurrentScore;}
			int NewScore = 0;
			if (Board[a][b] == 0){
				if (FirstPlayer){NextGenerations[0][a][b] = 1;}
				else{NextGenerations[0][a][b] = 2;}
			}
			else{
				NextGenerations[0][a][b] = 0;
			}
			CalculateNextGens(NextGenerations);
			for (int c = std::max(a-NumGenerations,0);c<std::min(a+NumGenerations+1,Height);c++){
				for (int d = std::max(b-NumGenerations,0);d<std::min(b+NumGenerations+1,Width);d++){
					if (NextGenerations[NumGenerations][c][d] == 1){NewScore++;}
					if (NextGenerations[NumGenerations][c][d] == 2){NewScore--;}
				}
			}
			if (!FirstPlayer){NewScore = -NewScore;}
			Improvement[a][b] = NewScore - CurrentScore;
			NextGenerations[0][a][b] = Board[a][b];
			CalculateNextGens(NextGenerations);
			fprintf(stderr, "%d ", Improvement[a][b]);
		}
		fprintf(stderr, "\n");
	}
	int Largest = -500;
	int LargestA = 0;
	int LargestB = 0;
	for (int a = 0;a<Width;a++){
		for (int b = 0;b<Height;b++){
			if (Improvement[a][b] > Largest and Board[a][b]>0){
				LargestA = a;
				LargestB = b;
				Largest = Improvement[a][b];
			}
		}
	}
	Move.Changed.first = LargestA;
	Move.Changed.second = LargestB;
	return 0;
}

int DoMove(){
	if (Board[Move.Changed.first][Move.Changed.second] == 0){
		if (not MyTurn xor FirstPlayer){
			Board[Move.Changed.first][Move.Changed.second] = 1;
		}
		else{
			Board[Move.Changed.first][Move.Changed.second] = 2;
		}
		Board[Move.FirstSource.first][Move.FirstSource.second] = 0;
		Board[Move.SecondSource.first][Move.SecondSource.second] = 0;
	}
	else{
		Board[Move.Changed.first][Move.Changed.second] = 0;
	}
	short int NewBoard[20][20];
	for (int a = 0;a<Height;a++){
		for (int b = 0;b<Width;b++){
			int FirstCount = 0;
			int SecondCount = 0;
			int NoneCount = 0;
			if (a>0 and b>0 and Board[a-1][b-1] == 1){FirstCount++;}
			if (a>0 and Board[a-1][b] == 1){FirstCount++;}
			if (a>0 and b<Width-1 and Board[a-1][b+1] == 1){FirstCount++;}
			if (b>0 and Board[a][b-1] == 1){FirstCount++;}
			if (b<Width-1 and Board[a][b+1] == 1){FirstCount++;}
			if (a<Height-1 and b>0 and Board[a+1][b-1] == 1){FirstCount++;}
			if (a<Height-1 and Board[a+1][b] == 1){FirstCount++;}
			if (a<Height-1 and b<Width-1 and Board[a+1][b+1] == 1){FirstCount++;}

			if (a>0 and b>0 and Board[a-1][b-1] == 2){SecondCount++;}
			if (a>0 and Board[a-1][b] == 2){SecondCount++;}
			if (a>0 and b<Width-1 and Board[a-1][b+1] == 2){SecondCount++;}
			if (b>0 and Board[a][b-1] == 2){SecondCount++;}
			if (b<Width-1 and Board[a][b+1] == 2){SecondCount++;}
			if (a<Height-1 and b>0 and Board[a+1][b-1] == 2){SecondCount++;}
			if (a<Height-1 and Board[a+1][b] == 2){SecondCount++;}
			if (a<Height-1 and b<Width-1 and Board[a+1][b+1] == 2){SecondCount++;}

			if (a>0 and b>0 and Board[a-1][b-1] == 3){NoneCount++;}
			if (a>0 and Board[a-1][b] == 3){NoneCount++;}
			if (a>0 and b<Width-1 and Board[a-1][b+1] == 3){NoneCount++;}
			if (b>0 and Board[a][b-1] == 3){NoneCount++;}
			if (b<Width-1 and Board[a][b+1] == 3){NoneCount++;}
			if (a<Height-1 and b>0 and Board[a+1][b-1] == 3){NoneCount++;}
			if (a<Height-1 and Board[a+1][b] == 3){NoneCount++;}
			if (a<Height-1 and b<Width-1 and Board[a+1][b+1] == 3){NoneCount++;}

			fprintf(stderr, "%d ", FirstCount+SecondCount+NoneCount);
			if (Board[a][b] == 0 and ((BirthBM & (1<<(FirstCount+SecondCount+NoneCount))) != 0)){
				if (FirstCount > SecondCount){NewBoard[a][b] = 1;}
				if (FirstCount == SecondCount){NewBoard[a][b] = 3;}
				if (FirstCount < SecondCount){NewBoard[a][b] = 2;}
			}
			else if (Board[a][b] != 0 and ((SurviveBM & (1<<(FirstCount+SecondCount+NoneCount))) == 0)){
				NewBoard[a][b] = 0;
			}
			else {
				NewBoard[a][b] = Board[a][b];
			}
		}
		fprintf(stderr, "\n");

	}
	for (int a = 0;a<Height;a++){
		for (int b = 0;b<Width;b++){
			Board[a][b] = NewBoard[a][b];
		}
	}
	return 0;
}

bool GameFinished(){
	int Player1Pieces = 0;
	int Player2Pieces = 0;
	for (int a = 0;a<Height;a++){
		for (int b = 0;b<Width;b++){
			switch(Board[a][b]){
				case 1: Player1Pieces++;break;
				case 2: Player2Pieces++;break;
			}
		}
	}
	if (Player1Pieces == 0 or Player2Pieces == 0){
		return true;
	}
	else{
		return false;
	}
}

int main(){
	/*
	Import Rulesets
	BirthBM and SurviveBM are both Bitmasks
	for both: 0 live around has value 1, 8 live around has value 512
	BirthBM describes how many alive cells has to be around an empty cell for it to come alive
	SurviveBM describes how many alive cells has to be around an alive cell for it to stay alive
	*/
	scanf("%d %d",&BirthBM,&SurviveBM);
	//Import size of board
	scanf("%d %d",&Width,&Height);
	//Import board itself
	for (int a = 0;a<Height;a++){
		for (int b = 0;b<Width;b++){
			scanf("%1d",&Board[a][b]);
		}
	}
	//Import if starting player
	char PlayerNumber[10];
	scanf("%s",PlayerNumber);
	switch(PlayerNumber[0]){
		case '1': MyTurn = true;FirstPlayer = true;break;
		case '2': MyTurn = false;FirstPlayer = false;break;
	}
	while (GameFinished() == false){
		if (MyTurn){
			GenerateMove();
			printf("%d %d\n",Move.Changed.first,Move.Changed.second);
			if (Board[Move.Changed.first][Move.Changed.second] == 0){
				printf("%d %d\n",Move.FirstSource.first,Move.FirstSource.second);
				printf("%d %d\n",Move.SecondSource.first,Move.SecondSource.second);
			}
			DoMove();
		}
		else{
			scanf("%d %d",&Move.Changed.first,&Move.Changed.second);
			if (Board[Move.Changed.first][Move.Changed.second] == 0){
				scanf("%d %d",&Move.FirstSource.first,&Move.FirstSource.second);
				scanf("%d %d",&Move.SecondSource.first,&Move.SecondSource.second);
			}
			DoMove();
		}
		MyTurn = not MyTurn;
		// for (int a = 0;a<Height;a++){
		// 	for (int b = 0;b<Width;b++){
		// 		switch(Board[a][b]){
		// 			case 0: fprintf(stderr, "\x1b[40m  \x1b[0m");break;
		// 			case 1: fprintf(stderr, "\x1b[41m  \x1b[0m");break;
		// 			case 2: fprintf(stderr, "\x1b[44m  \x1b[0m");break;
		// 			case 3: fprintf(stderr, "\x1b[47m  \x1b[0m");break;
		// 		}
		// 	}
		// 	fprintf(stderr, "\n");
		// }
		// fprintf(stderr, "----------\n");
	}
	return 0;
}
