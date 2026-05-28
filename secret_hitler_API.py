import ctypes

game_lib = ctypes.CDLL("./secret_hitler.so")

_game_ptr = ctypes.c_void_p

# ── Lifecycle ──────────────────────────────────────────────────────────────────
game_lib.create_game.argtypes = []
game_lib.create_game.restype = _game_ptr

game_lib.destroy_game.argtypes = [_game_ptr]
game_lib.destroy_game.restype = None

# ── Player management ──────────────────────────────────────────────────────────
game_lib.add_player.argtypes = [_game_ptr, ctypes.c_char_p]
game_lib.add_player.restype = ctypes.c_int

game_lib.remove_player.argtypes = [_game_ptr, ctypes.c_char_p]
game_lib.remove_player.restype = ctypes.c_int

# ── Game flow ──────────────────────────────────────────────────────────────────
game_lib.start_game.argtypes = [_game_ptr]
game_lib.start_game.restype = ctypes.c_int

game_lib.select_chancellor.argtypes = [_game_ptr, ctypes.c_char_p, ctypes.c_char_p]
game_lib.select_chancellor.restype = ctypes.c_int

game_lib.cast_vote.argtypes = [_game_ptr, ctypes.c_char_p, ctypes.c_int]
game_lib.cast_vote.restype = ctypes.c_int

game_lib.president_discard.argtypes = [_game_ptr, ctypes.c_char_p, ctypes.c_int]
game_lib.president_discard.restype = ctypes.c_int

game_lib.chancellor_discard.argtypes = [_game_ptr, ctypes.c_char_p, ctypes.c_int]
game_lib.chancellor_discard.restype = ctypes.c_int

game_lib.game_state_next.argtypes = [_game_ptr]
game_lib.game_state_next.restype = None

# ── State queries ──────────────────────────────────────────────────────────────
game_lib.get_player_count.argtypes = [_game_ptr]
game_lib.get_player_count.restype = ctypes.c_int

game_lib.get_player_name.argtypes = [_game_ptr, ctypes.c_int]
game_lib.get_player_name.restype = ctypes.c_char_p

game_lib.get_player_role.argtypes = [_game_ptr, ctypes.c_int]
game_lib.get_player_role.restype = ctypes.c_int

game_lib.get_player_status.argtypes = [_game_ptr, ctypes.c_int]
game_lib.get_player_status.restype = ctypes.c_int

game_lib.get_player_vote.argtypes = [_game_ptr, ctypes.c_int]
game_lib.get_player_vote.restype = ctypes.c_int

game_lib.get_game_state.argtypes = [_game_ptr]
game_lib.get_game_state.restype = ctypes.c_int

game_lib.get_current_president.argtypes = [_game_ptr]
game_lib.get_current_president.restype = ctypes.c_int

game_lib.get_current_chancellor.argtypes = [_game_ptr]
game_lib.get_current_chancellor.restype = ctypes.c_int

game_lib.get_chancellor_candidate.argtypes = [_game_ptr]
game_lib.get_chancellor_candidate.restype = ctypes.c_int

game_lib.get_bypass_count.argtypes = [_game_ptr]
game_lib.get_bypass_count.restype = ctypes.c_int

game_lib.get_drawn_policy_count.argtypes = [_game_ptr]
game_lib.get_drawn_policy_count.restype = ctypes.c_int

game_lib.get_drawn_policy.argtypes = [_game_ptr, ctypes.c_int]
game_lib.get_drawn_policy.restype = ctypes.c_int

game_lib.get_fascist_policy_count.argtypes = [_game_ptr]
game_lib.get_fascist_policy_count.restype = ctypes.c_int

game_lib.get_liberal_policy_count.argtypes = [_game_ptr]
game_lib.get_liberal_policy_count.restype = ctypes.c_int

game_lib.is_hitler_zone.argtypes = [_game_ptr]
game_lib.is_hitler_zone.restype = ctypes.c_int

# ── Constants (mirror C++ enums) ───────────────────────────────────────────────
ROLE_UNDEFINED = -1
ROLE_LIBERAL   =  0
ROLE_FASCIST   =  1
ROLE_HITLER    =  2

ERR_NOT_INITIALIZED    = -1
ERR_NO_ERROR           =  0
ERR_INVALID_NAME       =  1
ERR_INVALID_GAME_STATE =  2
ERR_INVALID_PLAYER_SIZE=  3

STATE_UNDEFINED               = -1
STATE_INITIALIZING            =  0
STATE_SELECT_PRESIDENT        =  1
STATE_SELECT_CHANCELLOR       =  2
STATE_VOTING                  =  3
STATE_PRESIDENT_DISCARDING    =  4
STATE_CHANCELLOR_DISCARDING   =  5
STATE_PRESIDENT_KILLING       =  6
STATE_PRESIDENT_INVESTIGATING =  7
STATE_PRESIDENT_EXAMINING     =  8
STATE_LIBERAL_WIN             =  9
STATE_FASCIST_WIN             = 10

POLICY_UNDEFINED = -1
POLICY_LIBERAL   =  0
POLICY_FASCIST   =  1

_STATE_NAMES = {
    STATE_INITIALIZING:            "initializing",
    STATE_SELECT_PRESIDENT:        "select_president",
    STATE_SELECT_CHANCELLOR:       "select_chancellor",
    STATE_VOTING:                  "voting",
    STATE_PRESIDENT_DISCARDING:    "president_discarding",
    STATE_CHANCELLOR_DISCARDING:   "chancellor_discarding",
    STATE_PRESIDENT_KILLING:       "president_killing",
    STATE_PRESIDENT_INVESTIGATING: "president_investigating",
    STATE_PRESIDENT_EXAMINING:     "president_examining",
    STATE_LIBERAL_WIN:             "liberal_win",
    STATE_FASCIST_WIN:             "fascist_win",
}

# ── Python helpers ─────────────────────────────────────────────────────────────

def get_players(game):
    """Return list of (name, role) for all players."""
    count = game_lib.get_player_count(game)
    return [
        (game_lib.get_player_name(game, i).decode(), game_lib.get_player_role(game, i))
        for i in range(count)
    ]

def get_state_name(game):
    return _STATE_NAMES.get(game_lib.get_game_state(game), "unknown")

def get_president_name(game):
    idx = game_lib.get_current_president(game)
    return game_lib.get_player_name(game, idx).decode() if idx >= 0 else None

def get_chancellor_name(game):
    idx = game_lib.get_current_chancellor(game)
    return game_lib.get_player_name(game, idx).decode() if idx >= 0 else None

def get_candidate_name(game):
    idx = game_lib.get_chancellor_candidate(game)
    return game_lib.get_player_name(game, idx).decode() if idx >= 0 else None

def get_drawn_policies(game):
    count = game_lib.get_drawn_policy_count(game)
    labels = {POLICY_LIBERAL: "Liberal", POLICY_FASCIST: "Fascist"}
    return [labels.get(game_lib.get_drawn_policy(game, i), "?") for i in range(count)]


if __name__ == "__main__":
    game = game_lib.create_game()

    for name in ["semih", "arif", "bugra", "demokan", "ferdi"]:
        game_lib.add_player(game, name.encode())

    game_lib.start_game(game)
    print(f"State: {get_state_name(game)}")
    print(f"President: {get_president_name(game)}")

    for name, role in get_players(game):
        label = {ROLE_LIBERAL: "Liberal", ROLE_FASCIST: "Fascist", ROLE_HITLER: "Hitler"}.get(role, "?")
        print(f"  {name}: {label}")

    game_lib.destroy_game(game)
