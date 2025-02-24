import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def fit_linear(x,y,yerr):
    func = lambda x,a,b: a*x+b
    popt, pcov = curve_fit(func,x,y,sigma=yerr)
    slope = popt[0]
    slopeerr = pcov[0,0]**0.5
    shift = popt[1]
    shifterr = pcov[1,1]**0.5
    return slope, slopeerr, shift, shifterr

def fit_err_nsmp(x,y,yerr):
    func = lambda x,a: a*x**(-0.5)
    popt, pcov = curve_fit(func,x,y,sigma=yerr)
    aave = popt[0]
    aerr = pcov[0,0]**0.5
    return aave, aerr

def fit_exp(x,y,yerr):
    func = lambda x,a,b: 2**(a*x+b)
    popt, pcov = curve_fit(func,x,y,sigma=yerr)
    slope = popt[0]
    slopeerr = pcov[0,0]**0.5
    shift = popt[1]
    shifterr = pcov[1,1]**0.5
    return slope, slopeerr, shift, shifterr


def main():
    Ls = np.arange(16,64,4)
    skips = np.array([0,0,0,0,0,0,1,3,4,5,6,7])
    Ls = Ls[::-1]
    skips = skips[::-1]
    dat = []
    for L in Ls:
        dat.append(np.loadtxt("../dat_L"+"{}".format(L)))
    dat = np.array(dat)
    #print(dat)

#----

    aas = []
    for i,L in enumerate(Ls):
        aave, aerr = fit_err_nsmp(2**dat[i,skips[i]:,1],dat[i,skips[i]:,2]/dat[i,skips[i]:,0],dat[i,skips[i]:,3]/dat[i,skips[i]:,0])
        aas.append([L,aave,aerr])
    aas = np.array(aas)
    #print(aas)
    np.savetxt("dat_coeff",aas)

    cmap = plt.get_cmap("tab20")
    fig = plt.figure()
    plt.xlabel(r"$N_{\mathrm{total}}$")
    plt.ylabel(r"standard error of Renyi entanglement density $\sigma_{S_2/L}$")
    plt.xscale("log")
    plt.yscale("log")
    plt.xlim(1e3,1e10)
    plt.ylim(1e-6,2e-2)
    plt.grid()
    for i,L in enumerate(Ls):
        plt.errorbar(2**dat[i,skips[i]:,1],dat[i,skips[i]:,2]/dat[i,skips[i]:,0],yerr=dat[i,skips[i]:,3]/dat[i,skips[i]:,0],
            capsize=4.5,fmt="o",mew=1.5,elinewidth=1.5,ls="none",ms=7.5,mfc="none",color=cmap(i),
            label=r"$L="+"{}".format(L)+"$")
    xx = np.linspace(1e3,1e10,33)
    for i,L in enumerate(Ls):
        plt.plot(xx,aas[i,1]*xx**(-0.5),color=cmap(i))
    i=0
    plt.plot(xx,aas[i,1]*xx**(-0.5),color=cmap(i),label=r"$c(L)/\sqrt{N_{\mathrm{total}}}$")
    plt.legend(loc="lower left",fontsize=8)
    fig.savefig("fig_error_vs_nsmp.pdf",bbox_inches="tight")
    plt.close()

#----

    mask = (aas[:,0]>=40) ## fit for L>=40
    slope, slopeerr, shift, shifterr = fit_exp(aas[mask,0],aas[mask,1],aas[mask,2])
    #slope, slopeerr, shift, shifterr = fit_exp(aas[:,0],aas[:,1],aas[:,2])
    #slope, slopeerr, shift, shifterr = fit_linear(aas[:,0],np.log(aas[:,1])/np.log(2.0),np.abs(np.log(aas[:,1]+aas[:,2])/np.log(2.0)-np.log(aas[:,1])/np.log(2.0)))
    #slope, slopeerr, shift, shifterr = fit_linear(aas[:,0],np.log(aas[:,1])/np.log(2.0),1e-12*aas[:,0])
    np.savetxt("dat_slope",np.array([slope,slopeerr,shift,shifterr]).reshape(1,4))

    cmap = plt.get_cmap("tab20")
    fig = plt.figure()
    plt.xlabel(r"$L$")
    plt.ylabel(r"$c(L)$")
    plt.yscale("log")
    plt.xlim(10,70)
    plt.ylim(0.1,10)
    plt.grid(which="both")
    plt.errorbar(aas[:,0],aas[:,1],yerr=aas[:,2],
        capsize=4.5,fmt="o",mew=1.5,elinewidth=1.5,ls="none",ms=7.5,mfc="none",color=cmap(0))
    xx = np.linspace(10,70,129)
    plt.plot(xx,2**(slope*xx+shift),label=r"$c(L)=2^{"+"{:+.4f}".format(slope)+"L"+"{:+.4f}".format(shift)+"}$")
    plt.legend(loc="upper left",fontsize=16)
    fig.savefig("fig_coeff.pdf",bbox_inches="tight")
    plt.close()

if __name__ == "__main__":
    main()
