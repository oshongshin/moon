#include <stdio.h>
#include <time.h>
#include <unistd.h>
#define ROW 8 /*행(가로)*/
#define COL 7 /*열(세로)*/
int board[ROW][COL];

int initialize_board(int board[ROW][COL]) {/*모든 칸을 0으로 초기화*/
    for (int R = 0;R < ROW;R++) {
        for (int C = 0;C < COL;C++) {
            board[R][C] = 0;
        }
    }
    return 0;
}
int print_board(int board[ROW][COL]) {/*각 칸을 0,1,2 로 표시해서 출력, */
    printf("A B C D E F G\n");
    for (int R = 0;R < ROW;R++) {
        for (int C = 0;C < COL;C++) {
            printf("%d ", board[R][C]);
            /*맨 위에 열 이름 출력(A-G)*/
        }
        printf("\n");
    }
}
int drop_piece(int board[ROW][COL], int col,int player) {
    /* 만약 column이 범위를 벗어나거나 꽉 차 있다면 실패 (0 반환)
    아래부터 위로 올라가며 빈 칸 찾기
    찾으면 player 번호로 채우고 성공 (1 반환)
    */
    if (board[row][col] != 0) {/*꽉 찬 스택, 빈 칸 없으면 실패*/
        return 0;
    }
    while (1) {
        if (board[row][col] == 0 && board[row][col - 1] != 0) {/*빈 스택 중 가장 위에 있는 칸 발견*/
            board[row][col] = player;
            return 1;
        }
        col--;
        if (col <= 0) {
            printf("오류");
            return 0;
            break;     
        }
    }
}
double three_second() {/*3초 측정 함수*/
    clock_t start, end;
    start = clock();
    //성능 측정- 아직 구현 덜 됨. main에서 start, end 받아야할듯?
    end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    return duration;

}
int get_agent_move(agent_path, player, board) {
    /*
    pipe()로 자식 프로세스와 통신할 파이프 생성
    fork()로 자식 프로세스 생성
    자식은 표준 입출력을 pipe로 리다이렉트 후 execl(agent 실행)
    부모는 보드 상태를 자식에게 보내고, 응답을 3초 동안 기다림
        응답 없거나 잘못된 값이면 'X' 반환
    응답 문자(move)를 받아 대문자로 변환해서 반환
    */
}
int check_winner(board) {
    /* 각 칸을 검사해서
    가로, 세로, 대각선(↘, ↙)으로 4개 연속이면 승리 (해당 player 번호 반환)
    없으면 0 반환
    */
    int winner;
    while (1){
    for (int R = ROW-1;R >=0;R--) {
        for (int C = 0;C < COL;C++) {
            if (board[R][C] == board[R][C - 1] &&
                board[R][C - 1] == board[R][C - 2] &&
                board[R][C - 2] == board[R][C - 3]) {/*가로*/
                winner = board[R][C];
                return winner;
            }
            else if (board[R][C] == board[R - 1][C] &&
                board[R-1][C] == board[R - 2][C] &&
                board[R-2][C] == board[R - 3][C]) {/*세로*/
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
    명령행 인자 확인 (-X agent1 -Y agent2 형식)
    보드 초기화
    초기 보드 출력

    while true:
        현재 턴의 agent로부터 move 받아오기
        move 문자를 열 번호로 변환

        실패 조건 확인:
            - 3초 내 응답 없으면 -> 잘못된 move 처리 ('X')
            - move가 유효하지 않은 문자거나 범위 벗어나면 패배
            - drop_piece 실패 (열이 꽉 참)하면 패배

        보드에 말 놓기
        보드 출력

        승자 확인:
            - 승자 있으면 종료

        다음 턴으로 전환
    */
}
