import numpy as np
import random
import matplotlib
import matplotlib.pyplot as plt

def jackknife(x):
    ntot = len(x)
    y = np.array([np.average(np.delete(x,i)) for i in range(ntot)])
    return y, np.average(y), np.std(y)*np.sqrt(ntot-1)

def main():
    ntot = 2**8
    np.random.seed(12345)
    #x = np.random.normal(loc=300.0,size=ntot)
    x = np.random.uniform(low=-np.sqrt(3.0),high=np.sqrt(3.0),size=ntot)
    #print("x",x)
    print("ave(x),std(x)",np.average(x),np.std(x))

    y, y_ave, y_err = jackknife(x)
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
