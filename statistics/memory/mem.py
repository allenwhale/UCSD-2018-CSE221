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
    plt.text(0.8, 0.5, 'L1 Cache Size', rotation=90)
    plt.axvline(x=4, color='#000F1C', linestyle='--')
    plt.text(3.8, 0.5, 'L2 Cache Size', rotation=90)
    fig.savefig(f'{FILENAME}.png')
    # plt.show()


if __name__ == '__main__':
    memorylatency()
