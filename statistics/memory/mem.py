import seaborn as sns
from matplotlib import pyplot as plt
import numpy as np


def memorylatency():
    FILENAME = 'mem_acc_latency'
    data = list(map(float, open(FILENAME, 'r').readlines()))

    fig = plt.figure(figsize=(12, 8))

    size = 11
    strides = list(range(4, 11, 2))
    plt.xlabel('Memory Size (kB)')
    plt.xticks(np.arange(size), [1 << (i + 4) for i in np.arange(size)])
    plt.ylabel('latency (us)')
    for i in range(0, len(data), size):
        y = data[i:i + size]
        sns.lineplot(x=np.arange(size), y=y,
                     label=f"stride-{1 << (strides[i // size])} B")
    plt.axvline(x=1, color='#BCBBBB', linestyle='--')
    plt.text(0.8, 0.1, 'L1 Cache Size', rotation=90)
    plt.axvline(x=4, color='#000F1C', linestyle='--')
    plt.text(3.8, 0.1, 'L2 Cache Size', rotation=90)
    fig.savefig(f'{FILENAME}.png')
    # plt.show()


def memrwLatency():
    FILENAME = 'mem_rw_latency_steps'
    data = list(map(float, open(FILENAME, 'r').readlines()))
    fig = plt.figure()
    fig.savefig(f'{FILENAME}.png')


def pagefaultLatency():
    FILENAME = 'mem_pagefault_latency'
    data = list(map(float, open(FILENAME, 'r').readlines()))
    fig = plt.figure()
    sns.kdeplot(
        data, label=f'pagefault(us): mean({round(np.mean(data), 6)}), std({round(np.std(data), 6)})')
    fig.savefig(f'{FILENAME}.png')


if __name__ == '__main__':
    memorylatency()
