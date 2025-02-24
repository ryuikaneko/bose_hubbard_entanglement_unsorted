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
    func = lambda x,a: np.sqrt(np.abs(a/x))
    popt, pcov = curve_fit(func,x,y,sigma=yerr)
    aave = np.abs(popt[0])
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
#    Ls = np.array([[4,4],[6,4],[6,6],[8,6],[8,8],[10,8],[10,10],[12,10],[12,12]])
#    skips = np.array([0,0,0,0,0,0,0,0,0])
    Ls = np.array([[4,4],[6,4],[6,6],[8,6],[8,8],[10,8],[10,10],[12,10]])
    skips = np.array([0,0,0,0,0,10,10,12])
    Ls = Ls[::-1]
    skips = skips[::-1]
    dat = []
    for L in Ls:
        dat.append(np.loadtxt("../dat"+"_Lx{}".format(L[0])+"_Ly{}".format(L[1])))
    dat = np.array(dat)
    #print(dat)

#----

    aas = []
    for i in range(Ls.shape[0]):
        Lx = Ls[i,0]
        Ly = Ls[i,1]
        Ns = Lx*Ly
        aave, aerr = fit_err_nsmp(2**dat[i,skips[i]:,2],dat[i,skips[i]:,3]/Ns,dat[i,skips[i]:,4]/Ns)
        aas.append([Lx,Ly,aave,aerr])
    aas = np.array(aas)
    #print(aas)
    np.savetxt("dat_coeff_2d",aas)

    cmap = plt.get_cmap("tab20")
    fig = plt.figure()
    plt.xlabel(r"$N_{\mathrm{total}}$")
    plt.ylabel(r"standard error of Renyi entanglement density $\sigma_{S_2/N_{\mathrm{s}}}$")
    plt.xscale("log")
    plt.yscale("log")
    #plt.xlim(1e3,1e10)
    plt.xlim(1e3,5e10)
    #plt.ylim(1e-6,2e-2)
    plt.ylim(5e-7,2e-2)
    plt.grid()
    for i in range(Ls.shape[0]):
        Lx = Ls[i,0]
        Ly = Ls[i,1]
        Ns = Lx*Ly
        plt.errorbar(2**dat[i,skips[i]:,2],dat[i,skips[i]:,3]/Ns,yerr=dat[i,skips[i]:,4]/Ns,
            capsize=4.5,fmt="o",mew=1.5,elinewidth=1.5,ls="none",ms=7.5,mfc="none",color=cmap(i),
            label=r"$N_{\mathrm{s}}="+"{}".format(Lx)+"\\times"+"{}".format(Ly)+"$")
    xx = np.linspace(1e3,5e10,33)
    for i in range(Ls.shape[0]):
        plt.plot(xx,np.sqrt(np.abs(aas[i,2]/xx)),color=cmap(i))
    i=0
    plt.plot(xx,np.sqrt(np.abs(aas[i,2]/xx)),color=cmap(i),label=r"$\sqrt{c(N_{\mathrm{s}})/N_{\mathrm{total}}}$")
    plt.legend(loc="lower left",fontsize=8)
    fig.savefig("fig_error_vs_nsmp_2d.pdf",bbox_inches="tight")
    plt.close()

#----

    mask = (aas[:,0]*aas[:,1]>=48) ## fit for Lx*Ly>=48
    slope, slopeerr, shift, shifterr = fit_exp(aas[mask,0]*aas[mask,1],aas[mask,2],aas[mask,3])
    np.savetxt("dat_slope_2d",np.array([slope,slopeerr,shift,shifterr]).reshape(1,4))

    cmap = plt.get_cmap("tab20")
    fig = plt.figure()
    plt.xlabel(r"$N_{\mathrm{s}}$")
    plt.ylabel(r"$c(N_{\mathrm{s}})$")
    plt.yscale("log")
    plt.xlim(10,150)
    plt.ylim(1e-2,1e3)
    plt.grid(which="both")
    plt.errorbar(aas[:,0]*aas[:,1],aas[:,2],yerr=aas[:,3],
        capsize=4.5,fmt="o",mew=1.5,elinewidth=1.5,ls="none",ms=7.5,mfc="none",color=cmap(0))
    xx = np.linspace(10,150,129)
    plt.plot(xx,2**(slope*xx+shift),label=r"$c(N_{\mathrm{s}})=2^{"+"{:+.4f}".format(slope)+"N_{\mathrm{s}}"+"{:+.4f}".format(shift)+"}$")
    plt.legend(loc="upper left",fontsize=16)
    fig.savefig("fig_coeff_2d.pdf",bbox_inches="tight")
    plt.close()

if __name__ == "__main__":
    main()
