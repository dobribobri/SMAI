# -*- coding: utf-8 -*-

from __future__ import division, print_function
import sys
from argparse import ArgumentParser
from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import regex as re


def createArgParser() -> ArgumentParser:
    ap = ArgumentParser()
    ap.add_argument('--filepath', default='tmp00.txt')
    ap.add_argument('--scatter3d', action='store_true', default=False)
    ap.add_argument('--scatter2d', action='store_true', default=False)
    ap.add_argument('--colored', action='store_true', default=False)
    ap.add_argument('--colortype', default='rgba')
    ap.add_argument('--every', default=1)
    ap.add_argument('--plot2d', action='store_true', default=False)
    ap.add_argument('--altitude', action='store_true', default=False)
    ap.add_argument('--xlabel', default='X')
    ap.add_argument('--ylabel', default='Y')
    ap.add_argument('--zlabel', default='Z')
    return ap


def __parse_line(line: str) -> list:
    return [f for f in re.split(r'[, \t]', re.sub(r'[^0-9\s{}\t]'.format(',.'), '', re.sub('[\r\n]', '', line)))
            if f not in [',', ' ', '\t', '']]


def scatter3d(filepath: str, x_label: str = 'X', y_label: str = 'Y', z_label: str = 'Z',
              every: int = 1, colored=False, colortype: str = 'rgba') -> None:
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    ax.set_zlabel(z_label)
    xs, ys, zs, color = [], [], [], []
    with open(filepath, 'r') as file:
        for i, line in enumerate(file):
            if i % every:
                continue
            line = __parse_line(line)
            if not line:
                if not i:
                    continue
                if colored:
                    Axes3D.scatter(ax, xs=xs, ys=ys, zs=zs, c=color)
                else:
                    Axes3D.scatter(ax, xs=xs, ys=ys, zs=zs)
                continue
            xs.append(float(line[0]))
            ys.append(float(line[1]))
            zs.append(float(line[2]))
            if colored:
                if colortype in ['rgb', 'RGB']:
                    R, G, B = line[3:6]
                    color.append((float(R), float(G), float(B)))
                else:
                    R, G, B, A = line[3:7]
                    color.append((float(R), float(G), float(B), float(A)))
            if i % 10000:
                sys.stdout.write("\rСтрок обработано:\t%d" % i)
                sys.stdout.flush()
        sys.stdout.write("\rСтрок обработано:\t%d" % i)
        sys.stdout.flush()
        print()


def scatter2d(filepath: str, x_label: str = 'X', y_label: str = 'Y', altitude=False,
           every: int = 1, colored=False, colortype: str = 'rgba') -> None:
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    x, y, color = [], [], []
    with open(filepath, 'r') as file:
        for i, line in enumerate(file):
            if i % every:
                continue
            line = __parse_line(line)
            if not line:
                if not i:
                    continue
                if altitude:
                    x, y = y, x
                if colored:
                    plt.scatter(y, x, c=color)
                else:
                    plt.scatter(y, x)
                continue
            x.append(float(line[0]))
            y.append(float(line[1]))
            if colored:
                if colortype in ['rgb', 'RGB']:
                    R, G, B = line[3:6]
                    color.append((float(R), float(G), float(B)))
                else:
                    R, G, B, A = line[3:7]
                    color.append((float(R), float(G), float(B), float(A)))
            sys.stdout.write("\rСтрок обработано:\t%d" % i)
            sys.stdout.flush()
        print()


def plot2d(filepath: str, x_label: str = 'X', y_label: str = 'Y', altitude=False,
           every: int = 1) -> None:
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)
    x, y = [], []
    with open(filepath, 'r') as file:
        for i, line in enumerate(file):
            if i % every:
                continue
            line = __parse_line(line)
            if not line:
                if not i:
                    continue
                if altitude:
                    x, y = y, x
                plt.plot(y, x)
                continue
            x.append(float(line[0]))
            y.append(float(line[1]))
            sys.stdout.write("\rСтрок обработано:\t%d" % i)
            sys.stdout.flush()
        print()


if __name__ == "__main__":
    parser = createArgParser()
    namespace = parser.parse_args(sys.argv[1:])

    if bool(namespace.scatter3d):
        scatter3d(str(namespace.filepath),
                  str(namespace.xlabel), str(namespace.ylabel), str(namespace.zlabel),
                  int(namespace.every), bool(namespace.colored), str(namespace.colortype))
        plt.show()

    if bool(namespace.scatter2d):
        scatter2d(str(namespace.filepath),
                  str(namespace.xlabel), str(namespace.ylabel),
                  bool(namespace.altitude),
                  int(namespace.every), bool(namespace.colored), str(namespace.colortype))
        plt.show()

    if bool(namespace.plot2d):
        plot2d(str(namespace.filepath),
                  str(namespace.xlabel), str(namespace.ylabel),
                  bool(namespace.altitude),
                  int(namespace.every))
        plt.show()

