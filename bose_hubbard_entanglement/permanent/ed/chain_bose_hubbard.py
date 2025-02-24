# https://doi.org/10.1103/PhysRevLett.109.017202

from quspin.operators import hamiltonian # operators
from quspin.basis import boson_basis_1d
import numpy as np # general math functions
import scipy.sparse.linalg
import argparse
from numba import jit

def parse_args():
    parser = argparse.ArgumentParser(description="Bose-Hubbard chain, dynamics")
    parser.add_argument("-L",metavar="L",dest="L",type=int,default=8,help="set L(=8, size)")
    parser.add_argument("-Nb",metavar="Nb",dest="Nb",type=int,default=8,help="set Nb(=8, number of bosons)")
#    parser.add_argument("-Nsps",metavar="Nsps",dest="Nsps",type=int,default=3,help="set Nsps(=3, local Hilbert space)")
    parser.add_argument("-J",metavar="J",dest="J",type=np.float,default=1.0,help="set J(=1.0, hopping -J)")
    parser.add_argument("-U",metavar="U",dest="U",type=np.float,default=0.0,help="set U(=0.0, intreaction U)")
    return parser.parse_args()

def prepare_vec_op(L,Nb,Nsps):
    basis_1d = boson_basis_1d(L,Nb=Nb,sps=Nsps)
#    print(basis_1d)
#    print(basis_1d.Ns)
#
    # prepare Mott Ins state
    U0s = [[1.0,i,i] for i in range(L)]
    Usft0s = [[-1.0,i] for i in range(L)]
    static0 = [["nn",U0s],["n",Usft0s]]
    # prepare CDW state
#    Usft0s = [[(-1.0)**i,i] for i in range(L)]
#    static0 = [["n",Usft0s]]
#
    no_checks = dict(check_symm=False,check_pcon=False,check_herm=False)
    H0 = hamiltonian(static0,[],static_fmt="csr",basis=basis_1d,dtype=np.float64,**no_checks)
    ene,vec = H0.eigsh(time=0.0,which="SA",k=1)
    vec = vec[:,0]
#    print(ene)
#    print(vec)
#    print(np.linalg.norm(vec))
    # operator for n, n^2
    int_n = [[1.0,i] for i in range(L)]
    static_n = [["n",int_n]]
    op_n = hamiltonian(static_n,[],static_fmt="csr",basis=basis_1d,dtype=np.float64,**no_checks).tocsr(time=0)
#
    int_nstag = [[-(-1)**i,i] for i in range(L)]
    static_nstag = [["n",int_nstag]]
    op_nstag = hamiltonian(static_nstag,[],static_fmt="csr",basis=basis_1d,dtype=np.float64,**no_checks).tocsr(time=0)
#
    int_n2 = [[1.0,i,i] for i in range(L)]
    static_n2 = [["nn",int_n2]]
    op_n2 = hamiltonian(static_n2,[],static_fmt="csr",basis=basis_1d,dtype=np.float64,**no_checks).tocsr(time=0)
    # operator for correlations
    int_aa = []
    for j in range(L//2):
        int_aa += [[[1.0,i,(i+j+1)%L] for i in range(L)]]
#    print(int_aa)
    op_aa = []
    for j in range(L//2):
        op_aa += [hamiltonian([["+-",int_aa[j]],["-+",int_aa[j]]],[],static_fmt="csr",basis=basis_1d,dtype=np.float64,**no_checks).tocsr(time=0)]
#    print(op_aa)
    int_nn = []
    for j in range(L//2):
        int_nn += [[[1.0,i,(i+j+1)%L] for i in range(L)]]
#    print(int_nn)
    op_nn = []
    for j in range(L//2):
        op_nn += [hamiltonian([["nn",int_nn[j]]],[],static_fmt="csr",basis=basis_1d,dtype=np.float64,**no_checks).tocsr(time=0)]
#    print(op_nn)
    return basis_1d, vec, op_n, op_nstag, op_n2, op_aa, op_nn

def prepare_H(basis_1d,L,J,U):
    # build hamiltonian
#    Js = [[-J,i,(i+1)%L] for i in range(L)] ## periodic BC
    Js = [[-J,i,(i+1)%L] for i in range(L-1)] ## open BC
    Us = [[0.5*U,i,i] for i in range(L)]
    Usfts = [[-0.5*U,i] for i in range(L)]
    static = [["+-",Js],["-+",Js],["nn",Us],["n",Usfts]]
    no_checks = dict(check_symm=False,check_pcon=False,check_herm=False)
#    H = hamiltonian(static,[],static_fmt="csr",basis=basis_1d,dtype=np.float64)
    H = hamiltonian(static,[],static_fmt="csr",basis=basis_1d,dtype=np.float64,**no_checks)
    H = H.tocsr(time=0)
    return H

#def apply_expm(basis_1d,L,dt,H,vec,op_n,op_nstag,op_n2,op_aa,op_nn):
def apply_expm(basis_1d,list_int_half,L,Nb,Nsps,dt,H,vec,op_n,op_nstag,op_n2,op_aa,op_nn):
    vec2 = (scipy.sparse.linalg.expm_multiply((-1j)*dt*H,vec,start=0.0,stop=1.0,num=2,endpoint=True))[1]
    norm = np.linalg.norm(vec2)
    norm2 = norm**2
    ene = (np.conjugate(vec2).dot(H.dot(vec2)) / norm2).real / L
    n = (np.conjugate(vec2).dot(op_n.dot(vec2)) / norm2).real / L
    nstag = (np.conjugate(vec2).dot(op_nstag.dot(vec2)) / norm2).real / L
    n2 = (np.conjugate(vec2).dot(op_n2.dot(vec2)) / norm2).real / L
    aa = np.array([(np.conjugate(vec2).dot(op_aa[j].dot(vec2)) / norm2).real / L for j in range(L//2)])
    nn = np.array([(np.conjugate(vec2).dot(op_nn[j].dot(vec2)) / norm2).real / L for j in range(L//2)])
    # entanglement
#    s1 = basis_1d.ent_entropy(vec2/norm,alpha=1.0)["Sent_A"]
#    s2 = basis_1d.ent_entropy(vec2/norm,alpha=2.0)["Sent_A"]
#    return vec2, norm2, ene, n, nstag, n2, aa, nn, s1, s2
    psi = calc_psi4ee(vec2/norm,L,Nb,Nsps,basis_1d._basis,list_int_half)
    s1, s2 = calc_ee(psi)
    return vec2, norm2, ene, n, nstag, n2, aa, nn, s1/(L//2), s2/(L//2)

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
def calc_psi4ee(vec,L,Nb,Nsps,basis_1d_full_basis,list_int_half):
    lenpsi = len(list_int_half)
#    print(lenpsi)
    psi = np.zeros((lenpsi,lenpsi),dtype=np.complex128)
    dshift = Nsps**(L//2)
    for vecind,num in enumerate(basis_1d_full_basis):
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
    L = args.L
    Nb = args.Nb
#    Nsps = args.Nsps
    Nsps = Nb+1
    J = args.J
    U = args.U
    times = np.linspace(0,20,201)
    npoints = len(times)
#
    basis_1d, vec, op_n, op_nstag, op_n2, op_aa, op_nn = prepare_vec_op(L,Nb,Nsps)
    H = prepare_H(basis_1d,L,J,U)
#    print(H)
#
    basis_1d_half = [boson_basis_1d(L//2,Nb=i,sps=Nsps) for i in range(Nb+1)]
    list_int_half = np.sort(np.array([x for i in range(Nb+1) for x in basis_1d_half[i]._basis]))[::-1] ## descending order
#
    time = 0.0
    dt = 0.0
#    vec, norm2, ene, n, nstag, n2, aa, nn, s1, s2 = apply_expm(basis_1d,L,dt,H,vec,op_n,op_nstag,op_n2,op_aa,op_nn)
    vec, norm2, ene, n, nstag, n2, aa, nn, s1, s2 = apply_expm(basis_1d,list_int_half,L,Nb,Nsps,dt,H,vec,op_n,op_nstag,op_n2,op_aa,op_nn)
    print("#s",time,J,L,ene,norm2,nstag,n,s1,s2)
    print("#nn",time,J,L,*nn.flatten())
    print("#nn-n2",time,J,L,*(nn-n**2).flatten())
    print("#aa",time,J,L,*aa.flatten())
#
    for i in range(1,npoints):
        time = times[i]
        dt = times[i]-times[i-1]
#        vec, norm2, ene, n, nstag, n2, aa, nn, s1, s2 = apply_expm(basis_1d,L,dt,H,vec,op_n,op_nstag,op_n2,op_aa,op_nn)
        vec, norm2, ene, n, nstag, n2, aa, nn, s1, s2 = apply_expm(basis_1d,list_int_half,L,Nb,Nsps,dt,H,vec,op_n,op_nstag,op_n2,op_aa,op_nn)
        print("#s",time,J,L,ene,norm2,nstag,n,s1,s2)
        print("#nn",time,J,L,*nn.flatten())
        print("#nn-n2",time,J,L,*(nn-n**2).flatten())
        print("#aa",time,J,L,*aa.flatten())

if __name__ == "__main__":
    main()
