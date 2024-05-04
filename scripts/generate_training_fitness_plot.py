import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

sns.set_theme(style="darkgrid")

filenames = ['1712489855022775484_breast-cancer_fold_1.csv',
                '1712489882249218718_breast-cancer_fold_2.csv',
                '1712489909964249845_breast-cancer_fold_3.csv',
                '1712489937792821875_breast-cancer_fold_4.csv',
                '1712489966488603531_breast-cancer_fold_5.csv']

data_series = []

for i, filename in enumerate(filenames):
    df = pd.read_csv(f'outputs/training_fitness/{filename}', header=None, names=['Value'])
    data_series.append(df['Value'])


plt.figure(figsize=(10, 6))

colors = sns.color_palette('tab10', n_colors=len(filenames))

for i, series in enumerate(data_series):
    plt.plot(series, label=f'Partición {i+1}', color=colors[i])

plt.title('Fitness de entrenamiento de la búsqueda local del primer mejor en Breast Cancer')
plt.legend()
plt.xlabel('Iteración')
plt.ylabel('Fitness')

plt.show()

