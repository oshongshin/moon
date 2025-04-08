#include <stdio.h>
#include <time.h>
#include <unistd.h>
#define ROW 8 /*��(����)*/
#define COL 7 /*��(����)*/
int board[ROW][COL];

int initialize_board(int board[ROW][COL]) {/*��� ĭ�� 0���� �ʱ�ȭ*/
    for (int R = 0;R < ROW;R++) {
        for (int C = 0;C < COL;C++) {
            board[R][C] = 0;
        }
    }
    return 0;
}
int print_board(int board[ROW][COL]) {/*�� ĭ�� 0,1,2 �� ǥ���ؼ� ���, */
    printf("A B C D E F G\n");
    for (int R = 0;R < ROW;R++) {
        for (int C = 0;C < COL;C++) {
            printf("%d ", board[R][C]);
            /*�� ���� �� �̸� ���(A-G)*/
        }
        printf("\n");
    }
}
int drop_piece(int board[ROW][COL], int col,int player) {
    /* ���� column�� ������ ����ų� �� �� �ִٸ� ���� (0 ��ȯ)
    �Ʒ����� ���� �ö󰡸� �� ĭ ã��
    ã���� player ��ȣ�� ä��� ���� (1 ��ȯ)
    */
    if (board[row][col] != 0) {/*�� �� ����, �� ĭ ������ ����*/
        return 0;
    }
    while (1) {
        if (board[row][col] == 0 && board[row][col - 1] != 0) {/*�� ���� �� ���� ���� �ִ� ĭ �߰�*/
            board[row][col] = player;
            return 1;
        }
        col--;
        if (col <= 0) {
            printf("����");
            return 0;
            break;     
        }
    }
}
double three_second() {/*3�� ���� �Լ�*/
    clock_t start, end;
    start = clock();
    //���� ����- ���� ���� �� ��. main���� start, end �޾ƾ��ҵ�?
    end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    return duration;

}
int get_agent_move(agent_path, player, board) {
    /*
    pipe()�� �ڽ� ���μ����� ����� ������ ����
    fork()�� �ڽ� ���μ��� ����
    �ڽ��� ǥ�� ������� pipe�� �����̷�Ʈ �� execl(agent ����)
    �θ�� ���� ���¸� �ڽĿ��� ������, ������ 3�� ���� ��ٸ�
        ���� ���ų� �߸��� ���̸� 'X' ��ȯ
    ���� ����(move)�� �޾� �빮�ڷ� ��ȯ�ؼ� ��ȯ
    */
}
int check_winner(board) {
    /* �� ĭ�� �˻��ؼ�
    ����, ����, �밢��(��, ��)���� 4�� �����̸� �¸� (�ش� player ��ȣ ��ȯ)
    ������ 0 ��ȯ
    */
    int winner;
    while (1){
    for (int R = ROW-1;R >=0;R--) {
        for (int C = 0;C < COL;C++) {
            if (board[R][C] == board[R][C - 1] &&
                board[R][C - 1] == board[R][C - 2] &&
                board[R][C - 2] == board[R][C - 3]) {/*����*/
                winner = board[R][C];
                return winner;
            }
            else if (board[R][C] == board[R - 1][C] &&
                board[R-1][C] == board[R - 2][C] &&
                board[R-2][C] == board[R - 3][C]) {/*����*/
                winner = board[R][C];
                return winner;
            }
            else if (board[R][C] == board[R - 1][C - 1] &&
                board[R-1][C-1] == board[R - 2][C - 2] &&
                board[R-2][C-2] == board[R - 3][C - 3]) {
                winner = board[R][C];
                return winner;
            }
            else {
                return 0;
            }
        }
    }
}
int main() {
    /*
    ����� ���� Ȯ�� (-X agent1 -Y agent2 ����)
    ���� �ʱ�ȭ
    �ʱ� ���� ���

    while true:
        ���� ���� agent�κ��� move �޾ƿ���
        move ���ڸ� �� ��ȣ�� ��ȯ

        ���� ���� Ȯ��:
            - 3�� �� ���� ������ -> �߸��� move ó�� ('X')
            - move�� ��ȿ���� ���� ���ڰų� ���� ����� �й�
            - drop_piece ���� (���� �� ��)�ϸ� �й�

        ���忡 �� ����
        ���� ���

        ���� Ȯ��:
            - ���� ������ ����

        ���� ������ ��ȯ
    */
}
