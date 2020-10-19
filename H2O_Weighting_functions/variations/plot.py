
import regex as re
from collections import defaultdict, OrderedDict
from matplotlib import pyplot as plt
from matplotlib import rc
rc('font', **{'family': 'serif'})
rc('text', usetex=True)
rc('text.latex', preamble=r"\usepackage[T2A]{fontenc}")
rc('text.latex', preamble=r"\usepackage[utf8]{inputenc}")
rc('text.latex', preamble=r"\usepackage[russian]{babel}")

additional_linestyles = OrderedDict(
                [('solid', (0, ())),
                 ('loosely dotted', (0, (1, 10))),
                 ('dotted', (0, (1, 5))),
                 ('densely dotted', (0, (1, 1))),

                 ('loosely dashed', (0, (5, 10))),
                 ('dashed', (0, (5, 5))),
                 ('densely dashed', (0, (5, 1))),

                 ('loosely dashdotted', (0, (3, 10, 1, 10))),
                 ('dashdotted', (0, (3, 5, 1, 5))),
                 ('densely dashdotted', (0, (3, 1, 1, 1))),

                 ('loosely dashdotdotted', (0, (3, 10, 1, 10, 1, 10))),
                 ('dashdotdotted', (0, (3, 5, 1, 5, 1, 5))),
                 ('densely dashdotdotted', (0, (3, 1, 1, 1, 1, 1)))])

ls = [additional_linestyles['densely dotted'], '--', additional_linestyles['solid'],
      additional_linestyles['densely dashdotted'], additional_linestyles['densely dashed'],
      ':', '-.']
c = ['#2E86C1', '#1F618D', '#117A65', '#28B463',
     '#CA6F1E', '#8E44AD', '#9B59B6', '#A93226', '#E74C3C']
fc = ['#2E86C180', '#1F618D80', '#117A6580', '#28B46380', '#CA6F1E80',
      '#8E44AD80', '#9B59B680', '#A9322680', '#E74C3C80']

# freqs = [18.0, 19.0, 20.0, 21.0, 22.2, 23.0, 24.0, 25.0, 26.0, 27.0]
# freqs = [18, 19, 20, 21, 23, 24, 25, 26, 27]
# freqs = [21.0, 21.5, 22.0, 22.2, 22.24, 22.3, 22.4, 22.5, 23.0]
freqs = [18.0, 19.0, 20.0, 21.0, 21.5, 22.0, 22.2, 22.24, 22.3, 22.4, 22.5, 23.0, 24.0, 25.0, 26.0, 27.0]

# with open("wfuncs_rho0_3-20_f_18-21_23-27GHz.txt", "r") as file:
# with open("wfuncs_rho0_3-20_f_21-23GHz.txt", "r") as file:
# with open("wfuncs_rho_p1_f_18-21_23-27GHz.txt", "r") as file:
# with open("wfuncs_rho_p0.1_f_21-23GHz.txt", "r") as file:
# with open("stddev_3.0_exp_full_range.txt", "r") as file:
# with open("stddev_3.0_exp_21-23GHz.txt", "r") as file:
with open("wfuncs_rho_3_20_full_range.txt", "r") as file:
    file.readline()
    H = []
    Curve = defaultdict(list)
    cme = defaultdict(list)
    cpe = defaultdict(list)
    for j, line in enumerate(file):
        line = [float(s) for s in re.split(' ', re.sub('[\r\n]', '', line)) if s]
        if not line:
            continue
        H.append(line[0])
        for i in range(len(freqs)):
            Curve[freqs[i]].append( (line[i + 1] + line[i + 1 + len(freqs)]) / 2 )
            cme[freqs[i]].append(line[i + 1])
            cpe[freqs[i]].append(line[i + 1 + len(freqs)])
            # Curve[freqs[i]].append(line[i + 1])
            # me = line[i + 1] - line[i + 1 + len(freqs)]
            # if me < 0:
            #     me = 0
            # cme[freqs[i]].append(me)
            # cpe[freqs[i]].append(line[i + 1] + line[i + 1 + len(freqs)])

plt.figure(0)
plt.title(r"$\rho(h)= 3..20 \cdot\exp{\left(-\frac{h}{2.1}\right)}$ г$\cdot$м$^{-3}$")
# plt.title(r"$\rho(h)= 7.5 \cdot\exp{\left(-\frac{h}{2.1}\right)}~~$ до $+1$ г$\cdot$м$^{-3}$")
# plt.title(r"$\rho(h)= 7.5 \cdot\exp{\left(-\frac{h}{2.1}\right)}~~$ до $+0.1$ г$\cdot$м$^{-3}$")
# plt.title(r"$\rho(h)=7.5\cdot\exp{\left(-\frac{h}{2.1}\right)}$ $~~~$ " +
#           r"$\sigma\left(\rho(h)\right)=3\cdot\exp{\left(-\frac{h}{2.1}\right)}$")
plt.xlabel(r"Весовая функция $W(\nu,h)$ Дб$\cdot$м$^2\cdot$кг$^{-1}$")
plt.ylabel("Высота h, км")
plt.ylim(0, 40)
# plt.ylim(0, 25)
# plt.ylim(0, 5)
# plt.xlim(0.015, 0.04)
plt.axes().set_xscale('log')
# for f in freqs:
# ls = {1: ':', 3: '-.', 4: '-', 6: '--'}
# for i, f in [(1, 18), (3, 21), (4, 23), (6, 25)]:
# ls = {0: ':', 6: ':', 1: '--', 5: '--', 2: '--', 3: '-', 4: '-.'}
# c = ['#3498DB', '#1A5276', 'black', 'black',
#      'black',  '#7D3C98', '#F39C12']
# fc = ['#3498DB80', '#1A527680', '#ffffff80', '#ffffff80',
#      '#ffffff80',  '#7D3C9880', '#F39C1280']
# for i, f in enumerate([21.0, 22.0, 22.2, 22.24, 22.3, 22.5, 23.0]):
#     w = 2
#     if f in [22.2, 22.24, 22.3]:
#         w = 1
#     plt.plot(Curve[f], H, label='{} ГГц'.format(f), color=c[i], linewidth=w, linestyle=ls[i])
# c = ['#3498DB', '#28B463', '#CA6F1E']
# fc = ['#3498DB80', '#28B46380', '#CA6F1E80']
# ls = {0: '--', 1: '-.', 2: '-'}
# for i, f in enumerate([21.0, 23.0, 22.24]):
# c = ['#4134cc', '#1d440b', '#e02ba1', '#5e6bbd', '#f58e84',
#      '#f2070b', '#345ec8', '#b44b2c', '#8e8a1d', '#190222']
# fc = ['#4134cc80', '#1d440b80', '#e02ba180', '#5e6bbd80', '#f58e8480',
#       '#f2070b80', '#345ec880', '#b44b2c80', '#8e8a1d80', '#19022280']
# for i, f in enumerate(freqs):
#     plt.plot(Curve[f], H, label='{} ГГц'.format(f), color=c[i], linewidth=2)
# c = ['#A569BD', '#45B39D', '#99A3A4']
# fc = ['#A569BD80', '#45B39D80', '#99A3A480']
c = ['#2E86C1', '#117A65', '#28B463',
     '#CA6F1E', '#00BFFF', '#9B59B6', '#E74C3C']
# fc = ['#2E86C180', '#117A6580', '#28B46380', '#CA6F1E80',
#       '#00BFFF80', '#9B59B680', '#E74C3C80']
fc = ['#2E86C130', '#117A6530', '#28B46330', '#CA6F1E30',
      '#00BFFF30', '#9B59B630', '#E74C3C30']
for i, f in enumerate([21.0, 22.0, 22.2, 22.24, 22.3, 22.4, 23.0]):
# for i, f in enumerate([18, 20, 21, 23, 24, 25, 27]):
    plt.plot(Curve[f], H, label='{} ГГц'.format(f), color=c[i], linewidth=2, linestyle=ls[i])
    plt.fill_betweenx(H, cme[f], cpe[f], color=fc[i])
plt.legend(loc='best')
# plt.legend(loc='lower right')
# plt.savefig("wfuncs_rho0_3-20_f_18-21_23-27GHz.png", dpi=600)
# plt.savefig("wfuncs_rho0_3-20_f_21-23GHz.png", dpi=600)
# plt.savefig("wfuncs_rho_p1_f_18-21_23-27GHz.png", dpi=600)
# plt.savefig("wfuncs_rho_p0.1_f_21-23GHz.png", dpi=600)
# plt.savefig("stddev_3.0_exp_full_range.png", dpi=600)
# plt.savefig("wfuncs_rho_3_20_full_range.png", dpi=600)
plt.savefig("wfuncs_rho_3_20_f_21_23GHz.png", dpi=600)
plt.show()
