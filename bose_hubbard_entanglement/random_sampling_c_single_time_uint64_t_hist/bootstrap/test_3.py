## https://www.digitalocean.com/community/tutorials/bootstrap-sampling-in-python

import numpy as np
import random
import matplotlib
import matplotlib.pyplot as plt

def bootstrap(x,nboot):
    ntot = len(x)
    y = []
    for i in range(nboot):
        rnd = np.random.randint(ntot,size=ntot)
        y.append(np.average(x[rnd]))
    return y, np.average(y), np.std(y)

def main():
    ntot = 2**8
    np.random.seed(12345)
    #x = np.random.normal(loc=300.0,size=ntot)
    x = np.random.uniform(low=-np.sqrt(3.0),high=np.sqrt(3.0),size=ntot)
    #print("x",x)
    print("ave(x),std(x)",np.average(x),np.std(x))

    nboot = 2**16
    y, y_ave, y_err = bootstrap(x,nboot)
    #print("y",y)
    print("ave(y),std(y)",y_ave,y_err)
    print("1/sqrt(ntot)",1.0/np.sqrt(ntot))

    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 48
    plt.hist(x,bins=bins,density=True)
    plt.hist(y,bins=bins,density=True)
    fig.savefig("fig_hist.pdf",bbox_inches="tight")
    plt.close()

    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 48
#    plt.hist(x,bins=bins,density=True)
    plt.hist(y,bins=bins,density=True)
    fig.savefig("fig_hist_y.pdf",bbox_inches="tight")
    plt.close()


if __name__ == "__main__":
    main()
