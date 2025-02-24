! https://gcc.gnu.org/onlinedocs/gfortran/SELECTED_005fINT_005fKIND.html
! https://qiita.com/implicit_none/items/ab0632357bab0f549cf2

program large
  implicit none

  integer,parameter :: ip = selected_int_kind(r=18)  ! integer precision
  integer,parameter :: dp = selected_real_kind(p=15) ! double precision
  integer(ip) :: i
  real(dp) :: x

  print *, huge(i), huge(x)
end program large
