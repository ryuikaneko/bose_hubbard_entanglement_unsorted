! https://gcc.gnu.org/onlinedocs/gfortran/SELECTED_005fINT_005fKIND.html
! https://qiita.com/implicit_none/items/ab0632357bab0f549cf2

program large_integers
  integer,parameter :: k5 = selected_int_kind(5)
  integer,parameter :: k14 = selected_int_kind(14)
  integer,parameter :: k15 = selected_int_kind(15)
  integer,parameter :: k16 = selected_int_kind(16)
  integer(kind=k5) :: i5
  integer(kind=k14) :: i14
  integer(kind=k15) :: i15
  integer(kind=k16) :: i16

  print *, huge(i5), huge(i14), huge(i15), huge(i16)

  ! The following inequalities are always true
  print *, huge(i5) >= 10_k5**5-1
  print *, huge(i14) >= 10_k14**14-1
  print *, huge(i15) >= 10_k15**15-1
  print *, huge(i16) >= 10_k16**16-1
end program large_integers
