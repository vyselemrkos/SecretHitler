import ctypes

# Load the shared library
game_lib = ctypes.CDLL("./secret_hitler.so", winmode=0)

gamehandle = ctypes.POINTER(ctypes.c_void_p)
string = ctypes.POINTER(ctypes.c_char)

game_lib.create_game.argtypes = None
game_lib.create_game.restype = gamehandle

game_lib.add_players.argtypes = [gamehandle, string]
game_lib.add_players.restype = None

game_lib.print.argtypes = [gamehandle]
game_lib.print.restype = None

astring= "semih"
game = game_lib.create_game()
game_lib.add_players(game, astring.encode('utf-8'))
game_lib.print(game)
