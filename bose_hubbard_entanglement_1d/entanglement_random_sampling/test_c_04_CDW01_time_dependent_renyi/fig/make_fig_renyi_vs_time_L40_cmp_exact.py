import numpy as np
import matplotlib
import matplotlib.pyplot as plt

def main():
    Ls = [40]
    data = []
    for L in Ls:
        data.append(np.loadtxt("../grep_renyi/dat_L"+"{}".format(L)))

    data_e = []
    for L in Ls:
        data_e.append(np.loadtxt("../grep_renyi/exact_cdw/dat_ee_L"+"{}".format(L)))


    #cmap = plt.get_cmap("tab10")
    cmap = plt.get_cmap("tab20")
    fig = plt.figure()
    plt.xlabel("time $tJ/L$")
    plt.ylabel("Renyi entanglement $S_2/L$")
    plt.xlim(0,2)
    plt.ylim(0,0.4)
    for i,L in enumerate(Ls):
        plt.errorbar(data[i][:,0]/data[i][:,9],data[i][:,1]/data[i][:,9],yerr=data[i][:,2]/data[i][:,9],
            capsize=3,fmt="o",mew=1,elinewidth=1,ls="none",ms=5,mfc="none",color=cmap(2*i),
            label=r"$L="+"{}".format(L)+"$, random sampling")
        plt.plot(data_e[i][:,0]/L,data_e[i][:,1]/L,color=cmap(2*i+1),
            label=r"$L="+"{}".format(L)+"$, exact")
    plt.legend(loc="lower right")
    fig.savefig("fig_renyi_vs_time_L40_cmp_exact.pdf",bbox_inches="tight")
    plt.close()

if __name__ == "__main__":
    main()
