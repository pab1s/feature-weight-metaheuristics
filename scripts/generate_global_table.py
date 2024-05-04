import pandas as pd
import os
import re

def format_value(value, is_time=False):
    if is_time:
        return "{:.2e}".format(value)
    else:
        return "{:.2f}".format(value)

def get_csv_filename(dataset, algorithm):
    pattern = f"{dataset}_{algorithm}_\\d{{4}}-\\d{{2}}-\\d{{2}}_\\d{{2}}-\\d{{2}}-\\d{{2}}\\.csv"
    for f in os.listdir('outputs/results/'):
        if re.match(pattern, f):
            return os.path.join('outputs/results/', f)
    return None

algorithms = ['1nn', 'relief', 'local-search']

datasets = ['ecoli', 'parkinsons', 'breast-cancer']

latex_table_header = """
\\begin{table}[H]
\\begin{adjustbox}{width=\\columnwidth,center}
\\begin{tabular}{|l|cccc|cccc|cccc|}
\\hline
& \\multicolumn{4}{c|}{Ecoli} & \\multicolumn{4}{c|}{Parkinsons} & \\multicolumn{4}{c|}{Breast-cancer} \\\\
& \\% clas & \\% red & Fit. & T(s) & \\% clas & \\% red & Fit. & T(s) & \\% clas & \\% red & Fit. & T(s) \\\\
\\hline
"""

latex_table = latex_table_header

for algorithm in algorithms:
    latex_table += f"{algorithm.upper().replace('1NN', '1-NN')} "
    for dataset in datasets:
        filename = get_csv_filename(dataset, algorithm)
        if filename is not None:
            df = pd.read_csv(filename)
            mean_values = df.mean()
            latex_table += "& " + " & ".join(format_value(mean_values[col], is_time=(col == 'time')) for col in ['test_class_rate', 'red_rate', 'fitness', 'time']) + " "
        else:
            latex_table += "& " + " & ".join(["N/A"] * 4) + " "
    latex_table += "\\\\\n\\hline\n"

latex_table += "\\end{tabular}\n\\end{adjustbox}\n\\caption{Comparativa de la eficiencia de los algoritmos 1-NN, RELIEF y BL en los conjuntos de datos Ecoli, Parkinsons y Breast-cancer.}\n\\label{tab:resultados}\n\\end{table}\n"

print(latex_table)
