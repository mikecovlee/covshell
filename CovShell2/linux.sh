echo 正在编译目标文件...
g++ shell.cpp -c -std=c++11 -Wfatal-errors -o tcos_shell.o
echo 正在编译二进制文件...
g++ covariate_shell.o -std=c++11 -Wfatal-errors -o covariate_shell.bin
echo 成功!