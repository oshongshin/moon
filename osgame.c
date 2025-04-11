// windeow + MinGW ȯ�濡���� `fork(), sys/wait.h ���Ұ�`
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define READ 0
#define WRITE 1

char board[6][7];
char player_symbols[2] = { '1', '2' };
pid_t current_child_pid;

int moves = 0;

void handle_sigchld(int sig) {
    int saved_errno = errno; // ����Ʈ���� ��ȣ��
    while (waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

void visualize_board(int current_player, int moves)
{
    printf("\nCurrent Board:\n");
    printf("A B C D E F G\n");
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            char symbol = board[i][j];
            printf("%c ", symbol);
        }
        printf("\n");
    }
    char player_char = (player_symbols[current_player] == '1') ? 'X' : 'Y';
    printf("\nTurn %d-Agent%c move the stone#%d\n", moves + 1, player_char, (moves / 2) + 1);
}

int validate_choice(char choice)
{
    return (choice >= 'A' && choice <= 'G');
}

int update_board(char choice, int player)
{
    int col = choice - 'A';
    for (int i = 5; i >= 0; i--) {
        if (board[i][col] == '0') {
            board[i][col] = player_symbols[player];
            return 1;
        }
    }
    return 0;
}

int check_winner(int player)
{
    char symbol = player_symbols[player];

    // ����, ����, �밢������ 4���� ���ӵǾ����� �˻�
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            if (board[row][col] != symbol)
                continue;

            // ���� üũ
            if (col <= 3 && board[row][col + 1] == symbol && board[row][col + 2] == symbol && board[row][col + 3] == symbol)
                return 1;
            // ���� üũ
            if (row <= 2 && board[row + 1][col] == symbol && board[row + 2][col] == symbol && board[row + 3][col] == symbol)
                return 1;
            // �밢�� üũ(������ �Ʒ�)
            if (row <= 2 && col <= 3 && board[row + 1][col + 1] == symbol && board[row + 2][col + 2] == symbol && board[row + 3][col + 3] == symbol)
                return 1;
            // �밢�� üũ(���� �Ʒ�)
            if (row <= 2 && col >= 3 && board[row + 1][col - 1] == symbol && board[row + 2][col - 2] == symbol && board[row + 3][col - 3] == symbol)
                return 1;
        }
    }
    return 0;
}


void timeout_handler(int sig) {
    printf("\nAgent timed out! Opponent wins!\n");
    kill(current_child_pid, SIGKILL); // �ڽ� ���μ��� ���� ����
    exit(EXIT_FAILURE); // ���� ���μ����� ������� ���� ����
}

void handle_sigint(int sig)
{
    printf("\nInterrupted! Cleaning up...\n");
    kill(current_child_pid, SIGKILL);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    if (argc != 5 || strcmp(argv[1], "-X") != 0 || strcmp(argv[3], "-Y") != 0) {
        printf("Usage: ./gamatch -X <agent-binary> -Y <agent-binary>\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handle_sigint); // Ctrl+C �ڵ鷯
    signal(SIGALRM, timeout_handler);
    signal(SIGCHLD, handle_sigchld);

    memset(board, '0', sizeof(board));
    int current_player = 0;
    int winner = -1;

    while (winner == -1 && moves < 42) {
        visualize_board(current_player, moves);
        sleep(1);

        int ptoc[2], ctop[2];
        pipe(ptoc);
        pipe(ctop);

        current_child_pid = fork();

        if (current_child_pid == 0) {
            dup2(ptoc[READ], STDIN_FILENO);
            dup2(ctop[WRITE], STDOUT_FILENO);
            close(ptoc[WRITE]);
            close(ctop[READ]);

            execl(argv[current_player * 2 + 2], argv[current_player * 2 + 2], NULL);
            perror("exec");
            exit(EXIT_FAILURE);
        }

        close(ptoc[READ]);
        close(ctop[WRITE]);

        FILE* agent_input = fdopen(ptoc[WRITE], "w");
        fprintf(agent_input, "%d\n", current_player + 1);
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 7; j++) {
                fprintf(agent_input, "%c ", board[i][j]);
            }
            fprintf(agent_input, "\n");
        }
        fclose(agent_input);


        struct itimerval timer;
        timer.it_value.tv_sec = 3;
        timer.it_value.tv_usec = 0;
        timer.it_interval.tv_sec = 0;
        timer.it_interval.tv_usec = 0;
        setitimer(ITIMER_REAL, &timer, NULL);

        int status;
        waitpid(current_child_pid, &status, 0);

        timer.it_value.tv_sec = 0;
        setitimer(ITIMER_REAL, &timer, NULL);

        char agent_choice = '\0';
        read(ctop[READ], &agent_choice, 1);
        close(ctop[READ]);

        if (!validate_choice(agent_choice) || !update_board(agent_choice, current_player)) {
            printf("Invalid move! Opponent wins!\n");
            winner = 1 - current_player;
            break;
        }

        if (check_winner(current_player)) {
            winner = current_player;
            break;
        }

        current_player = 1 - current_player;
        moves++;
    }

    visualize_board(current_player, moves);
    if (winner != -1) {
        char winner_symbol = player_symbols[winner];
        char winner_name = (winner_symbol == '1') ? 'X' : 'Y';
        int point = (moves + 1) / 2;
        printf("Player %c wins!\n", winner_name);
        printf("%d point\n", point);
    }
    else {
        printf("The game ended in a draw!\n");
    }

    return EXIT_SUCCESS;
}

/*
Main()
��
������ �ʱ�ȭ
��     ������ ��ɾ� �μ� �Ľ�
��     ������ Signal �ڵ鷯 ���(signal, setitimer)
��     ������ ���� ���� �ʱ�ȭ
��
������ ���� ���� (while)
      ������ ���� �ð�ȭ (visualize_board)
      ������ ������ ����(pipe)
      ������ ���μ��� ����(fork)
      ��    ���� �ڽ� ���μ���(exec)
      ��    ���� �θ� ���μ���
      ��        ���� ������Ʈ �Է� ����
      ��        ���� SIGALRM ��� Ÿ�Ӿƿ� ���� (setitimer)
      ��        ���� �ڽ� ���μ��� ���(waitpid)
      ��        ���� IPC�� ���� ���� ���� �� ����
      ������ ���� ���� ���� üũ(check_winner)
*/