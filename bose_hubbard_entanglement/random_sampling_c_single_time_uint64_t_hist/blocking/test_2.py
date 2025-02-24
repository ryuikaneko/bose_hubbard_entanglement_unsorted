import numpy as np
import random
import matplotlib
import matplotlib.pyplot as plt

def blocking(x,m):
    ntot = len(x)
    nblock = ntot//m
    y = np.array([np.average(x[i*m:(i+1)*m]) for i in range(nblock)])
    return y, np.average(y), np.std(y)*np.sqrt(nblock/(nblock-1))

def get_r(x,y,m):
    ntot = len(x)
    nblock = ntot//m
    return m*np.var(y)*(nblock/(nblock-1))/np.var(x)

def main():
    ntot = 2**20
    np.random.seed(12345)
    #x = np.random.normal(loc=300.0,size=ntot)
    x = np.random.uniform(low=-np.sqrt(3.0),high=np.sqrt(3.0),size=ntot)
    #print("x",x)
    print("ave(x),std(x)",np.average(x),np.std(x))

    dat = []
    for i in range(0,20):
        m = 2**i
        y, y_ave, y_err = blocking(x,m)
        #print("y",y)
        print("ave(y),std(y)",y_ave,y_err)
        print("1/sqrt(m)",1.0/np.sqrt(m))
        r = get_r(x,y,m)
        print("r",r)
        dat.append([m,r])
    dat = np.array(dat)

    fig = plt.figure()
    plt.xlim(1,ntot)
    plt.ylim(0,2)
    plt.xscale("log")
    plt.xlabel("$m$")
    plt.ylabel("$R$")
    plt.plot(dat[:,0],dat[:,1],marker="o",ls="none")
    fig.savefig("fig_r_m.pdf",bbox_inches="tight")
    plt.close()


if __name__ == "__main__":
    main()
