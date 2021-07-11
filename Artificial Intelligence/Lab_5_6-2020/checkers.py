# Autori:
# Oloieri Alexandru IIIA2
# Vranceanu Andi-Costin IIIA2

# pip install graphics.py

from graphics import GraphWin, Circle, Rectangle, color_rgb, Point, color_rgb
import math
import random
import queue

TABLE_SIZE = 4
PLAYER_PIECES = [1, 2]
PLAYER_TARGET_LINES = [TABLE_SIZE - 1, 0]
DLINE = [-1, -1, 0, 1, 1, 1, 0, -1]
DCOL = [0, 1, 1, 1, 0, -1, -1, -1]
DDMIN = 8
UI_SQUARE_DIM = 160

PLAYERS_COLORS = [
    color_rgb(127, 52, 0),
    color_rgb(120, 200, 47),
    color_rgb(0, 255, 255)
]

INF = 10000000000000000000000
INF_MINIMAX = INF - 1
MINIMAX_START_LEVEL = 4


class Table:
    def __init__(self, size):
        self.size = size
        self.t = [self.get_table_line(size, i) for i in range(size)]

    def get_table_line(self, size, line_index):
        line = [0 for _ in range(size)]

        if line_index == 0 or line_index + 1 == size:
            current_player = PLAYER_PIECES[0] if line_index < size // 2 else PLAYER_PIECES[1]
            for i in range(size):
                line[i] = current_player

        return line

    def get_piece(self, line, column):
        return self.t[line][column]

    def interior(self, i, j):
        return i >= 0 and j >= 0 and i < self.size and j < self.size

    def get_next_moves(self, line, column, player):
        if self.t[line][column] != player:
            return []

        next_moves = []
        for k in range(DDMIN):
            nline = line + DLINE[k]
            ncol = column + DCOL[k]
            if not self.interior(nline, ncol):
                continue
            if self.t[nline][ncol] == 0:
                next_moves.append((nline, ncol))
        return next_moves

    def apply_move(self, from_line, from_col, to_line, to_col):
        self.t[to_line][to_col] = self.t[from_line][from_col]
        self.t[from_line][from_col] = 0

    def game_ended(self):
        player_won = True
        ai_won = True
        for i in range(self.size):
            if self.t[0][i] != 2:
                ai_won = False
                break
        for i in range(self.size):
            if self.t[self.size - 1][i] != 1:
                player_won = False
                break
        if player_won:
            return 1
        elif ai_won:
            return 2
        else:
            return None

    def lee(self, line, col, target_line):
        vis = set()
        vis.add((line, col))
        distances = [[7 for j in range(self.size)] for i in range(self.size)]

        q = queue.Queue()
        q.put((line, col))
        distances[line][col] = 0
        while not q.empty():
            act = q.get()
            i = act[0]
            j = act[1]

            for k in range(DDMIN):    
                nline = i + DLINE[k]
                ncol = j + DCOL[k]

                if not self.interior(nline, ncol):
                    continue
                if ((nline, ncol) in vis):
                    continue
                vis.add((nline, ncol))
                if self.t[nline][ncol] == 0 or nline == target_line:
                    distances[nline][ncol] = distances[i][j] + 1
                    q.put((nline, ncol))
        
        return distances

    def evaluation(self, to_print = False):
        human_dists = []
        ai_dists = []

        for i in range(self.size):
            for j in range(self.size):
                if self.t[i][j] == 0:
                    continue
                    
                target_line = self.size - 1 if self.t[i][j] == PLAYER_PIECES[0] else 0

                dist = self.lee(i, j, target_line)
                if self.t[i][j] == PLAYER_PIECES[0]: # human
                    human_dists.append(dist[target_line])
                else: # ai
                    ai_dists.append(dist[target_line])

        # print(human_dists)
        # print(ai_dists)

        human_v = None
        ai_v = None

        for i_1 in range(self.size):
            for i_2 in range(self.size):
                if i_2 == i_1:
                    continue
                for i_3 in range(self.size):
                    if i_3 == i_1 or i_3 == i_2:
                        continue
                    for i_4 in range(self.size):
                        if i_4 == i_1 or i_4 == i_2 or i_4 == i_3:
                            continue
                        v_h = human_dists[0][i_1] + human_dists[1][i_2] + human_dists[2][i_3] + human_dists[3][i_4]
                        v_ai = ai_dists[0][i_1] + ai_dists[1][i_2] + ai_dists[2][i_3] + ai_dists[3][i_4]

                        if human_v is None:
                            ai_v = v_ai
                            human_v = v_h    
                        else:
                            ai_v = min(ai_v, v_ai)
                            human_v = min(human_v, v_h)

        return human_v - ai_v

    def old_evaluation(self):
        player_sum = 0
        ai_sum = 0
        for i in range(self.size):
            for j in range(self.size):
                if self.t[i][j] == PLAYER_PIECES[0]:
                    player_sum += i
                if self.t[i][j] == PLAYER_PIECES[1]:
                    ai_sum += (self.size - 1 - i)
        return ai_sum - player_sum

    def __hash__(self):
        hsh = 0
        pw = 1
        MOD = 1000000007

        for i in range(self.size):
            for j in range(self.size):
                hsh = (self.t[i][j] * pw) % MOD
                pw = (pw * 3) % MOD
        
        return hsh

class Game:
    def __init__(self, size):
        self.size = size
        self.table = Table(size)

    def init_game(self):
        side_dim = UI_SQUARE_DIM * self.size

        self.win = GraphWin(width=side_dim, height=side_dim)
        self.win.setCoords(self.size, 0, 0, self.size)

    def draw_table(self):
        for i in range(self.size):
            for j in range(self.size):
                piece = self.table.get_piece(i, j)

                r = Rectangle(Point(j, i), Point(j+1, i+1))
                r.setFill(color_rgb(200, 10, 10))
                r.draw(self.win)

                if piece != 0:
                    c = Circle(Point(j+0.5, i+0.5), 0.4)
                    c.setFill(PLAYERS_COLORS[piece - 1])
                    c.draw(self.win)

    def start(self):
        self.init_game()
        self.draw_table()

        turn = 1
        running = True
        while running:
            if turn == 1:
                self.player_move()
            else:
                self.ai_move_min_max()
            ended = self.table.game_ended()
            if ended != None:
                print('Player {} won!'.format(2 if turn == 0 else 1))
                running = False
                continue
            turn = 1 - turn

    def player_move(self):
        clicked = ()
        expected_moves = []
        while True:
            click = self.get_mouse_click()
            click_int = (int(click[0]), int(click[1]))

            if clicked == ():
                if self.table.get_piece(click_int[0], click_int[1]) == PLAYER_PIECES[0]:
                    clicked = click_int
                    expected_moves = self.table.get_next_moves(
                        clicked[0], clicked[1], PLAYER_PIECES[0])
                    self.highlight_piece(clicked[0], clicked[1], -1)
            else:
                self.highlight_piece(clicked[0], clicked[1])
                if click_int in expected_moves:
                    break
                else:
                    if self.table.get_piece(click_int[0], click_int[1]) == PLAYER_PIECES[0]:
                        clicked = click_int
                        expected_moves = self.table.get_next_moves(
                            clicked[0], clicked[1], PLAYER_PIECES[0])
                        self.highlight_piece(clicked[0], clicked[1], -1)
                    else:
                        clicked = ()
        # clicked -> click_int
        self.table.apply_move(
            clicked[0], clicked[1], click_int[0], click_int[1])
        self.highlight_piece(clicked[0], clicked[1])
        self.highlight_piece(click_int[0], click_int[1])

    def ai_move(self):
        best = -math.inf
        chosen_from = None
        chosen_to = None
        for i in range(self.size):
            for j in range(self.size):
                piece = self.table.get_piece(i, j)

                if piece != PLAYER_PIECES[1]:
                    continue

                next_moves = self.table.get_next_moves(i, j, PLAYER_PIECES[1])
                for move in next_moves:
                    # (i,j) -> (move[0], move[1])
                    self.table.apply_move(i, j, move[0], move[1])
                    cost = self.table.evaluation()
                    self.table.apply_move(move[0], move[1], i, j)
                    if cost > best:
                        best = cost
                        chosen_from = (i, j)
                        chosen_to = move
                    # elif cost == best:
                    #     chosen_to.append(move)
        # chosen_to = random.choice(chosen_to)
        if chosen_from is not None and chosen_to is not None:
            self.table.apply_move(
                chosen_from[0], chosen_from[1], chosen_to[0], chosen_to[1])
            self.highlight_piece(chosen_from[0], chosen_from[1])
            self.highlight_piece(chosen_to[0], chosen_to[1])

    def ai_move_min_max(self):
        move = self.min_max()

        from_x = move[0][0]
        from_y = move[0][1]
        to_x = move[1][0]
        to_y = move[1][1]
        
        self.table.apply_move(
            from_x, from_y, to_x, to_y)
        self.highlight_piece(from_x, from_y)
        self.highlight_piece(to_x, to_y)

        self.table.evaluation(True)

    def get_mouse_click(self):
        mouse_coordinate = self.win.checkMouse()
        while mouse_coordinate == None:
            mouse_coordinate = self.win.checkMouse()
        return (mouse_coordinate.getY(), mouse_coordinate.getX())

    def highlight_piece(self, line, column, color=None):
        piece = self.table.get_piece(line, column)

        r = Rectangle(Point(column, line), Point(column+1, line+1))
        r.setFill(color_rgb(200, 10, 10))
        r.draw(self.win)

        if piece == 0:
            return
        c = Circle(Point(column+0.5, line+0.5), 0.4)
        if color == None:
            c.setFill(PLAYERS_COLORS[piece - 1])
        else:
            c.setFill(PLAYERS_COLORS[-1])
        c.draw(self.win)

    def min_max(self):
        # Normal min-max
        # min_max_result = self.min_max_recursive(MINIMAX_START_LEVEL, True)
        # Alpha-beta prunning
        min_max_result = self.min_max_alpha_beta_prunning(MINIMAX_START_LEVEL, -INF, INF, True)
        print(min_max_result)
        return min_max_result[1]


    def min_max_recursive(self, depth, maximizing):
        if depth == 0:
            return (self.table.evaluation(True), None)

        ended = self.table.game_ended()
        if ended == 1:
            return (-10000 if maximizing else 10000, None)
        elif ended == 2:
            return (10000 if maximizing else -10000, None)

        player = PLAYER_PIECES[1] if maximizing else PLAYER_PIECES[0]
        best_move = None

        best_yet = -INF
        if not maximizing:
            best_yet = INF

        for i in range(self.size):
            for j in range(self.size):
                piece = self.table.get_piece(i, j)

                if piece != player:
                    continue

                next_moves = self.table.get_next_moves(i, j, player)
                for move in next_moves:
                    # (i,j) -> (move[0], move[1])
                    self.table.apply_move(i, j, move[0], move[1])

                    rec_call_result = self.min_max_recursive(
                        depth - 1, not maximizing)

                    self.table.apply_move(move[0], move[1], i, j)

                    if maximizing:
                        if best_yet < rec_call_result[0]:
                            best_yet = rec_call_result[0]
                            best_move = ((i,j),move)
                    else:
                        if best_yet > rec_call_result[0]:
                            best_yet = rec_call_result[0]
                            best_move = ((i,j),move)
        return (best_yet, best_move)

    def min_max_alpha_beta_prunning(self, depth, alpha, beta, maximizing):
        if depth == 0:
            return (self.table.evaluation(True), None)

        ended = self.table.game_ended()
        if ended == 1:
            return (-10000 if maximizing else 10000, None)
        elif ended == 2:
            return (10000 if maximizing else -10000, None)

        player = PLAYER_PIECES[1] if maximizing else PLAYER_PIECES[0]
        best_move = None

        best_yet = -INF
        if not maximizing:
            best_yet = INF

        for i in range(self.size):
            for j in range(self.size):
                piece = self.table.get_piece(i, j)

                if piece != player:
                    continue

                next_moves = self.table.get_next_moves(i, j, player)
                for move in next_moves:
                    # (i,j) -> (move[0], move[1])
                    self.table.apply_move(i, j, move[0], move[1])

                    rec_call_result = self.min_max_alpha_beta_prunning(
                        depth - 1, alpha, beta, not maximizing)

                    self.table.apply_move(move[0], move[1], i, j)

                    if maximizing:
                        if best_yet < rec_call_result[0]:
                            best_yet = rec_call_result[0]
                            best_move = ((i,j),move)

                        alpha = max(alpha, rec_call_result[0])
                        if beta <= alpha:
                            break
                    else:
                        if best_yet > rec_call_result[0]:
                            best_yet = rec_call_result[0]
                            best_move = ((i,j),move)

                        beta = min(beta, rec_call_result[0])
                        if beta <= alpha:
                            break
        return (best_yet, best_move)


if __name__ == "__main__":
    game = Game(TABLE_SIZE)
    game.start()
