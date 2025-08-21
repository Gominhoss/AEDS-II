@echo off
REM Script para compilar o projeto de gerenciamento de supermercado.

REM Define o compilador GCC
set COMPILER=gcc

REM Define a pasta de origem dos arquivos .c
set SRC_DIR=src

REM Define a pasta de include para os arquivos .h
set INCLUDE_DIR=include

REM Define a pasta de saida para o executavel
set BIN_DIR=bin

REM Define o nome do executavel de saida
set EXECUTABLE_NAME=main.exe

REM Lista de todos os arquivos .c a serem compilados
set SOURCES=%SRC_DIR%\cliente.c %SRC_DIR%\estoque.c %SRC_DIR%\fornecedor.c %SRC_DIR%\funcionario.c %SRC_DIR%\log.c %SRC_DIR%\main.c %SRC_DIR%\produto.c %SRC_DIR%\utils.c

REM Comando de compilacao
echo Compilando o projeto...
%COMPILER% -o %BIN_DIR%\%EXECUTABLE_NAME% %SOURCES% -I%INCLUDE_DIR%

REM Verifica se a compilacao foi bem-sucedida
if %errorlevel% equ 0 (
    echo.
    echo Compilacao concluida com sucesso!
    echo Executavel criado em: %BIN_DIR%\%EXECUTABLE_NAME%
    echo Para executar, navegue ate a pasta 'bin' e rode o comando: .\main.exe
) else (
    echo.
    echo Erro durante a compilacao.
)

pause
