import numpy as np
import matplotlib
import matplotlib.pyplot as plt

def main():
    Ls = [20,40,60,80,100]
    data = []
    for L in Ls:
        data.append(np.loadtxt("../grep_renyi/dat_L"+"{}".format(L)))

    cmap = plt.get_cmap("tab10")
    fig = plt.figure()
    plt.xlabel("time $tJ/L$")
    plt.ylabel("Renyi entanglement $S_2/L$")
    plt.xlim(0,2)
    plt.ylim(0,0.4)
    for i,L in enumerate(Ls):
        plt.errorbar(data[i][:,0]/data[i][:,9],data[i][:,1]/data[i][:,9],yerr=data[i][:,2]/data[i][:,9],
            capsize=3,fmt="o",mew=1,elinewidth=1,ls="none",ms=5,mfc="none",color=cmap(i),
            label=r"$L="+"{}".format(L)+"$")
    plt.legend(loc="lower right")
    fig.savefig("fig_renyi_vs_time_L100.pdf",bbox_inches="tight")
    plt.close()

if __name__ == "__main__":
    main()
