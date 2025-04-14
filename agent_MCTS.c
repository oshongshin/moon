
// agent_X.c
//���� �켱���� ��ǥ 3���� ���, �����ϰ� �δ� ���� 10�� �ùķ��̼��̶�� 
//�ʹ� �ٺ� ����, ������ �ͺ��� �������ϵ� ���� Ȯ���Ͽ� 
//����
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//���� �� �� �ִ�  board�� ��ǥ ����+ �켱���� ����

int blank[7][3] = { 0, };//���� ���� �ڸ�

int player = 0;// �÷��̾� ��ȣ �Է�
char symbols[2] = { '1', '2' };
char player_symbol = '1';//�ӽ� �ʱⰪ
char enemy_symbol = '2';

#define NO_THREAT 0
#define SINGLE_THREAT 1
#define DOUBLE_THREAT 2


void valid_col(char board[6][7]) {//�� �� �ִ� �� ã��_ blank �� ����
    for (int col = 0; col < 7; col++) {
        int placed = 0;
        for (int row = 5; row >= 0; row--) {
            if (board[row][col] == '0') {
                blank[col][0] = row;  // ���� ��
                blank[col][1] = col;  // �ش� ��
                placed = 1;
                break;
            }
        }
        if (!placed) {
            blank[col][0] = -1;  // ���� �� ����
            blank[col][1] = -1;
        }
    }
}
//��ȿ�� ���� ��ġ Ž��

int count_dir(char board[6][7], int r, int c, int dr, int dc, char symbol) {
    //Ư�� ����(dr.dc)��  ���ӵ� �÷��̾� ���� ����� 
    int count = 0;
    while (r >= 0 && r < 6 && c >= 0 && c < 7 && board[r][c] == symbol) {
        count++;//�� ���ӵɽ�  count����
        r += dr;//���������� �̵�
        c += dc;
    }
    return count;//�ش� ���⿡�� ���ӵ� �� ����
}
//�ش� ���� ���� ����� Ž��

int make_num(char board[6][7], int r, int c, char symbol, int num) {
    int total;
    int double_threat_check = 0;
    // ����(���� ������ �ջ�)
    total = count_dir(board, r, c, 0, -1, symbol) + count_dir(board, r, c + 1, 0, 1, symbol);
    if (total >= num) double_threat_check++;

    // ����
    total = count_dir(board, r, c, -1, 0, symbol) + count_dir(board, r + 1, c, 1, 0, symbol);
    if (total >= num) double_threat_check++;

    // �� �밢��
    total = count_dir(board, r, c, -1, -1, symbol) + count_dir(board, r + 1, c + 1, 1, 1, symbol);
    if (total >= num) double_threat_check++;

    // �� �밢��
    total = count_dir(board, r, c, -1, 1, symbol) + count_dir(board, r + 1, c - 1, 1, -1, symbol);
    if (total >= num) double_threat_check++;

    if (double_threat_check == 1) return SINGLE_THREAT;
    else if (double_threat_check > 1) return DOUBLE_THREAT;
    else return NO_THREAT;
}
//�� ���� ������ ���ӵ� num���� ���� ���� �� �ִ��� �˻�

int find_risk(char board[6][7], int r, int c) {//��밡 ���� ���� �� ��(r,c)�� ��� ����� �޼��� �� ���� 
    if (r + 1 >= 6 || board[r + 1][c] != '0') return 0;  // ���� �ٴ��̸� ���� ���� ����

    int enemy_can_4 = make_num(board, r + 1, c, enemy_symbol, 4);
    return enemy_can_4;
}
// ���� ���� �������� ��ġ (r, c)�� �ٷ� �Ʒ� ĭ (r+1, c)��
// ��밡 ���� �Ͽ� ���� �� �ְ�, �� �ڸ����� 4���� �Ǵ����� �Ǻ�

void set_priority(char board[6][7]) {

    for (int i = 0;i < 7; i++) {
        int enemy_num3 = make_num(board, blank[i][0], blank[i][1], enemy_symbol, 3);
        int player_num3 = make_num(board, blank[i][0], blank[i][1], player_symbol, 3);
        int enemy_num4 = make_num(board, blank[i][0], blank[i][1], enemy_symbol, 4);
        int player_num4 = make_num(board, blank[i][0], blank[i][1], player_symbol, 4);

        if (blank[i][0] == -1) {
            blank[i][2] = 10;
            continue; // ��ȿ���� ������ �켱���� ���� �ǳʶ�
        }
        blank[i][2] = 7; //���� ���� ��� 7
        if (i == 2 || i == 3 || i == 4) {//���� ���� ���(�߾�3��) 6
            blank[i][2] = 6;
        }

        if (enemy_num3 == SINGLE_THREAT && blank[i][2] > 4) {//��� ��� ���� 4
            blank[i][2] = 4;
        }
        if (player_num3 == SINGLE_THREAT && blank[i][2] > 3) {//�� ��� ����� 3
            blank[i][2] = 3;
        }

        if (player_num3 == DOUBLE_THREAT && blank[i][2] > 3) {//��������(�� 2���� ���) 2
            blank[i][2] = 2;
        }
        if (enemy_num4 == SINGLE_THREAT && blank[i][2] > 1) {//��� ��� ���� 1
            blank[i][2] = 1;
        }
        if (player_num4 == SINGLE_THREAT && blank[i][2] > 0) {//�� ��� ����� 0
            blank[i][2] = 0;
        }
        if (enemy_num3 == DOUBLE_THREAT && blank[i][2] > 0) { // ��� �������� ȸ��
            blank[i][2] = 8;
        }
        int risk = find_risk(board, blank[i][0], blank[i][1]);
        if (risk >= 1 && blank[i][2] > 0) {//�ǵ��� ���ؾ��� �ڸ�. ��밡 ���� ��� ��� ���� 9
            blank[i][2] = 9;
        }
    }
}
//�켱���� �Է�

int simulate_game(char board[6][7], int idx, char symbol) {
    // �� �ڸ��� ���� ���� ������ �÷��� �غ� ��
    // symbol�� �̱�ٸ� return 1, �ƴϸ� return 0
    char sim_board[6][7];

    // 1. ���� ����
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            sim_board[i][j] = board[i][j];
        }
    }

    // 2. ���õ� ���� �� ����
    int row = -1;
    for (int r = 5; r >= 0; r--) {
        if (sim_board[r][idx] == '0') {
            sim_board[r][idx] = symbol;
            row = r;
            break;
        }
    }

    if (row == -1) return 0; // ���� �� ����

    // 3. �ٷ� 4���� �Ǵ��� Ȯ��
    if (make_num(sim_board, row, idx, symbol, 4) != NO_THREAT) return 1;

    // 4. ��� �ɺ��� ����
    char cur_symbol = (symbol == '1') ? '2' : '1';

    // 5. �������� �����ư��� ���� �δٰ� �º� �Ǻ�
    for (int turn = 0; turn < 20; turn++) {
        int valid_cols[7];
        int count = 0;

        for (int c = 0; c < 7; c++) {
            if (sim_board[0][c] == '0') valid_cols[count++] = c;
        }

        if (count == 0) break; // ���º�

        int col = valid_cols[rand() % count];

        int placed_row = -1;
        for (int r = 5; r >= 0; r--) {
            if (sim_board[r][col] == '0') {
                sim_board[r][col] = cur_symbol;
                placed_row = r;
                break;
            }
        }

        if (placed_row != -1 && make_num(sim_board, placed_row, col, cur_symbol, 4) != NO_THREAT) {
            return (cur_symbol == symbol) ? 1 : 0;
        }

        cur_symbol = (cur_symbol == '1') ? '2' : '1';
    }

    // ���� ����
    return 0;
}

int choose_best_col(char board[6][7], char symbol) {
    int candidate_index[3]; // blank �迭���� �ε����� ����
    int candidate_count = 0;

    // �켱���� ����(����) ������ �ִ� 3�� �ĺ� �߸���
    for (int priority = 0; priority < 9 && candidate_count < 3; priority++) {
        for (int i = 0; i < 7 && candidate_count < 3; i++) {
            if (blank[i][0] != -1 && blank[i][2] == priority) {
                candidate_index[candidate_count++] = i;
            }
        }
    }

    // �ĺ��� �ִٸ� MCTS ������
    if (candidate_count > 0) {
        int best_idx = -1;
        int best_score = -1;

        for (int k = 0; k < candidate_count; k++) {
            int idx = candidate_index[k];
            int win_count = 0;
            //10�� �ݺ�
            for (int trial = 0; trial < 10; trial++) {
                win_count += simulate_game(board, idx, symbol);
            }

            if (win_count > best_score) {
                best_score = win_count;
                best_idx = idx;
            }
        }

        if (best_idx != -1)
            return blank[best_idx][1];  // �� ��ȣ ��ȯ
    }

    // �߾� �켱
    if (blank[3][0] != -1) return 3;
    else if (blank[2][0] != -1) return 2;
    else if (blank[4][0] != -1) return 4;

    // �� �� ������ ���� �� ��ȯ
    while (1) {
        int r = rand() % 7;
        if (blank[r][0] != -1) return r;
    }
}
//�켱������ ���� ���� ��ȯ, ��� 5�Ͻ� �׳� D(�߾�)����


int main() {
    int player_num;
    char board[6][7];

    scanf("%d", &player_num); //�÷��̾� ��ȣ �Է¹ޱ�
    player = player_num - 1;
    player_symbol = symbols[player];
    enemy_symbol = symbols[1 - player];

    for (int i = 0; i < 6; i++) {  // ���� ���� �Է�
        for (int j = 0; j < 7; j++) {
            scanf(" %c", &board[i][j]);
        }
    }
    //���� �õ� ����
    srand(time(NULL));

    //���� ����
    valid_col(board);
    set_priority(board);
    int best_col = choose_best_col(board, player_symbol);
    printf("%c", 'A' + best_col);

    // ��� ��� (A~G)
    printf("%c", 'A' + best_col);

    /////////////////////////////////////������
    // ����� ���: best_col ��ǥ �� �켱���� (stderr��)
    int min_priority = 11;
    int best_index = -1;
    for (int i = 0; i < 7; i++) {
        if (blank[i][2] < min_priority) {
            min_priority = blank[i][2];
            best_index = i;
        }
    }

    if (best_index != -1) {
        fprintf(stderr, "\n[DEBUG] best_col: %d, row: %d, col: %d, priority: %d\n",
            best_index,
            blank[best_index][0],
            blank[best_index][1],
            blank[best_index][2]);

        // ��ü blank �迭 �켱���� ���
        fprintf(stderr, "[DEBUG] ��ü blank ����:\n");
        for (int i = 0; i < 7; i++) {
            fprintf(stderr, "col %d �� row: %d, priority: %d\n",
                blank[i][1], blank[i][0], blank[i][2]);
        }
    }

    return 0;

}