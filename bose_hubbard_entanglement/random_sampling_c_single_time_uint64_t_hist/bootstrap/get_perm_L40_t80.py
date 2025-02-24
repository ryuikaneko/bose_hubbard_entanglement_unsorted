import numpy as np
import random
import matplotlib
import matplotlib.pyplot as plt

def bootstrap(x,nboot):
    ntot = len(x)
    y = np.array([np.average(x[ np.random.randint(ntot,size=ntot) ]) for i in range(nboot)])
    return y, np.average(y), np.std(y)

def main():
    x = np.loadtxt("../dat_hist_L40_t80.0000000000")
    x = x[:,0]
    ntot = len(x)
    print("ntot",ntot)
    print("log2(ntot)",np.log2(ntot))
    #print("x",x)
    print("ave(x),std(x)",np.average(x),np.std(x))

    nboot = 2**12 ## ~10sec
    #nboot = 2**13 ## ~20sec
    y, y_ave, y_err = bootstrap(x,nboot)
    print("ave(y),std(y)",y_ave,y_err)
    print("1/sqrt(ntot)",1.0/np.sqrt(ntot))

    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 48
    plt.hist(x,bins=bins,density=True)
    plt.hist(y,bins=bins,density=True)
    fig.savefig("fig_perm_L40_t80_hist.pdf",bbox_inches="tight")
    plt.close()

    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 48
    plt.hist(x,bins=bins,density=True)
    fig.savefig("fig_perm_L40_t80_hist_x.pdf",bbox_inches="tight")
    plt.close()

    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 48
#    plt.hist(x,bins=bins,density=True)
    plt.hist(y,bins=bins,density=True)
    fig.savefig("fig_perm_L40_t80_hist_y.pdf",bbox_inches="tight")
    plt.close()


if __name__ == "__main__":
    main()
