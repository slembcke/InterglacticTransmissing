#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sysexits.h>
//#include <pthread.h>
#include <dispatch/dispatch.h>

#include <string.h>
#include <alloca.h>

typedef uint64_t Row;

static inline bool
Get(const Row *board, const int x, const int y)
{
	return ((board[y] >> x)&1);
}

static inline void
Set(Row *board, const int x, const int y)
{
	board[y] |= (1 << x);
}

static void
PrintBoard(const int size, const Row *board)
{
	for(int i=0; i<size; i++){
		printf("%X ", i);
		for(int j=0; j<size; j++){
			printf("%c", Get(board, j, i) ? 'o' : '.');
		}
		printf("\n");
	}
	
	printf("  ");
	for(int i=0; i<size; i++) printf("%X", i);
	printf("\n");
}

static inline bool
Blocked(const int x, const int y, const int size, const Row *board)
{
	return
		x < 0 || size <= x ||
		y < 0 || size <= y ||
		Get(board, x, y);
}

static bool
Check(const int size, Row *board)
{
	const Row solution = (1<<size) - 1;
	Row fold = solution;
	for(int i=0; i<size; i++) fold &= board[i];
	
	return (fold == solution);
}

struct Move {
	const int x, y;
	const char *dir;
	const int choices;
	
	struct Move *next;
};

static struct Move *
NewMove(const int x, const int y, const char *dir, const int choices, struct Move *next)
{
	struct Move *move = malloc(sizeof(struct Move));
	(*move) = (struct Move){x, y, dir, choices, next};
	
	return move;
}

static void
FreeMoves(struct Move * moves)
{
	if(moves){
		FreeMoves(moves->next);
		free(moves);
	}
}

static struct Move * Splat(const int x, const int y, const int size, const Row *board);

static struct Move * const
Trace(const int x, const int y, const int size, const Row *_board, const int dx, const int dy)
{
	Row board[size];
	memcpy(board, _board, sizeof(board));
	Set(board, x, y);
	
	int tx = x + dx, ty = y + dy;
	while(true){
		Set(board, tx, ty);
		
		if(Blocked(tx + dx, ty + dy, size, board)){
			break;
		}
		
		tx += dx, ty += dy;
	}
	
	if(Check(size, board)){
		return NewMove(tx, ty, "!", 0, NULL);
	} else {
		return Splat(tx, ty, size, board);
	}
}

static struct Move *
Splat(const int x, const int y, const int size, const Row *board)
{
	bool l = !Blocked(x - 1, y + 0, size, board);
	bool d = !Blocked(x + 0, y - 1, size, board);
	bool r = !Blocked(x + 1, y + 0, size, board);
	bool u = !Blocked(x + 0, y + 1, size, board);
	int choices = !!l + !!d + !!r + !!u;
	
	struct Move *moves;
	if(l && (moves = Trace(x, y, size, board, -1,  0))) return NewMove(x, y, "left", choices, moves);
	if(d && (moves = Trace(x, y, size, board,  0, -1))) return NewMove(x, y, "up", choices, moves);
	if(r && (moves = Trace(x, y, size, board,  1,  0))) return NewMove(x, y, "right", choices, moves);
	if(u && (moves = Trace(x, y, size, board,  0,  1))) return NewMove(x, y, "down", choices, moves);
	
	return NULL;
}

static void
RandomBoard(const int size, Row *board, const int blockers)
{
	for(int i=0; i<blockers;){
		const int x = arc4random()%size, y = arc4random()%size;
		
		if(!Get(board, x, y)){
			Set(board, x, y);
			i++;
		}
	}
}

static struct Move *
Solve(const int size, const Row *board)
{
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			if(!Get(board, i, j)){
				struct Move *moves = Splat(i, j, size, board);
				if(moves) return moves;
			}
		}
	}
	
	return false;
}

static Row * const
FindPuzzle(const int size, const int blockers, int *unsolvable)
{
	Row board[size];
	
	for(;;){
		bzero(board, sizeof(board));
		RandomBoard(size, board, blockers);
		
		struct Move *moves = Solve(size, board);
		if(moves){
			FreeMoves(moves);
			
			Row *copy = malloc(sizeof(board));
			memcpy(copy, board, sizeof(board));
			
			return copy;
		} else if(unsolvable) {
			(*unsolvable)++;
		}
	}
}

static void
PrintSolution(struct Move *moves)
{
	int choices = 1, steps = 1;
	printf(" 1) Start at (%d, %d). (1 choice)\n", moves->x, moves->y);
	
	for(struct Move *move = moves; move; move = move->next){
		if(move->next){
			printf("% 2d) Go %s. (%d choices)\n", steps + 1, move->dir, move->choices);
			choices += move->choices;
			steps++;
		} else {
			printf("Puzzle completed.\n");
		}
	}
	
	printf("Total choices: %d. Total steps: %d. Extra choices: %d.\n", choices, steps, choices - steps);
}

static void
PrintAsJS(const int size, const Row *board, struct Move *moves)
{
	printf("\t\t{puzz:\"");
	
	for(int i=0; i<size; i++){
		unsigned row = board[i];
		printf("0x%04X,", row);
	}
	printf("\", ");
	
	printf("start:[%d, %d]},\n", moves->x, moves->y);
}

static void
RunDispatch(const int count, const int size, const int blockers)
{
	dispatch_queue_t puzzles = dispatch_queue_create("PuzzleQueue", NULL);
	dispatch_group_t join = dispatch_group_create();
	
	dispatch_queue_t io = dispatch_queue_create("IOQueue", NULL);
	__block int puzzle = 1;
	__block int total_unsolved = 0;
	
	for(int i=0; i<count; i++){
		dispatch_group_async(join, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
			int unsolved;
			Row *board = FindPuzzle(size, blockers, &unsolved);
			
			dispatch_group_async(join, io, ^{
				printf("Puzzle %d of %d.\n", puzzle, count);
				puzzle++;
				
				PrintBoard(size, board);
				printf("\n");
				
				struct Move *moves = Solve(size, board);
				
				PrintAsJS(size, board, moves);
				printf("\n");
				
				PrintSolution(moves);
				FreeMoves(moves);
				free(board);
				
				printf("Tried %d unsolvable puzzles.\n\n\n", unsolved);
				fflush(stdout);
				
				total_unsolved += unsolved;
			});
		});
	}
	
	dispatch_group_wait(join, DISPATCH_TIME_FOREVER);
	printf("Total unsolved puzzles tried: %d", total_unsolved);
}

int
main(int argc, char **argv)
{
	if(argc != 4) errx(EX_USAGE, "genpuzzles count size blockers");
	const int count = atoi(argv[1]);
	const int size = atoi(argv[2]);
	const int blockers = atoi(argv[3]);
	
	RunDispatch(count, size, blockers);
	
	return EX_OK;
}
