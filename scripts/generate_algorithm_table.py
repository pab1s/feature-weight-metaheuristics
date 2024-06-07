import pandas as pd

def format_value(value, is_time=False):
    if is_time:
        return "{:.2e}".format(value)
    else:
        return "{:.2f}".format(value)

filenames = [
    'ecoli_ils-sa_2024-05-30_17-13-00.csv',
    'parkinsons_ils-sa_2024-05-30_17-13-33.csv',
    'breast-cancer_ils-sa_2024-05-30_17-13-53.csv'
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
