import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import pandas as pd
import numpy as np
import math

# Leitura do primeiro conjunto de dados
df = pd.read_csv("./Registros/temperature_readings.csv", names=['Date', 'ADC', 'Temperature', 'Nothing'])

df.drop(columns=['Nothing'], inplace=True)
df['Date'] = pd.to_datetime(df['Date'], format='%Y-%m-%d %H:%M:%S')

dates = df["Date"]
temp = df["Temperature"]

def moving_average(data, window_size):
    return np.convolve(data, np.ones(window_size)/window_size, mode='valid')

window_size = 100

# Identificação de lacunas nos dados
df['Time_Diff'] = df['Date'].diff().dt.total_seconds()
gap_threshold = 300  # em segundos
df['Gap'] = df['Time_Diff'] > gap_threshold
df['Group'] = df['Gap'].cumsum()

# Listas para armazenar dados suavizados e datas correspondentes
smoothed_dates_list = []
smoothed_data_list = []

# Iterar sobre cada grupo contínuo
for group_num, group_df in df.groupby('Group'):
    temp_values = group_df['Temperature'].values
    date_values = group_df['Date'].values

    if len(temp_values) >= window_size:
        smoothed_temp = moving_average(temp_values, window_size)
        smoothed_dates = date_values[window_size - 1:]
        smoothed_dates_list.append(smoothed_dates)
        smoothed_data_list.append(smoothed_temp)
    else:
        continue

data_df = pd.read_csv("./Registros/wunderground_temperature.csv", dayfirst=True)

# Ensure that 'Data' and 'Hora' are strings
data_df['Data'] = data_df['Data'].astype(str)
data_df['Hora'] = data_df['Hora'].astype(str)

# Combine the columns and parse them into datetime
data_df['Data_Hora'] = pd.to_datetime(data_df['Data'] + ' ' + data_df['Hora'], dayfirst=True)


# Limpeza e conversão dos dados de temperatura
data_df['Temperatura'] = data_df['Temperatura'].str.replace(' °F', '').astype(float)
# Conversão para Celsius
data_df['Temperatura_C'] = (data_df['Temperatura'] - 32) * 5/9

# Preparação dos dados para plotagem
data_dates = data_df['Data_Hora']
data_temps = data_df['Temperatura_C']

# Plotagem
plt.figure(figsize=(10, 6))

# Plotar o primeiro conjunto de dados
for dates_segment, temps_segment in zip(smoothed_dates_list, smoothed_data_list):
    plt.plot(dates_segment, temps_segment, label='Registro nosso', color="red")

# Plotar o segundo conjunto de dados
plt.plot(data_dates, data_temps, label='Registro wunderground', linestyle='--')

plt.xlabel('Hora')
plt.ylabel('Temperatura (°C)')
plt.title('Temperatura ao longo do tempo 27/09/2024 - 28/09/2024')
plt.grid(True)

# Formatação do eixo x para mostrar apenas dia e hora
ax = plt.gca()
locator = mdates.HourLocator()  # Coloca ticks a cada hora
formatter = mdates.DateFormatter('%H:%M')  # Formato: dia e hora:minuto
ax.xaxis.set_major_locator(locator)
ax.xaxis.set_major_formatter(formatter)

plt.yticks(np.arange(math.floor(min(data_temps)), max(data_temps)+4, 1))

plt.gcf().autofmt_xdate()  # Rotaciona as labels para melhor visualização
plt.legend()
plt.savefig("./Imagens/Gráfico Temperatura.png")
plt.show()
