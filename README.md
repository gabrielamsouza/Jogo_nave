# Jogo da Nave Espacial

Este projeto contém uma versão de jogo de nave em C++ e uma versão em HTML/JavaScript para rodar no navegador.

## Visão geral

- `main.cpp`: jogo em terminal, em estilo arcade, com nave, asteroides e laser.
- `index.html`: versão visual em navegador, com canvas e controles por teclado.

## Como jogar

### Versão C++

1. Abra um terminal no diretório do projeto.
2. Compile com o compilador g++:

```bash
g++ main.cpp -o main.exe
```

3. Execute:

```bash
./main.exe
```

### Versão web

1. Abra o arquivo `index.html` em um navegador.
2. Use as teclas:
   - `A` para mover para esquerda
   - `D` para mover para direita
   - `R` para reiniciar após o game over

## Objetivo

- Desviar dos asteroides
- Atirar para destruir os inimigos
- Acumular pontos
- Desbloquear o modo laser ao atingir 40 pontos

## Controles da versão terminal

- Setas esquerda/direita para mover a nave
- `Q` para sair

## Requisitos

- Compilador C++ (como `g++` ou MinGW no Windows)
- Navegador moderno para a versão web

## Estrutura do projeto

```text
Jogo_nave_em_C++/
├── main.cpp
├── index.html
├── README.md
├── .gitignore
└── arquivos gerados pela compilação (ignorados pelo Git)
```

## Licença

Este projeto foi desenvolvido como exercício de aprendizado e pode ser utilizado e modificado livremente.
