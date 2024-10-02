# IoT ICC035-TEPVI

Este repositório refere-se aos códigos relacionados ao pequeno projeto realizado na disciplina TEPVI (Tópicos Especiais em Programação IV). Esta disciplina apresenta os fundamentos básicos de IoT (Internet das Coisas), seus elementos e as tecnologias de base que dão suporte a essa tecnologia.

## Objetivo do Projeto

Desenvolver uma aplicação prática em IoT para a medição contínua da temperatura de um corpo hídrico, utilizando um sensor de temperatura NTC 10K MF58 conectado a um Arduino Uno, aplicando os conceitos básicos de sensores, coleta de dados e monitoramento.

## Código

### main.py: Arquivo responsável por ler da entrada Serial os dados passados pelo Arduino Uno e salvar no arquivo .csv

### arduino.cpp: Arquivo responsável pelo código que estará sendo executando pelo Arduino Uno durante o monitoramento, ele lê uma entrada analógica e manda a leitura para porta Serial.

### analiseDados.py: Arquivo responsável pela plotagem de gráficos e analise dos dados obtidos

### Relatório.pdf: Arquivo com o relatório completo do projeto.

### requirements.txt: Arquivo com as bibliotecas python para executar o código, basta executar:

```bash
pip install -r requirements.txt
```
