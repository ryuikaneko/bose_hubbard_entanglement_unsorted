import numpy as np
import random
import matplotlib
import matplotlib.pyplot as plt

def blocking(x,m):
    ntot = len(x)
    nblock = ntot//m
    xblock = np.array([np.average(x[i*m:(i+1)*m]) for i in range(nblock)])
    return xblock, np.average(xblock), np.std(xblock)*np.sqrt(nblock/(nblock-1))

def bootstrap(x,nboot):
    ntot = len(x)
    y = np.array([np.average(x[ np.random.randint(ntot,size=ntot) ]) for i in range(nboot)])
    return y, np.average(y), np.std(y)

def main():
    x = np.loadtxt("../../dat_hist_L40_t1.0000000000")
    #x = x[:,0]
    x = x[:2**16,0]
    ntot = len(x)
    print("ntot",ntot)
    print("log2(ntot)",np.log2(ntot))
    #print("x",x)
    print("ave(x),std(x)",np.average(x),np.std(x))

    nblock = 2**10
    #m = 2**10
    m = ntot//nblock
    xblock, xblock_ave, xblock_err = blocking(x,m)
    print("ave(xblock),std(xblock)",xblock_ave,xblock_err)
    print("1/sqrt(m)",1.0/np.sqrt(m))
    #print("xblock",xblock)
    #nboot = 2**15
    nboot = 2**18
    y, y_ave, y_err = bootstrap(xblock,nboot)
    print("ave(y),std(y)",y_ave,y_err)
    print("1/sqrt(m) * 1/sqrt(ntot/m)",1.0/np.sqrt(m) * 1.0/np.sqrt(ntot//m))

    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 48
    plt.hist(x,bins=bins,density=True)
    plt.hist(xblock,bins=bins,density=True)
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
    plt.hist(xblock,bins=bins,density=True)
    fig.savefig("fig_perm_L40_t1_hist_xblock.pdf",bbox_inches="tight")
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
