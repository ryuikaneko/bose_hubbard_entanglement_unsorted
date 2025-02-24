! https://gcc.gnu.org/onlinedocs/gfortran/TRANSFER.html
! https://stackoverflow.com/questions/15057429/convert-logical-type-to-double-in-fortran
! https://stackoverflow.com/questions/39454349/numerical-equivalent-of-true-is-1
! https://stackoverflow.com/questions/37293641/implicit-conversion-integer-logical-in-fortran-if-statement

! cast logic --> integer by "transfer"
! probably, safely works only for gfortran

program main
  implicit none
  integer :: a, b
  a = 100
  b = 200
  write(*,*) a
  write(*,*) b
  write(*,*) transfer(a>b,1) - transfer(a<b,1)
  a = 100
  b = 100
  write(*,*) a
  write(*,*) b
  write(*,*) transfer(a>b,1) - transfer(a<b,1)
  a = 200
  b = 100
  write(*,*) a
  write(*,*) b
  write(*,*) transfer(a>b,1) - transfer(a<b,1)
end program main
