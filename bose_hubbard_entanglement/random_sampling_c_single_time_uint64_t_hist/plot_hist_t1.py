import numpy as np
import matplotlib
import matplotlib.pyplot as plt

def main():
#    perm = np.loadtxt("dat_hist_L40_t80.0000000000")
    perm = np.loadtxt("dat_hist_L40_t1.0000000000")

    print(np.average(perm[:,0]),np.std(perm[:,0])/np.sqrt(len(perm[:,0])))
    print(np.average(perm[:,1]),np.std(perm[:,1])/np.sqrt(len(perm[:,1])))
#    print(np.average(perm[:,0]),np.std(perm[:,0]))
#    print(np.average(perm[:,1]),np.std(perm[:,1]))
    print(-np.log(np.average(perm[:,0])))

    mask = (perm[:,0]>0)
    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
#    plt.hist(perm,density=True)
#    bins = 48
    bins = 96
#    plt.hist(perm,bins=bins,density=True)
#    plt.hist(perm[:,0],bins=bins,density=True)
    plt.hist(-np.log(perm[:,0][mask]),bins=bins,density=False,alpha=0.5,color="r")
    plt.hist(-np.log(-perm[:,0][~mask]),bins=bins,density=False,alpha=0.5,color="b")
#    fig.savefig("fig_hist_perm_real_t80.pdf",bbox_inches="tight")
    fig.savefig("fig_hist_perm_real_t1.pdf",bbox_inches="tight")
    plt.close()

    mask = (perm[:,1]>0)
    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
#    plt.hist(perm,density=True)
#    bins = 48
    bins = 96
#    plt.hist(perm,bins=bins,density=True)
#    plt.hist(perm[:,0],bins=bins,density=True)
    plt.hist(-np.log(perm[:,1][mask]),bins=bins,density=False,alpha=0.5,color="r")
    plt.hist(-np.log(-perm[:,1][~mask]),bins=bins,density=False,alpha=0.5,color="b")
#    fig.savefig("fig_hist_perm_imag_t80.pdf",bbox_inches="tight")
    fig.savefig("fig_hist_perm_imag_t1.pdf",bbox_inches="tight")
    plt.close()

if __name__ == "__main__":
    main()

