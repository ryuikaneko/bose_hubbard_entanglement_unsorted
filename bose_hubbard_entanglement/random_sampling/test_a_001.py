import numpy as np
import matplotlib.pyplot as plt
from numba import jit
import time

@jit(nopython=True)
def calc_rsks(L):
    rs = np.arange(1,L+1) ## from 1 to L
    ks = np.linspace(np.pi/(L+1.0),L*np.pi/(L+1.0),L)
    return rs,ks

@jit(nopython=True)
def calc_eks(J,ks):
    return -2.0*J*np.cos(ks)

@jit(nopython=True)
def calc_xkrs(L,ks,rs):
    return np.sqrt(2.0/(L+1.0))*np.sin(np.outer(ks,rs))

@jit(nopython=True)
def calc_ys(t,eks,xkrs):
    xc = (1.0+0.0j)*xkrs # for numba
    expeks = np.diag(np.exp(1j*t*eks))
    return xc @ expeks @ xc

# @jit(nopython=True)
# def calc_fs(ys,system):
#     diag = (1.0+0.0j)*np.diag(system)
#     return ys @ diag @ ys.T.conjugate()

@jit(nopython=True)
def calc_fs_cdw(ys,system,period=1):
    diag = (1.0+0.0j)*np.diag(system)
    ycuts = np.copy(ys[::period,:]) ## avoid NumbaPerformanceWarning to make contiguous arrays by deep copy
#    ycuts = ys[::period,:]
    return ycuts @ diag @ ycuts.T.conjugate()

@jit(nopython=True)
def calc_z(L,system,t,eks,xkrs,period=1):
    ys = calc_ys(t,eks,xkrs)
#    fs = calc_fs(ys,system)
    fs = calc_fs_cdw(ys,system,period)
    dlt = np.eye(fs.shape[0],dtype=np.complex128)
    return np.vstack( (np.hstack((fs,dlt-fs)), np.hstack((dlt-fs,fs))) )

@jit(nopython=True)
def get_dat(L,system,J,ts,period=1):
    rs, ks = calc_rsks(L)
    eks = calc_eks(J,ks)
    xkrs = calc_xkrs(L,ks,rs)
    data = np.zeros((len(ts),2),dtype=np.float64)
    for i,t in enumerate(ts):
        matz = calc_z(L,system,t,eks,xkrs,period)
        data[i] = np.array([t,-np.log(perm_bbfg(matz).real)])
    return data

@jit(nopython=True)
def get_dat_2(L,system,J,ts,Nsmp,rnd,period=1):
    rs, ks = calc_rsks(L)
    eks = calc_eks(J,ks)
    xkrs = calc_xkrs(L,ks,rs)
    data = np.zeros((len(ts),2),dtype=np.float64)
    #
    #seed = 12345
    #rng = np.random.default_rng(seed)
    #Nsmp = 2**10
    data2 = np.zeros((len(ts),2),dtype=np.float64)
    data2err1 = np.zeros((len(ts),2),dtype=np.float64)
    data2err2 = np.zeros((len(ts),2),dtype=np.float64)
    #
    for i,t in enumerate(ts):
        matz = calc_z(L,system,t,eks,xkrs,period)
        #data[i] = np.array([t,-np.log(perm_bbfg(matz).real)])
        #
        lenmatz = matz.shape[0]
        bounds = np.zeros(Nsmp,dtype=np.float64)
        for step in range(Nsmp):
            #rnd = np.pi * rng.uniform(-1,1,lenmatz)
            vec = np.cos(rnd[step]) + 1j * np.sin(rnd[step])
            #bounds[step] = (np.sum(np.abs(matz.dot(vec)))/lenmatz)**lenmatz
            #bounds[step] = np.prod(np.abs(matz.dot(vec)))
            bounds[step] = np.prod(vec.conjugate()*matz.dot(vec)).real
        #data2[i] = np.array([t,-np.log(np.average(bounds))])
        #data2[i] = np.array([t,-np.log(np.sum(bounds)/len(bounds))])
        ave = np.sum(bounds)/len(bounds)
        var = np.sum((bounds-ave)**2)/len(bounds)
        err = np.sqrt(var/len(bounds))
        data2[i] = np.array([t,-np.log(ave)])
        data2err1[i] = np.array([t,-np.log(ave+err)])
        data2err2[i] = np.array([t,-np.log(np.abs(ave-err))])
        #
    return data, data2, data2err1, data2err2

@jit(nopython=True)
def calc_renyi(period,J,ts,Ls,rnd,Nsmp):
    datas = np.zeros((len(Ls),len(ts),2),dtype=np.float64)
    datas2 = np.zeros((len(Ls),len(ts),2),dtype=np.float64)
    datas2err1 = np.zeros((len(Ls),len(ts),2),dtype=np.float64)
    datas2err2 = np.zeros((len(Ls),len(ts),2),dtype=np.float64)
    for i,L in enumerate(Ls):
        #start = time.time()
        #
        #system = np.array([1.0 if i<L//2 else 0.0 for i in range(L)])
        system = np.zeros(L,dtype=np.float64)
        for j in range(L//2):
            system[j] = 1.0
        #datas[i] = get_dat(L,system,J,ts,period)
        datas[i], datas2[i], datas2err1[i], datas2err2[i] = get_dat_2(L,system,J,ts,Nsmp,rnd,period)
        #end = time.time()
        #print("L,calctime:",L,end-start)
    return datas, datas2, datas2err1, datas2err2


def main():
    ## CDW 010101... initial
    period = 2
    J = 1.0
#    Ls = [16]
#    Ls = [64]
    Ls = [72]
    Ls = np.array(Ls)
    ts = np.linspace(0,2*Ls[0],64+1)
    seed = 12345
    Nsmp = 2**(Ls[0]//4+4)
    #
    rng = np.random.default_rng(seed)
    rnd = np.zeros((Nsmp,Ls[0]),dtype=np.float64)
    for step in range(Nsmp):
        rnd[step] = np.pi * rng.uniform(-1,1,Ls[0])
    #
    start = time.time()
    datas, datas2, datas2err1, datas2err2 = calc_renyi(period,J,ts,Ls,rnd,Nsmp)
    end = time.time()
    print("L,calctime:",Ls[0],end-start)
    np.savetxt("dat_L"+"{}".format(Ls[0])+"_calctime",[end-start])
    np.savetxt("dat_L"+"{}".format(Ls[0])+"_ave",datas2[0])
    np.savetxt("dat_L"+"{}".format(Ls[0])+"_err1",datas2err1[0])
    np.savetxt("dat_L"+"{}".format(Ls[0])+"_err2",datas2err2[0])


if __name__ == "__main__":
    main()
