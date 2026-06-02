# chess-qt
Chess application with Qt.
# Prerequisites
OS: macOS Tahoe 26.5
Qt: 6.11.1
# Build & Run
Create Build Directory:
```bash\
mkdir -p ./build/
```
First Time Build:
```bash\
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH="$HOME/Qt/6.11.1/macos"
```
Build:
```bash\
cmake --build build
```
Run App:
```bash\
./build/app/chess_app
```
Run Unit Test:
```bash\
./build/tests/unit_tests
