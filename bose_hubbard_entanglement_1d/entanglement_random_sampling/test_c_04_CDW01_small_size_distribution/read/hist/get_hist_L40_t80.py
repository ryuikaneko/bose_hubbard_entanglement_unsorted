import numpy as np
import random
import matplotlib
import matplotlib.pyplot as plt
from struct import unpack

def read_file(filename):
    with open(filename,"rb") as f:
        dat = f.read()
    x = []
    for i in range(0,len(dat),16):
        re, im = unpack("<dd",dat[i:i+16])
        #print(re, im)
        x.append([re,im])
    return np.array(x)

def main():
    filename = "../../dat/dat_histtotal_binary_L40_t80.0000000000_e20_r42_s54"
    perm = read_file(filename)

    mask = (perm[:,0]>0)
    ratio = np.sum(mask)*1.0/len(perm[:,0])
    print(ratio)
    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 128
    counts1, bins1 = np.histogram(-np.log(perm[:,0][mask]),bins=bins,density=True)
    plt.hist(bins1[:-1],bins1,weights=counts1*ratio,alpha=0.25,color="r")
    plt.hist(bins1[:-1],bins1,weights=counts1*ratio,histtype=u"step",color="r",label=r"$x=-\log(+\mathrm{perm}A)$ (if $\mathrm{perm}A>0$)")
    counts2, bins2 = np.histogram(-np.log(-perm[:,0][~mask]),bins=bins,density=True)
    plt.hist(bins2[:-1],bins2,weights=counts2*(1.0-ratio),alpha=0.25,color="b")
    plt.hist(bins2[:-1],bins2,weights=counts2*(1.0-ratio),histtype=u"step",color="b",ls="--",label=r"$x=-\log(-\mathrm{perm}A)$ (if $\mathrm{perm}A<0$)")
    #plt.hist(-np.log(perm[:,0][mask]),bins=bins,density=True,alpha=0.5,color="r")
    #plt.hist(-np.log(-perm[:,0][~mask]),bins=bins,density=True,alpha=0.5,color="b")
    plt.legend(fontsize=8)
    fig.savefig("fig_hist_perm_real_t80.pdf",bbox_inches="tight")
    plt.close()

    mask = (perm[:,1]>0)
    ratio = np.sum(mask)*1.0/len(perm[:,0])
    print(ratio)
    fig = plt.figure()
    plt.xlabel("$x$")
    plt.ylabel("$P(x)$")
    bins = 128
    counts1, bins1 = np.histogram(-np.log(perm[:,1][mask]),bins=bins,density=True)
    plt.hist(bins1[:-1],bins1,weights=counts1*ratio,alpha=0.25,color="r")
    plt.hist(bins1[:-1],bins1,weights=counts1*ratio,histtype=u"step",color="r",label=r"$x=-\log(+\mathrm{perm}A)$ (if $\mathrm{perm}A>0$)")
    counts2, bins2 = np.histogram(-np.log(-perm[:,1][~mask]),bins=bins,density=True)
    plt.hist(bins2[:-1],bins2,weights=counts2*(1.0-ratio),alpha=0.25,color="b")
    plt.hist(bins2[:-1],bins2,weights=counts2*(1.0-ratio),histtype=u"step",color="b",ls="--",label=r"$x=-\log(-\mathrm{perm}A)$ (if $\mathrm{perm}A<0$)")
    #plt.hist(-np.log(perm[:,1][mask]),bins=bins,density=False,alpha=0.5,color="r")
    #plt.hist(-np.log(-perm[:,1][~mask]),bins=bins,density=False,alpha=0.5,color="b")
    plt.legend(fontsize=8)
    fig.savefig("fig_hist_perm_imag_t80.pdf",bbox_inches="tight")
    plt.close()


if __name__ == "__main__":
    main()
