import pandas as pd

def format_value(value, is_time=False):
    if is_time:
        return "{:.2e}".format(value)
    else:
        return "{:.2f}".format(value)

'''
filenames = [
    'ecoli_1nn_2024-04-07_13-36-42.csv',
    'parkinsons_1nn_2024-04-07_13-36-42.csv',
    'breast-cancer_1nn_2024-04-07_13-36-42.csv'
]

filenames = [
    'ecoli_relief_2024-04-07_13-36-42.csv',
    'parkinsons_relief_2024-04-07_13-36-43.csv',
    'breast-cancer_relief_2024-04-07_13-36-43.csv'
]
filenames = [
    'ecoli_local-search_2024-04-07_13-36-44.csv',
    'parkinsons_local-search_2024-04-07_13-36-49.csv',
    'breast-cancer_local-search_2024-04-07_13-37-06.csv'
]'''
filenames = [
    'ecoli_best-local-search_2024-04-07_13-39-26.csv',
    'parkinsons_best-local-search_2024-04-07_13-39-56.csv',
    'breast-cancer_best-local-search_2024-04-07_13-40-14.csv'
]

dataset_names = ['Ecoli', 'Parkinsons', 'Breast-cancer']

latex_table_header = """
\\begin{table}[H]
\\begin{adjustbox}{width=\columnwidth,center}
\\begin{tabular}{|l|cccc|cccc|cccc|}
\\hline
& \\multicolumn{4}{c|}{Ecoli} & \\multicolumn{4}{c|}{Parkinsons} & \\multicolumn{4}{c|}{Breast-cancer} \\\\
& \\% clas & \\% red & Fit. & T(s) & \\% clas & \\% red & Fit. & T(s) & \\% clas & \\% red & Fit. & T(s) \\\\
\\hline
"""

latex_table = latex_table_header

means = []
stds = []

for filename in filenames:
    df = pd.read_csv(f'outputs/results/{filename}')
    
    dataset_means = df[['test_class_rate', 'red_rate', 'fitness', 'time']].mean()
    dataset_stds = df[['test_class_rate', 'red_rate', 'fitness', 'time']].std()
    means.append(dataset_means)
    stds.append(dataset_stds)
    
for fold in range(1, 6):
    latex_table += f"Partición {fold} "
    for i, dataset_name in enumerate(dataset_names):
        fold_data = pd.read_csv(f'outputs/results/{filenames[i]}')
        fold_data = fold_data[fold_data['fold'] == fold]

        for col in ['test_class_rate', 'red_rate', 'fitness', 'time']:
            latex_table += "& " + format_value(fold_data[col].iloc[0], is_time=(col == 'time')) + " "
    latex_table += "\\\\\n"

latex_table += "\\hline\nMedia "
for mean in means:
    for col in ['test_class_rate', 'red_rate', 'fitness', 'time']:
        latex_table += "& " + format_value(mean[col], is_time=(col == 'time')) + " "
latex_table += "\\\\\n"

latex_table += "Desv. Est. "
for std in stds:
    for col in ['test_class_rate', 'red_rate', 'fitness', 'time']:
        latex_table += "& " + format_value(std[col], is_time=False) + " "
latex_table += "\\\\\n"

latex_table += "\\hline\n\\end{tabular}\n\\end{adjustbox}\n\\caption{Resultados}\n\\label{tab:resultados}\n\\end{table}\n"

print(latex_table)
