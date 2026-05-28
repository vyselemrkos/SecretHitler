Notes to self:

# Build test binary
g++ -DTEST secret_hitler.cpp secret_hitler_board.cpp test_main.cpp -o test_main

# Build shared library (macOS/Linux)
g++ -DTEST -shared -fPIC -fvisibility=hidden -o secret_hitler.so secret_hitler.cpp secret_hitler_board.cpp secret_hitler_wrapper.cpp

# Build shared library (Windows)
g++ -DTEST -shared -o secret_hitler.dll secret_hitler.cpp secret_hitler_board.cpp secret_hitler_wrapper.cpp