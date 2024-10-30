# IoT ICC035-TEPVI

Este repositório refere-se aos códigos relacionados ao pequeno projeto realizado na disciplina TEPVI (Tópicos Especiais em Programação IV). Esta disciplina apresenta os fundamentos básicos de IoT (Internet das Coisas), seus elementos e as tecnologias de base que dão suporte a essa tecnologia.

## Objetivo do Projeto

Desenvolver uma aplicação prática em IoT para a medição contínua da temperatura de um corpo hídrico, utilizando um sensor de temperatura NTC 10K MF58 conectado a um Arduino Uno, aplicando os conceitos básicos de sensores, coleta de dados e monitoramento. A temperatura é medida com o sensor e enviada utilizando um LoRa ESP32 v3 para outro LoRa receber os pacotes via wireless.

### Imagens:

Gráficos das temperaturas lidas. Valores de temperatura são lidos em um intervalo de 24 horas a cada um segundo.

### Registros:

Arquivos .csv das temperaturas lidas usando os sensores.

### relatorios:

Relatórios da disciplina como forma de obtenção de nota parcial na mesma.

### src:

Códigos utilizados para fazer a programação do Arduino, LoRa e etc.

### requirements.txt: Arquivo com as bibliotecas python para executar o código, basta executar:

```bash
pip install -r requirements.txt
```
## Gráficos de temperatura gerados:

![Gráfico Temperatura](https://github.com/user-attachments/assets/b50f08f7-9039-4051-8235-cf053feb173e)
