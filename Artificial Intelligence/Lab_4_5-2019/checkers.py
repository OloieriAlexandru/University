# pip install graphics.py

from graphics import GraphWin, Circle, Line, Oval, Text, Point, Rectangle, color_rgb

TABLE_SIZE = 8
PLAYER_1_PIECE = 1
PLAYER_2_PIECE = 2
UI_SQUARE_DIM = 80
PLAYERS_COLORS = [
    color_rgb(255, 255, 255),
    color_rgb(0, 0, 0),
    color_rgb(200, 200, 200),
    color_rgb(55, 55, 55)
]
TABLE_COLORS = [
    color_rgb(255, 255, 255),
    color_rgb(150, 150, 150),
    color_rgb(255, 255, 255),
    color_rgb(150, 150, 150)
]
DLINE = {
    1: [1, 1],
    2: [-1, -1],
    3: [-1, -1, 1, 1],
    4: [-1, -1, 1, 1]
}
DCOL = {
    1: [-1, 1],
    2: [-1, 1],
    3: [-1, 1, -1, 1],
    4: [-1, 1, -1, 1]
}
INF = 10000000000000000000000
INF_MINIMAX = INF - 1
MINIMAX_START_LEVEL = 8


def in_interior(i, j):
    return i >= 0 and j >= 0 and i < TABLE_SIZE and j < TABLE_SIZE


class Table:
    def __init__(self, size):
        self.size = size
        self.t = [self.get_table_line(size, i) for i in range(size)]

    def get_table_line(self, size, current_line):
        line = []

        for i in range(size):
            line.append(0)

        if not (current_line == size // 2 or current_line == size // 2 - 1):
            start = 1
            current_player = PLAYER_1_PIECE if current_line < size // 2 else PLAYER_2_PIECE
            if current_line % 2 == 1:
                start = 0

            for i in range(size // 2):
                line[start + i * 2] = current_player

        return line

    def count_players_pieces(self):
        found_pieces = [0, 0]

        for line in self.t:
            for piece in line:
                if piece == 0:
                    continue

                piece = (piece - 1) % 2
                found_pieces[piece] += 1

        return found_pieces

    def set_piece(self, player, line, column):
        self.t[line][column] = player

    def get_piece(self, line, column):
        return self.t[line][column]

    def free_spot(self, line, column):
        return self.t[line][column] == 0

    def get_player_pieces(self, player):
        pieces = []

        for i in range(self.size):
            for j in range(self.size):
                if self.t[i][j] != 0 and (self.t[i][j] % 2) == (player % 2):
                    if (self.piece_can_capture(i, j, player)):
                        return self.get_player_pieces_capture(player)
                    pieces.append((i, j))

        return (pieces, False)

    def get_player_pieces_capture(self, player):
        pieces = []

        for i in range(self.size):
            for j in range(self.size):
                if self.t[i][j] != 0 and (self.t[i][j] % 2) == (player % 2) and self.piece_can_capture(i, j, player):
                    pieces.append((i, j))

        return (pieces, True)

    def piece_can_capture(self, line, column, player):
        for k in range(len(DLINE[player])):
            adj_line = line + DLINE[player][k]
            adj_col = column + DCOL[player][k]

            if not in_interior(adj_line, adj_col) or self.t[adj_line][adj_col] == 0 or (self.t[adj_line][adj_col] % 2) == (self.t[line][column] % 2):
                continue

            oth_line = line + 2 * DLINE[player][k]
            oth_col = column + 2 * DCOL[player][k]

            if not in_interior(oth_line, oth_col) or self.t[oth_line][oth_col] != 0:
                continue

            return True
        return False

    def get_possible_moves_from_position(self, player, move, captured):
        line = move[0]
        column = move[1]
        moves = []

        if captured:
            for k in range(len(DLINE[player])):
                adj_line = line + DLINE[player][k]
                adj_col = column + DCOL[player][k]

                if not in_interior(adj_line, adj_col) or self.t[adj_line][adj_col] == 0 \
                        or (self.t[adj_line][adj_col] % 2) == (self.t[line][column] % 2):
                    continue

                oth_line = line + 2 * DLINE[player][k]
                oth_col = column + 2 * DCOL[player][k]

                if not in_interior(oth_line, oth_col) or self.t[oth_line][oth_col] != 0:
                    continue

                moves.append((oth_line, oth_col))
        else:
            for k in range(len(DLINE[player])):
                adj_line = line + DLINE[player][k]
                adj_col = column + DCOL[player][k]

                if not in_interior(adj_line, adj_col) or self.t[adj_line][adj_col] != 0:
                    continue

                moves.append((adj_line, adj_col))

        return moves

    def apply_move(self, last_move, next_move, player, get_old=False):
        diff_line = abs(next_move[0] - last_move[0])
        diff_col = abs(next_move[1] - last_move[1])
        old = None

        if diff_col == 2 or diff_line == 2:
            elim_line = (next_move[0] + last_move[0]) // 2
            elim_col = (next_move[1] + last_move[1]) // 2
            if get_old:
                old = self.t[elim_line][elim_col]
            self.t[elim_line][elim_col] = 0

        if (player == 1 and next_move[0] == TABLE_SIZE - 1) or (player == 2 and next_move[0] == 0):
            player += 2
        self.t[next_move[0]][next_move[1]] = player
        self.t[last_move[0]][last_move[1]] = 0

        if get_old:
            return old

    def revert_move(self, last_move, next_move, player, set_removed=None):
        diff_line = abs(next_move[0] - last_move[0])
        diff_col = abs(next_move[1] - last_move[1])

        if diff_col == 2 or diff_line == 2:
            elim_line = (next_move[0] + last_move[0]) // 2
            elim_col = (next_move[1] + last_move[1]) // 2
            self.t[elim_line][elim_col] = 3 - \
                player if set_removed == None else set_removed

        self.t[next_move[0]][next_move[1]] = 0
        self.t[last_move[0]][last_move[1]] = player

    def apply_set_moves(self, set_of_moves, player):
        old_values = []
        for i in range(len(set_of_moves)-1):
            old_values.append(self.apply_move(
                set_of_moves[i], set_of_moves[i+1], player, get_old=True))
        return old_values

    def revert_set_moves(self, set_of_moves, player, old_values):
        for i in range(len(set_of_moves)-1, 0, -1):
            self.revert_move(
                set_of_moves[i-1], set_of_moves[i], player, set_removed=old_values[i-1])


class Game:
    def __init__(self, size):
        self.size = size
        self.table = Table(size)

    def start(self):
        turn = 1

        self.init_game()
        self.draw_table()
        while not self.game_is_finished():
            self.player_move(turn)
            turn = 3 - turn

    def init_game(self):
        side_dim = UI_SQUARE_DIM * self.size

        self.win = GraphWin(width=side_dim, height=side_dim)
        self.win.setCoords(0, 0, self.size, self.size)

    def draw_table(self):
        for i in range(self.size):
            for j in range(self.size):
                piece = self.table.get_piece(i, j)

                r = Rectangle(Point(j, i), Point(j+1, i+1))
                r.setFill(TABLE_COLORS[(i+j) % 2])
                r.draw(self.win)

                if piece != 0:
                    c = Circle(Point(j+0.5, i+0.5), 0.4)
                    c.setFill(PLAYERS_COLORS[piece - 1])
                    c.draw(self.win)

    def game_is_finished(self):
        pieces_on_table = self.table.count_players_pieces()
        if pieces_on_table[0] != 0 and pieces_on_table[1] != 0:
            return False

        return True

    def player_move(self, turn):
        if turn == 1:
            self.do_player_move(PLAYER_1_PIECE)
        else:
            # self.do_player_move(PLAYER_2_PIECE); return
            moves = self.min_max()
            self.table.apply_set_moves(
                moves, self.table.t[moves[0][0]][moves[0][1]])
            self.draw_table()

    def do_player_move(self, player_piece):
        start_move = self.table.get_player_pieces(player_piece)
        start_pieces = start_move[0]
        selected_square = []
        next_pieces = []

        found_move = False
        while not found_move:
            clicked_square = self.get_normalized_player_clicked()
            if clicked_square in start_pieces:
                selected_square = clicked_square
                next_pieces = self.table.get_possible_moves_from_position(
                    self.table.t[selected_square[0]][selected_square[1]], clicked_square, start_move[1])
            elif selected_square != [] and clicked_square in next_pieces:
                self.table.apply_move(
                    selected_square, clicked_square, self.table.t[selected_square[0]][selected_square[1]])
                selected_square = clicked_square
                found_move = True

                if start_move[1]:
                    next_pieces = self.table.get_possible_moves_from_position(
                        self.table.t[selected_square[0]][selected_square[1]], clicked_square, start_move[1])
                else:
                    next_pieces = []

            self.draw_table()

        while len(next_pieces) > 0:
            clicked_square = self.get_normalized_player_clicked()
            if clicked_square in next_pieces:
                self.table.apply_move(
                    selected_square, clicked_square, self.table.t[selected_square[0]][selected_square[1]])
                selected_square = clicked_square

                next_pieces = self.table.get_possible_moves_from_position(
                    self.table.t[selected_square[0]][selected_square[1]], clicked_square, start_move[1])

            self.draw_table()

    def get_normalized_player_clicked(self):
        clicked_square = self.win.getMouse()
        return (int(self.normalize_board_value(clicked_square.getY())), int(self.normalize_board_value(clicked_square.getX())))

    def normalize_board_value(self, value):
        return value

    def min_max(self):
        min_max_result = self.min_max_recursive(
            MINIMAX_START_LEVEL, -INF, INF, True)
        print(min_max_result)
        return min_max_result[1]

    def min_max_recursive(self, depth, alpha, beta, maximizing):
        if depth == 0:
            return (self.evaluate_current_state(), None)

        pieces_count = self.table.count_players_pieces()
        if pieces_count[0] == 0:
            return (INF_MINIMAX, None)
        elif pieces_count[1] == 0:
            return (-INF_MINIMAX, None)

        player = PLAYER_2_PIECE if maximizing else PLAYER_1_PIECE
        all_moves = self.generate_all_player_moves(player)
        best_move = None

        if maximizing:
            max_eval = -INF
            for move in all_moves:
                starting_player = self.table.t[move[0][0]][move[0][1]]
                old_values = self.table.apply_set_moves(move, starting_player)
                rec_call_result = self.min_max_recursive(
                    depth - 1, alpha, beta, not maximizing)
                self.table.revert_set_moves(move, starting_player, old_values)

                if max_eval < rec_call_result[0]:
                    best_move = move
                    max_eval = rec_call_result[0]
                elif max_eval == rec_call_result[0] and best_move != None:
                    if len(move) > len(best_move):
                        best_move = move

                alpha = max(alpha, rec_call_result[0])
                if beta <= alpha:
                    break

            return (max_eval, None if depth != MINIMAX_START_LEVEL else best_move)
        else:
            min_eval = INF
            for move in all_moves:
                starting_player = self.table.t[move[0][0]][move[0][1]]
                old_values = self.table.apply_set_moves(
                    move, self.table.t[move[0][0]][move[0][1]])
                rec_call_result = self.min_max_recursive(
                    depth - 1, alpha, beta, not maximizing)
                self.table.revert_set_moves(move, starting_player, old_values)

                if min_eval > rec_call_result[0]:
                    best_move = move
                    min_eval = rec_call_result[0]
                elif min_eval == rec_call_result[0] and best_move != None:
                    if len(move) > len(best_move):
                        best_move = move

                beta = min(beta, rec_call_result[0])
                if beta <= alpha:
                    break

            return (min_eval, None if depth != MINIMAX_START_LEVEL else best_move)

    def generate_all_player_moves(self, player):
        start_move = self.table.get_player_pieces(player)
        start_pieces = start_move[0]
        all_moves = []

        for piece in start_pieces:
            current_move = [piece]
            next_pieces = self.table.get_possible_moves_from_position(
                self.table.t[piece[0]][piece[1]], piece, start_move[1])
            self.generate_all_player_moves_rec(
                self.table.t[piece[0]][piece[1]], all_moves, current_move, next_pieces, piece, start_move[1])

        return all_moves

    def generate_all_player_moves_rec(self, player, all_moves, current_move, next_pieces, last_position, capture):
        if len(next_pieces) == 0 or (not capture and len(current_move) >= 2):
            if len(current_move) > 1:
                all_moves.append(tuple(current_move))
            return

        for piece in next_pieces:
            current_move.append(piece)

            next_call_pieces = self.table.get_possible_moves_from_position(
                player, piece, capture)
            old = self.table.apply_move(
                last_position, piece, player, get_old=True)
            self.generate_all_player_moves_rec(
                player, all_moves, current_move, next_call_pieces, piece, capture)
            self.table.revert_move(last_position, piece,
                                   player, set_removed=old)

            current_move.pop()

    def evaluate_current_state(self):
        pieces_count = self.table.count_players_pieces()

        if pieces_count[0] == 0:
            return INF_MINIMAX
        elif pieces_count[1] == 0:
            return -INF_MINIMAX

        return 10 * (pieces_count[1] - pieces_count[0])

    def update_table(self, move, turn):
        self.table.set_piece(turn, move[0], move[1])


if __name__ == "__main__":
    game = Game(TABLE_SIZE)
    game.start()
