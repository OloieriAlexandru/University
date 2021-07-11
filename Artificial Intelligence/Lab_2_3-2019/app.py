
import random
import time
import copy

MOD = 1000000007


def pow_mod(val, pow, mod):
    if pow == 0:
        return 1
    if pow % 2 == 1:
        return (val * pow_mod((val * val) % mod, pow // 2, mod)) % mod
    return pow_mod((val * val) % mod, pow // 2, mod)


pow_31 = [pow_mod(31, i, MOD) for i in range(1, 7)]


class Transition:
    def __init__(self, starting_side, missionaries_num, cannibals_num):
        if not (starting_side == 1 or starting_side == 2):
            raise Exception("Invalid transition parameters")
        self.s_s = starting_side
        self.m_n = missionaries_num
        self.c_n = cannibals_num

    def is_valid_helper(self, side_m_c, side_c_c, b_p, b_c, oth_side_m_c, oth_side_c_c):
        # the transition is applied to a wrong state (the sides don't match)
        if self.s_s != b_p:
            return False
        if self.m_n > side_m_c:  # the state requires to move more missionaries than there are on the current side
            return False
        if self.c_n > side_c_c:  # the state requires to move more cannibals than there are on the current side
            return False
        if (self.m_n + self.c_n) > b_c:  # exceeds the boat capacity
            return False
        # there can't be more missionaries than cannibals on the current side (after applying the transition)
        if (side_m_c - self.m_n) < (side_c_c - self.c_n) and (side_m_c - self.m_n) > 0:
            return False
        # there can't be more missionaries than cannibals on the other side (after applying the transition)
        if (oth_side_m_c + self.m_n) < (oth_side_c_c + self.c_n) and (oth_side_m_c + self.m_n) > 0:
            return False
        # the boat has to carry at least one person
        if self.m_n + self.c_n <= 0:
            return False
        return True

    def is_valid(self, state):
        if self.s_s == 1:
            return self.is_valid_helper(state.m_c_1, state.c_c_1, state.b_p, state.b_c, state.m_c_2, state.c_c_2)
        else:
            return self.is_valid_helper(state.m_c_2, state.c_c_2, state.b_p, state.b_c, state.m_c_1, state.c_c_1)


class State:
    def __init__(self, missionaries_count, cannibals_count, boat_capacity):
        # the number of missionaries on the first side of the river
        self.m_c_1 = missionaries_count
        self.c_c_1 = cannibals_count  # the number of cannibals on the first side of the river
        self.m_c_2 = 0  # the number of missionaries on the second side of the river
        self.c_c_2 = 0  # the number of cannibals on the second side of the river
        self.b_c = boat_capacity  # boat capacity
        self.b_p = 1  # boat position

    def is_final(self):
        return self.m_c_1 == 0 and self.c_c_1 == 0

    def is_valid(self, transition):
        return transition.is_valid(self)

    def get_valid_transitions_helper(self, miss_count, cann_count, boat_pos):
        transitions = []
        for i in range(miss_count + 1):
            for j in range(cann_count + 1):
                # try all combinations of (missionaries_count, cannibals_count)
                tr = Transition(boat_pos, i, j)
                if tr.is_valid(self) == True:
                    transitions.append(tr)
        return transitions

    def get_valid_transitions(self):
        if self.b_p == 1:
            return self.get_valid_transitions_helper(self.m_c_1, self.c_c_1, self.b_p)
        else:
            return self.get_valid_transitions_helper(self.m_c_2, self.c_c_2, self.b_p)

    def apply_transition(self, transition):
        mul_1 = 1
        mul_2 = 1
        if self.b_p == 1:
            mul_1 = -1
        else:
            mul_2 = -1
        self.m_c_1 += mul_1 * transition.m_n
        self.c_c_1 += mul_1 * transition.c_n
        self.m_c_2 += mul_2 * transition.m_n
        self.c_c_2 += mul_2 * transition.c_n
        self.b_p = 3 - self.b_p

    def heuristic(self):
        return self.m_c_1 + self.c_c_1

    def __str__(self):
        return 'Boat pos: {self.b_p}, miss c. 1: {self.m_c_1}, cann c. 1: {self.c_c_1}, miss c. 2: {self.m_c_2}, cann c. 2: {self.c_c_2}'.format(self=self)

    def __hash__(self):
        return (pow_31[0] * self.m_c_1 + pow_31[1] * self.c_c_1 + pow_31[2] * self.m_c_2
                + pow_31[3] * self.c_c_2 + pow_31[4] * self.b_p + pow_31[5] * self.b_c) % MOD


class Instance:
    def __init__(self, max_m, max_c, max_b, min_m=3, min_c=3, min_b=2):
        self.cann_count = random.randint(min_c, max_c)
        self.miss_count = random.randint(self.cann_count, max_m)
        self.boat_cap = random.randint(min_b, max_b)


def get_inital_state(missionaries_count, cannibals_count, boat_capacity):
    return State(missionaries_count, cannibals_count, boat_capacity)


def state_is_final(state):
    return state.is_final()


def transition(state, trans):
    if not state.is_valid(trans):
        return False
    new_state = copy.deepcopy(state)
    new_state.apply_transition(trans)
    return new_state


def strategy_random(instance):
    begin = State(instance.miss_count, instance.cann_count, instance.boat_cap)
    curr_state = copy.deepcopy(begin)
    tries = 0
    while not state_is_final(curr_state):
        tries += 1
        if tries > 50:  # after 50 random runs stop
            return []
        iterations_count = 0
        curr_state = copy.deepcopy(begin)  # the first state
        past_states = set()  # hashset
        past_states.add(hash(curr_state))  # mark the initial state as visited
        # state history (the returned value of this function)
        state_history = [curr_state]
        while iterations_count < 1000 and not state_is_final(curr_state):
            iterations_count += 1

            # get all the valid transitions from the current state
            valid_transitions = curr_state.get_valid_transitions()

            perm = [i for i in range(len(valid_transitions))]
            # generate a random permutation of the transitions
            random.shuffle(perm)
            for idx in perm:  # choose the first one that will get us to a state that wasn't visited before by doing the following checks:
                # apply the transition to the current state
                next_state = transition(curr_state, valid_transitions[idx])
                hsh = hash(next_state)  # compute the new state's hash
                if hsh in past_states:  # if it is in the hash set
                    continue  # check the next one
                past_states.add(hsh)  # add the hash to the hash set
                curr_state = next_state  # we have a new current state
                # append the new state to the answer
                state_history.append(curr_state)
                break  # we found the desired transition
    return state_history


def strategy_bkt_bkt(state, past_states, state_history):
    past_states.add(hash(state))
    state_history.append(state)

    if state_is_final(state):
        return True

    valid_transitions = state.get_valid_transitions()
    for trans in valid_transitions:
        next_state = transition(state, trans)
        hsh = hash(next_state)
        if hsh in past_states:
            continue
        rec_result = strategy_bkt_bkt(
            next_state, past_states, state_history)
        if rec_result == True:
            return True

    state_history.pop()
    return False


def strategy_bkt(instance):
    begin = State(instance.miss_count, instance.cann_count, instance.boat_cap)
    sol = []
    dfs_result = strategy_bkt_bkt(begin, set(), sol)
    if dfs_result:
        return sol
    return []

# we're at state {{state}}, we have visited all the states in the {{past_states}} hash set
# on the stack we have all the states from the {{state_history}} array
# we can do {{more}}  moves


def strategy_iddfs_dfs(state, past_states, state_history, more):
    past_states.add(hash(state))
    state_history.append(state)

    if more <= 0:
        return False

    if state_is_final(state):
        return True

    valid_transitions = state.get_valid_transitions()
    for trans in valid_transitions:
        next_state = transition(state, trans)
        hsh = hash(next_state)
        if hsh in past_states:
            continue
        rec_result = strategy_iddfs_dfs(
            next_state, past_states, state_history, more - 1)
        if rec_result == True:
            return True

    state_history.pop()
    return False


def strategy_iddfs(instance):
    begin = State(instance.miss_count, instance.cann_count, instance.boat_cap)
    for i in range(1, 1001):
        sol = []
        dfs_result = strategy_iddfs_dfs(begin, set(), sol, i)
        if dfs_result:
            return sol
    return []


def strategy_astar_get_path(state):
    path = []
    while state[2] != None:
        path.append(state[0])
        state = state[2]
    path.append(state[0])

    return list(reversed(path))

def strategy_astar(instance):
    begin = State(instance.miss_count, instance.cann_count, instance.boat_cap)
    initial_state = [begin, 0, None] # state, distance, parent

    states = [initial_state] # simulated priority queue
    all_states = [initial_state] # vector with all the states
    state_position = { hash(initial_state[0]): 0 } # map that helps finding the position of a state in the {{all_states}} array

    while len(states) > 0:
        current_state = states[0]
        states.pop(0)
        if state_is_final(current_state[0]):
            return strategy_astar_get_path(current_state)
        
        valid_transitions = current_state[0].get_valid_transitions()
        for trans in valid_transitions: # apply all the transitions on the current state
            next_state = transition(current_state[0], trans) # get the next state
            hsh = hash(next_state) # and its hash value
            
            if hsh in state_position: # if it's not a new state
                pos = state_position[hsh] # find its position in {{all_states}} array
                if all_states[pos][1] > current_state[1] + 1: # check if we found a better path
                    all_states[pos][1] = current_state[1] + 1 # update the distance
                    all_states[pos][2] = current_state # update the parrent
                    states.append(all_states[pos]) # add to the queue

            else:
                new_state = [next_state, current_state[1] + 1, current_state] # we got to a new state
                
                state_position[hsh] = len(all_states) - 1 # set its index in {{all_states}} array
                all_states.append(new_state) # append it to the {{all_states}} array

                states.append(new_state) # add to the queue
        states = sorted(states, key=lambda el: el[0].heuristic() + el[1]) # sort the queue
    
    return []


def chrono_decorator(func):
    def wrapper_strategy(*args, **kwargs):
        tic = time.perf_counter()
        solution = func(*args, **kwargs)
        duration = time.perf_counter() - tic
        return (solution, round(duration, 4))

    return wrapper_strategy


def print_strategy_statistics(results, name):
    print('-----------------')
    print("Strategy: {0}".format(name))

    solutions_found = 0
    total_solutions_len = 0
    total_duration = 0
    for res in results:
        if len(res[0]) == 0:
            continue

        solutions_found += 1
        total_solutions_len += len(res[0])
        total_duration += res[1]

    print('Total solutions found: {0}'.format(solutions_found))
    solutions_found = 1 if solutions_found == 0 else solutions_found
    print('Average solution length: {0}'.format(
        round((1.0 * total_solutions_len) / solutions_found, 4)))
    print('Average duration: {0}'.format(
        round((1.0 * total_duration) / solutions_found, 4)))


def run_on_random_instances(instances_num, strategies, strategies_names):
    if len(strategies) != len(strategies_names):
        raise Exception(
            "len(strategies) should be equal to len(strategies_names)")

    strategies_results = []
    for i in range(len(strategies)):
        strategies_results.append([])
    for _ in range(instances_num):
        instance = Instance(15, 15, 5)

        for i in range(len(strategies)):
            solution, duration = strategies[i](instance)
            strategies_results[i].append((solution, duration))

    print('Number of runs: {0}'.format(instances_num))
    for i in range(len(strategies)):
        print_strategy_statistics(strategies_results[i], strategies_names[i])


if __name__ == "__main__":
    strategies = [chrono_decorator(strategy_random), chrono_decorator(strategy_bkt),
                  chrono_decorator(strategy_iddfs), chrono_decorator(strategy_astar)]
    strategies_names = ["Random strategy", "BKT", "IDDFS", "A*"]
    run_on_random_instances(10, strategies, strategies_names)
