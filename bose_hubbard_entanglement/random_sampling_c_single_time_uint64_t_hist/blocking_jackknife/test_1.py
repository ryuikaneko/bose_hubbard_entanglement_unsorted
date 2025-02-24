import numpy as np
import random
import matplotlib
import matplotlib.pyplot as plt

def blocking(x,m):
    ntot = len(x)
    nblock = ntot//m
    xblock = np.array([np.average(x[i*m:(i+1)*m]) for i in range(nblock)])
    return xblock, np.average(xblock), np.std(xblock)*np.sqrt(nblock/(nblock-1))

def jackknife(x):
    ntot = len(x)
    y = np.array([np.average(np.delete(x,i)) for i in range(ntot)])
    return y, np.average(y), np.std(y)*np.sqrt(ntot-1)

def direct(x,m):
    ntot = len(x)
    nblock = ntot//m
    xblock = np.array([np.average(x[i*m:(i+1)*m]) for i in range(nblock)])
    jackknife_err = np.sqrt(np.var(xblock)/(nblock-1))
    return jackknife_err

def main():
    ntot = 2**15
    np.random.seed(12345)
    #x = np.random.normal(loc=300.0,size=ntot)
    x = np.random.uniform(low=-np.sqrt(3.0),high=np.sqrt(3.0),size=ntot)
    #print("x",x)
    print("ave(x),std(x)",np.average(x),np.std(x))

    m = 2**5
    xblock, xblock_ave, xblock_err = blocking(x,m)
    print("ave(xblock),std(xblock)",xblock_ave,xblock_err)
    print("1/sqrt(m)",1.0/np.sqrt(m))
    #print("xblock",xblock)
    y, y_ave, y_err = jackknife(xblock)
    print("ave(y),std(y)",y_ave,y_err)
    jackknife_err = direct(x,m)
    print("direct calculation: jackknife_err",jackknife_err)
    print("1/sqrt(m) * 1/sqrt(ntot/m)",1.0/np.sqrt(m) * 1.0/np.sqrt(ntot//m))

    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 48
    plt.hist(x,bins=bins,density=True)
    plt.hist(xblock,bins=bins,density=True)
    plt.hist(y,bins=bins,density=True)
    fig.savefig("fig_hist.pdf",bbox_inches="tight")
    plt.close()

    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 48
    plt.hist(x,bins=bins,density=True)
    fig.savefig("fig_hist_x.pdf",bbox_inches="tight")
    plt.close()

    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 48
#    plt.hist(x,bins=bins,density=True)
    plt.hist(xblock,bins=bins,density=True)
    fig.savefig("fig_hist_xblock.pdf",bbox_inches="tight")
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
