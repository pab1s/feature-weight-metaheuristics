import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

sns.set_theme(style="darkgrid")

filenames = ['1714997370887197447_breast-cancer_fold_3.csv',
                '1714997547137587590_breast-cancer_fold_3.csv',
                '1714997694857295044_breast-cancer_fold_3.csv']

data_series = []

for i, filename in enumerate(filenames):
    df = pd.read_csv(f'outputs/training_fitness/{filename}', header=None, names=['Value'])
    data_series.append(df['Value'])


plt.figure(figsize=(10, 6))

colors = sns.color_palette('tab10', n_colors=len(filenames))

#for i, series in enumerate(data_series):
#    plt.plot(series, label=f'Partición {i+1}', color=colors[i])
plt.plot(data_series[0], label='AM(10, 1, 0)', color=colors[0])
plt.plot(data_series[1], label='AM(10, 0.1, 0)', color=colors[1])
plt.plot(data_series[2], label='AM(10, 0.1, 1)', color=colors[2])

#plt.title('Fitness de entrenamiento de la AGE-CA en Parkinsons')
plt.legend()
plt.xlabel('Iteración')
plt.ylabel('Fitness')

plt.show()

