# 108501026 assignment 2

## 程式架構

* Basic
使用只有一層的hidden layer 的 fully connected Neural Network(簡稱FCNN)

* activation function
使用 sigmoid
* loss function 使用 MSE

## Compile & Run

```sh
# Compile
gcc -o main main.c -lev
# Run
./main -n -s NewGame.txt # New game
./main -1 NewGame.txt # Load file
```
