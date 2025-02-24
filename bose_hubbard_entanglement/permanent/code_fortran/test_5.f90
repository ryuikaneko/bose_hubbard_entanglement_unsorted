module precision
  implicit none
  integer,parameter :: ip = selected_int_kind(r=18)  ! integer precision
  integer,parameter :: dp = selected_real_kind(p=15) ! double precision
end module precision

program main
  use precision, only : ip, dp
  implicit none
  integer(ip) :: i
  real(dp) :: x
  i = 2_ip ** 62_ip + 2_ip ** 62_ip - 1_ip
!  i = 2_ip ** 62_ip + 2_ip ** 62_ip
  x = 0.0_dp
  write(*,*) i
  write(*,*) huge(i)
  write(*,*) x
end program main
