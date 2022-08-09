#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#define INDEX_NUMBER 8 //盤面の大きさ
#define WIN_NUMBER 5

enum Menu
{
	GameEnd = 0,
	Gomoku = 1,
	Reversi = 2,
	ReturnMenu = 9
};

enum Storn
{
	None,
	White,
	Black
};

enum Direction
{
	Up,
	Down,
	Left,
	Right,
	UpRight,
	DownRight,
	UpLeft,
	DownLeft
};

enum Put
{
	Yes,
	No
};

int PlayerInput(void)
{
	//プレイヤーの入力が正しいか確認する関数

	char input_c[10]; //9文字までは受け付けるようにしている
	int i = 0, len, num;

	while (1)
	{
		scanf_s("%9s", input_c, 10);
		len = (int)strlen(input_c);

		//1文字以上入力されたの場合
		if (len > 1)
		{
			printf("再入力してください：");
			continue;
		}

		//文字が入力された場合
		if (!isdigit(input_c[0])) //0であれば数字以外が入力されたことになる
		{
			printf("再入力してください：");
			continue;
		}

		if (input_c[1] == '\0')
			break;
	}

	//文字になっているためキャストして数字(int)にする
	num = (int)(input_c[0] - '0');

	return num;
}

void DrawMenu(void)
{
	//メニューの表示をする関数

	printf("\n");
	printf("+--------------------MENU--------------------+\n");
	printf("|                                            |\n");
	printf("|           ボードゲームで遊べます           |\n");
	printf("|   遊びたいゲームの数字を入力してください   |\n");
	printf("|                                            |\n");
	printf("|       ・五目並べ → 1  ・オセロ → 2       |\n");
	printf("|                                            |\n");
	printf("|                                  終了 → 0 |\n");
	printf("+--------------------------------------------+\n");
}

void DrawBoard(int board[][INDEX_NUMBER])
{
	//盤面の表示をする関数

	for (int k = 0; k < INDEX_NUMBER + 1; k++)
		printf("+--");

	printf("+\n");

	for (int k = 0; k < INDEX_NUMBER + 1; k++)
	{
		if (k == 0)
			printf("|  |");
		else
			printf(" %d|", k);
	}

	printf("\n");

	for (int i = 0; i < INDEX_NUMBER; i++)
	{
		for (int k = 0; k < INDEX_NUMBER + 1; k++)
			printf("+--");
		printf("+\n| %d|", i + 1);

		for (int j = 0; j < INDEX_NUMBER; j++)
		{
			if (board[i][j] == White)
				printf("●|");
			else if (board[i][j] == Black)
				printf("〇|");
			else
				printf("　|");
		}

		printf("\n");
	}

	for (int k = 0; k < INDEX_NUMBER + 1; k++)
		printf("+--");
	printf("+\n");
}

void Initialization(int board[][INDEX_NUMBER])
{
	//盤面の初期化をする関数

	for (int i = 0; i < INDEX_NUMBER; i++)
	{
		for (int j = 0; j < INDEX_NUMBER; j++)
			board[i][j] = None;
	}
}

#pragma region Gomoku

int Gomoku_HightJudge(int board[][INDEX_NUMBER], int x, int d, int color)
{
	//縦方向の判定をする関数

	int stornJudge = 0;

	if ((d == Up) || (d == Down))
	{
		for (int i = 0; i < INDEX_NUMBER; i++)
		{
			//printf("%d, %d\n", i, x);
			if (board[i][x] == color)
			{
				stornJudge++;
			}
			/*else
				break;*/
		}
	}

	return stornJudge;
}

int Gomoku_WidthJudge(int board[][INDEX_NUMBER], int y, int d, int color)
{
	//横方向の判定をする関数

	int stornJudge = 0;

	if ((d == Left) || (d == Right))
	{
		for (int j = 0; j < INDEX_NUMBER; j++)
		{
			//printf("%d, %d\n", y, j);
			if (board[y][j] == color)
				stornJudge++;
			/*else
				break;*/
		}
	}

	return stornJudge;
}

int Gomoku_UpRightJudge(int board[][INDEX_NUMBER], int x, int y, int d, int color)
{
	//右肩上がり(/)の判定をする関数
	//右肩上がりだと片方は+1、もう片方は-1ずつ座標が変化していく

	int stornJudge = 0;
	int i, j;

	if ((d == UpRight) || (d == DownLeft))
	{
		i = y, j = x;

		while (board[i][j] == color) //右上
		{
			stornJudge++;
			i++;
			j--;

			if (i >= INDEX_NUMBER || j < 0)
				break;
		}

		if (stornJudge <= WIN_NUMBER)
		{
			//rowとcolを起点にすると1回分重なるためずらしてからループを回す
			i = y - 1, j = x + 1;

			while (board[i][j] == color) //左下
			{
				stornJudge++;
				i--;
				j++;

				if (i < 0 || j >= INDEX_NUMBER)
					break;
			}
		}
	}

	return stornJudge;
}

int Gomoku_DownRightJudge(int board[][INDEX_NUMBER], int x, int y, int d, int color)
{
	//右肩下がりの判定をする関数
	//右肩下がりだと両方+1、もしくは両方-1ずつ座標が変化していく

	int stornJudge = 0;
	int i, j;

	if ((d == UpLeft) || (d == DownRight))
	{
		i = y, j = x;

		while (board[i][j] == color) //右下
		{
			stornJudge++;
			i++;
			j++;

			if (i >= INDEX_NUMBER || j >= INDEX_NUMBER)
				break;
		}

		if (stornJudge <= WIN_NUMBER)
		{
			//rowとcolを起点にすると1回分重なるためずらしてからループを回す
			i = y - 1, j = x - 1;

			while (board[i][j] == color) //左上
			{
				stornJudge++;
				i--;
				j--;

				if (i < 0 || j < 0)
					break;
			}
		}
	}

	return stornJudge;
}

int Gomoku_AllJudgement(int board[][INDEX_NUMBER], int x, int y, int color)
{
	//各方向の判定をまとめている関数

	int stornCheck = 0;

	//周り8方向の確認
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			//盤面の外に行かないようにする
			if ((y + i) < 0 || (y + i) >= INDEX_NUMBER ||
				(x + j) < 0 || (x + j) >= INDEX_NUMBER)
				continue;

			if (i == 0 && j == 0)
				continue;

			if (board[y + i][x + j] != color)
				continue;

			//隣が自分の石の場合、見ている方向の確認をする
			if (i == -1)
			{
				switch (j)
				{
				case -1:
					//printf("左上:");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_DownRightJudge(board, x, y, UpLeft, color); //左上
					break;
				case 0:
					//printf("上:");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_HightJudge(board, x, Up, color); //上
					break;
				case 1:
					//printf("右上:");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_UpRightJudge(board, x, y, UpRight, color); //右上
					break;
				default:
					break;
				}
			}
			if (i == 0)
			{
				switch (j)
				{
				case -1:
					//printf("左\n");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_WidthJudge(board, y, Left, color); //左
					break;
				case 1:
					//printf("右\n");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_WidthJudge(board, y, Right, color); //右
					break;
				default:
					break;
				}
			}
			if (i == 1)
			{
				switch (j)
				{
				case -1:
					//printf("左下:");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_UpRightJudge(board, x, y, DownLeft, color); //左下
					break;
				case 0:
					//printf("下:");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_HightJudge(board, x, Down, color); //下
					break;
				case 1:
					//printf("右下:");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_DownRightJudge(board, x, y, DownRight, color); //右下
					break;
				default:
					break;
				}
			}

			if (stornCheck >= WIN_NUMBER)
				break;
		}

		if (stornCheck >= WIN_NUMBER)
			break;
	}

	//printf("check:%d\n", stornCheck);

	return stornCheck;
}

void Gomoku_Result(int board[][INDEX_NUMBER], int wJ, int bJ)
{
	//結果の表示をする関数

	if (wJ >= WIN_NUMBER)
	{
		DrawBoard(board);
		printf("\nプレイヤーの勝利\n");
	}
	else if (bJ >= WIN_NUMBER)
	{
		DrawBoard(board);
		printf("\nＣＯＭの勝利\n");
	}
	else
	{
		DrawBoard(board);
		printf("\n引き分け\n");
	}

	printf("\n");
}

void Gomoku_COM(int board[][INDEX_NUMBER])
{
	int posSelect = rand() % 2; //rowをランダムにするかcolをランダムにするか決める変数
	int putFlag = 0; //置いたかどうか判別するためのフラグ

	//rowまたはcolをランダムで決めてforでNONEに当たるまで回す
	if (posSelect == 0)
	{
		while (putFlag == 0)
		{
			int posX = rand() % INDEX_NUMBER; //col
			for (int i = posX; i < INDEX_NUMBER; i++)
			{
				for (int j = 0; j < INDEX_NUMBER; j++)
				{
					if (board[j][i] == None)
					{
						board[j][i] = Black;
						putFlag = 1; //石を置いたら1にしてループを抜ける
						break;
					}
				}

				if (putFlag == 1)
					break;
			}
		}
	}
	else
	{
		while (putFlag == 0)
		{
			int posY = rand() % INDEX_NUMBER; //row
			for (int i = posY; i < INDEX_NUMBER; i++)
			{
				for (int j = INDEX_NUMBER - 1; j >= 0; j--)
				{
					if (board[i][j] == None)
					{
						board[i][j] = Black;
						putFlag = 1;
						break;
					}
				}

				if (putFlag == 1)
					break;
			}
		}
	}
}

#pragma endregion Gomoku

#pragma region Riversi

int Reversi_CanPutJudge(int board[][INDEX_NUMBER], int x, int y, int color)
{
	//置けるかどうか判定する関数

	//既に石が置かれて置かれている場合
	if (board[y][x] != None)
		return No;

	int stornCount_R = 0;

	//指定された場所の周り8方向の確認
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (i == 0 && j == 0)
				continue;
			if (board[y + i][x + j] == color || board[y + i][x + j] == None)
				continue;

			//隣に相手の石が置かれている場合のみ走査する
			for (int m = 2; m < INDEX_NUMBER; m++)
			{
				if (y + i * m >= 0 && y + i * m < INDEX_NUMBER &&
					x + j * m >= 0 && x + j * m < INDEX_NUMBER)
				{
					if (board[y + i * m][x + j * m] == None)
						break;
					if (board[y + i * m][x + j * m] == color)
						stornCount_R++;
				}
			}
		}
	}

	if (stornCount_R == 0)
		return No;

	return Yes;
}

void Reversi_PutStorn(int board[][INDEX_NUMBER], int x, int y, int color)
{
	//石を置いてひっくり返す関数

	//Reversi_CanPutJudgeとほぼ同じ処理
	//周り8方向見て、隣が相手の石か確認する
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (i == 0 && j == 0)
				continue;
			if (board[y + i][x + j] == color || board[y + i][x + j] == None)
				continue;

			for (int m = 2; m < INDEX_NUMBER; m++)
			{
				if (y + i * m >= 0 && y + i * m < INDEX_NUMBER &&
					x + j * m >= 0 && x + j * m < INDEX_NUMBER)
				{
					if (board[y + i * m][x + j * m] == None)
						break;

					//見ている先に自分の石があればその間の石をひっくり返す
					if (board[y + i * m][x + j * m] == color)
					{
						board[y][x] = color;
						for (int n = 1; n < m; n++)
							board[y + i * n][x + j * n] = color;
					}

				}
			}
		}
	}
}

int Reversi_NextTurnPlay(int board[][INDEX_NUMBER], int color)
{
	//次のターン置けるかどうか判定する関数

	int nextPlayColor = None;

	if (color == White)
		nextPlayColor = Black;
	else
		nextPlayColor = White;

	//相手が置ける場合
	for (int i = 0; i < INDEX_NUMBER; i++)
	{
		for (int j = 0; j < INDEX_NUMBER; j++)
		{
			if (Reversi_CanPutJudge(board, i, j, nextPlayColor) == Yes)
				return nextPlayColor;
		}
	}

	//連続で自分が置ける場合
	for (int i = 0; i < INDEX_NUMBER; i++)
	{
		for (int j = 0; j < INDEX_NUMBER; j++)
		{
			if (Reversi_CanPutJudge(board, i, j, color) == Yes)
				return color;
		}
	}

	//どちらも置けない場合
	return None;
}

void Reversi_COM(int board[][INDEX_NUMBER], int color)
{
	//ＣＯＭの処理をする関数

	//置けるまで乱数で座標を決める
	while (1)
	{
		int posY = rand() % INDEX_NUMBER;
		int posX = rand() % INDEX_NUMBER;

		if (Reversi_CanPutJudge(board, posX, posY, color) == No)
			continue;
		else
		{
			board[posY][posX] = Black;
			Reversi_PutStorn(board, posX, posY, color);
			break;
		}
	}
}

void Riversi_Result(int board[][INDEX_NUMBER])
{
	//石の集計及び結果の表示をする関数

	int whiteCount = 0;
	int blackCount = 0;

	for (int i = 0; i < INDEX_NUMBER; i++)
	{
		for (int j = 0; j < INDEX_NUMBER; j++)
		{
			if (board[i][j] == White)
				whiteCount++;
			else if (board[i][j] == Black)
				blackCount++;
		}
	}

	printf("\n結果　プレイヤー：%d個　ＣＯＭ：%d個\n", whiteCount, blackCount);

	if (whiteCount > blackCount)
		printf("\nプレイヤーの勝利\n");
	else if (blackCount > whiteCount)
		printf("\nＣＯＭの勝利\n");
	else
		printf("\n引き分け\n");

	printf("\n");
}

#pragma endregion Reversi

int main(void)
{
	//共通で使用する変数
	int board[INDEX_NUMBER][INDEX_NUMBER];
	int row, col;
	int stornCount;

	//終了 → 0、五目並べ → 1、オセロ → 2、メニューに戻る → 9
	int menuSelect = ReturnMenu;

	//五目並べ
	int whiteJudge, blackJudge;

	//オセロ
	int playColor, nextPlayColor;

	srand((unsigned int)time(NULL));

	while (menuSelect != GameEnd)
	{
		//メニューの表示
		DrawMenu();
		printf("\n数字を入力してください：");
		menuSelect = PlayerInput(); printf("\n");

		if (menuSelect == GameEnd)
			break;

		//初期化
		whiteJudge = 0; blackJudge = 0;
		stornCount = INDEX_NUMBER * INDEX_NUMBER;
		playColor = White, nextPlayColor = Black;
		Initialization(board);

		//五目並べのメイン部分
		while (menuSelect == Gomoku)
		{
			if (stornCount == INDEX_NUMBER * INDEX_NUMBER)
			{
				printf("五目並べが選択されました\n");
				printf("プレイヤーは白、ＣＯＭは黒です\n");
				printf("途中でメニューに戻る場合は 9 を入力してください\n\n");

				DrawBoard(board);
			}

			//プレイヤー
			printf("\nプレイヤーの番です\n");
			printf("行数(縦)：");
			row = PlayerInput();

			if (row == ReturnMenu)
				break;

			printf("列数(横)：");
			col = PlayerInput();
			printf("\n");

			if (col == ReturnMenu)
				break;

			//1～8で入力するようになっているため、ずれないよう-1してから配列に格納
			row--; col--;

			//既に石がある場合は再入力
			if (board[row][col] != None)
			{
				printf("指定した場所には既に石が置かれています\n");
				printf("再入力してください\n");
				continue;
			}

			board[row][col] = White;

			whiteJudge = Gomoku_AllJudgement(board, col, row, White);
			if (whiteJudge >= WIN_NUMBER)
			{
				Gomoku_Result(board, whiteJudge, blackJudge); //結果の表示
				break;
			}

			DrawBoard(board);

			stornCount--;
			//printf("\n%d\n", stornCount);

			//石を置ける場所がなくなったら終了
			if (stornCount == 0)
			{
				Gomoku_Result(board, whiteJudge, blackJudge); //結果の表示
				break;
			}

			//ＣＯＭ
			printf("\nＣＯＭの番です\n");
			Gomoku_COM(board);
			blackJudge = Gomoku_AllJudgement(board, col, row, Black);

			if (blackJudge >= WIN_NUMBER)
			{
				Gomoku_Result(board, whiteJudge, blackJudge);
				break;
			}

			DrawBoard(board);

			stornCount--;

			if (stornCount == 0)
			{
				Gomoku_Result(board, whiteJudge, blackJudge);
				break;
			}
		}

		//オセロのメイン部分
		while (menuSelect == Reversi)
		{
			if (stornCount == INDEX_NUMBER * INDEX_NUMBER)
			{
				printf("オセロが選択されました\n");
				printf("プレイヤーは白、ＣＯＭは黒です\n");
				printf("途中でメニューに戻る場合は 9 を入力してください\n\n");

				board[INDEX_NUMBER / 2][INDEX_NUMBER / 2] = White;
				board[INDEX_NUMBER / 2 - 1][INDEX_NUMBER / 2 - 1] = White;
				board[INDEX_NUMBER / 2 - 1][INDEX_NUMBER / 2] = Black;
				board[INDEX_NUMBER / 2][INDEX_NUMBER / 2 - 1] = Black;

				DrawBoard(board);
				stornCount--; //開始時にしか出ないようにするためカウントを減らす
			}

			//プレイヤー
			nextPlayColor = Reversi_NextTurnPlay(board, playColor);

			if (nextPlayColor == playColor)
				printf("\n置けないためスキップします\n");
			else if (nextPlayColor == None)
			{
				printf("\n試合終了です\n");
				Riversi_Result(board);
				break;
			}

			playColor = White;

			printf("\nプレイヤーの番です\n");
			printf("行数(縦)：");
			row = PlayerInput();

			if (row == ReturnMenu)
				break;

			printf("列数(横)：");
			col = PlayerInput();
			printf("\n");

			if (col == ReturnMenu)
				break;

			//1～8で入力するようになっているため、ずれないよう-1してから配列に格納
			row--; col--;

			if (Reversi_CanPutJudge(board, col, row, playColor) == No)
			{
				printf("指定場所には置けません\n");
				printf("再入力してください\n");
				continue;
			}

			board[row][col] = White;

			Reversi_PutStorn(board, col, row, playColor);
			DrawBoard(board);

			//ＣＯＭ
			nextPlayColor = Reversi_NextTurnPlay(board, playColor);

			if (nextPlayColor == playColor)
				printf("\n置けないためスキップします\n");
			else if (nextPlayColor == None)
			{
				printf("\n試合終了です\n");
				Riversi_Result(board);
				break;
			}

			printf("\nＣＯＭの番です\n\n");
			playColor = Black;
			Reversi_COM(board, playColor);
			DrawBoard(board);
		}

	}

	printf("プログラムを終了します");

	return 0;
}