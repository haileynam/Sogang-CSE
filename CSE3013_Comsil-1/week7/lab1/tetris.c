#include "tetris.h"

static struct sigaction act, oact;
int B,count;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	srand((unsigned int)time(NULL));
	createRankList();

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	//assignment1
	nextBlock[2]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	// assignment 1
	// 기존의 DrawBlock 자리에 대신 넣는다
	//DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
	// assignment1: 다다음 블럭을 보여주는 테두리.
	DrawBox(9, WIDTH+10, 4, 8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	//move(9,WIDTH+10);
	move(15, WIDTH+10);
	printw("SCORE");
	//DrawBox(10,WIDTH+10,1,8);
	DrawBox(16, WIDTH+10, 1, 8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}

//assignment1: 배치가 변경됨-> PrintScore 위치도 변경.
void PrintScore(int score){
	//move(11,WIDTH+11);
	move (17, WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}

	// assignment1: Draw another next block window like nextBlock[1]
	for (i = 0; i < 4; i++){
		move(10+i, WIDTH+13);
		for (j = 0; j < 4; j++){
			if (block[nextBlock[2]][0][i][j] == 1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (block[currentBlock][blockRotate][i][j] == 1)
			{
				// 목표한 포지션. 
				int x = blockX + j;
				int y = blockY + i;
				// 만약 해당 좌표가 field를 벗어나면 invalid
				if ((x>=WIDTH)|| (x <0) || (y>=HEIGHT) || (y <0))
				//if (!(0 <= x < WIDTH && 0 <= y < HEIGHT))
					return 0;
				// 해당 좌표에 블럭이 있어도 invalid
				if (field[y][x] == 1)
					return 0;
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	// 이전-> 명령어와 반대로 움직인다.
	switch(command)
	{
		case KEY_UP:
			blockRotate = (blockRotate+3)%4;
			break;
		case KEY_DOWN:
			blockY--;
			break;
		case KEY_RIGHT:
			blockX--;
			break;
		case KEY_LEFT:
			blockX++;
			break;
		default: 
			break;	
	}
	// //2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	// // 이전의 위치 .으로 바꿔서 지우기. 
	// for (int i = 0; i < 4; i++)
	// {
	// 	for (int j = 0; j < 4; j++)
	// 	{
	// 		if ((block[currentBlock][blockRotate][i][j] == 1) &&( i+blockY >= 0))
	// 		{
	// 			move (i+blockY+1, j+blockX + 1);
	// 			printw(".");
	// 		}
	// 	}

	// }
    //     move(HEIGHT, WIDTH + 10);
	
	DrawField();

	//3. 새로운 블록 정보를 그린다. 
	// 위치 원복시키기
	switch(command)
	{
		case KEY_UP:
			blockRotate = (blockRotate+1)%4;
			break;
		case KEY_DOWN:
			blockY++;
			break;
		case KEY_RIGHT:
			blockX++;
			break;
		case KEY_LEFT:
			blockX--;
			break;
		default: 
			break;	
	}
	// 블록 그리기
	// assignment 1
	// DrawBlock 자리를 DrawBlockWithFeatures로 수정
	//DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
	
}

void BlockDown(int sig){
	// user code

	//강의자료 p26-27의 플로우차트를 참고한다.
	// 아래로 내려갈 수 있으면-> Y좌표 1 올리고 블록을 그린다. 
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX) == 1)
	{
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else 
	{
		// 맨 위까지 꽉 차면 게임오버
		if (blockY == -1)
			gameOver = 1;
		
		// 더 못 내려가지만 꽉차지 않으면 다음 블럭 내려옴.
		// 올라갈 스코어가 있으면 갱신.
		// assignment1: AddBlockToField도 touched에 의한 점수 리턴이 있음.
		//AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);
		// 현재 블럭이 내려갔으니 다음 블럭 목록을 갱신함.
		nextBlock[0] = nextBlock[1];
		// assignment1: increased next block info.
		//nextBlock[1] = rand() % 7;
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand() % 7;
		// 새로 내려올 블럭의 initialize
		blockRotate = 0;
		blockY = -1;
		blockX = WIDTH /2 - 2;
		// 만들어진 nextBlock을 해당 창에 그려줘야함.
		DrawNextBlock(nextBlock);
		DrawField();
		PrintScore(score);
		// assignment 1 
		// DrawBlock을 DrawBlockWithFeatures로 대체
		//DrawBlock(blockY, blockX, nextBlock[0], blockRotate, ' ');
		//DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);
	}
	timed_out = 0;

}

// void형을 int로 바꿈.
int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//Block이 추가된 영역의 필드값을 바꾼다.
	// 블록이 추가되면 block의 값이 1-> field에도 1을 넣어 반영. 
	// assignment1: 맞닿은 면적에 의한 점수도 반영하기.
	int touched = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (block[currentBlock][blockRotate][i][j] == 1)
			{
				f[blockY+i][blockX+j] = 1;
				// assignment 1;
				//4×4 배열의 블록을 이중 loop를 통해서 탐색하면서, 블록 나타내는 배열의 원소가 1이고, 바로 아래 필드의 요소가 1이면,
				//변수 touched를 1 증가시킨다
				if ((blockY+i+1) == HEIGHT) touched ++;
			}
		}
	}
	return touched * 10;
	
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
	int cnt = 0;
        bool isFull = true;
	for (int i = 0; i < HEIGHT;i++) {
		for (int j = 0; j < WIDTH; j++) {
                        // 블럭이 없다 -> no delete
			if (f[i][j] == 0) {
			        isFull = false;
				break;
			}
                        // 맨 위가 그냥 쭉 평평한 부분인 경우 -> 지워줘야 한다.  
		        if (f[i][j] == 1 && j == WIDTH -1)
                                isFull = true;
                }
		if (isFull) {
			for (int j = i; j > 0; --j) {
				for (int k = 0; k < WIDTH; ++k) {
					f[j][k] = f[j - 1][k];
				}
			}
			cnt++;
		}
	}
	return (cnt * cnt * 100);

}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
	// 더 내려갈 수 없다면 checktomove 함수 0
	while (CheckToMove(field, blockID, blockRotate, y+1, x) == 1) 
	{
		y++;
	}
	DrawBlock(y, x, blockID, blockRotate, '/');
}

//이 함수는 위 두 가지 DrawBlock(), DrawShadow() 함수를 호출하는 함수로, 
//기존의 DrawBlock() 함수의 위치에 삽입하여 움직임이 갱신될 때마다 현재 블록과 함께 그림자를 그린다.
void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawShadow(y, x, blockID, blockRotate);
}

//////////// 두번째 실습 ///////////////
void createRankList(){
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
	// 1. "rank.txt"열기
	// 2. 파일에서 랭킹정보 읽어오기
	// 3. LinkedList로 저장
	// 4. 파일 닫기
	FILE *fp;
	int i, j;

	//1. 파일 열기
	fp = fopen("rank.txt", "r");
	if (fp == NULL) 
	{
		fclose(fp);
		fp = fopen("rank.txt", "w");
	}
	// 2. 정보읽어오기
	/* int fscanf(FILE* stream, const char* format, ...);
	stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
	format: 형식지정자 등등
	변수의 주소: 포인터
	return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
	// EOF(End Of File): 실제로 이 값은 -1을 나타냄, EOF가 나타날때까지 입력받아오는 if문
	fscanf(fp, "%d", &score_number);

	char input_nam[NAMELEN+1];
	int input_score = 0;
	Node *newNode;
	head = (Node*)malloc(sizeof(Node));
	head->link = NULL;
	Node *cur = head; 
	for (i = 0; i < score_number; i++)
	{
		Node *newNode = malloc(sizeof(Node));
		fscanf(fp, "%s%d", newNode->name, &(newNode->score));
		newNode->link = NULL;
		cur->link = newNode;
		cur = newNode;
	}
	// 4. 파일닫기
	fclose(fp);
}

void rank(){
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	int X=1, Y=score_number, ch, i, j, cnt = 0, linecnt =0;
	clear();

	//2. printw()로 3개의 메뉴출력
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by specific name\n");
	printw("3. delete a specific rank\n");

	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
	ch = wgetch(stdscr);
	FILE *fp_d = fopen("Debug.txt", "w");

	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
	if (ch == '1') {
		
		printw("X: ");
		echo();
		scanw("%d ", &X);
		printw("Y: ");
		scanw("%d ", &Y);
		noecho();

		fprintf(fp_d, "ch : %d\n", ch);
		fprintf(fp_d, "score_number : %d\n", score_number);
		fprintf(fp_d, "X: %d Y: %d\n", X, Y);
		// debugging
		printw("      name      |     score   \n");
		printw("-------------------------------\n");
		
		Node *cur = head;
		if (X == 0) X == 1;
		if (Y>score_number) Y = score_number;
		if (X > Y || score_number == 0)
		{
			printw("search failure: no rank in the list\n");
		}
		else 
		{
			for (i = 0; i< X; i++) 
			{
				cur = cur->link;
			}

			for (i = X; i<=Y; i++)
			{
				printw("%12s    |      %d\n", cur->name, cur->score);
				cur= cur->link; 
			}
		}
	
	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if ( ch == '2') {
		char str[NAMELEN+1];
		int check = 0;


	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if ( ch == '3') {
		int num;

	}
	getch();

}

void writeRankFile(){
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
	int sn, i;
	//1. "rank.txt" 연다
	FILE *fp = fopen("rank.txt", "w");

	//2. 랭킹 정보들의 수를 "rank.txt"에 기록
	fprintf(fp, "%d\n", score_number);
	//3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
	Node *cur = head->link;
	while (cur != NULL)
	{
		fprintf(fp, "%s %d\n", cur->name, cur->score);
		cur = cur->link;
	}

	fclose(fp);
	// if ( sn == score_number) return;
	// else {


	// }
	// for ( i= 1; i < score_number+1 ; i++) {
	// 	free(a.rank_name[i]);
	// }
	// free(a.rank_name);
	// free(a.rank_score);
}

void newRank(int score){
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN+1];
	int i, j;
	clear();
	//1. 사용자 이름을 입력받음
	FILE *fp = fopen("rank.txt", "w");
	printw("your name: ");
	echo();
	scanw("%s", str);
	noecho();
	
	//2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가 0이면 head에 새로운 노드 연결
	Node *newNode = malloc(sizeof(Node));
	strcpy(newNode->name, str);
	newNode->score = score;
	newNode->link = NULL;
	
	// 비교할 앞 노드
	Node *prev = head;
	// 비교할 뒷 노드
	Node *cur = prev->link;
	
	if(score_number == 0) {
		head->link = newNode;
	}

	else {
		while (cur->link)
		{
			prev = prev->link;
			cur = cur->link;
			if (prev->score >= score && cur->score <score) 
			{
				// 내림차순 만족하는 구간. prev와 cur 사이에 삽입한다.
				prev->link = newNode;
				newNode->link = cur;
			}
		
			else if (prev->score < score && cur->score < score)
			{
				// 제일 큰 점수
				head->link = newNode;
				newNode->link = prev;
			}
		}
		if (newNode->link == NULL)
		{
			cur->link = newNode;
		}

		
		
		
	}
	// 새로 기록했으니 score_number은 1 늘어난다.
	score_number++;
	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

// int recommend(char fieldOri[HEIGHT][WIDTH],int lv){

// }

void recommendedPlay(){
	// user code
}
