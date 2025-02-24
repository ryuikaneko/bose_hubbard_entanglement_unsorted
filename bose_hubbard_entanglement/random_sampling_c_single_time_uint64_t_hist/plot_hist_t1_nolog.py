import numpy as np
import matplotlib
import matplotlib.pyplot as plt

def main():
#    perm = np.loadtxt("dat_hist_L40_t80.0000000000")
    perm = np.loadtxt("dat_hist_L40_t1.0000000000")

    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
#    plt.hist(perm,density=True)
#    bins = 48
    bins = 96
#    plt.hist(perm,bins=bins,density=True)
    plt.hist(perm[:,0],bins=bins,density=True)
    fig.savefig("fig_hist_perm_real_t1_nolog.pdf",bbox_inches="tight")
    plt.close()

    mask = (perm[:,1]>0)
    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
#    plt.hist(perm,density=True)
#    bins = 48
    bins = 96
#    plt.hist(perm,bins=bins,density=True)
    plt.hist(perm[:,1],bins=bins,density=True)
    fig.savefig("fig_hist_perm_imag_t1_nolog.pdf",bbox_inches="tight")
    plt.close()

if __name__ == "__main__":
    main()

