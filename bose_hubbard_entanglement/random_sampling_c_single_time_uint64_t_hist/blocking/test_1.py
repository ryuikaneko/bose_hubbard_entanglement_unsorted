import numpy as np
import random
import matplotlib
import matplotlib.pyplot as plt

def blocking(x,m):
    ntot = len(x)
    nblock = ntot//m
    y = np.array([np.average(x[i*m:(i+1)*m]) for i in range(nblock)])
    return y, np.average(y), np.std(y)*np.sqrt(nblock/(nblock-1))

def main():
    ntot = 2**20
    np.random.seed(12345)
    #x = np.random.normal(loc=300.0,size=ntot)
    x = np.random.uniform(low=-np.sqrt(3.0),high=np.sqrt(3.0),size=ntot)
    #print("x",x)
    print("ave(x),std(x)",np.average(x),np.std(x))

    m = 2**5
    y, y_ave, y_err = blocking(x,m)
    #print("y",y)
    print("ave(y),std(y)",y_ave,y_err)
    print("1/sqrt(m)",1.0/np.sqrt(m))

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
