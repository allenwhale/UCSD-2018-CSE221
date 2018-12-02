import seaborn as sns
from matplotlib import pyplot as plt
import numpy as np


def band_rw():
    FILENAME = 'band_rw'
    data = list(map(float, open(FILENAME, 'r').readlines()))
    tx = data[::2]
    rx = data[1::2]
    fig = plt.figure()
    size = len(data) // 2
    plt.xlabel('packet size (KB)')
    plt.xticks(np.arange(size), [1 << i for i in np.arange(size)])
    plt.ylabel('bandwidth (MB/s)')
    sns.lineplot(x=np.arange(size), y=tx, label="tx")
    sns.lineplot(x=np.arange(size), y=rx, label="rx")
    fig.savefig(f'{FILENAME}.png')
    # plt.show()


if __name__ == '__main__':
    band_rw()
