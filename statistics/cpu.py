import seaborn as sns
from matplotlib import pyplot as plt
import numpy as np

def mesureoverhead():
    FILENAME = 'csthread100'
    # LOOPNAME = 'loopdata10000'
    data = list(map(float, open(FILENAME, 'r').readlines()))
    # loop = list(map(float, open(LOOPNAME, 'r').readlines()))
    fig = plt.figure()
    sns.kdeplot(data, shade=True, label=f'context switch thread overhead: mean({round(np.mean(data), 6)}), std({round(np.std(data), 6)})')
    fig.savefig(f'{FILENAME}.png')
    # plt.show()

def procedureoverhead():
    FILENAME = 'procdata100000'
    data = np.array(list(map(lambda line: list(map(float, line.split())), open(FILENAME, 'r').readlines())))
    fig = plt.figure()
    for i in range(8):
        sns.kdeplot(data[:, i], label=f'{str(i)} args')
    fig.savefig(f'{FILENAME}.png')
    

if __name__ == '__main__':
    mesureoverhead()