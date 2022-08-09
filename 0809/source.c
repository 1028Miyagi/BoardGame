#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#define INDEX_NUMBER 8 //�Ֆʂ̑傫��
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
	//�v���C���[�̓��͂����������m�F����֐�

	char input_c[10]; //9�����܂ł͎󂯕t����悤�ɂ��Ă���
	int i = 0, len, num;

	while (1)
	{
		scanf_s("%9s", input_c, 10);
		len = (int)strlen(input_c);

		//1�����ȏ���͂��ꂽ�̏ꍇ
		if (len > 1)
		{
			printf("�ē��͂��Ă��������F");
			continue;
		}

		//���������͂��ꂽ�ꍇ
		if (!isdigit(input_c[0])) //0�ł���ΐ����ȊO�����͂��ꂽ���ƂɂȂ�
		{
			printf("�ē��͂��Ă��������F");
			continue;
		}

		if (input_c[1] == '\0')
			break;
	}

	//�����ɂȂ��Ă��邽�߃L���X�g���Đ���(int)�ɂ���
	num = (int)(input_c[0] - '0');

	return num;
}

void DrawMenu(void)
{
	//���j���[�̕\��������֐�

	printf("\n");
	printf("+--------------------MENU--------------------+\n");
	printf("|                                            |\n");
	printf("|           �{�[�h�Q�[���ŗV�ׂ܂�           |\n");
	printf("|   �V�т����Q�[���̐�������͂��Ă�������   |\n");
	printf("|                                            |\n");
	printf("|       �E�ܖڕ��� �� 1  �E�I�Z�� �� 2       |\n");
	printf("|                                            |\n");
	printf("|                                  �I�� �� 0 |\n");
	printf("+--------------------------------------------+\n");
}

void DrawBoard(int board[][INDEX_NUMBER])
{
	//�Ֆʂ̕\��������֐�

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
				printf("��|");
			else if (board[i][j] == Black)
				printf("�Z|");
			else
				printf("�@|");
		}

		printf("\n");
	}

	for (int k = 0; k < INDEX_NUMBER + 1; k++)
		printf("+--");
	printf("+\n");
}

void Initialization(int board[][INDEX_NUMBER])
{
	//�Ֆʂ̏�����������֐�

	for (int i = 0; i < INDEX_NUMBER; i++)
	{
		for (int j = 0; j < INDEX_NUMBER; j++)
			board[i][j] = None;
	}
}

#pragma region Gomoku

int Gomoku_HightJudge(int board[][INDEX_NUMBER], int x, int d, int color)
{
	//�c�����̔��������֐�

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
	//�������̔��������֐�

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
	//�E���オ��(/)�̔��������֐�
	//�E���オ�肾�ƕЕ���+1�A�����Е���-1�����W���ω����Ă���

	int stornJudge = 0;
	int i, j;

	if ((d == UpRight) || (d == DownLeft))
	{
		i = y, j = x;

		while (board[i][j] == color) //�E��
		{
			stornJudge++;
			i++;
			j--;

			if (i >= INDEX_NUMBER || j < 0)
				break;
		}

		if (stornJudge <= WIN_NUMBER)
		{
			//row��col���N�_�ɂ����1�񕪏d�Ȃ邽�߂��炵�Ă��烋�[�v����
			i = y - 1, j = x + 1;

			while (board[i][j] == color) //����
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
	//�E��������̔��������֐�
	//�E�������肾�Ɨ���+1�A�������͗���-1�����W���ω����Ă���

	int stornJudge = 0;
	int i, j;

	if ((d == UpLeft) || (d == DownRight))
	{
		i = y, j = x;

		while (board[i][j] == color) //�E��
		{
			stornJudge++;
			i++;
			j++;

			if (i >= INDEX_NUMBER || j >= INDEX_NUMBER)
				break;
		}

		if (stornJudge <= WIN_NUMBER)
		{
			//row��col���N�_�ɂ����1�񕪏d�Ȃ邽�߂��炵�Ă��烋�[�v����
			i = y - 1, j = x - 1;

			while (board[i][j] == color) //����
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
	//�e�����̔�����܂Ƃ߂Ă���֐�

	int stornCheck = 0;

	//����8�����̊m�F
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			//�Ֆʂ̊O�ɍs���Ȃ��悤�ɂ���
			if ((y + i) < 0 || (y + i) >= INDEX_NUMBER ||
				(x + j) < 0 || (x + j) >= INDEX_NUMBER)
				continue;

			if (i == 0 && j == 0)
				continue;

			if (board[y + i][x + j] != color)
				continue;

			//�ׂ������̐΂̏ꍇ�A���Ă�������̊m�F������
			if (i == -1)
			{
				switch (j)
				{
				case -1:
					//printf("����:");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_DownRightJudge(board, x, y, UpLeft, color); //����
					break;
				case 0:
					//printf("��:");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_HightJudge(board, x, Up, color); //��
					break;
				case 1:
					//printf("�E��:");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_UpRightJudge(board, x, y, UpRight, color); //�E��
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
					//printf("��\n");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_WidthJudge(board, y, Left, color); //��
					break;
				case 1:
					//printf("�E\n");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_WidthJudge(board, y, Right, color); //�E
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
					//printf("����:");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_UpRightJudge(board, x, y, DownLeft, color); //����
					break;
				case 0:
					//printf("��:");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_HightJudge(board, x, Down, color); //��
					break;
				case 1:
					//printf("�E��:");
					//printf("%d, %d\n", row + i, col + j);
					stornCheck = Gomoku_DownRightJudge(board, x, y, DownRight, color); //�E��
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
	//���ʂ̕\��������֐�

	if (wJ >= WIN_NUMBER)
	{
		DrawBoard(board);
		printf("\n�v���C���[�̏���\n");
	}
	else if (bJ >= WIN_NUMBER)
	{
		DrawBoard(board);
		printf("\n�b�n�l�̏���\n");
	}
	else
	{
		DrawBoard(board);
		printf("\n��������\n");
	}

	printf("\n");
}

void Gomoku_COM(int board[][INDEX_NUMBER])
{
	int posSelect = rand() % 2; //row�������_���ɂ��邩col�������_���ɂ��邩���߂�ϐ�
	int putFlag = 0; //�u�������ǂ������ʂ��邽�߂̃t���O

	//row�܂���col�������_���Ō��߂�for��NONE�ɓ�����܂ŉ�
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
						putFlag = 1; //�΂�u������1�ɂ��ă��[�v�𔲂���
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
	//�u���邩�ǂ������肷��֐�

	//���ɐ΂��u����Ēu����Ă���ꍇ
	if (board[y][x] != None)
		return No;

	int stornCount_R = 0;

	//�w�肳�ꂽ�ꏊ�̎���8�����̊m�F
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (i == 0 && j == 0)
				continue;
			if (board[y + i][x + j] == color || board[y + i][x + j] == None)
				continue;

			//�ׂɑ���̐΂��u����Ă���ꍇ�̂ݑ�������
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
	//�΂�u���ĂЂ�����Ԃ��֐�

	//Reversi_CanPutJudge�Ƃقړ�������
	//����8�������āA�ׂ�����̐΂��m�F����
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

					//���Ă����Ɏ����̐΂�����΂��̊Ԃ̐΂��Ђ�����Ԃ�
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
	//���̃^�[���u���邩�ǂ������肷��֐�

	int nextPlayColor = None;

	if (color == White)
		nextPlayColor = Black;
	else
		nextPlayColor = White;

	//���肪�u����ꍇ
	for (int i = 0; i < INDEX_NUMBER; i++)
	{
		for (int j = 0; j < INDEX_NUMBER; j++)
		{
			if (Reversi_CanPutJudge(board, i, j, nextPlayColor) == Yes)
				return nextPlayColor;
		}
	}

	//�A���Ŏ������u����ꍇ
	for (int i = 0; i < INDEX_NUMBER; i++)
	{
		for (int j = 0; j < INDEX_NUMBER; j++)
		{
			if (Reversi_CanPutJudge(board, i, j, color) == Yes)
				return color;
		}
	}

	//�ǂ�����u���Ȃ��ꍇ
	return None;
}

void Reversi_COM(int board[][INDEX_NUMBER], int color)
{
	//�b�n�l�̏���������֐�

	//�u����܂ŗ����ō��W�����߂�
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
	//�΂̏W�v�y�ь��ʂ̕\��������֐�

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

	printf("\n���ʁ@�v���C���[�F%d�@�b�n�l�F%d��\n", whiteCount, blackCount);

	if (whiteCount > blackCount)
		printf("\n�v���C���[�̏���\n");
	else if (blackCount > whiteCount)
		printf("\n�b�n�l�̏���\n");
	else
		printf("\n��������\n");

	printf("\n");
}

#pragma endregion Reversi

int main(void)
{
	//���ʂŎg�p����ϐ�
	int board[INDEX_NUMBER][INDEX_NUMBER];
	int row, col;
	int stornCount;

	//�I�� �� 0�A�ܖڕ��� �� 1�A�I�Z�� �� 2�A���j���[�ɖ߂� �� 9
	int menuSelect = ReturnMenu;

	//�ܖڕ���
	int whiteJudge, blackJudge;

	//�I�Z��
	int playColor, nextPlayColor;

	srand((unsigned int)time(NULL));

	while (menuSelect != GameEnd)
	{
		//���j���[�̕\��
		DrawMenu();
		printf("\n��������͂��Ă��������F");
		menuSelect = PlayerInput(); printf("\n");

		if (menuSelect == GameEnd)
			break;

		//������
		whiteJudge = 0; blackJudge = 0;
		stornCount = INDEX_NUMBER * INDEX_NUMBER;
		playColor = White, nextPlayColor = Black;
		Initialization(board);

		//�ܖڕ��ׂ̃��C������
		while (menuSelect == Gomoku)
		{
			if (stornCount == INDEX_NUMBER * INDEX_NUMBER)
			{
				printf("�ܖڕ��ׂ��I������܂���\n");
				printf("�v���C���[�͔��A�b�n�l�͍��ł�\n");
				printf("�r���Ń��j���[�ɖ߂�ꍇ�� 9 ����͂��Ă�������\n\n");

				DrawBoard(board);
			}

			//�v���C���[
			printf("\n�v���C���[�̔Ԃł�\n");
			printf("�s��(�c)�F");
			row = PlayerInput();

			if (row == ReturnMenu)
				break;

			printf("��(��)�F");
			col = PlayerInput();
			printf("\n");

			if (col == ReturnMenu)
				break;

			//1�`8�œ��͂���悤�ɂȂ��Ă��邽�߁A����Ȃ��悤-1���Ă���z��Ɋi�[
			row--; col--;

			//���ɐ΂�����ꍇ�͍ē���
			if (board[row][col] != None)
			{
				printf("�w�肵���ꏊ�ɂ͊��ɐ΂��u����Ă��܂�\n");
				printf("�ē��͂��Ă�������\n");
				continue;
			}

			board[row][col] = White;

			whiteJudge = Gomoku_AllJudgement(board, col, row, White);
			if (whiteJudge >= WIN_NUMBER)
			{
				Gomoku_Result(board, whiteJudge, blackJudge); //���ʂ̕\��
				break;
			}

			DrawBoard(board);

			stornCount--;
			//printf("\n%d\n", stornCount);

			//�΂�u����ꏊ���Ȃ��Ȃ�����I��
			if (stornCount == 0)
			{
				Gomoku_Result(board, whiteJudge, blackJudge); //���ʂ̕\��
				break;
			}

			//�b�n�l
			printf("\n�b�n�l�̔Ԃł�\n");
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

		//�I�Z���̃��C������
		while (menuSelect == Reversi)
		{
			if (stornCount == INDEX_NUMBER * INDEX_NUMBER)
			{
				printf("�I�Z�����I������܂���\n");
				printf("�v���C���[�͔��A�b�n�l�͍��ł�\n");
				printf("�r���Ń��j���[�ɖ߂�ꍇ�� 9 ����͂��Ă�������\n\n");

				board[INDEX_NUMBER / 2][INDEX_NUMBER / 2] = White;
				board[INDEX_NUMBER / 2 - 1][INDEX_NUMBER / 2 - 1] = White;
				board[INDEX_NUMBER / 2 - 1][INDEX_NUMBER / 2] = Black;
				board[INDEX_NUMBER / 2][INDEX_NUMBER / 2 - 1] = Black;

				DrawBoard(board);
				stornCount--; //�J�n���ɂ����o�Ȃ��悤�ɂ��邽�߃J�E���g�����炷
			}

			//�v���C���[
			nextPlayColor = Reversi_NextTurnPlay(board, playColor);

			if (nextPlayColor == playColor)
				printf("\n�u���Ȃ����߃X�L�b�v���܂�\n");
			else if (nextPlayColor == None)
			{
				printf("\n�����I���ł�\n");
				Riversi_Result(board);
				break;
			}

			playColor = White;

			printf("\n�v���C���[�̔Ԃł�\n");
			printf("�s��(�c)�F");
			row = PlayerInput();

			if (row == ReturnMenu)
				break;

			printf("��(��)�F");
			col = PlayerInput();
			printf("\n");

			if (col == ReturnMenu)
				break;

			//1�`8�œ��͂���悤�ɂȂ��Ă��邽�߁A����Ȃ��悤-1���Ă���z��Ɋi�[
			row--; col--;

			if (Reversi_CanPutJudge(board, col, row, playColor) == No)
			{
				printf("�w��ꏊ�ɂ͒u���܂���\n");
				printf("�ē��͂��Ă�������\n");
				continue;
			}

			board[row][col] = White;

			Reversi_PutStorn(board, col, row, playColor);
			DrawBoard(board);

			//�b�n�l
			nextPlayColor = Reversi_NextTurnPlay(board, playColor);

			if (nextPlayColor == playColor)
				printf("\n�u���Ȃ����߃X�L�b�v���܂�\n");
			else if (nextPlayColor == None)
			{
				printf("\n�����I���ł�\n");
				Riversi_Result(board);
				break;
			}

			printf("\n�b�n�l�̔Ԃł�\n\n");
			playColor = Black;
			Reversi_COM(board, playColor);
			DrawBoard(board);
		}

	}

	printf("�v���O�������I�����܂�");

	return 0;
}