import numpy as np
import matplotlib.pyplot as plt
data_200k = np.loadtxt('../data/200k_05/histogram-z.csv', delimiter=',')
data_1000k = np.loadtxt('../data/1000k_05/histogram-z.csv', delimiter=',')


fig = plt.figure(figsize=(10, 4))
ax = fig.add_subplot(211)


ax.set_xscale("log")
ax.set_xlim(1, 1e3)  # <-- check this as pointed out by @tillsten
ax.set_ylim(1, max(data_200k[2:-1, 2]))  # <--
# ax.set_aspect(0.5)
ax.set_title("Energy per depth slice (eV/nm)")
ax.plot(data_200k[:, 0], data_200k[:, 2], "r-", label="200k")
ax.plot(data_1000k[:, 0], 0.2*data_1000k[:, 2], "b--", label="1000k")
ax.legend(loc="lower left", ncol=2)

ax = fig.add_subplot(212, aspect='equal')
ax.set_xscale("log")
ax.set_xlim(1, 1e3)  # <-- check this as pointed out by @tillsten
ax.set_ylim(1,  max(data_200k[2:-1, 3]))  # <--
ax.set_xlabel('Depth (nm)')
# ax.set_aspect(0.5)
ax.set_title("Dissipation per depth slice (eV/nm)")


ax.plot(data_200k[:, 0], data_200k[:, 3], "r-", label="200k")
ax.plot(data_1000k[:, 0], 0.2*data_1000k[:, 3], "b--", label="1000k")
ax.legend(loc="lower left", ncol=2)

plt.subplots_adjust(left=0.1, bottom=0.15, right=0.9,
                    top=0.9, wspace=0.1, hspace=0.7)
plt.savefig('./result.svg')
plt.show()
