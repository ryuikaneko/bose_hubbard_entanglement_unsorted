module permanent
  implicit none
  integer,parameter :: ip = selected_int_kind(r=15)  !! integer precision
  integer,parameter :: dp = selected_real_kind(p=15) !! double precision
contains
!
  function perm(n,a) result(ret)
    integer(ip) :: n
    real(dp), dimension(:,:) :: a
    real(dp) :: ret
    integer(ip) :: ii, j
    logical  :: x( size(a,1) )
    real(dp) :: w( size(a,1) )
    ret = 0.0_dp
    x = .false.
    w = a( :, n ) - 0.50_dp * sum (a, 2)
!
    write(*,*) x
!
    do ii = 1_ip, 2_ip**(n-2)
       ret = ret - real (product (w), dp)
       x( 1 ) = .not. x( 1 )
       if ( x( 1 ) ) then
          w = w + a( :, 1 )
       else
          w = w - a( :, 1 )
       end if
       ret = ret + real (product (w), dp)
       j = 1
       do while ( .not. x( j ) )
!
          write(*,*) j
!
          j = j + 1
       end do
       j = j + 1
       x( j ) = .not. x( j )
       if ( x( j ) ) then
          w = w + a( :, j )
       else
          w = w - a( :, j )
       end if
!
       write(*,*) x
!
    end do
    ret = ret * 2.00_dp * real ((-1)**n, dp)
  end function
end module permanent

program main
  use permanent
  implicit none
  integer(ip) :: i,j,n
  real(dp) :: val
  real(dp), dimension(:,:), allocatable :: f
!! prepare matrix
!  n = 30
  n = 4
  allocate( f(n,n) )
  do j = 1,n
    do i = 1,n
      f(i,j) = 1.0_dp
!      f(i,j) = ((i-1_ip)*n+(j-1_ip)+1_ip) * 1.0_dp
    end do
  end do
!! fortran style print
!  do j = 1,n
!    write(*,*) f(:,j)
!  end do
!  write(*,*)
!! c style print
!  do i = 1,n
!    write(*,*) f(i,:)
!  end do
!  write(*,*)
!! calculate permanent
  val = perm(n,f)
  write(*,*) val
!! deallocate matrix
  deallocate(f)
end program main
