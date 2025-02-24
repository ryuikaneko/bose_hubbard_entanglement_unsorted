# https://doi.org/10.1103/PhysRevLett.109.017202

from quspin.operators import hamiltonian # operators
#from quspin.basis import boson_basis_1d
from quspin.basis import boson_basis_general
import numpy as np # general math functions
import scipy.sparse.linalg
import argparse
from numba import jit

def parse_args():
    parser = argparse.ArgumentParser(description="Bose-Hubbard chain, dynamics")
    parser.add_argument("-Lx",metavar="Lx",dest="Lx",type=int,default=3,help="set Lx(=3, size)")
    parser.add_argument("-Ly",metavar="Ly",dest="Ly",type=int,default=4,help="set Ly(=4, size)")
    parser.add_argument("-Nb",metavar="Nb",dest="Nb",type=int,default=6,help="set Nb(=6, number of bosons)")
#    parser.add_argument("-Nsps",metavar="Nsps",dest="Nsps",type=int,default=13,help="set Nsps(=13, local Hilbert space)")
    parser.add_argument("-Nsps",metavar="Nsps",dest="Nsps",type=int,default=7,help="set Nsps(=7, local Hilbert space)")
    parser.add_argument("-J",metavar="J",dest="J",type=np.float,default=1.0,help="set J(=1.0, hopping -J)")
    parser.add_argument("-U",metavar="U",dest="U",type=np.float,default=0.0,help="set U(=0.0, intreaction U)")
#    parser.add_argument("-U",metavar="U",dest="U",type=np.float,default=1.0,help="set U(=1.0, intreaction U)")
    parser.add_argument("-W",metavar="W",dest="W",type=np.float,default=0.0,help="set W(=0.0)")
#    parser.add_argument("-W",metavar="W",dest="W",type=np.float,default=5.0,help="set W(=5.0)")
    parser.add_argument("-seed",metavar="seed",dest="seed",type=int,default=10000,help="set seed(=10000)")
#    parser.add_argument("-seed",metavar="seed",dest="seed",type=int,default=12345,help="set seed(=12345)")
#    parser.add_argument("-Tau",metavar="Tau",dest="Tau",type=np.float,default=40.0,help="set Tau(=40)")
#    parser.add_argument("-dt",metavar="dt",dest="dt",type=np.float,default=0.1,help="set dt")
#    parser.add_argument("-npoints",metavar="npoints",dest="npoints",type=int,default=201,help="set npoints")
    return parser.parse_args()

def prepare_vec_op(Lx,Ly,Nb,Nsps):
#def prepare_vec_op(L,Nb,Nsps):
    N_2d = Lx*Ly
    basis_2d = boson_basis_general(N_2d,Nb=Nb,sps=Nsps)
#    basis_1d = boson_basis_1d(L,Nb=Nb,sps=Nsps)
#    print(basis_1d)
#    print(basis_1d.Ns)
#
    # prepare Mott Ins state
#    U0s = [[1.0,i,i] for i in range(L)]
#    Usft0s = [[-1.0,i] for i in range(L)]
#    static0 = [["nn",U0s],["n",Usft0s]]
    # prepare CDW state
#    Usft0s = [[(-1.0)**i,i] for i in range(L)]
#    static0 = [["n",Usft0s]]
#
    no_checks = dict(check_symm=False,check_pcon=False,check_herm=False)
#    H0 = hamiltonian(static0,[],static_fmt="csr",basis=basis_1d,dtype=np.float64,**no_checks)
#    ene,vec = H0.eigsh(time=0.0,which="SA",k=1)
#    vec = vec[:,0]
#    print(ene)
#    print(vec)
#    print(np.linalg.norm(vec))
#
#    string = "1"*L
#    string = "20"*(L//2)
#    string = "1"*(L//2-1)+"0"+"2"+"1"*(L//2-1)
#    string = "10"*(L//2)
    string = "010101010101" ## for 3x4 lattice
#    print(string)
#    print(basis_1d)
    ind = basis_2d.index(string)
    vec = np.zeros(basis_2d.Ns,dtype=np.float64)
    vec[ind] = 1.0
#
    # operator for n, n^2
#    int_n = [[1.0,i] for i in range(L)]
#    static_n = [["n",int_n]]
#    op_n = hamiltonian(static_n,[],static_fmt="csr",basis=basis_2d,dtype=np.float64,**no_checks).tocsr(time=0)
#
#    int_nstag = [[-(-1)**i,i] for i in range(L)]
#    static_nstag = [["n",int_nstag]]
#    op_nstag = hamiltonian(static_nstag,[],static_fmt="csr",basis=basis_2d,dtype=np.float64,**no_checks).tocsr(time=0)
#
#    int_n2 = [[1.0,i,i] for i in range(L)]
#    static_n2 = [["nn",int_n2]]
#    op_n2 = hamiltonian(static_n2,[],static_fmt="csr",basis=basis_2d,dtype=np.float64,**no_checks).tocsr(time=0)
    # operator for correlations
#    int_aa = []
#    for j in range(L//2):
#        int_aa += [[[1.0,i,(i+j+1)%L] for i in range(L)]]
##    print(int_aa)
#    op_aa = []
#    for j in range(L//2):
#        op_aa += [hamiltonian([["+-",int_aa[j]],["-+",int_aa[j]]],[],static_fmt="csr",basis=basis_2d,dtype=np.float64,**no_checks).tocsr(time=0)]
##    print(op_aa)
#    int_nn = []
#    for j in range(L//2):
#        int_nn += [[[1.0,i,(i+j+1)%L] for i in range(L)]]
##    print(int_nn)
#    op_nn = []
#    for j in range(L//2):
#        op_nn += [hamiltonian([["nn",int_nn[j]]],[],static_fmt="csr",basis=basis_2d,dtype=np.float64,**no_checks).tocsr(time=0)]
##    print(op_nn)
    return basis_2d, vec
#    return basis_2d, vec, op_n, op_nstag, op_n2, op_aa, op_nn

def prepare_H(basis_2d,Lx,Ly,J,U,W,seed=12345):
#def prepare_H(basis_2d,L,J,U,W,seed=12345):
    N_2d = Lx*Ly
    # build hamiltonian
##    Js = [[-J,i,(i+1)%L] for i in range(L)]
#    Js = [[-J,i,(i+1)%L] for i in range(L-1)]
#    Us = [[0.5*U,i,i] for i in range(L)]
#    Usfts = [[-0.5*U,i] for i in range(L)]
    Js = [[-J,x+Lx*y,(x+1)+Lx*y] for y in range(Ly) for x in range(Lx) if x+1<Lx] \
       + [[-J,x+Lx*y,x+Lx*(y+1)] for y in range(Ly) for x in range(Lx) if y+1<Ly]
    Us = [[0.5*U,i,i] for i in range(N_2d)]
    Usfts = [[-0.5*U,i] for i in range(N_2d)]
## https://numpy.org/doc/stable/reference/random/index.html#random-quick-start
    rng = np.random.default_rng(seed)
    rnd = rng.random(N_2d)
#    print(rnd)
    mus = [[W*(2.0*rnd[i]-1.0),i] for i in range(N_2d)]
    static = [["+-",Js],["-+",Js],["nn",Us],["n",Usfts],["n",mus]]
    no_checks = dict(check_symm=False,check_pcon=False,check_herm=False)
#    H = hamiltonian(static,[],static_fmt="csr",basis=basis_2d,dtype=np.float64)
    H = hamiltonian(static,[],static_fmt="csr",basis=basis_2d,dtype=np.float64,**no_checks)
    H = H.tocsr(time=0)
    return H

def apply_expm(basis_2d,list_int_half,N_2d,Nb,Nsps,dt,H,vec):
#def apply_expm(basis_2d,list_int_half,N_2d,Nb,Nsps,dt,H,vec,op_n,op_nstag,op_n2,op_aa,op_nn):
    vec2 = (scipy.sparse.linalg.expm_multiply((-1j)*dt*H,vec,start=0.0,stop=1.0,num=2,endpoint=True))[1]
    norm = np.linalg.norm(vec2)
    norm2 = norm**2
    ene = (np.conjugate(vec2).dot(H.dot(vec2)) / norm2).real / N_2d
#    n = (np.conjugate(vec2).dot(op_n.dot(vec2)) / norm2).real / N_2d
#    nstag = (np.conjugate(vec2).dot(op_nstag.dot(vec2)) / norm2).real / N_2d
#    n2 = (np.conjugate(vec2).dot(op_n2.dot(vec2)) / norm2).real / N_2d
#    aa = np.array([(np.conjugate(vec2).dot(op_aa[j].dot(vec2)) / norm2).real / N_2d for j in range(N_2d//2)])
#    nn = np.array([(np.conjugate(vec2).dot(op_nn[j].dot(vec2)) / norm2).real / N_2d for j in range(N_2d//2)])
    # entanglement
#    s1 = basis_2d.ent_entropy(vec2/norm,alpha=1.0)["Sent_A"]
#    s2 = basis_2d.ent_entropy(vec2/norm,alpha=2.0)["Sent_A"]
#    return vec2, norm2, ene, n, nstag, n2, aa, nn, s1, s2
    psi = calc_psi4ee(vec2/norm,N_2d,Nb,Nsps,basis_2d._basis,list_int_half)
    s1, s2 = calc_ee(psi)
    return vec2, norm2, ene, s1/(N_2d//2), s2/(N_2d//2)
#    return vec2, norm2, ene, n, nstag, n2, aa, nn, s1/(N_2d//2), s2/(N_2d//2)

## list_1: descending order
@jit(nopython=True)
def find_state(state,list_1,maxind):
    imin = 0
    imax = maxind-1
    i = imin
    while True:
        i = (imin+imax)//2
#        print("#",i,imin,imax,maxind,state,list_1[i])
#        if (state < list_1[i]):
        if (state > list_1[i]):
            imax = i-1
#        elif (state > list_1[i]):
        elif (state < list_1[i]):
            imin = i+1
        else:
            break
        if (imin > imax):
            return -1
    return i

@jit(nopython=True)
def calc_psi4ee(vec,N_2d,Nb,Nsps,basis_2d_full_basis,list_int_half):
    lenpsi = len(list_int_half)
#    print(lenpsi)
    psi = np.zeros((lenpsi,lenpsi),dtype=np.complex128)
    dshift = Nsps**(N_2d//2)
    for vecind,num in enumerate(basis_2d_full_basis):
        r = num%dshift
        l = num//dshift
#        i0 = np.where(list_int_half==r)[0]
        i = find_state(r,list_int_half,lenpsi)
#        j0 = np.where(list_int_half==l)[0]
        j = find_state(l,list_int_half,lenpsi)
        psi[i,j] = vec[vecind]
#        print(vecind,num,i0,i,j0,j,vec[vecind])
    return psi

def calc_ee(psi):
    u, s, v = scipy.linalg.svd(psi)
    s[s<1e-16] = np.finfo(s.dtype).eps
    ee1 = np.sum(-s*s*np.log(s*s))
    ee2 = -np.log(np.sum(s**4))
    return ee1, ee2


def main():
    args = parse_args()
    Lx = args.Lx
    Ly = args.Ly
    Nb = args.Nb
    Nsps = args.Nsps
    J = args.J
    U = args.U
    W = args.W
    seed = args.seed
#    Tau = args.Tau
#    dt = args.dt
#    npoints = args.npoints
#    npoints = int(Tau/dt + 1e-6) + 1
#    Times = np.linspace(0.0,Tau,npoints)
#
#    t = np.linspace(0,0.95,20)
#    t = np.hstack((t,np.linspace(1,9.5,18)))
#    t = np.hstack((t,np.linspace(10,95,18)))
#    t = np.hstack((t,np.linspace(100,950,18)))
#    t = np.hstack((t,np.linspace(1000,9500,18)))
#    t = np.hstack((t,np.linspace(10000,95000,18)))
#    t = np.hstack((t,np.linspace(100000,950000,18)))
#    t = np.hstack((t,np.linspace(1000000,9500000,18)))
#    t = np.hstack((t,np.linspace(10000000,95000000,18)))
#    t = np.hstack((t,np.linspace(100000000,950000000,18)))
#    t = np.hstack((t,np.linspace(1000000000,1000000000,1)))
#    print(t)
#
## 0.05*(10^(2/10))^52 ~ 1.256*10^9
## 0.05*(10^(2/10))^27 ~ 1.256*10^4
## 0.05*(10^(1/10))^54 ~ 1.256*10^4
#    t = np.array([0]+[0.05*(10.0**0.2)**i for i in range(53)])
#    t = np.array([0]+[0.05*(10.0**0.2)**i for i in range(28)])
#    t = np.array([0]+[0.05*(10.0**0.1)**i for i in range(55)])
    t = np.linspace(0,80,801)
    Times = t
    npoints = len(t)
    N_2d = Lx*Ly
#
    basis_2d, vec = prepare_vec_op(Lx,Ly,Nb,Nsps)
#    basis_2d, vec, op_n, op_nstag, op_n2, op_aa, op_nn = prepare_vec_op(Lx,Ly,Nb,Nsps)
    H = prepare_H(basis_2d,Lx,Ly,J,U,W,seed)
#    print(H)
#
    basis_2d_half = [boson_basis_general(N_2d//2,Nb=i,sps=Nsps) for i in range(Nb+1)]
    list_int_half = np.sort(np.array([x for i in range(Nb+1) for x in basis_2d_half[i]._basis]))[::-1] ## descending order
#
    Time = 0.0
    dt = 0.0
    vec, norm2, ene, s1, s2 = apply_expm(basis_2d,list_int_half,N_2d,Nb,Nsps,dt,H,vec)
#    vec, norm2, ene, n, nstag, n2, aa, nn, s1, s2 = apply_expm(basis_2d,list_int_half,N_2d,Nb,Nsps,dt,H,vec,op_n,op_nstag,op_n2,op_aa,op_nn)
    print(Time,J,N_2d,ene,norm2,s1,s2)
#    print("#nn",Time,J,L,*nn.flatten())
#    print("#nn-n2",Time,J,L,*(nn-n**2).flatten())
#    print("#aa",Time,J,L,*aa.flatten())
#
    for i in range(1,npoints):
        Time = Times[i]
        dt = Times[i]-Times[i-1]
        vec, norm2, ene, s1, s2 = apply_expm(basis_2d,list_int_half,N_2d,Nb,Nsps,dt,H,vec)
#        vec, norm2, ene, n, nstag, n2, aa, nn, s1, s2 = apply_expm(basis_2d,list_int_half,L,Nb,Nsps,dt,H,vec,op_n,op_nstag,op_n2,op_aa,op_nn)
        print(Time,J,N_2d,ene,norm2,s1,s2)
#        print("#nn",Time,J,L,*nn.flatten())
#        print("#nn-n2",Time,J,L,*(nn-n**2).flatten())
#        print("#aa",Time,J,L,*aa.flatten())

if __name__ == "__main__":
    main()
