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
    plt.axvline(x=1, color='#BCBBBB', linestyle='--')
    plt.text(0.7, 400, 'L1 Cache Size', rotation=90)
    plt.axvline(x=4, color='#000F1C', linestyle='--')
    plt.text(3.7, 400, 'L2 Cache Size', rotation=90)
    fig.savefig(f'{FILENAME}.png')
    # plt.show()


def file_read():
    FILENAME = 'file_read'
    data = [1 / d for d in map(float, open(FILENAME, 'r').readlines())]

    fig = plt.figure()
    size = len(data) // 2
    plt.xlabel('file size (KB)')
    plt.xticks(np.arange(size), [1 << (i + 4) for i in np.arange(size)])
    plt.ylabel('time (us)')
    sns.lineplot(x=np.arange(size), y=data[:size], label="sequential read")
    sns.lineplot(x=np.arange(size), y=data[size:], label="random read")
    plt.axvline(x=1, color='#BCBBBB', linestyle='--')
    plt.text(0.7, 40, 'L1 Cache Size', rotation=90)
    plt.axvline(x=4, color='#000F1C', linestyle='--')
    plt.text(3.7, 40, 'L2 Cache Size', rotation=90)
    fig.savefig(f'{FILENAME}.png')
    # plt.show()


def file_content():
    FILENAME = 'file_content'
    data = list(map(float, open(FILENAME, 'r').readlines()))
    ydata = []
    i = 1
    base = 0
    while i < 64:
        ydata.append(1 / (np.mean(data[base: base + i]) / 4096))
        base += i
        i <<= 1
    fig = plt.figure()
    size = len(ydata)
    plt.xlabel('# of processes')
    plt.xticks(np.arange(size), [1 << i for i in np.arange(size)])
    plt.ylabel('block access time (us)')
    sns.lineplot(x=np.arange(size), y=ydata, label="sequential read")
    fig.savefig(f'{FILENAME}.png')


if __name__ == '__main__':
    file_content()
