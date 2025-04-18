
// agent_X.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//돌이 들어갈 수 있는  board의 좌표 저장+ 우선순위 저장

int blank[7][3] = { 0, };//돌이 놓일 자리

int player = 0;// 플레이어 번호 입력
char symbols[2] = { '1', '2' };
char player_symbol = '1';//임시 초기값
char enemy_symbol = '2';

#define NO_THREAT 0
#define SINGLE_THREAT 1
#define DOUBLE_THREAT 2

void valid_col(char board[6][7]) {//둘 수 있는 열 찾기_ blank 에 저장
    for (int col = 0; col < 7; col++) {
        int placed = 0;
        for (int row = 5; row >= 0; row--) {
            if (board[row][col] == '0') {
                blank[col][0] = row;  // 놓일 행
                blank[col][1] = col;  // 해당 열
                placed = 1;
                break;
            }
        }
        if (!placed) {
            blank[col][0] = -1;  // 놓을 수 없음
            blank[col][1] = -1;
        }
    }
}
//유효한 돌의 위치 탐색

int count_dir(char board[6][7], int r, int c, int dr, int dc, char symbol) {
    //특정 방향(dr.dc)에  연속된 플레이어 돌은 몇개인지 
    int count = 0;
    while (r >= 0 && r < 6 && c >= 0 && c < 7 && board[r][c] == symbol) {
        count++;//말 연속될시  count증가
        r += dr;//방향쪽으로 이동
        c += dc;
    }
    return count;//해당 방향에서 연속된 말 갯수
}
//해당 방향 돌이 몇개인지 탐색

int make_num(char board[6][7], int r, int c, char symbol, int num) {
    int total;
    int double_threat_check = 0;
    // 가로(왼쪽 오른쪽 합산)
    total = count_dir(board, r, c, 0, -1, symbol) + count_dir(board, r, c + 1, 0, 1, symbol);
    if (total >= num) double_threat_check++;

    // 세로
    total = count_dir(board, r, c, -1, 0, symbol) + count_dir(board, r + 1, c, 1, 0, symbol);
    if (total >= num) double_threat_check++;

    // ↘ 대각선
    total = count_dir(board, r, c, -1, -1, symbol) + count_dir(board, r + 1, c + 1, 1, 1, symbol);
    if (total >= num) double_threat_check++;

    // ↗ 대각선
    total = count_dir(board, r, c, -1, 1, symbol) + count_dir(board, r + 1, c - 1, 1, -1, symbol);
    if (total >= num) double_threat_check++;

    if (double_threat_check == 1) return SINGLE_THREAT;
    else if (double_threat_check > 1) return DOUBLE_THREAT;
    else return NO_THREAT;
}
//그 돌을 놓으면 연속된 num개의 돌을 만들 수 있는지 검사

int find_risk(char board[6][7],int r,int c) {//상대가 지금 놓은 내 돌(r,c)을 밟고 사목을 달성할 수 있음 
    if (r + 1 >= 6 || board[r + 1][c] != '0') return 0;  // 가장 바닥이면 밟힐 일이 없음

    int enemy_can_4 = make_num(board, r + 1, c, enemy_symbol, 4);
    return enemy_can_4;
}
// 내가 돌을 놓으려는 위치 (r, c)의 바로 아래 칸 (r+1, c)에
// 상대가 다음 턴에 놓을 수 있고, 그 자리에서 4목이 되는지를 판별

void set_priority(char board[6][7]) {
    
    for (int i = 0;i < 7; i++) {
        int enemy_num3 = make_num(board, blank[i][0], blank[i][1], enemy_symbol, 3);
        int player_num3 = make_num(board, blank[i][0], blank[i][1], player_symbol, 3);
        int enemy_num4 = make_num(board, blank[i][0], blank[i][1], enemy_symbol, 4);
        int player_num4 = make_num(board, blank[i][0], blank[i][1], player_symbol, 4);

        if (blank[i][0] == -1) {
            blank[i][2] = 10;
            continue; // 유효하지 않으면 우선순위 설정 건너뜀
        }
        blank[i][2] = 7; //랜덤 선택 대상 7
        if (i == 2 || i == 3 || i == 4) {//랜덤 선택 대상(중앙3열) 6
            blank[i][2] = 6;
        }

        if (enemy_num3 == SINGLE_THREAT && blank[i][2] > 4) {//상대 삼목 막기 4
            blank[i][2] = 4;
        }
        if (player_num3 == SINGLE_THREAT && blank[i][2] > 3) {//내 삼목 만들기 3
            blank[i][2] = 3;
        }
        
        if (player_num3 == DOUBLE_THREAT && blank[i][2] > 3) {//이중위협(내 2개의 삼목) 2
            blank[i][2] = 2;
        }
        if (enemy_num4 == SINGLE_THREAT && blank[i][2] > 1) {//상대 사목 막기 1
            blank[i][2] = 1;
        }
        if (player_num4 == SINGLE_THREAT && blank[i][2] > 0) {//내 사목 만들기 0
            blank[i][2] = 0;
        }
        if (enemy_num3 == DOUBLE_THREAT && blank[i][2] > 0) { // 상대 이중위협 회피
            blank[i][2] = 8;
        }
        int risk = find_risk(board, blank[i][0], blank[i][1]);
        if (risk>=1 && blank[i][2] > 0) {//되도록 피해야할 자리. 상대가 나를 밟고 사목 가능 9
            blank[i][2] = 9;
        }
    }
}
//우선순위 입력

int choose_best_col() {//우선순위가 높은 열을 반환, 모두 5일시 그냥 D(중앙)선택
    int priority = 0;
    int i = 0;
    while (priority < 9) {//우선순위가 높은 열이 가장 먼저 반환됨 
        for (int i = 0;i < 7;i++) {
            if (blank[i][0] != -1 && blank[i][2] == priority) return i;
        }
        priority++;
    }
    if (blank[3][0] != -1) return 3;
    else if (blank[2][0] != -1) return 2;
    else if (blank[4][0] != -1) return 4;

    // 그 외 가능한 랜덤 열 반환
    while (1) {
        int r = rand() % 7;
        if (blank[r][0] != -1) return r;
    }
}
//우선순위가 높은 열을 반환, 모두 5일시 그냥 D(중앙)선택


int main() {
    int player_num;
    char board[6][7];
    
    scanf("%d", &player_num); //플레이어 번호 입력받기
    player = player_num - 1;
    player_symbol = symbols[player];
    enemy_symbol = symbols[1 - player];

    for (int i = 0; i < 6; i++) {  // 보드 상태 입력
        for (int j = 0; j < 7; j++) {
            scanf(" %c", &board[i][j]);
        }
    }
    //랜덤 시드 설정
    srand(time(NULL));

    //전략 실행
    valid_col(board);
    set_priority(board);
    int best_col = choose_best_col();

    // 결과 출력 (A~G)
    printf("%c", 'A' + best_col);

    /////////////////////////////////////디버깅용
    // 결과 출력 (A~G)
    printf("[MOVE] %c\n", 'A' + best_col);

    // 디버깅 출력: best_col 좌표 및 우선순위 (stderr로)
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

        // 전체 blank 배열 우선순위 출력
        fprintf(stderr, "[DEBUG] 전체 blank 상태:\n");
        for (int i = 0; i < 7; i++) {
            fprintf(stderr, "col %d → row: %d, priority: %d\n",
                blank[i][1], blank[i][0], blank[i][2]);
        }
    }

    return 0;

}