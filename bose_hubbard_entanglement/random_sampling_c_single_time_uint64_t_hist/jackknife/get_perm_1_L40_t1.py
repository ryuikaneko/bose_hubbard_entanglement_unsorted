import numpy as np
import random
import matplotlib
import matplotlib.pyplot as plt

def jackknife(x):
    ntot = len(x)
    y = np.array([np.average(np.delete(x,i)) for i in range(ntot)])
    return y, np.average(y), np.std(y)*np.sqrt(ntot-1)

def main():
    x = np.loadtxt("../dat_hist_L40_t1.0000000000")
#    x = x[:,0] ## too many data
    x = x[:2**16,0]
    ntot = len(x)
    print("ntot",ntot)
    print("log2(ntot)",np.log2(ntot))
    #print("x",x)
    print("ave(x),std(x)",np.average(x),np.std(x))

    y, y_ave, y_err = jackknife(x)
    print("ave(y),std(y)",y_ave,y_err)
    print("1/sqrt(ntot)",1.0/np.sqrt(ntot))

    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 48
    plt.hist(x,bins=bins,density=True)
    plt.hist(y,bins=bins,density=True)
    fig.savefig("fig_perm_L40_t1_hist.pdf",bbox_inches="tight")
    plt.close()

    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 48
    plt.hist(x,bins=bins,density=True)
    fig.savefig("fig_perm_L40_t1_hist_x.pdf",bbox_inches="tight")
    plt.close()

    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 48
#    plt.hist(x,bins=bins,density=True)
    plt.hist(y,bins=bins,density=True)
    fig.savefig("fig_perm_L40_t1_hist_y.pdf",bbox_inches="tight")
    plt.close()


if __name__ == "__main__":
    main()
