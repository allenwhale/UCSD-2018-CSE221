import seaborn as sns
from matplotlib import pyplot as plt
import numpy as np


def mesureoverhead():
    FILENAME = 'measuredata1000'
    LOOPNAME = 'measuredata10000'
    data = list(map(float, open(FILENAME, 'r').readlines()))
    loop = list(map(float, open(LOOPNAME, 'r').readlines()))
    fig = plt.figure()
    sns.kdeplot(data, shade=True,
                label=f'measure(1000): mean({round(np.mean(data), 6)}), std({round(np.std(data), 6)})')
    sns.kdeplot(loop, shade=True,
                label=f'measure(10000): mean({round(np.mean(loop), 6)}), std({round(np.std(loop), 6)})')
    fig.savefig(f'measure.png')
    # plt.show()


def procedureoverhead():
    FILENAME = 'procdata10000'
    data = np.array(list(map(lambda line: list(
        map(float, line.split())), open(FILENAME, 'r').readlines())))
    fig = plt.figure()
    for i in range(8):
        sns.kdeplot(data[:, i], label=f'{str(i)} args')
    loop = list(map(float, open('loopdata10000', 'r').readlines()))
    sns.kdeplot(loop, shade=True, label='loop overhead')
    fig.savefig(f'{FILENAME}-withloop.png')


if __name__ == '__main__':
    procedureoverhead()
