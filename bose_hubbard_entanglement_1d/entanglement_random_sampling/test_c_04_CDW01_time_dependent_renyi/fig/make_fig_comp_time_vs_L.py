import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def fit_exp(x,y,yerr):
    func = lambda x,a,b: 2**(a*x+b)
    popt, pcov = curve_fit(func,x,y,sigma=yerr)
    slope = popt[0]
    slopeerr = pcov[0,0]**0.5
    shift = popt[1]
    shifterr = pcov[1,1]**0.5
    return slope, slopeerr, shift, shifterr

def main():
    data = np.loadtxt("../grep_time/dat_time")

    slope, slopeerr, shift, shifterr = fit_exp(data[:,0],data[:,1],data[:,2])

    cmap = plt.get_cmap("tab10")
    fig = plt.figure()
    plt.xlabel("size $L$")
    plt.ylabel("computation time [sec]")
    plt.xlim(0,120)
    plt.ylim(1e-2,1e6)
    plt.yscale("log")
    plt.grid(which="both")
    plt.errorbar(data[:,0],data[:,1],yerr=data[:,2],
            capsize=4.5,fmt="o",mew=1.5,elinewidth=1.5,ls="none",ms=7.5,mfc="none",color=cmap(0))
    oneday = 60*60*24
    plt.plot([0,120],[oneday,oneday],ls="--",label=r"one day",color=cmap(1))
    xx = np.linspace(0,120,33)
    plt.plot(xx,2**(slope*xx+shift),label=r"$\mathrm{time}=2^{"+"{:+.4f}".format(slope)+"L"+"{:+.4f}".format(shift)+"}$",color=cmap(0))
    plt.legend(loc="lower right")
    fig.savefig("fig_comp_time_vs_L.pdf",bbox_inches="tight")
    plt.close()

if __name__ == "__main__":
    main()
