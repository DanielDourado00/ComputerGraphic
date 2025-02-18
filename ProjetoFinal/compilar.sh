#!/bin/bash

# Exibe uma mensagem de status
echo "Compilando o projeto do Aquário 3D..."

# Comando de compilação
g++ \
  main.cpp \
  aquarium.cpp \
  camera.cpp \
  fish.cpp \
  bubbles.cpp \
  lighting.cpp \
  algae.cpp \
  decoration.cpp \
  fish3d.cpp \
  -o aquario \
  -lGL -lGLU -lglut -lSOIL \
  -lSDL2 -lSDL2_mixer \
  -std=c++11

# Se não houver erro, exibe:
if [ $? -eq 0 ]; then
  echo "Compilação concluída com sucesso!"
  echo "Para executar, use: ./aquario"
else
  echo "Ocorreu um erro na compilação."
fi
