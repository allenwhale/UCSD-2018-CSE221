import seaborn as sns
from matplotlib import pyplot as plt
import numpy as np


def file_cache():
    FILENAME = 'file_cache'
    data = list(map(float, open(FILENAME, 'r').readlines()))

    fig = plt.figure()
    size = len(data)
    plt.xlabel('file size (KB)')
    plt.xticks(np.arange(size), [1 << (i + 4) for i in np.arange(size)])
    plt.ylabel('bandwidth (MB/s)')
    sns.lineplot(x=np.arange(size), y=data, label="sequential read")
    fig.savefig(f'{FILENAME}.png')
    # plt.show()


def file_read():
    FILENAME = 'file_read'
    data = list(map(float, open(FILENAME, 'r').readlines()))

    fig = plt.figure()
    size = len(data) // 2
    plt.xlabel('file size (KB)')
    plt.xticks(np.arange(size), [1 << (i + 4) for i in np.arange(size)])
    plt.ylabel('latency (us/block)')
    sns.lineplot(x=np.arange(size), y=data[:size], label="sequential read")
    sns.lineplot(x=np.arange(size), y=data[size:], label="random read")
    fig.savefig(f'{FILENAME}.png')
    # plt.show()


if __name__ == '__main__':
    file_read()
